#include <stdio.h>

struct Process {
    int id, burst_time, arrival_time, queue;
    int waiting_time, turnaround_time, response_time;
};

void round_robin(struct Process p[], int n, int quantum) {
    int remaining_time[n], completed = 0, time = 0;
    for (int i = 0; i < n; i++) remaining_time[i] = p[i].burst_time;

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0) {
                if (remaining_time[i] > quantum) {
                    time += quantum;
                    remaining_time[i] -= quantum;
                } else {
                    time += remaining_time[i];
                    p[i].waiting_time = time - p[i].arrival_time - p[i].burst_time;
                    p[i].turnaround_time = time - p[i].arrival_time;
                    p[i].response_time = p[i].waiting_time;
                    remaining_time[i] = 0;
                    completed++;
                }
            }
        }
    }
}

void fcfs(struct Process p[], int n, int start_time) {
    int time = start_time;
    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival_time)
            time = p[i].arrival_time;

        p[i].waiting_time = time - p[i].arrival_time;
        p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
        p[i].response_time = p[i].waiting_time;
        time += p[i].burst_time;
    }
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process processes[n], system_queue[n], user_queue[n];
    int sys_count = 0, user_count = 0;

    printf("Enter Burst Time, Arrival Time and Queue of each process: \n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i + 1);
        scanf("%d %d %d", &processes[i].burst_time, &processes[i].arrival_time, &processes[i].queue);
        processes[i].id = i + 1;

        if (processes[i].queue == 1)
            system_queue[sys_count++] = processes[i];
        else if (processes[i].queue == 2)
            user_queue[user_count++] = processes[i];
    }

    int quantum = 2;
    round_robin(system_queue, sys_count, quantum);
    int last_exec_time = (sys_count > 0) ? system_queue[sys_count - 1].turnaround_time : 0;
    fcfs(user_queue, user_count, last_exec_time);

    printf("\nProcess\tWaiting Time\tTurn Around Time\tResponse Time\n");
    for (int i = 0; i < sys_count; i++)
        printf("P%d\t%d\t%d\t%d\n", system_queue[i].id, system_queue[i].waiting_time, system_queue[i].turnaround_time, system_queue[i].response_time);

    for (int i = 0; i < user_count; i++)
        printf("P%d\t%d\t%d\t%d\n", user_queue[i].id, user_queue[i].waiting_time, user_queue[i].turnaround_time, user_queue[i].response_time);

    float avg_wait = 0, avg_tat = 0, avg_resp = 0;
    for (int i = 0; i < sys_count; i++) {
        avg_wait += system_queue[i].waiting_time;
        avg_tat += system_queue[i].turnaround_time;
        avg_resp += system_queue[i].response_time;
    }
    for (int i = 0; i < user_count; i++) {
        avg_wait += user_queue[i].waiting_time;
        avg_tat += user_queue[i].turnaround_time;
        avg_resp += user_queue[i].response_time;
    }

    int total = sys_count + user_count;
    printf("\nAverage Waiting Time: %.2f", avg_wait / total);
    printf("\nAverage Turn Around Time: %.2f", avg_tat / total);
    printf("\nAverage Response Time: %.2f", avg_resp / total);
    printf("\nThroughput: %.2f\n", (float)total / avg_tat * total);

    return 0;
}
