#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  pid_t pid = fork();

  // Child process will always get pid of 0 from fork
  // because it's a marker.
  printf("There should be 2 outputs of this log. PID: %d\n", pid);

  if (pid < 0) {
    fprintf(stderr, "Fork failed bro.. %d", pid);
  } else if (pid == 0) {
    execlp("/bin/ls", "ls", NULL);
  } else {
    wait(NULL);
    printf("child complete");
  }
}
