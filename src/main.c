#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
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




int execute_tokens(char **tokens){

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









char *lex_it(char *buffer, int *curr){
	if((buffer[*curr]>='A' && buffer[*curr]<='Z') || (buffer[*curr]>='a' && buffer[*curr]<='z')){
//		printf("i am at this character: %c\n",buffer[*curr]);
		char *token = malloc(sizeof(char)*80);
		int token_index = 0;
		while((buffer[*curr]>='A' && buffer[*curr]<='Z') || (buffer[*curr]>='a' && buffer[*curr]<='z')){
			token[token_index] = buffer[*curr];
			token_index++;
			(*curr)++;
		}
		token[token_index] = '\0';
		return token;



	}else{
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
		if((buffer[current_buffer_index]>='A' && buffer[current_buffer_index]<='Z') || (buffer[current_buffer_index]>='a' && buffer[current_buffer_index]<='z')){
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

void mrishz_loop(){

	int status = 1;
	
	
	do{
	
		char *buffer = malloc(sizeof(char)*BUFFER_SIZE);
		int new_buffer_size  = BUFFER_SIZE;
		printf("\x1b[91mx386>> ");
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
//		printf("\n---------------------\n");
//		for(int i=0;tokens[i]!=NULL;i++){
//			printf("%s\n",tokens[i]);
//		}
//		printf("\n---------------------\n");

		status = execute_tokens(tokens);




	}while(status);

}



int main(){

	mrishz_loop();

}
