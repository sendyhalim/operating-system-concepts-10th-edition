#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_timeval.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char *argv []) {
  const int SHM_SIZE_IN_BYTES = 4096;
  const char *SHM_NAME = "TIMERTEST";
  struct timeval start_timestamp;
  int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

  // Sizing 4096 bytes -- more than enough.
  ftruncate(shm_fd, SHM_SIZE_IN_BYTES);

  // create a memory mapped file
  char *shared_memory_map_ptr = (char*) mmap(
    0, // Starting address
    SHM_SIZE_IN_BYTES,
    PROT_READ | PROT_WRITE, // Memory protection
    MAP_SHARED,
    shm_fd,
    0
  );

  pid_t pid = fork();

  if (pid < 0) {
    fprintf(stderr, "Fork failed bro.. %d\n", pid);
  } else if (pid == 0) {
    gettimeofday(&start_timestamp, NULL);

    sprintf(shared_memory_map_ptr, "%d", start_timestamp.tv_usec);

    char *command = argv[1];

    printf("Executing command %s\n", command);
    execlp(command, command, NULL);
  } else {
    wait(NULL); // Wait until child process is done
    printf("From shared memory: %s --\n", (char *) shared_memory_map_ptr);

    int start_micro_seconds = atoi((char *) shared_memory_map_ptr);

    printf("Waiting, start time: %d \n", start_micro_seconds);

    struct timeval end_timestamp;
    gettimeofday(&end_timestamp, NULL);

    printf("Elapsed time %f ms\n", ((float) end_timestamp.tv_usec - start_micro_seconds) / 1e3);

    shm_unlink(SHM_NAME);
  }

  return 0;
}
