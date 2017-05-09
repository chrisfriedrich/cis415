<<<<<<< HEAD
// Chris Friedrich - CIS 315 -- Project 1

#include <stdio.h>
#include <stdlib.h>
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
int proc_index = 0;

Proc* procs = NULL;

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

static void free_arg(char** args, int index)
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


int main(){
	
	int length = 0;
	char buffer[BUFFER_SIZE];
	
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
					free_arg(args, (index + 1));
					exit(1);
				}
			}
		}
		else
		{
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
			free_arg(args, arg_count);
			free_proc(procs);
			exit(1);
		}
		else if(pid == 0)
		{
			if(execvp(program_name, args) < 0)
			{
				free_arg(args, arg_count);
				free_proc(procs);
				exit(1);
			}
		}
		
		free_arg(args, arg_count);
	}
	
	int proc_status;
	int i = 0;
	
	for(i = 0; i < num_procs; i++)
	{
		wait(&proc_status);
	}
	
	free_proc(procs);
	
	return 0;
	
}
=======
// Chris Friedrich - CIS 315 -- Project 1

#include <stdio.h>
#include <stdlib.h>
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
int proc_index = 0;

Proc* procs = NULL;

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

static void free_arg(char** args, int index)
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


int main(){
	
	int length = 0;
	char buffer[BUFFER_SIZE];
	
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
					free_arg(args, (index + 1));
					exit(1);
				}
			}
		}
		else
		{
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
			free_arg(args, arg_count);
			free_proc(procs);
			exit(1);
		}
		else if(pid == 0)
		{
			if(execvp(program_name, args) < 0)
			{
				free_arg(args, arg_count);
				free_proc(procs);
				exit(1);
			}
		}
		
		free_arg(args, arg_count);
	}
	
	int proc_status;
	int i = 0;
	
	for(i = 0; i < num_procs; i++)
	{
		wait(&proc_status);
	}
	
	free_proc(procs);
	
	return 0;
	
}
>>>>>>> 16d9416b787ea9e46375609d60f69f4c7b647d9c
