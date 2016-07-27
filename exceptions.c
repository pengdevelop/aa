#include<stdio.h>
#include <internal/thread_def.h> 
#include <internal/trcctx_def.h> 


void dtabort()
{
    int a = 0xffffffff;
    *a = 1;
}

void prefatch1(int b, char c)
{
    int_fn_t a;
    a = 0xffffffff;
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
    *(int *)a = 0x3000ff0f;
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
    

    count_fun = 0;
    attrib.name =       "trc-test";
    attrib.priority =   50;
    attrib.options =    0;
    attrib.stack_size = 40000;
    bzero((char *)&params, sizeof(thread_entry_param_t));
    tid = kt_thread_create_obsolete(&attrib, (int_fn_t)0x3000ff0f, &params, TRUE);

    kt_thread_pause(2);
}




