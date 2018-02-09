#include <vxWorks.h>
typedef struct
{
    int start;
    int end;
    long long cnt;
}cnt_stat_t;

typedef struct
{


    int         max;
    int         min;
    int         cur;
    long long         total;

    int         max_real;
    int         min_real;
    int         cur_real;
    long long         total_real;

    cnt_stat_t  *cnt_stats;



    long long         cnt;
}stat_info_t;

int         priority;
int         ticks;
int         us_per_tick;
long long        cycles;
int         threads;
int         cnt_stat_num;
int frq;
stat_info_t *stat_info = NULL;
#if 0
#define LOG(fmt, args...)   \
    printf(fmt, ##args)

#define LOGT LOG

#else
#define LOG(fmt, args...)
#define LOGT(fmt, args...)   \
    printf(fmt, ##args)

#endif


#define GLOBAL_TMER
#ifdef GLOBAL_TMER
UINT32 global_timer_counter_get()
{
#if 0
        return (XLNX_ZYNQ7K_REGISTER_READ (CTXA9_GLOBAL_TMR_BASE));
#else
        unsigned long lo,hi;
        unsigned long long high,low;

#if 0
__asm__ __volatile__ ("mrrc p15, 0, %0, %1, c14"        \
                                    : "=r" (lo), "=r" (hi) : );
#else
            __asm__ volatile (
                      ".word 0xec510f1e \n" /* mrrc p15, 1, r0, r1, c14 */
                      "mov %0, r0 \n"
                      "mov %1, r1 \n"
                      : "=&r" (lo), "=&r" (hi)
                      );
#endif

        high = (unsigned long long)hi;
        low = (unsigned long long)lo;
        return low; /*(low|high<<32);*/

#endif
}


#define _M_TIMESTAMP()      (global_timer_counter_get(),global_timer_counter_get())
#else
#define _M_TIMESTAMP()      (sysTimestamp(),sysTimestamp())
#endif

static int thread_pause_test(void *arg)
{
    int diff = 0;
    int next;
    int next1;
    int next2;
    int idx = (int)arg;
    int i;
    int key;

    taskDelay(ticks);
    while(TRUE)
    {
        taskLock();
#if 1

        next = _M_TIMESTAMP();
        next1 = _M_TIMESTAMP();
        taskDelay(ticks);
        next2 = _M_TIMESTAMP();
        taskUnlock();
#else
        next = time_getus() + (ticks * us_per_tick);
        taskDelay(ticks);
        diff = time_getus() - next;
        taskUnlock();

#endif
        diff = (next2 - next1) - (next1 - next);

        if(diff <= 0)
            continue;

        /* »»Ëã³Éus */

        diff = (diff)/(frq/1000000) - (ticks * us_per_tick);


        LOG("thread #%d diff:%d\n", idx, diff);
        if(diff <= 0)
        {
            stat_info[idx].cur = 1;
        }
        else
        {
            stat_info[idx].cur = diff;
        }
        LOG("thread #%d cur:%d\n", idx, stat_info[idx].cur);

        if(stat_info[idx].cur > (stat_info[idx].min * 100))
        {
            LOGT("thread #%d bad cur:%d\n", idx, stat_info[idx].cur);
            break;
        }

        if(stat_info[idx].cur > stat_info[idx].max)
        {
            stat_info[idx].max = stat_info[idx].cur;
        }
        LOG("thread #%d max:%d\n", idx, stat_info[idx].max);


        if(stat_info[idx].cur < stat_info[idx].min)
        {
            stat_info[idx].min = stat_info[idx].cur;
        }
        LOG("thread #%d min:%d\n", idx, stat_info[idx].min);

        stat_info[idx].total += stat_info[idx].cur;
        LOG("thread #%d total:%d\n", idx, stat_info[idx].total);

        stat_info[idx].cnt++;
        LOG("thread #%d cnt:%d\n", idx, stat_info[idx].cnt);

        stat_info[idx].cur_real = diff + (ticks * us_per_tick);
        for(i = 0; i < cnt_stat_num; i++)
        {
            if((stat_info[idx].cur_real >= stat_info[idx].cnt_stats[i].start) &&
               (stat_info[idx].cur_real <= stat_info[idx].cnt_stats[i].end))
            {
                stat_info[idx].cnt_stats[i].cnt++;
            }
        }

        LOG("thread #%d cur_real:%d\n", idx, stat_info[idx].cur_real);
        if(stat_info[idx].cur_real > stat_info[idx].max_real)
        {
            stat_info[idx].max_real = stat_info[idx].cur_real;
        }
        LOG("thread #%d max_real:%d\n", idx, stat_info[idx].max_real);
        if(stat_info[idx].cur_real < stat_info[idx].min_real)
        {
            stat_info[idx].min_real = stat_info[idx].cur_real;
        }
        LOG("thread #%d min_real:%d\n", idx, stat_info[idx].min_real);
        stat_info[idx].total_real += stat_info[idx].cur_real;
        LOG("thread #%d total_real:%d\n", idx, stat_info[idx].total_real);

        if(stat_info[idx].cnt == cycles)
        {
            break;
        }
    }

    return 0;
}

static thread_pause_test_print(void *param)
{
    int i;
    int j;
    int ischecked = 0;
    int avg;
    int avg_real;

    while(TRUE)
    {
        taskDelay(sysClkRateGet());
        if(ischecked == 0)
        {
            for(i = 0; i < threads; i++)
            {
                if(stat_info[i].cnt == 0)
                {
                    break;
                }
            }
            if(i < threads)
            {
                continue;
            }

            ischecked = 1;
        }

        printf("#---------------------------\n");
        printf("# cyclictest current status:\n");
        for(i = 0; i < threads; i++)
        {
            printf("Thread:%2d Pri:%3d Interval:%7d ",
                    i, priority, (int)ticks * us_per_tick);
            printf("Min:%4d(%7d) ", stat_info[i].min, stat_info[i].min_real);
            printf("Act:%4d(%7d) ", stat_info[i].cur, stat_info[i].cur_real);
            if(stat_info[i].cnt != 0)
            {
                avg         = (int)(stat_info[i].total/stat_info[i].cnt);
                avg_real    = (int)(stat_info[i].total_real/stat_info[i].cnt);
            }
            else
            {
                avg = avg_real = 0;
            }
            printf("Avg:%4d(%7d) ", avg, avg_real);
            printf("Max:%4d(%7d) ", stat_info[i].max, stat_info[i].max_real);
            printf("Cycles:%lu(", stat_info[i].cnt);
            for(j = 0; j < cnt_stat_num; j++)
            {
                if(stat_info[i].cnt_stats[j].cnt == 0)
                {
                    continue;
                }

                printf(" %d~%d:%lu", stat_info[i].cnt_stats[j].start,
                       stat_info[i].cnt_stats[j].end, stat_info[i].cnt_stats[j].cnt);
            }
            printf(")\n");
        }
        printf("#---------------------------\n");

        for(i = 0; i < threads; i++)
        {
            if(stat_info[i].cnt >= cycles)
            {
                return 0;
            }
        }
    }
}
struct timeval {
        time_t      tv_sec;
        long long   tv_usec;
};


void user_entry1(void)
{
    int i;
    int j;
    priority    = 40;
    ticks       = 1;
    sysClkRateSet(100);
    us_per_tick = 1000000 / sysClkRateGet();
    cycles      = 1000000000;
    threads     = 4;
    cnt_stat_num= 64;
    stat_info   = (stat_info_t *)calloc(threads, sizeof(stat_info_t));
    frq = sysTimestampFreq();
    taskSpawn(NULL,250,0,0x1000,(FUNCPTR)thread_pause_test_print,0,0,0,0,0,0,0,0,0,0);

    for(i = 0; i < threads; i++)
    {
        stat_info[i].min = stat_info[i].min_real = 100000000;
        stat_info[i].max = 1;
        stat_info[i].cnt_stats = (cnt_stat_t *)calloc(cnt_stat_num, sizeof(cnt_stat_t));
        stat_info[i].cnt_stats[0].start = 0;
        stat_info[i].cnt_stats[0].end   = 3999;
        printf("range #0: 0 ~ 3999\n");

#if 1
        for(j = 1; j < cnt_stat_num; j++)
        {
            stat_info[i].cnt_stats[j].start = stat_info[i].cnt_stats[j - 1].end + 1;
            if((stat_info[i].cnt_stats[j - 1].end >= (us_per_tick - 100 - 1)) &&
                    (stat_info[i].cnt_stats[j - 1].end < (us_per_tick + 100 - 1)))
            {
                stat_info[i].cnt_stats[j].end = stat_info[i].cnt_stats[j - 1].end + 10;
            }
            else if((stat_info[i].cnt_stats[j - 1].end >= (us_per_tick - 1000 - 1)) &&
                    (stat_info[i].cnt_stats[j - 1].end < (us_per_tick + 1000 - 1)))
            {
                stat_info[i].cnt_stats[j].end = stat_info[i].cnt_stats[j - 1].end + 50;
            }
            else
            {
                stat_info[i].cnt_stats[j].end = stat_info[i].cnt_stats[j - 1].end + 1000;
            }
            if(i == 0)
            {
                printf("range #%d: %d ~ %d\n", j, stat_info[i].cnt_stats[j].start, stat_info[i].cnt_stats[j].end);
            }
        }
#else
        for(j = 1; j < cnt_stat_num; j++)
        {
            stat_info[i].cnt_stats[j].start = stat_info[i].cnt_stats[j - 1].end + 1;
            if(stat_info[i].cnt_stats[j - 1].end < 9000)
            {
                stat_info[i].cnt_stats[j].end = stat_info[i].cnt_stats[j - 1].end + 1000;
            }
            if((stat_info[i].cnt_stats[j - 1].end >= (us_per_tick - 100 - 1)) &&
                    (stat_info[i].cnt_stats[j - 1].end < (us_per_tick + 100 - 1)))
            {
                stat_info[i].cnt_stats[j].end = stat_info[i].cnt_stats[j - 1].end + 25;
            }
            else if((stat_info[i].cnt_stats[j - 1].end >= (us_per_tick - 1000 - 1)) &&
                    (stat_info[i].cnt_stats[j - 1].end < (us_per_tick + 1000 - 1)))
            {
                stat_info[i].cnt_stats[j].end = stat_info[i].cnt_stats[j - 1].end + 50;
            }
            else
            {
                stat_info[i].cnt_stats[j].end = stat_info[i].cnt_stats[j - 1].end + 1000;
            }
            printf("range #%d: %d ~ %d\n", j, stat_info[i].cnt_stats[j].start, stat_info[i].cnt_stats[j].end);
        }
#endif

        taskSpawn(NULL,priority,0,0x1000,(FUNCPTR)thread_pause_test,(void *)i,0,0,0,0,0,0,0,0,0);
    }

}


