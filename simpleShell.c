#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

#include <stdlib.h>
#include<signal.h>

#define SIGUSR1 10
#define MAX_ARGS 10
char *argumentos[MAX_ARGS];
int signal_flag = 0;

void sig_handler(int signo){
    if (signo == SIGUSR1){
		printf("\n\nreceived SIGUSR1\n");
		for (int i = 0; i < MAX_ARGS; ++i){
        	argumentos[i] = NULL;
    	}	
		//fclose(stdin);
		signal_flag =  1;
	}
}

void setCommand(){
	char comando[50];
    printf("Qual comando quer executar? ");
    scanf("%s",comando);
	//fgets(comando, 50, stdin);
	//printf( "You entered: %s\n", comando);
	argumentos[0] = strdup(comando);
	signal_flag =  0;
}

int getNumArgs(){
	int num_args;
	printf("Quantos argumentos você quer digitar? ");

	if (scanf("%d",&num_args) == 0){
		printf("Invalid input\n");
		exit(1);
	};
	printf("\n");
	return num_args;
}

int main(void){
	signal(SIGUSR1, sig_handler);

	int gettig_args = 1;
	while(gettig_args){
		setCommand();
		int num_args = getNumArgs();

		char aux[20];
		for (int i = 1; i <= num_args; i++){
			if (signal_flag == 1){	
				break;
			}
		
			printf("Digite o argumento %d: ", i);
			scanf("%s",aux);
			//fgets(aux, 20, stdin);
			argumentos[i] = strdup(aux);
		}
		if (argumentos[0] != NULL){
			gettig_args = 0;
		}
	}

	pid_t child_pid = fork();
	if (child_pid == 0) {
		//Run command
		execvp(argumentos[0],argumentos);
		
		//só executa se o execvp der algum erro
		printf("command not found: %s \n",argumentos[0]); 
		exit(1); 
	} 
	else {
		wait(NULL);
		printf("\nTask is done.\n");
		return 0;
	}
	printf("heheXD");
    return 0;
}

//execlp("/bin/ping","ping","8.8.8.8","-c","5",NULL);
//https://github.com/remzi-arpacidusseau/ostep-code/blob/master/cpu-api/p4.c
//https://stackoverflow.com/questions/46985677/how-know-if-a-command-given-to-exec-exists