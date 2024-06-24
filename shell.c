#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 1024
#define MAX_ARGS 64
char **parse_command(char *line) {
  int args_size = MAX_ARGS;
  int position = 0;
  // initial allocation for argumants
  char **args = malloc(sizeof(char *) * args_size);
  if (!args) {
    fprintf(stderr, "allocation error!");
    exit(EXIT_FAILURE);
  }

  // split input by space
  char *token = strtok(line, " ");
  // while there is a next token add it to args
  while (token != NULL) {
    args[position++] = token;

    // if input size is bigger than MAX_ARGS extend it
    if (position == args_size - 1) {
      args_size += MAX_ARGS;
      args = realloc(args, args_size);
      if (!args) {
        fprintf(stderr, "allocation error!");
        exit(EXIT_FAILURE);
      }
    }
    // get next token
    token = strtok(NULL, " ");
  }
  args[position] = NULL;
  return args;
}
char *read_input() {
  int buffer_size = MAX_LENGTH;
  int position = 0;
  // memory allocation for input
  char *line = malloc(sizeof(char) * buffer_size);
  // exit if allocation failed
  if (!line) {
    fprintf(stderr, "allocation error!");
    exit(EXIT_FAILURE);
  }
  char c;
  while (1) {

    c = getchar();
    // if input size is bigger than MAX_LENGTH extend it
    if (position == buffer_size - 1) {
      buffer_size += MAX_LENGTH;
      line = realloc(line, buffer_size);
      if (!line) {
        fprintf(stderr, "allocation error!");
        exit(EXIT_FAILURE);
      }
    }
    // check if input end or not
    if (c == '\n' || c == EOF) {
      line[position] = '\0';
      break;
    } else {
      line[position++] = c;
    }
  }
  return line;
}
int main() {
  char *line;
  char **args;
  while (1) {

    printf("V > ");
    line = read_input();
    args = parse_command(line);

    while (*args != NULL) {

      printf("%s\n", *args++);
    }
  }
  return 0;
}
