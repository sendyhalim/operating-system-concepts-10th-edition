#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  int i;

  pid_t pid;

  pid = fork();

  if (pid < 0) {
    printf("FAILED TO FORK");
  } else if (pid == 0) {
    printf("CHILD");
  } else {
    sleep(20);
    printf("PARENT");
  }

  return 0;
}
