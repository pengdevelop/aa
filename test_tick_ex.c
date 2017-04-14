
char tmp[200];
u32 th0_count[100000];
int idx0 ;

u32 th1_count[100000];
int idx1 = 0;
u32 old;
u32 diff;
int level[100];
int idxx;



void work_show3(int idd)
{
    int max = 0;
    int min = 0xffffff;
    u64 total = 0;
    int avg = 0;
    int i;
    int workq_idx;
    int *workq_arr;
    int max_idx = 0;
    int min_idx = 0;
#ifndef PSP_TI_OMAP5    
    int frq = 50000000;
#else
    int frq = 6000000;
#endif
    int k = 0;
    int len = 0;
    idxx = 0;
    if (idd == 0)
    {
        workq_arr = th0_count;
        workq_idx = idx0;
    
    }
    else if (idd == 1)
    {
            workq_arr = th1_count;
            workq_idx = idx1;
    }
printf("\n");
    for(i = 0; i < workq_idx; i++)
    {
        if ( workq_arr[i] < 0)
            continue;

#if 1 
        diff = (workq_arr[i])/(frq/1000000);
#else
        diff = (workq_arr[i]);
#endif
        if ( diff  > max)
        {
            max = diff;
            max_idx = i;
        }

        if (diff < min)
        {
            min = diff;
            min_idx = i;
        }
            
        if (diff < 10000)
            level[0]++;
        else if (diff >= 10000 && diff < 10010)
            level[1]++;
        else if (diff >= 10010 && diff < 10020)
            level[2]++;
        else if (diff >= 10020 && diff < 10030)
            level[3]++;
        else if (diff >= 10030 && diff < 10040)
            level[4]++;
        else if (diff >= 10040 && diff < 10050)
            level[5]++;
        else if (diff >= 10050 && diff < 10060)
            level[6]++;
        else if (diff >= 10060 && diff < 10070)
            level[7]++;
        else if (diff >= 10070 && diff < 10080)
            level[8]++;
        else if (diff >= 10080 && diff < 10090)
            level[9]++;
        else if (diff >= 10090 && diff < 10100)
            level[10]++;
        else if (diff >= 10100 && diff < 10110)
            level[11]++;
        else if (diff >= 10110 && diff < 10120)
            level[12]++;
        else if (diff >= 10120 && diff < 10130)
            level[13]++;
        else if (diff >= 10130 && diff < 20000)
            level[14]++;
        else if (diff >= 20000 && diff < 20010)
            level[15]++;
        else if (diff >= 20010 && diff < 20020)
            level[16]++;
        else
            level[17]++;
            
        total  += diff;
    }


    avg = (total - max - min)/(workq_idx-2);
    len = sprintf(tmp,"\r\nfirst value:%d\r\n",(workq_arr[0])/(frq/1000000));
    len += sprintf(tmp+len,"max:%d\r\n",max);
    len += sprintf(tmp+len,"min:%d\r\n",min);
    len += sprintf(tmp+len,"avg:%d\r\n",avg);
    len += sprintf(tmp+len,"maxidx:%d\r\n",max_idx);
    len += sprintf(tmp+len,"minidx:%d\r\n",min_idx);
    stringout(tmp);
    len = 0;
#if 1 
    for (i = 10000,k=0; i < 10150 || k < 18; i += 10)
    {

        if (level[k] == 0)
        {
            k++;
            continue;
        }
        
        if (k == 0)
            len = sprintf(tmp,"\r\n< 10000 : %d\r\n",level[k++]);
        else if (k == 14)
        {
            len += sprintf(tmp+len,"10130~20000 : %d\r\n",level[k]);
        }
        else if (k == 15)
        {
            len += sprintf(tmp+len,"20000~20010 : %d\r\n",level[k]);
        }
        else if (k == 16)
        {
            len += sprintf(tmp+len,"20010~20020 : %d\r\n",level[k]);
        }
        else if (k == 17)
        {
            len += sprintf(tmp+len,">=20020 : %d\r\n",level[k]);
        }


        else
            len += sprintf(tmp+len,"%d~%d : %d\r\n",i,i+10,level[k++]);
    
        k++; 
    }
    stringout(tmp);
#endif    

}


/* generic timer */
u32 global_timer_get()
{

    u32 count_lo;
    u32 count_hi;
    
    __asm__ __volatile__ ("mrrc p15, 0, %0, %1, c14"        \
                            : "=r" (count_lo), "=r" (count_hi) : );

    return count_lo;
}

/* global timer for zed */

#define CTXA9_GLOBAL_TMR_BASE       (0xF8F00200)                                                                             
#define CTXA9_GLOBAL_TMR_CONFIG     (0xF8F00208)                                                                              
#define XLNX_ZYNQ7K_REGISTER_READ(reg) \                                                                                    
         *(volatile u32 *)(reg)                                                                                               
                                                                                                                                  
#define XLNX_ZYNQ7K_REGISTER_WRITE(reg, data)\                                                                              
            *(volatile u32 *)(reg) = (data)

/* read op */
XLNX_ZYNQ7K_REGISTER_READ(CTXA9_GLOBAL_TMR_BASE)
/* write op */
XLNX_ZYNQ7K_REGISTER_WRITE(CTXA9_GLOBAL_TMR_CONFIG,1)
