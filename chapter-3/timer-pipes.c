#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_timeval.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv []) {
  struct timeval start_timestamp;

  int fd[2];

  // Create the pipe first
  if (pipe(fd) == -1) {
    fprintf(stderr, "Pipe failed");
    return 1;
  }

  pid_t pid = fork();

  // Child
  if (pid == 0) {
    close(fd[READ_END]); // Close unused READ_END from child's side

    char *command = argv[1];

    printf("Executing command %s\n", command);
    gettimeofday(&start_timestamp, NULL);


    write(fd[WRITE_END], &start_timestamp, sizeof(start_timestamp));

    char **cmd_params = malloc((argc) * sizeof(argv[0]));

    for (int i = 1; i < argc; i++) {
      cmd_params[i - 1] = malloc(strlen(argv[i]));
      strcpy(cmd_params[i - 1], argv[i]);
    }

    cmd_params[argc - 1] = NULL;

    printf("HOO %s %s\n", cmd_params[0], cmd_params[1]);

    // Try to run with `./timer-pipes sleep 2`
    execvp(command, cmd_params);
  } else if (pid > 0) {
    wait(NULL);
    close(fd[WRITE_END]); // Close unused WRITE_END from parent's side

    read(fd[READ_END], &start_timestamp, sizeof(start_timestamp));

    printf("Start timestamp %ld microsecs: %d\n", start_timestamp.tv_sec, start_timestamp.tv_usec);

    struct timeval end_timestamp;
    gettimeofday(&end_timestamp, NULL);

    printf(
      "Running time %ld %f\n",
      end_timestamp.tv_sec - start_timestamp.tv_sec,
      (end_timestamp.tv_usec - start_timestamp.tv_usec) / 1e3
    );
  } else { // Failed
    fprintf(stderr, "Fork failed :(");
  }

  return 0;
}
