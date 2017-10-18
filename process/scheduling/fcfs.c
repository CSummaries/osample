/**
 * ========================================
 * First Come First Serve (FCFS) simulation
 * ========================================
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct p_record {
    int pid; // Process ID
    int arrival; // Process arrival time
    int waiting; // Process waiting time
    int duration; // Process duration
} p_record;

void print_p_stats(p_record *process) {
    printf("=========================\n");
    printf("  Stats for Process #%d  \n", process->pid);
    printf("-------------------------\n");
    printf("| Arrival time: %d\n", process->arrival);
    printf("| Waiting time: %d\n", process->waiting);
    printf("| Execution time: %d\n", process->duration);
    printf("=========================\n");
}

/**
 * Comparator to compare between two struct p_record elements
 * @param  a first p_record element
 * @param  b second p_record element
 * @return   same as strcmp
 */
int compare_p_records(const void *a, const void *b) {
  struct p_record *pa = (struct p_record *) a;
  struct p_record *pb = (struct p_record *) b;

  return (pa->arrival > pb->arrival) - (pa->arrival < pb->arrival);
}

int main(int argc, char const *argv[]) {
    int process_count = 0;

    printf("Number of processes to run: ");
    scanf("%d", &process_count);

    if (process_count <= 0) {
        return 0;
    }

    struct p_record processes[process_count];

    int i;
    for (i = 0; i < process_count; i++) {
        processes[i].pid = i;
        printf("Arrival time for process #%d: ", i);
        scanf("%d", &(processes[i].arrival));
        printf("Execution time for process #%d: ", i);
        scanf("%d", &(processes[i].duration));
    }

    // Sort our processes by arrival time
    qsort(&processes, process_count, sizeof(p_record), compare_p_records);

    // Fast forward time to first process
    int cur_time = processes[0].arrival;

    for (i = 0; i < process_count; i++) {
        if (processes[i].arrival > cur_time) {
            // Fast forward time
            cur_time = processes[i].arrival;
        }
        processes[i].waiting = cur_time - processes[i].arrival;
        cur_time += processes[i].duration;
    }

    int total_waiting = 0;
    for (i = 0; i < process_count; i++) {
        total_waiting += processes[i].waiting;
        print_p_stats(&processes[i]);
    }

    printf("==================================\n");
    printf("          Global Statistics       \n");
    printf("----------------------------------\n");
    printf("| Average waiting time: %f        \n", (double)total_waiting/process_count);
    printf("==================================\n");

    return 0;
}
