#include "token_bucket.h"
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define CAPACITY  100
#define SPEED 10
#define BUFFSIZE 32
static int mycopy_token(FILE *file1, FILE *file2);
int main(int argc, char *argv[])
{

	FILE *file1 = NULL; 
	FILE *file2 = NULL;

	if(argc < 3){
		return -1;
	}
	file1 = fopen(argv[1], "r");
	if(NULL == file1){
		perror("fopen1()");
		return -1;
	}
	file2 = fopen(argv[2], "w");
	if(NULL == file2){
		perror("fopen2()");
		return -1;
	}
	mycopy_token(file1, file2);

	fclose(file1);  //不是free;
	fclose(file2);
		
}

static int mycopy_token(FILE *file1, FILE *file2)
{
	char buff[BUFFSIZE] = {};
	int token_number;
	int byteread;
	int pos;
	if(pos = Token_Bucket_Init(SPEED, CAPACITY) == -1){
		printf("令牌桶溢满！\n");
		return -1;
	}
	printf("*********************%d\n", pos);
	while(1){
		token_number = Token_Get(pos , BUFFSIZE); //返回值是控制流速
		byteread = fread(buff, sizeof(char), token_number, file1);
		if(byteread == 0){  //错误判断通过feof和ferror
			if(feof(file1)){
				break;
			}
			else if(ferror(file1)){
				perror("fread()");
				return -1;
			}	
		}
		if(fwrite(buff, sizeof(char), byteread, stdout) != byteread){
			perror("fwrite()");
			return -1;
		}
		fflush(NULL);
	}
	Destory_Token(pos);
	return 0;
}
