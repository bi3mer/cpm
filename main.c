#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 256

void list(const char *file_path) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    printf("No passwords stored...\n");
    exit(0);
  }

  char buffer[BUFFER_SIZE];
  char *token;

  while (fgets(buffer, BUFFER_SIZE, file)) {
    token = strtok(buffer, ",");
    printf("-- %s\n", token);
  }

  fclose(file);
}

void get(const char *file_path, const char *key) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    printf("%s not found\n", key);
    exit(1);
  }

  char buffer[BUFFER_SIZE];
  char *token;

  bool found = false;
  while (fgets(buffer, BUFFER_SIZE, file)) {
    token = strtok(buffer, ",");
    if (strcmp(token, key) == 0) {
      token = strtok(NULL, ",");
      token[strlen(token) - 1] = '\0';
      printf("-- %s\n", token);

      found = true;
      break;
    }
  }

  fclose(file);

  if (!found) {
    printf("%s not found\n", key);
  }
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    fprintf(stderr, "Please include an argument (--list, --add, --get)\n");
    exit(1);
  }

  const char *file_path = getenv("cmppath");
  printf("%s\n", file_path);
  if (!file_path) {
    fprintf(stderr, "Please set an enviornment variable `cmp_path` for where "
                    "you want passwords stored.\n");
    exit(1);
  }

  if (geteuid() != 0) {
    fprintf(stderr, "I need root privelleges to show you passwords! Don't "
                    "forget `sudo -E` to keep env variables.\n");
    exit(1);
  }

  const char *cmd = argv[1];
  if (strcmp(cmd, "--list") == 0) {
    list(file_path);
  } else if (strcmp(cmd, "--get") == 0) {
    get(file_path, argv[2]);
  } else {
    fprintf(stderr, "Unsupported argument: %s\n", cmd);
    return 1;
  }

  return 0;
}
