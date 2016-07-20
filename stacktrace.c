#include<stdio.h>
#include <internal/thread_def.h> 
#include <internal/trcctx_def.h> 

void **ptr_fun;
int count_fun;

extern int trcStackTrace (cpu_regset_t * pRegs, TRC_CTX * pCtx);


void fun0(int a, int b)
{
    
    printf("i am fun0\n");
  /*  callstack_trace_start();*/
    fun1();
}

void fun1(char *a)
{
   printf("i am fun1\n");
   fun2();
}

void fun2(void *ptrr)
{
   printf("i am fun2\n");
    fun3();
}

int fun3()
{
callstack_trace_start();
     printf("i am fun3\n");
    while(1)
    {
        kt_thread_pause(1);
    }
   
    return 0;
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
    tid = kt_thread_create_obsolete(&attrib, (int_fn_t)fun0, &params, TRUE);

    kt_thread_pause(2);
    
    kt_thread_suspend(tid);

    thread_regs_get(tid, &regset);

    trcStack(&regset, (int_fn_t)trcDefaultPrint, tid);

   
    
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

}
