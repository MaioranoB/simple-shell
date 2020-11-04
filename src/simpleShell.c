#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

#define MAX_SIZE 25
int SIGNAL_FLAG = 0;
int COMMAND_RUNNING = 0;

void sig_handler(int signo){
    if (signo == SIGUSR1 && !COMMAND_RUNNING){
		printf("\n\nReceived SIGUSR1\n");
		printf("Press enter to restart the shell\n");
		SIGNAL_FLAG =  1;
	}
}

int main(void){
	signal(SIGUSR1, sig_handler);

	int num_args; //Variável que vai receber o numero de argumentos
	char user_input[MAX_SIZE]; //Variável que vai receber as entradas do usuário
	char *argumentos[MAX_SIZE+2]; //Array de argumentos que sera passado pra função execvp
	//Exemplo: para o comando ping 8.8.8.8 -c 5
	//Devemos passar: execvp("ping",["ping","8.8.8.8","-c","5",NULL]);

	while(1){

		//PASSOS 1 e 2
		if (SIGNAL_FLAG == 0){
			printf("Qual comando você quer executar? ");
			fgets(user_input, MAX_SIZE, stdin);
    		for (int i=0; i<=MAX_SIZE; i++){
        		if (user_input[i] == '\n'){
            		user_input[i] = '\0';
        		}
    		}
			argumentos[0] = strdup(user_input);
		}

		//PASSOS 3 e 4
		if (SIGNAL_FLAG == 0){
			printf("Quantos argumentos você quer digitar? ");
			fgets(user_input,MAX_SIZE,stdin);
			num_args = atoi(user_input);
			
			//Verificar se o usuário passou uma entrada invalida pro número de args
			if (num_args == 0 && user_input[0] != '0' && SIGNAL_FLAG == 0){
				printf("Invalid input\n");
				exit(EXIT_FAILURE);
			}

			argumentos[num_args+1] = NULL;
			printf("\n");
		}

		//PASSO 5
		for (int i = 1; i <= num_args; i++){
			if (SIGNAL_FLAG == 1){
				break;
			}

			printf("Digite o argumento %d: ",i);
			fgets(user_input, MAX_SIZE, stdin);
			for (int j=0; j<=MAX_SIZE; j++){
        		if (user_input[j] == '\n'){
            		user_input[j] = '\0';
        		}
			}
			argumentos[i] = strdup(user_input);
		}

		if (SIGNAL_FLAG == 1){
			SIGNAL_FLAG = 0;
		}
		//Se chegar aqui sem a flag do sinal, deve sair do while infinito e executar o comando
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
		
		//O execvp só retorna se não encontrar o comando
		printf("\nCommand not found: %s\n",argumentos[0]); 
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
