<<<<<<< HEAD
// Chris Friedrich - CIS 315 -- Project 1

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "p1fxns.h"

#define BUFFER_SIZE 1024
#define DEFAULT_PROCS 25
#define LINE_LENGTH 64

typedef struct proc {
	pid_t pid;
	char* command_line;
} Proc;

int s_procs = -1;
int num_procs = 0;
int signals_received = 0;

int proc_index = 0;

Proc* procs = NULL;

static int add_proc(int pid, char* command_line)
{
	if (s_procs < 0)
	{
		procs = (Proc*)malloc(sizeof(Proc) * DEFAULT_PROCS);
		if (procs == NULL)
		{
			return -1;
		}
		s_procs = DEFAULT_PROCS;
	}
	else if (s_procs <= num_procs)
	{
		procs = realloc(procs, sizeof(Proc) * (DEFAULT_PROCS + s_procs));
		if (procs == NULL)
		{
			return -1;
		}
		s_procs += DEFAULT_PROCS;
	}

	procs[num_procs].pid = pid;
	procs[num_procs].command_line = p1strdup(command_line);

	num_procs++;

	return 1;
}

static void free_proc(Proc* procs)
{
	if(procs != NULL)
	{
		int i = 0;
		for(i = 0; i < num_procs; i++)
		{
			if(procs[i].command_line != NULL)
			{
				free(procs[i].command_line);
				procs[i].command_line = NULL;
			}
		}
		free(procs);
	}
}

static void free_args(char** args, int index)
{
	if(args != NULL)
	{
		int i = 0;
		for(i = 0; i < index; i++)
		{
			if(args[i] != NULL)
			{
				free(args[i]);
			}
		}
		free(args);
	}
}

static void signal_handler(int signo)
{
	if(signo == SIGUSR1)
	{
		signals_received++;
	}
}

int main(){
	
	int length = 0;
	char buffer[BUFFER_SIZE];
	
	if(signal(SIGUSR1, signal_handler) == SIG_ERR)
	{
		p1perror(1, "Unable to catch SIGUSR1.\n");
		return 1;
	}
	
	if(signal(SIGCONT, signal_handler) == SIG_ERR)
	{
		p1perror(1, "Unable to catch SIGCONT.\n");
		return 1;
	}
	
	while ((length = p1getline(0, buffer, BUFFER_SIZE)) > 0)
	{
		
		if(buffer[length - 1] == '\n')
		{
			buffer[length - 1] = '\0';
		}
		
		int index = 0;
		
		char** args;
		int arg_index = 0;
		int arg_count = 0;
		
		char temp[BUFFER_SIZE];
		char program_name[LINE_LENGTH];
		
		p1getword(buffer, 0, program_name);
		
		while((index = p1getword(buffer, index, temp)) != -1)
		{
			arg_count++;
		}
		
		args = (char**)calloc((arg_count + 1), sizeof(char*));
		
		if(args != NULL)
		{
			for(index = 0; index < arg_count; index++)
			{
				args[index] = (char*)calloc(LINE_LENGTH, sizeof(char));
				if(args[index] == NULL)
				{
					p1perror(1, "Memory allocation error.");
					free_args(args, (index + 1));
					exit(1);
				}
			}
		}
		else
		{
			p1perror(1, "Memory allocation error.");
			exit(1);
		}
		
		index = 0;
		
		while((index = p1getword(buffer, index, args[arg_index])) != -1)
		{
			arg_index++;
		}
		
		args[arg_count] = NULL;
		
		
		pid_t pid = fork();
		
		if(pid < 0)
		{
			p1perror(1, "Process error.  Unable to fork process.");
			free_args(args, arg_count);
			free_proc(procs);
			exit(1);
		}
		else if (pid > 0)
		{
			if (add_proc(pid, buffer) < 0)
			{
				free_args(args, arg_count);
				free_proc(procs);
				exit(1);
			}
		}
		else if(pid == 0)
		{
			while (!signals_received)
			{
				sleep(1);
			}

			if (execvp(program_name, args) < 0)
			{
				p1perror(1, "Unable to execute program.");
				free_args(args, arg_count);
				free_proc(procs);
				exit(1);
			}
		}
		free_args(args, arg_count);
	}
	
	int proc_status;
	int i;
	
	for(i = 0; i < num_procs; i++)
	{
		kill(procs[i].pid, SIGUSR1);
	}
	
	for(i = 0; i < num_procs; i++)
	{
		kill(procs[i].pid, SIGSTOP);
	}
	
	for(i = 0; i < num_procs; i++)
	{
		kill(procs[i].pid, SIGCONT);
	}
	
	for(i = 0; i < num_procs; i++)
	{
		wait(&procs[i].pid);
	}
	
	free_proc(procs);
	
	return 0;
	
}
=======
// Chris Friedrich - CIS 315 -- Project 1

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "p1fxns.h"

