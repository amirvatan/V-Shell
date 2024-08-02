#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#define MAX_LENGTH 1024
#define MAX_ARGS 64
#define DELIM " \t\r\n\a"

char **parse_command(char *line);
char *read_input();
int com_cd(char **args);
int com_pwd(char **args);
int com_help(char **args);
int com_clear(char **args);
int com_exit(char **args);
int com_exec(char **args);

char *com_builtin[] = {

    "cd", "pwd", "help", "clear", "exit"};

int (*builtin_func[])(char **) = {

    &com_cd, &com_pwd, &com_help, &com_clear, &com_exit

};

int main() {
  char *line;
  char **args;
  int status;
  while (1) {

    printf("V > ");
    line = read_input();
    args = parse_command(line);
    status = com_exec(args);
  }
  return 0;
}
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
  char *token = strtok(line, DELIM);
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
    token = strtok(NULL, DELIM);
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
int com_cd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "no path specifide\n");
    return -1;
  }
  int status = chdir(args[1]);
  if (status != 0) {
    fprintf(stderr, "there is no direvtory name : ");
    fprintf(stderr, "%s\n", args[1]);
  }
  return status;
}
int com_pwd(char **args) {
  int buffer_size = MAX_LENGTH;
  char *buffer = alloca(sizeof(char) * buffer_size);

  getcwd(buffer, buffer_size);
  if (buffer == NULL) {
    fprintf(stderr, "can't reach destination");
    exit(EXIT_FAILURE);
  } else {
    fprintf(stdout, "%s\n", buffer);
  }
  return 0;
}
int com_help(char **args) {
  if (args[1] == NULL) {
    fprintf(stdout, "V-Shell written by AmirVatan in c\n");
    fprintf(stdout,
            "this help is going to be fully document V-Shell commands\n");
    fprintf(stdout, "you can try help --commands for list of commands\n");

  } else if (strcmp(args[1], "--commands") == 0) {

    fprintf(stdout, "list of commands : \n");
    fprintf(stdout, "cd : change directory\n");
    fprintf(stdout, "pwd : show current path\n");
    fprintf(stdout, "help : show help for V-Shell\n");
  }
  return 0;
}

int com_clear(char **args) {

  printf("\e[1;1H\e[2J");
  return 0;
}
int com_exit(char **args) { exit(EXIT_SUCCESS); }
int com_exec(char **args) {

  if (args[0] == NULL) {
    return 1;
  }
  for (int i = 0; i < sizeof(com_builtin) / sizeof(char *); i++) {
    if (strcmp(args[0], com_builtin[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }
  pid_t child;
  int status;
  child = fork();
  if (child == 0) {
    status = execvp(args[0], args);
    if(status == -1){
	fprintf(stderr, "command not found\n");
	exit(1);
    }
  } 
  else {
    waitpid(child,&status,0);
  }
  return status;
}
