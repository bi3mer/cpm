#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define FILE_PATH "pass.csv"
#define BUFFER_SIZE 256

void list() {
  FILE *file = fopen(FILE_PATH, "r");
  if (file == NULL) {
    printf("No passwords stored...\n");
    return;
  }

  char buffer[BUFFER_SIZE];
  char *token;

  while (fgets(buffer, BUFFER_SIZE, file)) {
    token = strtok(buffer, ",");
    printf("-- %s\n", token);
  }

  fclose(file);
}

void get(const char *key) {
  FILE *file = fopen(FILE_PATH, "r");
  if (file == NULL) {
    printf("%s not found\n", key);
    return;
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
    printf("Please include an argument (--list, --add, --get)\n");
    return 1;
  }

  const char *cmd = argv[1];
  if (strcmp(cmd, "--list") == 0) {
    list();
  } else if (strcmp(cmd, "--get") == 0) {
    get(argv[2]);
  } else {
    printf("Unsupported argument: %s\n", cmd);
    return 1;
  }

  return 0;
}
