#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <sched.h>

static double diff_in_second(struct timespec t1, struct timespec t2)
{
	// I am a king.
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

char smallest_character(char str[],char fc)
{
    int n=strlen(str);
    int i;
    for(i=0; i<n; i++)
        if(str[i]>fc)return str[i];
    return str[0];
}

int main()
{
    FILE *file = fopen("q2_iterative.txt", "a");
    double cpu_time1;
    struct timespec start, end;
    srand(time(NULL));
    int random_len = (rand() % 10) + 1;
    char i;
    static const char alpha[] = "abcdefghijklmnopqrstuvwxyz";
    char str[random_len];
    struct sched_param param;
    int maxpri;
    maxpri = sched_get_priority_max(SCHED_FIFO);//max=99, min=1
    if(maxpri == -1) {
        perror("sched_get_priority_max() failed");
        exit(1);
    }
    param.sched_priority = maxpri;
    if (sched_setscheduler(getpid(), SCHED_FIFO, &param) == -1) {
        perror("sched_setscheduler() failed");
        exit(1);
    }
    memset(str, 0, sizeof(str));
    for(i=0; i<random_len; i++) str[i] = alpha[rand() % (strlen(alpha) - 1)];
    for(i=97; i<123; i++) { //test from a to z
        clock_gettime(CLOCK_REALTIME, &start);
        assert(smallest_character(str,i));
        clock_gettime(CLOCK_REALTIME, &end);
        printf("Output[%c]: %c\n",i, smallest_character(str,i));
        cpu_time1 = diff_in_second(start, end);
        fprintf(file, "%f\n", cpu_time1);
    }
    return 0;
}
