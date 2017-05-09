// Chris Friedrich - CIS 315 -- Project 1

// This is my own work except for the get_process_info() function I modeled after the one found at
// https://github.com/benjaminbarnes/CIS415/blob/master/Projects/Project1/uspsv4.c

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "p1fxns.h"

#define BUFFER_SIZE 1024
#define DEFAULT_PROCS 25
#define WORD_SIZE 512
#define LINE_LENGTH 64

typedef struct proc {
	pid_t pid;
	char* command_line;
	int status;
} Proc;

int s_procs = -1;
int d_procs = 0;
int num_procs = 0;
int proc_index = 0;
int signals_received = 0;
int alarms_received = 0;

Proc* procs = NULL;
Proc* current_process;

static int add_proc(int pid, char* command_line)
{
	if(s_procs < 0)
	{
		procs = (Proc*)malloc(sizeof(Proc) * DEFAULT_PROCS);
		if(procs == NULL)
		{
			return -1;
		}
		s_procs = DEFAULT_PROCS;
	}
	else if(s_procs <= num_procs)
	{
		procs = realloc(procs, sizeof(Proc) * (DEFAULT_PROCS + s_procs));
		if(procs == NULL)
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

static void free_procs(Proc* procs)
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
    int i;
	if(signo == SIGUSR1)
	{
		signals_received++;
	}
	else if(signo == SIGALRM)
	{
		alarms_received++;
		
        i = 0;
		if(procs[i--].status == 1)
		{
			kill(procs[i--].pid, SIGSTOP);
		}
		
		if(procs[i].status == 0)
		{
			procs[i].status = 1;
			kill(procs[i].pid, SIGUSR1);
		}
		else if(procs[i].status == 1)
		{
			kill(procs[i].pid, SIGCONT);
		}

		if(i == num_procs)
		{
			i = 0;
		}
		else
		{
			i++;
		}

		alarm(2);
	}
	else if(signo == SIGCHLD)
	{
		pid_t pid;
		int status;
		
		while((pid = waitpid(-1, &status, WNOHANG)) > 0)
		{
			d_procs++;
		}
	}
}

void get_process_info()
{
	char buffer[BUFFER_SIZE];
	char memory_path[BUFFER_SIZE];

	char word[WORD_SIZE];
	char pid_id[WORD_SIZE];
	char proc[8] = "|Proc|\0";
	char status[9] = "|Status|\0";
	char io[5] = "|IO|\0";
	char info1[8] = "|Info1|\0";
	char info2[8] = "|Info2|\0";
	char info3[8] = "|Info3|\0";
	char bytes_written[16] = "|Bytes Written|\0";
	char bytes_read[13] = "|Bytes Read|\0";
	char program[10] = "|Program|\0";
	char state[8] = "|State|\0";

	int p = current_process->pid;
	
	int i;
	
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		buffer[i] = '\0';
		memory_path[i] = '\0';
	}

	sprintf(pid_id, "%d", p);

	strcat(memory_path, proc);
	strcat(memory_path, pid_id);
	strcat(memory_path, status);

	int return_val = open(memory_path, O_RDONLY);

	p1getline(return_val, buffer, BUFFER_SIZE);
	p1putstr(1, &(info1[0]));
	p1putstr(1, &(pid_id[0]));
	p1putstr(1, &(info2[0]));
	p1putstr(1, &(program[0]));
	p1putstr(1, &(buffer[0]));
	p1putstr(1, &(state[0]));

	for (i = 0; i < BUFFER_SIZE; i++)
	{
		buffer[i] = '\0';
		p1getline(return_val, buffer, BUFFER_SIZE);
		p1putstr(1, &(buffer[0]));
	}

	for (i = 0; i < BUFFER_SIZE; i++)
	{
		buffer[i] = '\0';
		memory_path[i] = '\0';
	}

	strcat(memory_path, proc);
	strcat(memory_path, pid_id);
	strcat(memory_path, io);
	return_val = open(memory_path, O_RDONLY);
	p1getline(return_val, buffer, BUFFER_SIZE);

	p1putstr(1, &(bytes_read[0]));

	int location = 0;
	location = p1getword(buffer, location, word);
	p1getword(buffer, location, word);
	p1putstr(1, &(word[0]));

	for (i = 0; i < BUFFER_SIZE; i++)
	{
		buffer[i] = '\0';
	}

	p1getline(return_val, buffer, BUFFER_SIZE);
	p1putstr(1, &(bytes_written[0]));

	for (i = 0; i < WORD_SIZE; i++)
	{
		word[i] = '\0';
	}

	location = 0;
	location = p1getword(buffer, location, word);
	p1getword(buffer, location, word);
	p1putstr(1, &(word[0]));

	p1putstr(1, &(info3[0]));
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
	
	if(signal(SIGALRM, signal_handler) == SIG_ERR)
	{
		p1perror(1, "Unable to catch SIGALRM.\n");
		return 1;
	}
	
	if(signal(SIGCHLD, signal_handler) == SIG_ERR)
	{
		p1perror(1, "Unable to catch SIGCHLD.\n");
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
					free_args(args, (index + 1));
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
			p1perror(1, "Error. Unable to fork process.\n");
			free_args(args, arg_count);
			free_procs(procs);
			return 1;
		}
		else if(pid > 0)
		{
			if(add_proc(pid, buffer) < 0)
			{
				free_args(args, arg_count);
				free_procs(procs);
				return 1;
			}
		}
		else if(pid == 0)
		{
			while(!signals_received)
			{
				sleep(1);
			}
			
			if(execvp(program_name, args) < 0)
			{
				p1perror(1, "Error.  Unable to execute program.\n");
				free_args(args, arg_count);
				free_procs(procs);
				return 1;
			}
		}
		
		free_args(args, arg_count);
	}
	
	alarm(1);
	
	while(d_procs < num_procs)
	{
		sleep(1);
	}
	
	free_procs(procs);
	
	return 0;
	
}
