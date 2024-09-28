#include "token_bucket.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


//令牌桶结构
typedef struct{
	int capacity;
	int speed;
	int token_num;
}Tbf_t;

Tbf_t *cur_Tbf[NUMBER];

static int inited = 0;


static void alarm_handler(int s);

static int __Token_Add(void);
static int __Token_Bucket_Older(void);
int Token_Bucket_Init(int speed, int capacity)
{
	int pos;
	Tbf_t *my_Tbf = NULL;
	if(!inited){
		__Token_Add();
		inited = 1;
	}
	
	my_Tbf = (Tbf_t *)malloc(sizeof(Tbf_t));
	if(NULL == my_Tbf){
		return -1;
	}
	
	my_Tbf->capacity = capacity;
	my_Tbf->speed = speed;
	my_Tbf->token_num = 0;

	if((pos = __Token_Bucket_Older()) != -1){
		cur_Tbf[pos] = my_Tbf;
	}

	return pos;
}
static int __Token_Bucket_Older(void)
{

	int i = 0;
	for(i = 0; i < NUMBER; i++){
		if(cur_Tbf[i] == NULL){
			return i;
		}
	}
	return -1;
	
}

static int __Token_Add(void)
{
	struct sigaction act, oldact;

	act.sa_handler = alarm_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGALRM, &act, &oldact);


	alarm(1);
}

static void alarm_handler(int s)
{
	alarm(1);
	int i = 0;
	for(i = 0; i < NUMBER; i++){
		if(cur_Tbf[i]){
			cur_Tbf[i]->token_num += cur_Tbf[i]->speed;
			if(cur_Tbf[i]->token_num > cur_Tbf[i]->capacity){
				cur_Tbf[i]->token_num = cur_Tbf[i]->capacity;
			}
		}
	}
}


int Token_Get(int token_id, int need_number)
{
	while(cur_Tbf[token_id]->token_num == 0){  //等待要一直等待；不能用if
		pause();
	}
	if(need_number > cur_Tbf[token_id]->token_num){
		need_number = cur_Tbf[token_id]->token_num;
	}
	cur_Tbf[token_id]->token_num -= need_number;

	return need_number;
}

void Destory_Token(int token_id)
{
	alarm(0);
	signal(SIGALRM, SIG_DFL);
	free(cur_Tbf[token_id]);
	cur_Tbf[token_id] = NULL;
}
