#include <stdio.h>

int n = 1000;
int a[1000][1000];
int b[1000][1000];
int c[1000][1000];

int matrix_mul(int *arg,int p, int nn, int mm)
{
    int i, j, k;
    for (i = p; i < mm; i++)
    {
        for (j = 0; j < nn; j++)
        {
                c[i][j]=0;
                for (k = 0; k < nn; k++)
                {
                    c[i][j] += a[i][k] * b[k][j];
                }
        }   
    }
    *arg=1;
}
int test1()
{
    int           selfPriority;         /* priority of this task */
    int       taskId[4];               /* taskId of spawned task */
    int i;
    int countflag[4] = {0};
    int before,after;
    int ff,dd;
    int j; 
   for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
        {
            a[i][j] = b[i][j] = 1;
        }  
     /* setup */

    taskPriorityGet (0, &selfPriority);
    before = tick_ulong_get();
    /* create a new task */
    for (i = 0; i < 4; i++)
    {  
        ff = (n/4) * i;
        dd = (n/4) * (i+1);
         taskId[i] = taskSpawn ("task", selfPriority + 1 + i, 0, 0x8000,
                        (void *)matrix_mul,&countflag[i],ff,n,dd, 0L, 0L, 0L, 0L, 0L, 0L);

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
   for(i = 0; i < 20; i++)
   {
       for(j = 0; j < 20; j++)
       {
           printf("%d\t",c[i][j]);
       }
        printf("\n");
   }
}
