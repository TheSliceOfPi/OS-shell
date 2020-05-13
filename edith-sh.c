#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


char *read_command(void){
	int bufsize = 4; //A random starter size.
	char *buffer = malloc(sizeof(char) * bufsize);
	int pos = 0;
	while(1){
		char c = getchar();
		if(c !='\n'){
			if(pos==bufsize){ //Once the number of words we have is the same as the number of spaces. We must make more space on the heap.
				bufsize=bufsize+bufsize;
				buffer=realloc(buffer,sizeof(char) * bufsize);
				}
			buffer[pos++] = c;
		}
		else {
			buffer[pos] = '\0'; // null byte
			// Return the collected string
			return buffer;
		}
	}
}
int main(int argc, char *argv[])
{
	char curDir[]="Edith@machine:~";
	char closeDir[]="/$ ";
	while (1){
		int status;
		int paramWords=1; //Nice start since it could be a one argument command 
		printf(curDir);
		printf(closeDir);
		char *cmdline=read_command(); //Set the readline as a function so I could malloc/realloc properly.
		char **params=malloc(paramWords*sizeof(char *)); //2D Array so that each thing on the array can hold an array (aka a string)
		char *token=strtok(cmdline," ");
		int cont=1;
		while(cont && token!=NULL){//Checking for null in case params[0] doesnt even exist
			params[paramWords-1]=malloc(strlen(token)*sizeof(char));
			params[paramWords-1]=token;
			paramWords=paramWords+1; //reallocating one word at a time just so I do reallocate too much space.
			params=realloc(params,paramWords * sizeof(char *));//Set this so that the null is also counted and param[1] on only one liners (ex cd) is considered correctly on the if/else statements.
			token=strtok(NULL," ");
			if(token==NULL){
				cont=0;
			}
		}
		if(params[0]!=NULL){ //Placed before built-in so it wont create error when trying to read param[0]
			if (strcmp(params[0],"exit")==0){
				exit(0);
			 }
			if(strcmp(params[0],"cd")==0){
				if(params[1]==NULL || strcmp(params[1],"~")==0){
					chdir("/"); //Empty path
				}else{
					int verify= chdir(params[1]); //Keep track of errors or files not in the dir at that time
					if(verify==-1){
						printf("%s : No such file or directory.\n",params[0]);
					}
				}
			}else{
				if(fork()!=0){
					waitpid(-1,&status,0);
					}else{
						int checkCmd=execvp(params[0],params);
						if (checkCmd==-1){ //Since -1 is returned on error then only those commands that are crazy will go into the if statement below.
							printf("%s: command not found \n",params[0]);
						}
					}
			}
		}
		free(params); //Free up the params double pointer, free malloc from the heap.
	}
	
	return 0;
}
