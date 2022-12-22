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
  const char *SHM_NAME = "TIMERTEST";
  const int SHM_SIZE_IN_BYTES = sizeof(struct timeval);
  int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

  // Sizing 4096 bytes -- more than enough.
  ftruncate(shm_fd, SHM_SIZE_IN_BYTES);

  // create a memory mapped file
  struct timeval* shared_memory_map_ptr = (struct timeval*) mmap(
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
    gettimeofday(shared_memory_map_ptr, NULL);

    printf(
      "Done getting time of day sec: %ld microsec: %d\n",
      shared_memory_map_ptr->tv_sec,
      shared_memory_map_ptr->tv_usec
    );

    char *command = argv[1];

    printf("Executing command %s\n", command);
    execlp(command, command, NULL);
  } else {
    wait(NULL); // Wait until child process is done
    printf("From shared memory: %ld %d --\n", shared_memory_map_ptr->tv_sec, shared_memory_map_ptr->tv_usec);

    struct timeval end_timestamp;
    gettimeofday(&end_timestamp, NULL);

    printf("Elapsed time %f ms\n", ((float) end_timestamp.tv_usec - shared_memory_map_ptr->tv_usec) / 1e3);

    shm_unlink(SHM_NAME);
  }

  return 0;
}
