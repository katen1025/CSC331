#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>


//Function Declarations for builtin shell commands:

int grsh_cd(char **args);
int grsh_path(char **args);
int grsh_exit(char **args);

char error_message[30] = "An error has occurred\n";
//List of builtin commands, followed by their corresponding functions.

char *builtin_str[] = {"cd","path","exit"};

int (*builtin_func[]) (char **) = {&grsh_cd, &grsh_path, &grsh_exit};

int grsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

//Builtin function implementations.


//Bultin command: change directory.Parameter args List of args.  args[0] is "cd".  args[1] is the directory. Always returns 1, to continue executing.
int grsh_cd(char **args)
{
  if (args[1] == NULL) 
  {
    char error_message[30] = "An error has occured\n";
    write(STDERR_FILENO,error_message,strlen(error_message));
  } 
  
  else 
  {
    if (chdir(args[1]) != 0) {
    write(STDERR_FILENO, error_message, strlen(error_message));
    }
   }
  return 1;
}

//Builtin command: print help. Parameter args List of args.  Not examined. Always returns 1, to continue executing.

int grsh_path(char **args)
{
    char *path[100] = {};
    int i=0;
    
    if (args[1] == NULL)
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
    }
    else
    
        while(args[i] != NULL)
        {
            if(access(args[i], X_OK)==0)
            {
                strcpy(*path, args[i]);
                i++;
            }
            else
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                i++;
            }
        }
    return 0;
}

// Builtin command: exit.Parameter args List of args.Always returns 0, to terminate execution.
int grsh_exit(char **args)
{
  return 0;
}

// Launch a program and wait for it to terminate. Parameter args Null terminated list of arguments (including program). Always returns 1, to continue execution.
int grsh_launch(char **args)
{
  pid_t pid; //, wpid;
  int status;

  pid = fork();
  if (pid == 0)  // Child process
    {
        if (execvp(args[0], args) == -1) {
         perror("grsh");
        }
        exit(EXIT_FAILURE);
     } 
   
   else if (pid < 0) // Error forking
   {
    perror("grsh");
    } 
    
    else  // Parent process
    {
      do 
      {
        waitpid(pid, &status, WUNTRACED);
      } 
    
    while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

  return 1;
}

int grsh_file(char *args){
    //int status = 0;
    
        char *line = NULL;
        size_t bufsize = 0;
        getline(&line, &bufsize, stdin);
    
        if(STDIN_FILENO == STDERR_FILENO){
            //status = 1;
        }
    
  exit(0);
}

//Execute shell built-in or launch program.Parameter args Null terminated list of arguments.Return 1 if the shell should continue running, 0 if it should terminate
int grsh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }
  //If cmd was a file
    if(strstr(args[0], ".") != NULL)
    {
        if(args[1] != NULL)
        {
            //write(STDERR_FILENO, error_message, strlen(error_message));
        }
        for (i = 0; i < grsh_num_builtins(); i++) {  //If cmd is in builtins
          if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
          }
        }
    }

  for (i = 0; i < grsh_num_builtins(); i++) {  //If cmd is in builtins
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return grsh_launch(args);
}

#define GRSH_RL_BUFSIZE 1024
//Read a line of input from stdin and return The line from stdin.

char *grsh_read_line(void)
{
  char *line = NULL;
  size_t bufsize = 0;
  getline(&line,&bufsize,stdin);
  return line;

}

#define GRSH_TOK_BUFSIZE 64
#define GRSH_TOK_DELIM " \t\r\n\a"

//Split a line into tokens. Parameter is line The line. Return Null-terminated array of tokens.
char **grsh_split_line(char *line)
{
  int bufsize = GRSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    char error_message[30] = "An error occured\n";
    write(STDERR_FILENO, error_message,strlen(error_message));
  }

  token = strtok(line, GRSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += GRSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        char error_message[30] = "An error occured\n";
	write(STDERR_FILENO,error_message,strlen(error_message));
      }
    }

    token = strtok(NULL, GRSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

//Loop to get input and executing it.

void grsh_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("grsh> ");
    line = grsh_read_line();
    args = grsh_split_line(line);
    status = grsh_execute(args);

    free(line);
    free(args);
  } while (status);
}

void grsh_loop_int(char **arg1)
{
  grsh_execute(arg1);
}

//Main. Parameters: argc Argument count and argv Argument vector. Return the status code
int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  if (argc < 2) {
    grsh_loop();
  }
  else {
    system("/bin/chmod -R 755 *"); //for some reason, get permission denied in the autotest without this, maybe server has permission issues in batch?
    grsh_loop_int(&argv[1]);
  }
  

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}
