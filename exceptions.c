#include<stdio.h>
#include <internal/thread_def.h> 
#include <internal/trcctx_def.h> 


void dtabort()
{
    int a = 0xfffffff0;
    *(int *)a = 1;
}

void prefatch1(int b, char c)
{
    int_fn_t a;
    a = 0xfffffff0;
    a();
}

void undifined1()
{
    __asm__ __volatile__ (".word 0xffffffff" :  : );
}


void undifined2()
{
    int_fn_t *a;
    a = (int_fn_t *)malloc(100);
    *(int_fn_t *)a = 0x3000ff00;
    ((int_fn_t)a)();
}


void fun2()
{
   int b;
   b = 999;
   b /= 0;
   printf("b = %d\n",b);
}

void prefatch2()
{
    cpu_regset_t regset;
    thread_attr_t attrib;
	thread_entry_param_t params;
    int tid;
    

    attrib.name =       "trc-test";
    attrib.priority =   50;
    attrib.options =    0;
    attrib.stack_size = 40000;
    bzero((char *)&params, sizeof(thread_entry_param_t));
    tid = kt_thread_set_up(&attrib, (int_fn_t)0x3000ff00, &params, TRUE);

    kt_thread_pause(2);
}




