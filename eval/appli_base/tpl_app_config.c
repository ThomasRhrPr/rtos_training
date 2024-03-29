/**
 * @file tpl_app_config.c
 *
 * @section desc File description
 *
 * OS data structure generated from application my_cpu
 * Automatically generated by goil on Mon Jan  8 11:36:40 2024
 * from root OIL file appli_base.oil
 *
 * @section copyright Copyright
 *
 * Trampoline OS
 *
 * Trampoline is copyright (c) IRCCyN 2005-2007
 * Trampoline is protected by the French intellectual property law.
 *
 * This software is distributed under the Lesser GNU Public Licence
 *
 * @section infos File informations
 *
 * $Date$
 * $Rev$
 * $Author$
 * $URL$
 */

#include <stdbool.h>
#include <stdint.h>

#include "tpl_app_config.h"

#include "tpl_os_internal_types.h"
#include "tpl_machine.h"
#include "tpl_os_interrupt.h"
#include "tpl_os_interrupt_kernel.h"
#include "tpl_os_alarm_kernel.h"
#include "tpl_os_alarm.h"
#include "tpl_os_resource_kernel.h"
#include "tpl_os_resource.h"
#include "tpl_os_event_kernel.h"
#include "tpl_os_event.h"
#include "tpl_os_action.h"
#include "tpl_os_kernel.h"
#include "tpl_os_definitions.h"


/*=============================================================================
 * Application Modes tables for tasks, alarms and schedule tables
 * Application Modes masks are as follow:
 * std = 1
 */

CONST(tpl_application_mode, OS_CONST) std = 0; /* mask = 1 */
CONST(tpl_application_mode, OS_CONST) OSDEFAULTAPPMODE = 0;
CONST(tpl_appmode_mask, OS_CONST) tpl_task_app_mode[TASK_COUNT] = {
  1 /* task taskControl : std */ ,
  0 /* task taskDistance :  */ ,
  0 /* task taskMonitoring :  */ 
};

CONST(tpl_appmode_mask, OS_CONST) tpl_alarm_app_mode[ALARM_COUNT] = {
  1 /* alarm alarmDistance : std */ ,
  1 /* alarm alarmMonitoring : std */ 
};

#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Declaration of event masks
 */

/* Event evFillTank */
#define evFillTank_mask 1
CONST(EventMaskType, AUTOMATIC) evFillTank = evFillTank_mask;

/* Event evFuel */
#define evFuel_mask 2
CONST(EventMaskType, AUTOMATIC) evFuel = evFuel_mask;

/* Event evDistance */
#define evDistance_mask 4
CONST(EventMaskType, AUTOMATIC) evDistance = evDistance_mask;

/*=============================================================================
 * Declaration of processes IDs
 */

/* Task taskControl identifier */
#define taskControl_id 0
CONST(TaskType, AUTOMATIC) taskControl = taskControl_id;

/* Task taskDistance identifier */
#define taskDistance_id 1
CONST(TaskType, AUTOMATIC) taskDistance = taskDistance_id;

/* Task taskMonitoring identifier */
#define taskMonitoring_id 2
CONST(TaskType, AUTOMATIC) taskMonitoring = taskMonitoring_id;

/* ISR ISRFillTank identifier */
#define ISRFillTank_id 3
/*=============================================================================
 * Declaration of Alarm IDs
 */

/* Alarm alarmDistance identifier */
#define alarmDistance_id 0
CONST(AlarmType, AUTOMATIC) alarmDistance = alarmDistance_id;

/* Alarm alarmMonitoring identifier */
#define alarmMonitoring_id 1
CONST(AlarmType, AUTOMATIC) alarmMonitoring = alarmMonitoring_id;

