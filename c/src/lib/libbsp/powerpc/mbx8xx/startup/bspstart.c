/*  bspstart.c
 *
 *  This set of routines starts the application.  It includes application,
 *  board, and monitor specific initialization and configuration.
 *  The generic CPU dependent initialization has been performed
 *  before this routine is invoked.
 *
 *  COPYRIGHT (c) 1989-1998.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  Modifications for MBX860:
 *  Copyright (c) 1999, National Research Council of Canada
 *
 *  $Id$
 */

#include <bsp.h>
#include <rtems/libio.h>
 
#include <libcsupport.h>
 
#include <string.h>
 
#ifdef STACK_CHECKER_ON
#include <stackchk.h>
#endif

/*
 *  The original table from the application (in ROM) and our copy of it with
 *  some changes. Configuration is defined in <confdefs.h>. Make sure that
 *  our configuration tables are uninitialized so that they get allocated in
 *  the .bss section (RAM).
 */
extern rtems_configuration_table Configuration;
rtems_configuration_table  BSP_Configuration;

rtems_cpu_table Cpu_table;

char *rtems_progname;

/*
 *  Use the shared implementations of the following routines.
 *  Look in rtems/c/src/lib/libbsp/shared/bsppost.c and
 *  rtems/c/src/lib/libbsp/shared/bsplibc.c.
 */
void bsp_postdriver_hook(void);
void bsp_libc_init( void *, unsigned32, int );

/*
 *  bsp_pretasking_hook
 *
 *  Called when RTEMS initialization is complete but before interrupts and
 *  tasking are enabled. Used to setup libc and install any BSP extensions.
 *
 *  Must not use libc (to do io) from here, since drivers are not yet
 *  initialized.
 *
 *  Installed in the rtems_cpu_table defined in 
 *  rtems/c/src/exec/score/cpu/m68k/cpu.h in main() below. Called from
 *  rtems_initialize_executive() defined in rtems/c/src/exec/sapi/src/init.c
 *
 *  Input parameters: NONE
 *
 *  Output parameters: NONE
 *
 *  Return values: NONE
 */
void bsp_pretasking_hook(void)
{
  /* 
   *  These are assigned addresses in the linkcmds file for the BSP. This
   *  approach is better than having these defined as manifest constants and
   *  compiled into the kernel, but it is still not ideal when dealing with
   *  multiprocessor configuration in which each board as a different memory
   *  map. A better place for defining these symbols might be the makefiles.
   *  Consideration should also be given to developing an approach in which
   *  the kernel and the application can be linked and burned into ROM
   *  independently of each other.
   */
    extern unsigned char _HeapStart;
    extern unsigned char _HeapEnd;

    bsp_libc_init( &_HeapStart, &_HeapEnd - &_HeapStart, 0 );
  
#ifdef STACK_CHECKER_ON
  /*
   *  Initialize the stack bounds checker
   *  We can either turn it on here or from the app.
   */
  
  Stack_check_Initialize();
#endif /* STACK_CHECKER_ON */
  
#ifdef RTEMS_DEBUG
  rtems_debug_enable( RTEMS_DEBUG_ALL_MASK );
#endif
}


/*
 *  bsp_start()
 *
 *  Board-specific initialization code. Called from the generic boot_card()
 *  function defined in rtems/c/src/lib/libbsp/shared/main.c. That function
 *  does some of the board independent initialization. It is called from the
 *  MBX8xx entry point _start() defined in
 *  rtems/c/src/lib/libbsp/powerpc/mbx8xx/startup/start.S
 *
 *  _start() has set up a stack, has zeroed the .bss section, has turned off
 *  interrupts, and placed the processor in the supervisor mode. boot_card()
 *  has left the processor in that state when bsp_start() was called.
 *
 *  RUNS WITH ADDRESS TRANSLATION AND CACHING TURNED OFF!
 *  ASSUMES THAT THE VIRTUAL ADDRESSES WILL BE IDENTICAL TO THE PHYSICAL
 *  ADDRESSES. Software-controlled address translation would be required
 *  otherwise.
 *
 *  Input parameters: NONE
 *
 *  Output parameters: NONE
 *
 *  Return values: NONE
 */
void bsp_start(void)
{
  extern void *_WorkspaceBase;
  
  mmu_init();
  
  /*
   * Enable instruction and data caches. Do not force writethrough mode.
   */
  #ifdef INSTRUCTION_CACHE_ENABLE
	rtems_cache_enable_instruction();
  #endif

  #ifdef DATA_CACHE_ENABLE
	rtems_cache_enable_data();
  #endif
   
  /*
   *  Allocate the memory for the RTEMS Work Space.  This can come from
   *  a variety of places: hard coded address, malloc'ed from outside
   *  RTEMS world (e.g. simulator or primitive memory manager), or (as
   *  typically done by stock BSPs) by subtracting the required amount
   *  of work space from the last physical address on the CPU board.
   *
   *  In this case, the memory is not malloc'ed.  It is just
   *  "pulled from the air".
   */
  BSP_Configuration.work_space_start = (void *)&_WorkspaceBase;

  /*
   *  initialize the CPU table for this BSP
   */

  Cpu_table.pretasking_hook = bsp_pretasking_hook;  /* init libc, etc. */
  Cpu_table.postdriver_hook = bsp_postdriver_hook;
  if( Cpu_table.interrupt_stack_size < 4 * 1024 )
      Cpu_table.interrupt_stack_size = 4 * 1024;

  Cpu_table.clicks_per_usec = 1;  /* for 4MHz extclk */
  Cpu_table.serial_per_sec = 10000000;
  Cpu_table.serial_external_clock = 1;
  Cpu_table.serial_xon_xoff = 0;
  Cpu_table.serial_cts_rts = 1;
  Cpu_table.serial_rate = 9600;
#if ( defined(mbx821_001) || defined(mbx821_001b) || defined(mbx860_001b) )
  Cpu_table.clock_speed = 50000000;
  Cpu_table.timer_average_overhead = 3;
  Cpu_table.timer_least_valid = 3;
#else
  Cpu_table.clock_speed = 40000000;
  Cpu_table.timer_average_overhead = 3;
  Cpu_table.timer_least_valid = 3;
#endif

  /* 
   * Call this in case we use TERMIOS for console I/O
   */
  m8xx_uart_reserve_resources( &BSP_Configuration );

  m8xx.scc2.sccm=0;
  m8xx.scc2p.rbase=0;
  m8xx.scc2p.tbase=0;
  m8xx_cp_execute_cmd( M8xx_CR_OP_STOP_TX | M8xx_CR_CHAN_SCC2 );
}

