/**
 * ========================================================
 * A simple demonstration of shared memory on POSIX systems
 * ========================================================
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
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char const *argv[]) {
    int shared_memory_id; // ID of the shared memory
    int *sharedmem; // pointer to shared memory in process' local memory region
    int shared_memory_size = 10; // Size of our shared memory (in number of integers)

    // Creates (gets) a shared memory region
    shared_memory_id = shmget(IPC_PRIVATE, shared_memory_size * sizeof(int), IPC_CREAT | 0600);

    if (shared_memory_id == -1) {
        printf("Unable to create shared memory region!\n");
        exit(1);
    }

    printf("Shared memory ID: %d\n", shared_memory_id);

    // Attach shared memory region to process
    sharedmem = (int*)shmat(shared_memory_id, NULL, 0);

    if (sharedmem == (int*) -1) {
        printf("Unable to attach to shared memory with ID: %d\n", shared_memory_id);
    }

    // This is here to "hold" the program, so that the user can
    // open up a new process that would use the shared memory at his own pace.
    printf("Press enter to proceed...\n");
    getchar();

    // Do your read/write operations as per normal.
    // You can even do it in other processes!
    // Just attach the shared memory (using its ID) in the other program.
    int i;
    for (i = 0; i < shared_memory_size; i++) {
        printf("shmem[%d] = %d\n", i, sharedmem[i]);
    }

    printf("Destroying shared memory with ID: %d\n", shared_memory_id);

    // Detach shared memory region from process
    shmdt(sharedmem);
    // Destroys shared memory region
    shmctl(shared_memory_id, IPC_RMID, 0);

    return 0;
}
