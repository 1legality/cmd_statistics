#include <stdio.h>
#include<sys/wait.h> 
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

void ExecuteCommand(char *command[]);
void PrintStatistic(double elapsedTime, double CPUTime, int CPUInvoluntaryInterrupt, 
    int CPUVoluntaryIntterupts, int pageFaults, int pageReclaims);

void ExecuteCommand(char *command[]) {
    pid_t pid = fork();

    if (pid == 0) {
        int size = sizeof(command) / sizeof(command[0]);

        printf("Exec");
        for (int i = 0; i <= size + 1; i++) {
            printf(" %s", command[i]);
        }
        printf("\n");

        execvp(command[0], command);
    }
    else {
        struct timeval startTime, stopTime;
        struct rusage buf;
        int status;
        double elapsedTime, CPUTime, CPUInvoluntaryInterrupts, CPUVoluntaryIntterupts, pageFaults, pageReclaims;

        gettimeofday(&startTime, NULL);

        waitpid(pid, &status, 0);

        getrusage(RUSAGE_SELF, &buf);
        gettimeofday(&stopTime, NULL);
        elapsedTime = (stopTime.tv_sec - startTime.tv_sec) * 1000.0;      // sec to ms
        elapsedTime += (stopTime.tv_usec - startTime.tv_usec) / 1000.0;   // us to ms

        CPUTime = (buf.ru_utime.tv_sec + buf.ru_utime.tv_usec) / 1000.0;
        CPUInvoluntaryInterrupts = buf.ru_nivcsw;
        CPUVoluntaryIntterupts = buf.ru_nvcsw;

        pageFaults = buf.ru_majflt;
        pageReclaims = buf.ru_minflt;

        PrintStatistic(elapsedTime, CPUTime, CPUInvoluntaryInterrupts, CPUVoluntaryIntterupts, pageFaults, pageReclaims);
    }
}

void PrintStatistic(double elapsedTime, double CPUTime, int CPUInvoluntaryInterrupt, 
    int CPUVoluntaryIntterupts, int pageFaults, int pageReclaims) {
    printf("Statistics : \n");
    printf("Time taken (wall-clock) : %f ms\n", elapsedTime); 
    printf("Time taken (CPU Time) : %f ms\n", CPUTime); 
    printf("CPU Involuntary intterupts : %d\n", CPUInvoluntaryInterrupt); 
    printf("CPU Voluntary intterupts: %d\n", CPUVoluntaryIntterupts); 
    printf("Page faults : %d\n", pageFaults); 
    printf("Page faults serviced (page reclaims) : %d\n", pageReclaims); 
}

int main(int argc, char *argv[]) {

    if (argc >=2){
        char *command[argc - 1];

        for  (int i = 1; i <= argc; i++) {
            command[i - 1] = argv[i];
        }

        ExecuteCommand(command);
    }

    return 0;
}