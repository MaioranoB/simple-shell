#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include<signal.h>

int COMMAND_RUNNING = 0;

void ExecCommand(){
	COMMAND_RUNNING = 0;
	//int fgets_max = 50;
	char comando[50];
    int num_args;

	printf("Qual comando quer executar? ");
    scanf("%s",comando);
	
	printf("Quantos argumentos você quer digitar? ");
	if (scanf("%d",&num_args) == 0){
		printf("\nInvalid input\n");
		exit(EXIT_FAILURE);
	};
	printf("\n");

	char *argumentos[num_args+2];
	argumentos[0] = strdup(comando);
	argumentos[num_args+1] = NULL;

	char aux[20];
	for (int i = 1; i <= num_args; i++){
		printf("Digite o argumento %d: ", i);
		scanf("%s",aux);
		
		argumentos[i] = strdup(aux);
	}

	COMMAND_RUNNING = 1;
	pid_t child_pid = fork();
	if (child_pid < 0){ 
		// falha no fork; exit
		printf("fork failed\n");
		exit(EXIT_FAILURE);
	}
	else if (child_pid == 0) {
		//filho (novo processo); executar o comando
		execvp(argumentos[0],argumentos);
		
		//só executa se o execvp der algum erro
		printf("command not found: %s \n",argumentos[0]); 
		exit(EXIT_FAILURE);
	} 
	else {
		//pai segue por este caminho (processo original)
		wait(NULL);
		exit(EXIT_SUCCESS);
	}
}

void sig_handler(int signo){
	if (signo == SIGUSR1){
		if (COMMAND_RUNNING){
			printf("\n\nSIGUSR1 ignored!\n");
		}
		else {
			printf("\n\nreceived SIGUSR1\n");

			sigset_t new_set;
			sigemptyset(&new_set);
			sigaddset(&new_set,SIGUSR1);
			sigprocmask(SIG_UNBLOCK,&new_set,NULL);
			
			ExecCommand();
		}
	}
}

int main(){
	signal(SIGUSR1, sig_handler);
	ExecCommand();
    return 0;
}
