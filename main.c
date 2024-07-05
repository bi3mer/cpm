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
      printf("pass: %s\n", token);

      found = true;
      break;
    }
  }

  fclose(file);

  if (!found) {
    printf("%s not found\n", key);
  }
}

void add(const char *file_path, const char *key, const char *pass) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    file = fopen(file_path, "w");
    fprintf(file, "%s,%s\n", key, pass);
    fclose(file);
    printf("Stored.\n");
    exit(1);
  }

  FILE *temp = fopen("temp", "w");
  fprintf(temp, "%s,%s\n", key, pass); // store new pass

  char buffer[BUFFER_SIZE];
  char buffer2[BUFFER_SIZE];
  char *token;

  bool found = false;
  while (fgets(buffer, BUFFER_SIZE, file)) {
    // only write lines that don't use the same key, so this is basically
    // supporting overwritting
    strcpy(buffer2, buffer);
    const char *token = strtok(buffer, ",");
    if (strcmp(token, key) != 0) {
      fprintf(temp, "%s", buffer2);
    }
  }

  fclose(temp);
  fclose(file);

  remove(file_path);
  rename("temp", file_path);

  printf("Stored.\n");
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("Please include an argument:\n");
    printf("  --list\n");
    printf("  --get {key}\n");
    printf("  --add {key} {pass}\n");
    exit(1);
  }

  const char *file_path = getenv("cmppath");
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
  } else if (argc == 3 && strcmp(cmd, "--get") == 0) {
    get(file_path, argv[2]);
  } else if (argc == 4 && strcmp(cmd, "--add") == 0) {
    add(file_path, argv[2], argv[3]);
  } else {
    fprintf(stderr, "Unsupported command: %s\n", cmd);
    return 1;
  }

  return 0;
}