#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/**
 * The scheduler resource descriptor.
 * One scheduler resource is defined per core.
 *
 * @see #RES_SCHEDULER
 */

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
VAR(tpl_resource, OS_VAR) res_sched_rez_desc = {
  RES_SCHEDULER_PRIORITY,   /*  ceiling priority                            */
  0,                        /*  owner_prev_priority                         */
  INVALID_PROC_ID,          /*  owner                                       */
#if WITH_OSAPPLICATION == YES
  INVALID_OSAPPLICATION_ID, /*  OS Application id                           */
#endif

  NULL                      /*  next_res                                    */
};
#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONSTP2VAR(tpl_resource, AUTOMATIC, OS_APPL_DATA)
tpl_resource_table[RESOURCE_COUNT] = {
  &res_sched_rez_desc
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Definition and initialization of Counters related defines and structures
 */
/*-----------------------------------------------------------------------------
 * Counter SystemCounter descriptor
 */

#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_tick, OS_CONST) OSTICKSPERBASE = 1;
CONST(tpl_tick, OS_CONST) OSMAXALLOWEDVALUE = 65535;
CONST(tpl_tick, OS_CONST) OSMINCYCLE = 1;

#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_counter, OS_VAR) SystemCounter_counter_desc = {
  /* ticks per base       */  1,
  /* max allowed value    */  65535,
  /* minimum cycle        */  1,
  /* current tick         */  0,
  /* current date         */  0,
#if WITH_OSAPPLICATION == YES
    /* OS Application id    */  
#endif
    /* first alarm          */  NULL_PTR,
    /* next alarm to raise  */  NULL_PTR
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#include "tpl_os_kernel.h"          /* tpl_schedule */
#include "tpl_os_timeobj_kernel.h"  /* tpl_counter_tick */
#include "tpl_machine_interface.h"  /* tpl_switch_context_from_it */

#define OS_START_SEC_VAR_32BIT
#include "tpl_memmap.h"
volatile VAR(uint32, OS_VAR) tpl_time_counter = 0;
#define OS_STOP_SEC_VAR_32BIT
#include "tpl_memmap.h"

#define OS_START_SEC_CODE
#include "tpl_memmap.h"

FUNC(tpl_bool, OS_CODE) tpl_tick_SysTick(void)
{
  tpl_counter_tick(&SystemCounter_counter_desc);
  tpl_time_counter++;

  if (tpl_kern.need_schedule == TRUE)
  {
    tpl_schedule_from_running();
    LOCAL_SWITCH_CONTEXT()
  }

  return TRUE;
}

#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

/*=============================================================================
 * Descriptor(s) of the idle task(s)
 */
/*-----------------------------------------------------------------------------
 * Task IDLE_TASK descriptor
 */
#define OS_START_SEC_CODE
#include "tpl_memmap.h"
/*
 * Task IDLE_TASK function prototype
 */

FUNC(void, OS_APPL_CODE) idle_function(void);
#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * Task IDLE_TASK stack
 */
#define APP_Task_IDLE_TASK_START_SEC_STACK
#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) IDLE_TASK_stack_zone[IDLE_STACK_SIZE/sizeof(tpl_stack_word)];
#define APP_Task_IDLE_TASK_STOP_SEC_STACK
#include "tpl_memmap.h"

#define IDLE_TASK_STACK {IDLE_TASK_stack_zone, IDLE_STACK_SIZE}

/*
 * Task IDLE_TASK context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_core_context, OS_VAR) IDLE_TASK_int_context;

#define IDLE_TASK_CONTEXT &IDLE_TASK_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"




/*
  No timing protection
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of task IDLE_TASK
 */
CONST(tpl_proc_static, OS_CONST) IDLE_TASK_task_stat_desc = {
  /* context                  */  IDLE_TASK_CONTEXT,
  /* stack                    */  IDLE_TASK_STACK,
  /* entry point (function)   */  idle_function,
  /* internal ressource       */  NULL,
  /* task id                  */  IDLE_TASK_ID,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  0,
  /* max activation count     */  1,
  /* task type                */  TASK_BASIC,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES

  /* execution budget */        0,
  /* timeframe        */        0, 
  /* pointer to the timing
     protection descriptor    */ NULL

#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task IDLE_TASK
 */
VAR(tpl_proc, OS_VAR) IDLE_TASK_task_desc = {
  /* resources                      */  NULL,
#if WITH_OSAPPLICATION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_OSAPPLICATION */
  /* activate count                 */  0,
  /* task priority                  */  0,
  /* task state                     */  SUSPENDED
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"


/*=============================================================================
 * Definition and initialization of Task related defines and structures
 */
/*-----------------------------------------------------------------------------
 * Task taskControl descriptor
 */
#define APP_Task_taskControl_START_SEC_CODE

#include "tpl_memmap.h"
/*
 * Task taskControl function prototype
 */

FUNC(void, OS_APPL_CODE) taskControl_function(void);
#define APP_Task_taskControl_STOP_SEC_CODE

#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * Task taskControl stack
 */
#define APP_Task_taskControl_START_SEC_STACK
#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) taskControl_stack_zone[300/sizeof(tpl_stack_word)];
#define APP_Task_taskControl_STOP_SEC_STACK
#include "tpl_memmap.h"

#define taskControl_STACK {taskControl_stack_zone, 300}

/*
 * Task taskControl context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_core_context, OS_VAR) taskControl_int_context;

#define taskControl_CONTEXT &taskControl_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"




/*
  No timing protection
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of task taskControl
 */
CONST(tpl_proc_static, OS_CONST) taskControl_task_stat_desc = {
  /* context                  */  taskControl_CONTEXT,
  /* stack                    */  taskControl_STACK,
  /* entry point (function)   */  taskControl_function,
  /* internal ressource       */  NULL,
  /* task id                  */  taskControl_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  1,
  /* max activation count     */  1,
  /* task type                */  TASK_EXTENDED,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES

  /* execution budget */        0,
  /* timeframe        */        0, 
  /* pointer to the timing
     protection descriptor    */ NULL

#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task taskControl
 */
VAR(tpl_proc, OS_VAR) taskControl_task_desc = {
  /* resources                      */  NULL,
#if WITH_OSAPPLICATION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_OSAPPLICATION */
  /* activate count                 */  0,
  /* task priority                  */  1,
  /* task state                     */  AUTOSTART
};

/*
 * Event structure of task taskControl
 */
VAR(tpl_task_events, OS_VAR) taskControl_task_evts = {
  /* event set  */ 0,
  /* event wait */ 0
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Task taskDistance descriptor
 */
#define APP_Task_taskDistance_START_SEC_CODE

#include "tpl_memmap.h"
/*
 * Task taskDistance function prototype
 */

FUNC(void, OS_APPL_CODE) taskDistance_function(void);
#define APP_Task_taskDistance_STOP_SEC_CODE

#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * Task taskDistance stack
 */
#define APP_Task_taskDistance_START_SEC_STACK
#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) taskDistance_stack_zone[300/sizeof(tpl_stack_word)];
#define APP_Task_taskDistance_STOP_SEC_STACK
#include "tpl_memmap.h"

#define taskDistance_STACK {taskDistance_stack_zone, 300}

/*
 * Task taskDistance context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_core_context, OS_VAR) taskDistance_int_context;

#define taskDistance_CONTEXT &taskDistance_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"




/*
  No timing protection
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of task taskDistance
 */
CONST(tpl_proc_static, OS_CONST) taskDistance_task_stat_desc = {
  /* context                  */  taskDistance_CONTEXT,
  /* stack                    */  taskDistance_STACK,
  /* entry point (function)   */  taskDistance_function,
  /* internal ressource       */  NULL,
  /* task id                  */  taskDistance_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  2,
  /* max activation count     */  1,
  /* task type                */  TASK_BASIC,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES

  /* execution budget */        0,
  /* timeframe        */        0, 
  /* pointer to the timing
     protection descriptor    */ NULL

#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task taskDistance
 */
VAR(tpl_proc, OS_VAR) taskDistance_task_desc = {
  /* resources                      */  NULL,
#if WITH_OSAPPLICATION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_OSAPPLICATION */
  /* activate count                 */  0,
  /* task priority                  */  2,
  /* task state                     */  SUSPENDED
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Task taskMonitoring descriptor
 */
#define APP_Task_taskMonitoring_START_SEC_CODE

#include "tpl_memmap.h"
/*
 * Task taskMonitoring function prototype
 */

FUNC(void, OS_APPL_CODE) taskMonitoring_function(void);
#define APP_Task_taskMonitoring_STOP_SEC_CODE

#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * Task taskMonitoring stack
 */
#define APP_Task_taskMonitoring_START_SEC_STACK
#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) taskMonitoring_stack_zone[300/sizeof(tpl_stack_word)];
#define APP_Task_taskMonitoring_STOP_SEC_STACK
#include "tpl_memmap.h"

#define taskMonitoring_STACK {taskMonitoring_stack_zone, 300}

/*
 * Task taskMonitoring context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_core_context, OS_VAR) taskMonitoring_int_context;

#define taskMonitoring_CONTEXT &taskMonitoring_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"




/*
  No timing protection
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of task taskMonitoring
 */
CONST(tpl_proc_static, OS_CONST) taskMonitoring_task_stat_desc = {
  /* context                  */  taskMonitoring_CONTEXT,
  /* stack                    */  taskMonitoring_STACK,
  /* entry point (function)   */  taskMonitoring_function,
  /* internal ressource       */  NULL,
  /* task id                  */  taskMonitoring_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  2,
  /* max activation count     */  1,
  /* task type                */  TASK_BASIC,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES

  /* execution budget */        0,
  /* timeframe        */        0, 
  /* pointer to the timing
     protection descriptor    */ NULL

#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task taskMonitoring
 */
VAR(tpl_proc, OS_VAR) taskMonitoring_task_desc = {
  /* resources                      */  NULL,
#if WITH_OSAPPLICATION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_OSAPPLICATION */
  /* activate count                 */  0,
  /* task priority                  */  2,
  /* task state                     */  SUSPENDED
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"


/*=============================================================================
 * Definition and initialization of ISR2 related defines and structures
 */
/*-----------------------------------------------------------------------------
 * ISR ISRFillTank descriptor
 */
#define APP_ISR_ISRFillTank_START_SEC_CODE
#include "tpl_memmap.h"
/*
 * ISR ISRFillTank function prototype
 */
FUNC(void, OS_APPL_CODE) ISRFillTank_function(void);
#define APP_ISR_ISRFillTank_STOP_SEC_CODE
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures of isr ISRFillTank
 */
/*
 * ISR ISRFillTank stack
 */
#define APP_ISR_ISRFillTank_START_SEC_STACK
#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) ISRFillTank_stack_zone[200/sizeof(tpl_stack_word)];
#define APP_ISR_ISRFillTank_STOP_SEC_STACK
#include "tpl_memmap.h"

#define ISRFillTank_STACK {ISRFillTank_stack_zone, 200}

/*
 * ISR ISRFillTank context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_core_context, OS_VAR) ISRFillTank_int_context;

#define ISRFillTank_CONTEXT &ISRFillTank_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"


/*
  No timing protection
 */

#if WITH_AUTOSAR_TIMING_PROTECTION == YES
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
#endif

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of ISR ISRFillTank
 */
CONST(tpl_proc_static, OS_CONST) ISRFillTank_isr_stat_desc = {
  /* context                  */  ISRFillTank_CONTEXT,
  /* stack                    */  ISRFillTank_STACK,
  /* entry point (function)   */  ISRFillTank_function,
  /* internal ressource       */  NULL,
  /* ISR id                   */  ISRFillTank_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif

  /* ISR base priority       */  4,
  /* ISR activation count     */  1,
  /* ISR type                */  IS_ROUTINE,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES

  /* execution budget */        0,
  /* timeframe        */        0,
  /* pointer to the timing
     protection descriptor    */ NULL

#endif
};

CONST(tpl_isr_static, OS_CONST) ISRFillTank_isr_helper = {
  /* helper */ NULL,
  /* next */   NULL,
  /* id */     ISRFillTank_id
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of ISR ISRFillTank
 */
VAR(tpl_proc, OS_VAR) ISRFillTank_isr_desc = {
  /* resources                      */  NULL,
#if WITH_OSAPPLICATION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_OSAPPLICATION */
  /* activate count                 */  0,
  /* ISR priority                   */  4,
  /* ISR state                      */  SUSPENDED
};
#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_CODE
#include "tpl_memmap.h"
extern FUNC(void, OS_CODE) EXTI0_IRQ_ClearFlag(void);

FUNC(void, OS_CODE) EXTI0_IRQ_Handler(void)
{
  /* ISR1s */

  /* ISR2s */
  GET_CURRENT_CORE_ID(core_id)

#if WITH_STACK_MONITORING == YES
  GET_TPL_KERN_FOR_CORE_ID(core_id, kern)
  tpl_check_stack((tpl_proc_id)TPL_KERN_REF(kern).running_id);
#endif /* WITH_AUTOSAR_STACK_MONITORING */

  if(EXTI->PR & EXTI_PR_PR0) {
     tpl_fast_central_interrupt_handler(ISRFillTank_id);
     EXTI->PR = EXTI_PR_PR0; /* ack */
  }
  /* COUNTERs */

}

FUNC(void, OS_CODE) SysTick_Handler(void)
{
  /* ISR1s */

  /* ISR2s */
  /* COUNTERs */
  tpl_tick_SysTick();
  /* ACK */

}

#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
/*=============================================================================
 * Definition and initialization of process tables (tasks and isrs)
 */
CONSTP2CONST(tpl_proc_static, AUTOMATIC, OS_APPL_DATA)
tpl_stat_proc_table[TASK_COUNT+ISR_COUNT+1] = {
  &taskControl_task_stat_desc,
  &taskDistance_task_stat_desc,
  &taskMonitoring_task_stat_desc,
  &ISRFillTank_isr_stat_desc,
  &IDLE_TASK_task_stat_desc
};

CONSTP2VAR(tpl_proc, AUTOMATIC, OS_APPL_DATA)
tpl_dyn_proc_table[TASK_COUNT+ISR_COUNT+1] = {
  &taskControl_task_desc,
  &taskDistance_task_desc,
  &taskMonitoring_task_desc,
  &ISRFillTank_isr_desc,
  &IDLE_TASK_task_desc
};

CONSTP2VAR(tpl_task_events, AUTOMATIC, OS_APPL_DATA)
tpl_task_events_table[EXTENDED_TASK_COUNT] = {
  &taskControl_task_evts
};

CONSTP2CONST(tpl_isr_static, AUTOMATIC, OS_APPL_DATA)
tpl_isr_stat_table[ISR_COUNT] = {
  &ISRFillTank_isr_helper
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*=============================================================================
 * Definition and initialization of Alarm related structures
 */
/*-----------------------------------------------------------------------------
 * Alarm alarmDistance descriptor
 *
 * This alarm does the activation of task taskDistance
 */


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_task_activation_action, OS_CONST) alarmDistance_action = {
  {
    /* action function  */  tpl_action_activate_task
  },
  /* task id            */  taskDistance_id
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
 
#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_alarm_static, OS_CONST) alarmDistance_static = {
  {
    /* pointer to counter           */  &SystemCounter_counter_desc,
    /* pointer to the expiration    */  tpl_raise_alarm
#if (WITH_ID == YES)
    /* id of the alarm for tracing  */  , alarmDistance_id
#endif
#if WITH_OSAPPLICATION == YES
    /* OS application id            */  , 
#endif
  },
  /* action of the alarm  */  (tpl_action *)&alarmDistance_action
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_time_obj, OS_VAR) alarmDistance_alarm_desc = {
    /* pointer to the static part   */  (tpl_time_obj_static *)&alarmDistance_static,
    /* next alarm                   */  NULL,
    /* prev alarm                   */  NULL,
    /* cycle                        */  100,
    /* date                         */  1,
    /* State of the alarm           */  ALARM_AUTOSTART
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Alarm alarmMonitoring descriptor
 *
 * This alarm does the activation of task taskMonitoring
 */


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_task_activation_action, OS_CONST) alarmMonitoring_action = {
  {
    /* action function  */  tpl_action_activate_task
  },
  /* task id            */  taskMonitoring_id
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
 
#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_alarm_static, OS_CONST) alarmMonitoring_static = {
  {
    /* pointer to counter           */  &SystemCounter_counter_desc,
    /* pointer to the expiration    */  tpl_raise_alarm
#if (WITH_ID == YES)
    /* id of the alarm for tracing  */  , alarmMonitoring_id
#endif
#if WITH_OSAPPLICATION == YES
    /* OS application id            */  , 
#endif
  },
  /* action of the alarm  */  (tpl_action *)&alarmMonitoring_action
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_time_obj, OS_VAR) alarmMonitoring_alarm_desc = {
    /* pointer to the static part   */  (tpl_time_obj_static *)&alarmMonitoring_static,
    /* next alarm                   */  NULL,
    /* prev alarm                   */  NULL,
    /* cycle                        */  500,
    /* date                         */  1,
    /* State of the alarm           */  ALARM_AUTOSTART
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONSTP2VAR(tpl_time_obj, AUTOMATIC, OS_APPL_DATA)
  tpl_alarm_table[ALARM_COUNT] = {
  &alarmDistance_alarm_desc,
  &alarmMonitoring_alarm_desc
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Declaration of flags functions
 */
/* $FLAGSFUNCTIONS$ */

/*=============================================================================
 * Definition and initialization of Ready List structures
 */
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_heap_entry, OS_VAR) tpl_ready_list[6];
VAR(tpl_rank_count, OS_VAR) tpl_tail_for_prio[5] = {
  0,
  0,
  0,
  0
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/**
 * @internal
 *
 * tpl_kern gather informations on the current executing object and
 * the previous one
 */
VAR(tpl_kern_state, OS_VAR) tpl_kern =
{
  NULL,                      /* no running task static descriptor   */
  &IDLE_TASK_task_stat_desc, /* elected task to run is idle task    */
  NULL,                      /* no running task dynamic descriptor  */
  &IDLE_TASK_task_desc,      /* elected task to run is idle task    */
  INVALID_PROC_ID,           /* no running task so no ID            */
  INVALID_PROC_ID,           /* idle task has no ID                 */
  NO_NEED_SWITCH,            /* no context switch needed at start   */
  FALSE,                     /* no schedule needed at start         */
#if WITH_MEMORY_PROTECTION == YES
  1,                         /* at early system startup, we run in  */
                             /*  kernel mode, so in trusted mode    */
#endif /* WITH_MEMORY_PROTECTION */
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONSTP2CONST(char, AUTOMATIC, OS_APPL_DATA) proc_name_table[TASK_COUNT + ISR_COUNT + 1] = {

  "taskControl",
  "taskDistance",
  "taskMonitoring",
  "ISRFillTank",
  "*idle*"
};
#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/* End of file tpl_app_config.c */
