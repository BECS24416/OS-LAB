#include <stdio.h>
#include <stdbool.h>

int main() {
    int n, m;
    printf("Enter number of processes and number of resources:\n");
    scanf("%d %d", &n, &m);

    int alloc[n][m], request[n][m], avail[m];

    printf("Enter Allocation Matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);

    printf("Enter Request Matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &request[i][j]);

    printf("Enter Available Resources (%d values):\n", m);
    for (int i = 0; i < m; i++)
        scanf("%d", &avail[i]);

    int work[m];
    for (int i = 0; i < m; i++)
        work[i] = avail[i];

    bool finish[n];
    for (int i = 0; i < n; i++) {
        bool hasAllocation = false;
        for (int j = 0; j < m; j++) {
            if (alloc[i][j] != 0) {
                hasAllocation = true;
                break;
            }
        }
        finish[i] = hasAllocation ? false : true;
    }

    while (true) {
        bool progress = false;

        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool canGrant = true;

                for (int j = 0; j < m; j++) {
                    if (request[i][j] > work[j]) {
                        canGrant = false;
                        break;
                    }
                }

                if (canGrant) {
                    for (int j = 0; j < m; j++)
                        work[j] += alloc[i][j];
                    finish[i] = true;
                    progress = true;
                }
            }
        }

        if (!progress)
            break;
    }

    printf("\nDeadlock Detection Result:\n");
    bool deadlock = false;

    for (int i = 0; i < n; i++) {
        if (!finish[i]) {
            printf("Process P%d is DEADLOCKED\n", i);
            deadlock = true;
        } else {
            printf("Process P%d is NOT deadlocked\n", i);
        }
    }

    if (!deadlock)
        printf("\nNo deadlock detected in the system.\n");

    return 0;
}
