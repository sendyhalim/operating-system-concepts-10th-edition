#include <stdio.h>
#include <sys/_types/_off_t.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main() {
  char source_file_path[1000] = "";
  char destination_file_path[1000] = "";

  printf("Input a filepath that will be the source\n");
  fgets(source_file_path, 1000, stdin);

  // Fgets will include newline when user presses enter.
  *source_file_path = strtok(source_file_path, "\n")[0];

  printf("Ok now input the destination filepath, I'll copy from file %s to the given destination\n", source_file_path);
  fgets(destination_file_path, 1000, stdin);
  *destination_file_path = strtok(destination_file_path, "\n")[0];

  printf("Opening file %s", source_file_path);
  int source_file_descriptor = open(source_file_path, O_RDONLY);

  if (source_file_descriptor < 0) {
    int _errno = errno;
    fprintf(stderr, "Failed opening file %s: %s\n", source_file_path, strerror(_errno));
    return 1;
  }

  struct stat file_stat;
  fstat(source_file_descriptor, &file_stat);
  printf("file size %lli\n", file_stat.st_size);


  char *file_contents = malloc(file_stat.st_size + 1); // Add 1 more for '\0'

  read(source_file_descriptor, file_contents, file_stat.st_size);

  printf("the file_content %s", file_contents);

  int destination_file_descriptor = open(destination_file_path, O_WRONLY | O_CREAT, 0666);
  write(destination_file_descriptor, file_contents, file_stat.st_size);

  return 0;
}

