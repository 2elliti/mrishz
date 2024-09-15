#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define BUFFER_SIZE 1024
#define TOKEN_BUFFER_SIZE 1024

#define     plain "0" /* or "" */
#define        no "2"
#define    bright "1"
#define       dim "2"
#define    italic "3"
#define underline "4"
#define   reverse "7"
#define      with ";"
#define  ansi_esc "\x1b"
#define fmt(style) ansi_esc "[" style "m"






int execute_tokens(char **tokens,int flag){
	if(flag == 1){
		return 1;
	}
	if (tokens[0] == NULL) {  // No command entered
		return 1;
	}
	int return_value = fork();
	if(return_value<0){
		fprintf(stderr,"Error: can't fork\n");
		exit(1);
	}else if(return_value == 0){
		execvp(tokens[0],tokens);
	}else{
	int wait_return = wait(NULL);
	}

	return 1;
}


void check_exit(char *buffer){
	if(strcmp(buffer,"exit")==0){
		exit(1);
	}

}






char *lex_it(char *buffer, int *curr){
	if((buffer[*curr]>='A' && buffer[*curr]<='Z') || (buffer[*curr]>='a' && buffer[*curr]<='z') || buffer[*curr]=='.'){
//		printf("i am at this character: %c\n",buffer[*curr]);
		char *token = malloc(sizeof(char)*80);
		int token_index = 0;
		while((buffer[*curr]>='A' && buffer[*curr]<='Z') || (buffer[*curr]>='a' && buffer[*curr]<='z') || buffer[*curr]=='.'){
			token[token_index] = buffer[*curr];
			token_index++;
			(*curr)++;
		}
		token[token_index] = '\0';
		return token;



	}

	else{
		fprintf(stderr,"Error: Cant lex non alphabetical character\n");
		exit(1);
	}
}





char **lexing_tokens(char *buffer){
	char **tokens = malloc(sizeof(char *)*TOKEN_BUFFER_SIZE);
	int token_buffer_size = TOKEN_BUFFER_SIZE;
	int current_token_index = 0;
	int current_buffer_index = 0;

	while(buffer[current_buffer_index]!='\0'){
		if((buffer[current_buffer_index]>='A' && buffer[current_buffer_index]<='Z') || (buffer[current_buffer_index]>='a' && buffer[current_buffer_index]<='z') || buffer[current_buffer_index] == '.'){
			char *token;
			token = lex_it(buffer,&current_buffer_index);
			tokens[current_token_index] = token;
			current_token_index++;
		}else{
			current_buffer_index++;
		}
	}

	
	tokens[current_token_index] = NULL;

	return tokens;

}
int launch_builtins(char **tokens){
	int flag = 0;
	if (tokens[0] == NULL) {
		return 0;  // No command to execute
	}
	if(strcmp(tokens[0],"exit") == 0){
		flag = 1;
		exit(1);
	}
	else if(strcmp(tokens[0],"cd") == 0){
//		printf("%s\n",tokens[1]);
		if(chdir(tokens[1])!=0){
			fprintf(stderr,"Error while changing directory\n");
		}
		flag = 1;
	}

	return flag;
}


void mrishz_loop(){

	int status = 1;
	
	
	do{
	
		char *buffer = malloc(sizeof(char)*BUFFER_SIZE);
		int new_buffer_size  = BUFFER_SIZE;
		char *pathbuffer = malloc(sizeof(char)*80);
		printf("\x1b[30;42mx386:%s>> ",getcwd(pathbuffer,80));
		int current_index = 0;
		int ch;
		while((ch=getc(stdin))!='\n' && ch!=EOF){
			buffer[current_index] = ch;
			current_index++;

			if(current_index > BUFFER_SIZE){
				new_buffer_size *= 2;
				char *temp = realloc(buffer,sizeof(char)*new_buffer_size);
				if(temp == NULL){
					fprintf(stderr,"mrishz: can't allocate memory\n");
					exit(1);
				}
				buffer = temp;
			}
		}

		buffer[current_index] = '\0';
//		printf("%s\n",buffer);
		
		
		char **tokens;

		tokens = lexing_tokens(buffer);
//		builtcmd(tokens);
//		printf("\n---------------------\n");
//		for(int i=0;tokens[i]!=NULL;i++){
//			printf("%s\n",tokens[i]);
//		}
//		printf("\n---------------------\n");
		//
		int flag;
		flag = launch_builtins(tokens);

		status = execute_tokens(tokens,flag);
		free(buffer);  // Free allocated buffer memory
		free(tokens);
		free(pathbuffer);




	}while(status);

}



int main(){

	mrishz_loop();

}
