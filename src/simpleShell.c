#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

#define MAX_SIZE 50
int SIGNAL_FLAG = 0;
int COMMAND_RUNNING = 0;

void sig_handler(int signo){
    if (signo == SIGUSR1 && !COMMAND_RUNNING){
		printf("\n\nreceived SIGUSR1\n");
		SIGNAL_FLAG =  1;
	}
}

int main(void){
	signal(SIGUSR1, sig_handler);

	char *argumentos[MAX_SIZE]; //Array de argumentos que sera passado pra função execvp
	char comando[MAX_SIZE]; //Variável que vai receber o comando
	int num_args; //Variável que vai receber o numero de argumentos
	char arg[MAX_SIZE]; //Variável que vai receber cada argumento

	while(1){

		//PASSOS 1 e 2
		if (SIGNAL_FLAG == 0){
			printf("Qual comando você quer executar? ");
			scanf("%s",comando);
			argumentos[0] = strdup(comando);
		}

		//PASSOS 3 e 4
		if (SIGNAL_FLAG == 0){
			printf("Quantos argumentos você quer digitar? ");
			if (scanf("%d",&num_args) == 0){
				printf("Invalid input\n");
				exit(EXIT_FAILURE);
			}
			printf("\n");
			argumentos[num_args+1] = NULL;
		}

		//PASSO 5
		for (int i = 1; i <= num_args; i++){
			if (SIGNAL_FLAG == 1){
				break;
			}

			printf("Digite o argumento %d: ", i);
			scanf("%s",arg);
			argumentos[i] = strdup(arg);
		}

		if (SIGNAL_FLAG == 1){
			SIGNAL_FLAG = 0;
		}
		//Se chegar cheagar aqui sem a flag do sinal, deve sair do while infinito e executar o comando
		else{
			break;
		}
	}

	//PASSO 6
	COMMAND_RUNNING = 1;

	pid_t child_pid = fork();
	if (child_pid == 0) {
		//Run command
		execvp(argumentos[0],argumentos);
		
		//O execvp retorna se não encontrar o comando
		printf("Command %s not found!\n",argumentos[0]); 
		exit(EXIT_FAILURE); 
	}
	else {
		//PASSO 7
		wait(NULL);
		//printf("\nTask is done.\n");
		return 0;
	}
    return 0;
}
