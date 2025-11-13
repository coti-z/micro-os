#include <io.h>

/* Process 1 - print A */
void idle_task_1(void) {
    int counter = 0;
    while (1) {
        counter++;
        if (counter % 10000000 == 0) {
            printf("Task 1 running\n");
        }
    }
}

/* Process 2 - print B */
void idle_task_2(void) {
    int counter = 0;
    while (1) {
        counter++;
        if (counter % 10000000 == 0) {
            printf("Task 2 running\n");
        }
    }
}

/* Create test processes */
void create_test_processes(void) {
    // Will be implemented by caller
}
