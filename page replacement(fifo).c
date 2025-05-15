#include <stdio.h>

int main() {
    int frames, pages[50], n, frame[10], time[10];
    int i, j, k, avail, count = 0, current_time = 0, pos, lru_time;

    printf("Enter number of pages: ");
    scanf("%d", &n);

    printf("Enter the page reference string:\n");
    for(i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    printf("Enter number of frames: ");
    scanf("%d", &frames);

    for(i = 0; i < frames; i++) {
        frame[i] = -1;
        time[i] = -1;
    }

    printf("\nPage\tFrames\t\tPage Fault\n");

    for(i = 0; i < n; i++) {
        avail = 0;
        current_time++;

        for(j = 0; j < frames; j++) {
            if(frame[j] == pages[i]) {
                avail = 1;
                time[j] = current_time;
                break;
            }
        }

        if(avail == 0) {
            pos = 0;
            lru_time = time[0];

            for(j = 1; j < frames; j++) {
                if(time[j] < lru_time) {
                    lru_time = time[j];
                    pos = j;
                }
            }

            frame[pos] = pages[i];
            time[pos] = current_time;
            count++;

            printf("%d\t", pages[i]);
            for(k = 0; k < frames; k++) {
                if(frame[k] != -1)
                    printf("%d ", frame[k]);
                else
                    printf("- ");
            }
            printf("\tYes\n");
        } else {
            printf("%d\t", pages[i]);
            for(k = 0; k < frames; k++) {
                if(frame[k] != -1)
                    printf("%d ", frame[k]);
                else
                    printf("- ");
            }
            printf("\tNo\n");
        }
    }

    printf("\nTotal Page Faults = %d\n", count);
    return 0;
}
