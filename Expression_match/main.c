#include "api.h"
#include <stdio.h>
#include <ctype.h> //include
// The testdata only contains the first 100 mails (mail1 ~ mail100)
// and 2000 queries for you to debug.

int n_mails, n_queries;
mail *mails;
query *queries;



typedef struct stack_op
{
    int top;
    int maxsize;
    char *item;
}stack_op;

typedef struct stack_bo
{
	int top;
	int maxsize;
	bool *item;
}stack_bo;


struct stack_op* newStack(int length)
{
    struct stack_op *optr =(stack_op*)malloc(sizeof(stack_op));
    optr->top = -1;
    optr->maxsize = length;
    optr->item = (char*)malloc(sizeof(char) * length);
    return optr;
}

struct stack_bo* newStack2(int length)
{
    struct stack_bo *botr =(stack_bo*)malloc(sizeof(stack_bo));
    botr->top = -1;
    botr->maxsize = length;
    botr->item = (bool*)malloc(sizeof(bool) * length);
    return botr;
}

bool bo_pop(stack_bo *botr)
{
    if(botr->top == -1) exit(1);
    return botr->item[botr->top--];
}

char op_pop(stack_op *optr)
{
    if(optr->top == -1) exit(1);
    return optr->item[optr->top--];
}

void bo_push(stack_bo *botr , bool val)
{
    if(botr->top == botr->maxsize -1) exit(1);
    botr->item[++botr->top] = val;
}

void op_push(stack_op *optr , char ch)
{
    if(optr->top == optr->maxsize-1) exit(1);
    optr->item[++optr->top] = ch;
}

int op_preced(char c)
{
    switch(c)    {
        case '|':
            return 1;
        case '&':
            return 2;
        case '!':
            return 3;
    }
    return 0;
}


bool is_alpha_digit(char target)
{
	if (isdigit(target)!=0 ||isalpha(target)!= 0 ) return true;
	else return false;
	
}

bool subjectMatching(char pattern[] , int sublen,int num)
{
	int pattlen = strlen(pattern);
	char *ptpointer = pattern; // using for strncmp
	char *sjpointer = mails[num].subject; // using for strncmp
	int head , tail = 0;
	for (int i = 0; i < sublen; i++)
	{
		while (is_alpha_digit(mails[num].subject[i]))
		{
			i++;
		}
		head = i-1;
		if ((head-tail+1) == pattlen)
		{
			char ch1 = *(sjpointer+tail);
			char ch2 = *(ptpointer);
			if (strncasecmp(ptpointer,(sjpointer+tail),pattlen) == 0)
			{
				return true;
			}
			else
			{
				while (!is_alpha_digit(mails[num].subject[i]) && i<sublen) // move to next token
				{
					i++;
				}
				head = tail = i;
			}
		}
		else
		{
			while (!is_alpha_digit(mails[num].subject[i]) && i<sublen) // move to next token
			{
				i++;
			}
			head = tail = i;
		}
		
	}
	return false;
	
}

bool contentMatching(char pattern[] , int conlen,int num )
{
	int pattlen = strlen(pattern);
	char *ptpointer = pattern;// using for strncmp
	char *ctpointer = mails[num].content;//using for strncmp
	int head = 0 , tail = 0;
	for (int i = 0; i < conlen; i++)
	{
		while (is_alpha_digit(mails[num].content[i]))
		{
			i++;
		}
		head = i-1;
		if ((head-tail+1) == pattlen)
		{
			char ch1 = *(ctpointer+tail);
			char ch2 = *(ptpointer);
			int k =0;
			if (strncasecmp(ptpointer,(ctpointer+tail),pattlen) == 0)
			{
				return true;
			}
			while (!is_alpha_digit(mails[num].content[i]) && i < conlen) // move to next token
			{
				i++;
			}
			head = tail = i;		
		}
		else
		{
			while (!is_alpha_digit(mails[num].content[i]) && i < conlen) // move to next token
			{
				i++;
			}
			head = tail = i;
		}
		
	}
	return false;
	
}

bool Match(char pattern[] , int conlen , int sublen , int num)
{
	if (subjectMatching(pattern, sublen , num))
		return true;
	else
	{
		if(contentMatching(pattern , conlen , num)) return true;
		else return false;	
	}
}

bool cal(stack_op *optr , stack_bo* botr)
{
	char temp = op_pop(optr);
	if(temp == '!')
	{
		bool bo1 = bo_pop(botr);
		bo_push(botr , !bo1);
	}
	else if(temp == '&')
	{
		bool bo2 = bo_pop(botr);
		bool bo1 = bo_pop(botr);
		bo_push(botr,bo1&bo2);
	}
	else if (temp == '|')
	{
		bool bo2 = bo_pop(botr);
		bool bo1 = bo_pop(botr);
		bo_push(botr,bo1|bo2);
	}
}

bool eval(char expression[], int exlen , int conlen , int sublen, int num)
{
	stack_op *optr = newStack(exlen);
	stack_bo *botr = newStack2(exlen);

	for (int i = 0; i < exlen; i++)
	{
		if(expression[i] == '(') op_push(optr,expression[i]);
		else if (is_alpha_digit(expression[i]))
		{
			char temp[100];
			int k = 0;
			while (i < exlen && is_alpha_digit(expression[i]))
			{
				temp[k++] = expression[i++];
			}
			bo_push(botr,Match(temp,conlen,sublen,num));
			i--;
			
		}
		else if(expression[i] == ')')
		{
			while (optr->top!=-1 && optr->item[optr->top]!='(')
			{
				bool value;
				value = cal(optr,botr);
				bo_push(botr,value);
			}
			if(optr->top!=-1) op_pop(optr);//pop '('
		}
		else
		{
			while(optr->top!=-1 && op_preced(optr->item[optr->top])>=op_preced(expression[i])) // ! > & > |
            {
                bool value;
				value = cal(optr,botr);
                bo_push(botr,value);
            }
			op_push(optr,expression[i]);
		}
	}
	while (optr->top!= -1) // calculate bool until stackbo only have one element(final result)
	{
		bo_push(botr,cal(optr,botr));
	}
	free(botr);
	free(optr);
	return botr->item[botr->top];
}

void Expression_Match(char expression[] , int ans[] , int* len)
{
	//use stack to read expression then determine if expression is true or not
	int tep = (*len); //index for ans[]
	int exlen = strlen(expression);
	tep = 0;
	for (int i = 0; i < n_mails; i++) // expression for n_mails
	{
		int conlen = strlen(mails[i].content);
		int sublen = strlen(mails[i].subject);
		if (eval(expression, exlen , conlen ,sublen, i))
		{
			ans[tep++] = mails[i].id;
		}	
	}
	(*len) = tep;  // change len value
}

int main(void) {
	api.init(&n_mails, &n_queries, &mails, &queries);
	
	for(int i = 0; i < n_queries; i++)	
		if(queries[i].type == expression_match)
		{
			int ans[10000] , len;
			Expression_Match(queries[i].data.expression_match_data.expression , ans , &len);
		  	api.answer(queries[i].id, ans , len);
		}
    return 0;
}
