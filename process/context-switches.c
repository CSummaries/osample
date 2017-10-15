/**
 * ==============================================================
 * Simple code that examines the total number of context switches
 * ==============================================================
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
#include <sys/time.h>
#include <sys/resource.h> // struct rusage
#include <sys/wait.h> // waitpid
#include <unistd.h> // fork

/**
 * Method that simulates work.
 * This gives our process enough "time" to warrant a context switch.
 */
void do_work() {
    int* arr1 = (int*)malloc(64 * sizeof(int));
    int* arr2 = (int*)malloc(64 * sizeof(int));

    int i;
    for (i = 0; i < 10000; i++) {
        int j;
        for (j = 0; j < 10000; j++) {
            arr1[i%64] += arr2[j%64];
        }
    }

    free(arr1);
    free(arr2);
}

int main(int argc, char const *argv[]) {
    struct rusage *usage = (struct rusage*)malloc(sizeof(struct rusage));

    /**
     * Forking ensures that there's at least another process
     * that is contending for CPU resource usage.
     */
    int cid = fork();

    do_work();

    if (getrusage(RUSAGE_SELF, usage)) {
        printf("Unable to get resource usage stats!\n");
        return -1;
    }

    if (cid == 0) {
        printf("[CHILD] Voluntary context switches: %ld\n", usage->ru_nvcsw);
        printf("[CHILD] Involuntary context switches: %ld\n", usage->ru_nivcsw);
    } else {
        // Wait on child first
        waitpid(cid, NULL, 0);
        printf("[PARENT] Voluntary context switches: %ld\n", usage->ru_nvcsw);
        printf("[PARENT] Involuntary context switches: %ld\n", usage->ru_nivcsw);
    }

    return 0;
}
