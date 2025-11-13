#include "process.h"
#include "memory.h"
#include "spinlock.h"
#include "scheduler.h"
#include "io.h"

/* Process table */
static process_t process_table[MAX_PROCESSES];
static int next_pid = 1;

/* Static stacks for processes (outside heap) */
static uint8_t process_stacks[MAX_PROCESSES][PROCESS_STACK_SIZE] __attribute__((aligned(16)));

/* Initialize process management */
void process_init(void) {
    printf("Initializing process management...\n");

    /* Mark all process slots as unused */
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].state = PROCESS_UNUSED;
        process_table[i].pid = 0;
        process_table[i].stack = NULL;
        process_table[i].entry_point = NULL;
        /* Zero out the context structure */
        memset(&process_table[i].context, 0, sizeof(context_t));
    }

    printf("Process table initialized (%d slots)\n", MAX_PROCESSES);
}

/* Create a new process */
process_t *process_create(const char *name, void (*entry)(void)) {
    if (!entry) {
        printf("Error: NULL entry point\n");
        return NULL;
    }

    /* Find an unused process slot */
    process_t *proc = NULL;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].state == PROCESS_UNUSED) {
            proc = &process_table[i];
            break;
        }
    }

    if (!proc) {
        printf("Error: Process table full\n");
        return NULL;
    }

    /* Use static stack (outside heap to avoid conflicts) */
    int proc_index = proc - process_table;
    proc->stack = process_stacks[proc_index];

    /* Initialize process control block */
    proc->pid = next_pid++;
    proc->state = PROCESS_READY;
    proc->entry_point = entry;

    /* Copy process name */
    int i;
    for (i = 0; name[i] && i < 31; i++) {
        proc->name[i] = name[i];
    }
    proc->name[i] = '\0';

    /* Set up initial context for first scheduling.
     * When scheduler calls swtch(), it will jump to forkret,
     * which calls entry_point.
     *
     * NOTE: context is already zeroed in process_init()
     */
    proc->context.rip = (uint64_t)forkret;
    proc->context.rsp = (uint64_t)proc->stack + PROCESS_STACK_SIZE;

    printf("Process created: pid=%d, name=%s, entry=0x%p, stack=0x%p\n",
           proc->pid, proc->name, entry, proc->stack);

    return proc;
}

/* Get process by PID */
process_t *process_get(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].state != PROCESS_UNUSED &&
            process_table[i].pid == pid) {
            return &process_table[i];
        }
    }
    return NULL;
}

/* Get process by index (for scheduler) */
process_t *process_get_by_index(int index) {
    if (index >= 0 && index < MAX_PROCESSES) {
        return &process_table[index];
    }
    return NULL;
}
