#include<stdio.h>
#include <internal/thread_def.h> 
#include <internal/trcctx_def.h> 

void **ptr_fun;
int count_fun;

extern int trcStackTrace (cpu_regset_t * pRegs, TRC_CTX * pCtx);


void fun6(int b, char c)
{
    int_fn_t a;
    a = 0xffffffff;
    a();
}

void fun0()
{
__asm__ __volatile__ (".word 0xffffffff" :  : );


  #if 0 
   int_fn_t a;
    void *b;
   a = (char *)0xffffffff;
   a();
  /*  callstack_trace_start();*/
    #endif
}

void fun5()
{
    int_fn_t *a;
    a = (int_fn_t *)malloc(100);
    *(int *)a = 0x3000ff0f;
    ((int_fn_t)a)();
}

void fun1()
{
    int a[10];
    int i;
    for(i = 0; i < 100; i++)
        a[i] = 1;



    printf("%d\t%d\n",a[2],a[99]);
}

void fun2()
{
   int b;
   b = 999;
   b /= 0;
   printf("b = %d\n",b);
}

int fun3()
{
#if 0
callstack_trace_start();
     printf("i am fun3\n");
    while(1)
    {
        kt_thread_pause(1);
    }
#endif






strcpy((void *)-1,"aaa");
    return 0;
}

void fun4()
{
    cpu_regset_t regset;
    thread_attr_t attrib;
	thread_entry_param_t params;
    int tid;
    

    count_fun = 0;
    attrib.name =       "trc-test";
    attrib.priority =   50;
    attrib.options =    0;
    attrib.stack_size = 40000;
    bzero((char *)&params, sizeof(thread_entry_param_t));
    tid = kt_thread_create_obsolete(&attrib, (int_fn_t)0x3000ff0f, &params, TRUE);

    kt_thread_pause(2);
}

void trcDefaultPrint
(
    INSTR * callAdrs,
    INSTR * funcAdrs,
    int nArgs,
    int * args,
    int taskId,
    bool_t isKernelAdrs
)
{
    ptr_fun[count_fun++] = (void *)funcAdrs;
}

void trc_test1(void)
{
    cpu_regset_t regset;
    thread_attr_t attrib;
	thread_entry_param_t params;
    int tid;
    void **ptr_fun1;
    int i;

    count_fun = 0;
    attrib.name =       "trc-test";
    attrib.priority =   50;
    attrib.options =    0;
    attrib.stack_size = 40000;
    bzero((char *)&params, sizeof(thread_entry_param_t));
    ptr_fun = (void **)malloc(24);
    ptr_fun1 = (void **)malloc(24);
    tid = kt_thread_create_obsolete(&attrib, (int_fn_t)fun3, &params, TRUE);

    kt_thread_pause(2);
    
    kt_thread_suspend(tid);

    thread_regs_get(tid, &regset);

    trcStack(&regset, (int_fn_t)trcDefaultPrint, tid);
#if 0
   
    
    printf("%p\n",fun0);
    ptr_fun1[0] = (void *)fun0;
    ptr_fun1[1] = (void *)fun1;
    ptr_fun1[2] = (void *)fun2;
    ptr_fun1[3] = (void *)fun3;
    ptr_fun1[4] = (void *)trc_test1;
    for(i = 0; i < 5; i++)
    {
        printf("fun%d = %p\t%p\n",i,ptr_fun1[i],ptr_fun[i]);
        if(ptr_fun1[i] != ptr_fun[i])
            printf("\t%d\n",i);
    }
#endif
}

void testrun(char *ip,char *testName,char *pktSize,char *bufsize)
{
    *testName = 0xfffffff;
    *bufsize = 0xfffffff;
    printf("%s\n%s\n%d\n%d\n",ip,testName,pktSize,bufsize);
}



void test_task_spawn(char *ip,char *testName,char *pktSize,char *bufsize)
{
	/*kt_thread_create_3("zzz",100,0x4000,blasterUDP,"192.168.0.127",5000,10000);*/

	taskSpawn("zzz",100,0,0x400000,testrun,ip,testName,pktSize,bufsize,0,0,0,0,0,0);
	sleep(1);

}