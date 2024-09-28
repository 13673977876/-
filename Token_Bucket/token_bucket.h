#ifndef __TOKEM_BUCKET__
#define __TOKEN_BUCKET__

#define NUMBER 1024


//初始化令牌桶
int Token_Bucket_Init(int speed, int capacity);

//取令牌
int Token_Get(int token_id, int get_number); //通过*my_Tbf参数，来选择不同的桶；

//销毁
void Destory_Token(int token_id);

#endif
