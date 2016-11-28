/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*                          (c) Copyright 2009-2011; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                          APPLICATION CODE
*
*                                          Microsoft Windows
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : JJL
*                 FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>


/*
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            LOCAL VARIABLES
*											定义task栈空间
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];
static  CPU_STK  Task1Stk[APP_TASK_START_STK_SIZE];
static  CPU_STK  Task2Stk[APP_TASK_START_STK_SIZE];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart(void  *p_arg);
static	void  PeriodicTaskStart(void *p_arg);

/*
*********************************************************************************************************
*                                         定义任务详情 {完成所需时间，周期，开始时钟周期}
*********************************************************************************************************
*/
INT32S tasks[][3] = {
	{ 1,3,0 },
	{ 3,5,0 }
};


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

int  main (void)
{
    OSInit();                                                   /* Init uC/OS-II.                                       */
	CPU_Init();
    //OSTaskCreateExt((void(*)(void *))AppTaskStart,              /* Create the start task                                */
    //    (void          *) 0,
    //    (OS_STK        *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1],
    //    (INT8U          ) APP_TASK_START_PRIO,
    //    (INT16U         ) APP_TASK_START_PRIO,
    //    (OS_STK        *)&AppTaskStartStk[0],
    //    (INT32U         ) APP_TASK_START_STK_SIZE,
    //    (void          *) 0,
    //    (INT16U         )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

	OSTaskCreateExt(PeriodicTaskStart,
		(void          *) tasks[0],
		(OS_STK        *)&Task1Stk[APP_TASK_START_STK_SIZE - 1],
		(INT8U)1,
		(INT16U)1,
		(OS_STK        *)&Task1Stk[0],
		(INT32U)APP_TASK_START_STK_SIZE,
		(void          *) tasks[0],
		(INT16U)(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II).  */
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Argument(s) : p_arg       is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Note(s)     : (1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static void PeriodicTaskStart(void *p_arg) {
	INT32S start;
	INT32S period;
	INT32S delay;
	INT32S finishTime;
	start = 0;
	
	while (1) {
		while (OSTCBCur->leftCompTime > 0){}
		period = OSTCBCur->period;
		finishTime = OSTimeGet();
		delay = start + period - finishTime;
		
		//初始化下一周期任务
		start = start + period;
		OSTCBCur->leftCompTime = OSTCBCur->completeTime;
		//等待
		OSTimeDly(delay);
		printf(" complete 1 \n");
	}
}

//static  void  AppTaskStart (void *p_arg)
//{
//    OS_ERR  err;
//
//
//   (void)p_arg;
//
//    BSP_Init();                                                 /* Initialize BSP functions                             */
//    CPU_Init();                                                 /* Initialize uC/CPU services                           */
//
//#if OS_CFG_STAT_TASK_EN > 0u
//    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
//#endif
//
//    APP_TRACE_DBG(("uCOS-II is Running...\n\r"));
//
//    while (DEF_ON) {                                            /* Task body, always written as an infinite loop.       */
//        OSTimeDlyHMSM(0, 0, 1, 0);
//
//        APP_TRACE_DBG(("Time: %d\n\r", OSTimeGet(&err)));
//    }
//}
