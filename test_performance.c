#include <stdio.h>
#include <stdlib.h>
#define N 0x20

void swap(int *a, int *b)
{
    int *tmp;
    tmp=a;
    a=b;
    b=tmp;
}

int funn1(int n)
{
    if (n == 1)
        return 1;
    else 
        return n+funn1(n-1);
}

int funn2(int n)
{
    if (n == 1 || n == 0)
        return 1;
    else
        return (funn2(n-2)%0xffff)+(funn2(n-1)%0xffff);
}

void test_route(int *arg)
{
    int i,j;
    int *arr;
    int n;
    int x,y;
    arr = (int *)malloc(sizeof(int)*(N));
    if (arr == NULL)
    {
        *arg = 1;
        syslog_post(SYSLOG_INIT,"malloc is wrong\n");
        return;
    }
    syslog_post(SYSLOG_INIT,"arr = %p\n",arr);
    n = N - 1;
    for (i=0; i < N - 1; i++)
    {
        arr[i] = n--;
    }

    for (i=0; i < N - 1; i++)
    {
        for(j=0;j < N - 1; j++)
        {
            if (arr[i]>arr[j])
            {
                x = funn1(i);
                y = funn2(j);
                swap(&x,&y);
            }
        }
    }
    *arg=1; 
    free(arr);
}

int test()
{
    int           selfPriority;         /* priority of this task */
    int       taskId[4];               /* taskId of spawned task */
    int i;
    int countflag[4] = {0};
    int before,after; 
     /* setup */

    taskPriorityGet (0, &selfPriority);
    before = tick_ulong_get();
    /* create a new task */
    for (i = 0; i < 4; i++)
    {
         taskId[i] = taskSpawn ("task", selfPriority + 1 + i, 0, 0x8000,
                        (void *)test_route,&countflag[i],0L,0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L);

        if (taskId[i] == -1)
        {
            printf("Failed to create the task%d",i);
            return -1;
        }
    }
    
    while( !(countflag[0] && countflag[1] && countflag[2] && countflag[3]) ) 
    {
        taskDelay(1);
    }
 
    after = tick_ulong_get();
    printf("%d\t%d\t%d\t%d\n",countflag[0],countflag[1],countflag[2],countflag[3]);
    printf("before = %d\n",before);
    printf("after = %d\n",after);
    printf("during time = %d\n",after - before);
}