#define BUFFER_SIZE 1024
#define DEFAULT_PROCS 25
#define LINE_LENGTH 64

typedef struct proc {
	pid_t pid;
	char* command_line;
} Proc;

int s_procs = -1;
int num_procs = 0;
int signals_received = 0;

int proc_index = 0;

Proc* procs = NULL;

static int add_proc(int pid, char* command_line)
{
	if (s_procs < 0)
	{
		procs = (Proc*)malloc(sizeof(Proc) * DEFAULT_PROCS);
		if (procs == NULL)
		{
			return -1;
		}
		s_procs = DEFAULT_PROCS;
	}
	else if (s_procs <= num_procs)
	{
		procs = realloc(procs, sizeof(Proc) * (DEFAULT_PROCS + s_procs));
		if (procs == NULL)
		{
			return -1;
		}
		s_procs += DEFAULT_PROCS;
	}

	procs[num_procs].pid = pid;
	procs[num_procs].command_line = p1strdup(command_line);

	num_procs++;

	return 1;
}

static void free_proc(Proc* procs)
{
	if(procs != NULL)
	{
		int i = 0;
		for(i = 0; i < num_procs; i++)
		{
			if(procs[i].command_line != NULL)
			{
				free(procs[i].command_line);
				procs[i].command_line = NULL;
			}
		}
		free(procs);
	}
}

static void free_args(char** args, int index)
{
	if(args != NULL)
	{
		int i = 0;
		for(i = 0; i < index; i++)
		{
			if(args[i] != NULL)
			{
				free(args[i]);
			}
		}
		free(args);
	}
}

static void signal_handler(int signo)
{
	if(signo == SIGUSR1)
	{
		signals_received++;
	}
}

int main(){
	
	int length = 0;
	char buffer[BUFFER_SIZE];
	
	if(signal(SIGUSR1, signal_handler) == SIG_ERR)
	{
		p1perror(1, "Unable to catch SIGUSR1.\n");
		return 1;
	}
	
	if(signal(SIGCONT, signal_handler) == SIG_ERR)
	{
		p1perror(1, "Unable to catch SIGCONT.\n");
		return 1;
	}
	
	while ((length = p1getline(0, buffer, BUFFER_SIZE)) > 0)
	{
		
		if(buffer[length - 1] == '\n')
		{
			buffer[length - 1] = '\0';
		}
		
		int index = 0;
		
		char** args;
		int arg_index = 0;
		int arg_count = 0;
		
		char temp[BUFFER_SIZE];
		char program_name[LINE_LENGTH];
		
		p1getword(buffer, 0, program_name);
		
		while((index = p1getword(buffer, index, temp)) != -1)
		{
			arg_count++;
		}
		
		args = (char**)calloc((arg_count + 1), sizeof(char*));
		
		if(args != NULL)
		{
			for(index = 0; index < arg_count; index++)
			{
				args[index] = (char*)calloc(LINE_LENGTH, sizeof(char));
				if(args[index] == NULL)
				{
					p1perror(1, "Memory allocation error.");
					free_args(args, (index + 1));
					exit(1);
				}
			}
		}
		else
		{
			p1perror(1, "Memory allocation error.");
			exit(1);
		}
		
		index = 0;
		
		while((index = p1getword(buffer, index, args[arg_index])) != -1)
		{
			arg_index++;
		}
		
		args[arg_count] = NULL;
		
		
		pid_t pid = fork();
		
		if(pid < 0)
		{
			p1perror(1, "Process error.  Unable to fork process.");
			free_args(args, arg_count);
			free_proc(procs);
			exit(1);
		}
		else if (pid > 0)
		{
			if (add_proc(pid, buffer) < 0)
			{
				free_args(args, arg_count);
				free_proc(procs);
				exit(1);
			}
		}
		else if(pid == 0)
		{
			while (!signals_received)
			{
				sleep(1);
			}

			if (execvp(program_name, args) < 0)
			{
				p1perror(1, "Unable to execute program.");
				free_args(args, arg_count);
				free_proc(procs);
				exit(1);
			}
		}
		free_args(args, arg_count);
	}
	
	int proc_status;
	int i;
	
	for(i = 0; i < num_procs; i++)
	{
		kill(procs[i].pid, SIGUSR1);
	}
	
	for(i = 0; i < num_procs; i++)
	{
		kill(procs[i].pid, SIGSTOP);
	}
	
	for(i = 0; i < num_procs; i++)
	{
		kill(procs[i].pid, SIGCONT);
	}
	
	for(i = 0; i < num_procs; i++)
	{
		wait(&procs[i].pid);
	}
	
	free_proc(procs);
	
	return 0;
	
}
>>>>>>> 16d9416b787ea9e46375609d60f69f4c7b647d9c
