#include "api.h"
#include <stdio.h>
#include<stdbool.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#define TOKEN_MAX_LEN 80
#define HASH_SIZE 80000
#define MAX 10000
// The testdata only contains the first 100 mails (mail1 ~ mail100)
// and 2000 queries for you to debug.

int n_mails, n_queries;
mail *mails;
query *queries;

typedef struct disjoint_tree{
	struct disjoint_tree *parent;
	int rank;
}Tree;


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

/* Definition of 'drawer' type for hashing purpose */
typedef struct Drawer{
    bool yes;
    char* token_address;
    int token_len;
    struct Drawer* next;
}drawer;

drawer* createDrawer(){

    drawer* new_d = malloc(sizeof(drawer));
    new_d->yes = false;
    new_d->token_address = NULL;
    new_d->token_len = 0;
    new_d->next = NULL;

    return new_d;
}
/* End of definition */

/* Definition of 'token' and 'tokenList' for token set */
typedef struct Token{
    char str[TOKEN_MAX_LEN + 1];
    struct Token* next;
}token;

typedef struct TokenList{
    token* head;
}tokenList;

token* createToken(char* buffer, int input_len){
    token* new_t = malloc(sizeof(token));
    for (int i = 0; i < input_len; i++){
        new_t->str[i] = buffer[i];
    }
    new_t->str[input_len] = '\0';
    new_t->next = NULL;
    return new_t;
}
tokenList* createTokenList(){
    tokenList* new_tl = malloc(sizeof(tokenList));
    // dummy head node
    char dummy_buffer[2] = {'0', '\0'};
    new_tl->head = createToken(dummy_buffer, 2); 
    return new_tl;
}


Tree ds[MAX];
bool set[MAX];
int largecount[MAX];
char com[MAX][32]={};
char find_set(const char *data);
char makeset(const char *data);
char linkage(const char *ra,const char *rb);
int hash(const char* s)
{
	for(int i=0;i<MAX;i++)
	{
		if (strcmp(s,com[i])==0)
		{
			return i;
		}
		
	}
}

//source from TA's JUDGE PID 50//START
inline void static init(const char* data) {
    int i = hash(data);
    if (!set[i]) {
        makeset(data);
        set[i] = true;
    }
}
//source from TA's JUDGE PID 50//END
//Method 2 
char makeset(const char *data)
{
	int i=hash(data);//TBC
	ds[i].parent=i;
	ds[i].rank=0;
	//have to find a proper hash function inorder to sort names properly
	//maybe using linear hashing is a good way, however it might be time consuming
	
}
bool only(const int i)
{
	if (ds[i].parent==i)
	{
		return true;
	}
}
char find_set(const char *data)
{
	init(data);
	int i=hash(data);
	if (ds[i].parent==i)
	{
		return ds[i].parent;
	}
	else
	{
		return cset(i);
	}
	
}
//SOURCE FROM TA's DSAJUDGE PID50
int cset(int i){
	if (ds[i].parent==i)
	{
		return ds[i].parent;
	}
	else{
		ds[i].parent=cset(ds[i].parent);
	}
   return ds[i].parent;
}
//SOURCE FROM TA's DSAJUDGE PID50

char linkage(const char *ra,const char *rb)
{
	int a=find_set(ra), b=find_set(rb);
	if (ds[a].rank>ds[b].rank)
	{
		ds[b].parent=a;
	}
	else{
		ds[a].parent=b;
		if(ds[a].rank==ds[b].rank)
		{
			ds[b].rank++;
		}
	}
}
int count_group()
{
	int count=0;
	for(int i=0;i<sizeof(ds);i++)
	{
		if (set[i])//if this set exist
		{
			if(only(i))//if this set is the parent
			{
				count++;//++
			}
		}
	}
	return count;
}
int largest_group()
{
	int temp=0;
	for(int i=0;i<sizeof(ds);i++)
	{
		//traverse all elements, count their findset parents and add them into a hash table.
		largecount[find_set(ds)]=largecount[find_set(ds)]+1;
	}
	for (int i=0;i<sizeof(largecount);i++)
	{
		if (temp<largecount[i])
		{
			temp=largecount[i];
		}
	}
	return temp;
}
void Group_Analysis(int len,const int mids[],int ans[])
{
	//disjoint set noneed to compressed since only query once or....
	//if many queries impelemented, path compression is needed.
	//I think I need to decleare a hash table to do a "correspond table of ID and index.
	for(int i=0;i<sizeof(mids);i++)
	{
		makeset(mails[mids[i]]->from);
		makeset(mails[mids[i]]->to);
		linkage(mails[mids[i]]->from,mids[i]->to);
	}	
	//Method 3 should be better when calculating the number of nodes in each group.
	//Method 2 should be OK but code might be troublesome.
	//however, if we change the value in array become the index of root, than counting will be more faster. 
	//But overall, Method 3 has more stability and better to maintain, since we are going to do path compression.
	//path compression is necessary?
	//actually for 10000 mails and 20000 maximum possible names, it seems be necessary since it get makeset O(1) and findset O(1)
	ans[0]=count_group();
	ans[1]=largest_group();
	
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
			char temp[100] = {'\0'};
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
				bool value = cal(optr,botr);
				bo_push(botr,value);
			}
			if(optr->top!=-1) op_pop(optr);//pop '('
		}
		else
		{
			while(optr->top!=-1 && op_preced(optr->item[optr->top])>=op_preced(expression[i])) // ! > & > |
            {
				bool value = cal(optr,botr);
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
void print_tl(tokenList* tl){
    token* temp = tl->head;
    while(temp->next != NULL){
        temp = temp->next;
        int i = 0;
        while(temp->str[i] != '\0'){
            printf("%c", temp->str[i]);
            i++;
        }
        printf("\n");
    }
}
/* end of definition */


int hash_value(char c){
    // Convert to ascii value
    int ascii = c;
    // Digit (map to [0, 9])
    if (ascii <= 57) return ascii - '0';
    // Alphabet (map to [10, 35])
    // A ~ Z
    else if (ascii <= 90) return ascii - 55;
    // a ~ z
    else                  return ascii - 87;
}

bool is_valid_match(drawer* drawer_head, char* buff, int str_len){
    // Check string match or not in entire drawer linked-list
    drawer* drawer_curr = drawer_head;
    bool check;
    while (drawer_curr != NULL){
        char* check_str = drawer_curr->token_address;
        int check_str_len = drawer_curr->token_len;
        for (int k = 0; k < str_len; k++){
            char c = check_str[k];
            check = false;
            // Size are different
            if (check_str_len != str_len){
                break; 
            }
            // char are different
            if (check_str[k] != buff[k]){
                break; 
            } 
            // string match !
            if (k == str_len - 1) check = true;
        }
        if (check == true) break;
        drawer_curr = drawer_curr->next;
    }
    return check;
}

void construct_hash_table(drawer** hash_table, tokenList* tl, 
                          mail target_mail, int* A){
    // Initialize all the drawers in the table
    for (int i = 0; i < HASH_SIZE; i ++){
        hash_table[i] = createDrawer();
    }
    // Get token head
    token* curr_token = tl->head;
    // Subject and Content
    char* source_list[2];
    source_list[0] = target_mail.subject;
    source_list[1] = target_mail.content;
    int len_list[] = {256, 100000};
    for (int i = 0; i < 2; i++){
        // Initialization
        char* source = source_list[i];
        int max_len = len_list[i]; 
        int c_hash = 0;
        char buff[TOKEN_MAX_LEN];
        int buff_i = 0;

        for (int j = 0; j < max_len; j++){
            char c = source[j];
            // Keep reading until a token has formed
            if (isalpha(c) || isdigit(c)){
                // put in buffer
                buff[buff_i++] = c;
                // compute hash
                c_hash += (int)pow(hash_value(c), 2); 
            } 
            // Check hash value and store in hash table & token list
            else{
                if (buff_i > 0){
                    // Put in token list
                    token* new_t = createToken(buff, buff_i);
                    curr_token->next = new_t;
                    curr_token = new_t; 
                    // Get corresponding drawer
                    drawer* corres_drawer = hash_table[c_hash];
                    // No collision
                    if (corres_drawer->yes == false){
                        corres_drawer->yes = true;
                        corres_drawer->token_address = curr_token->str;  
                        corres_drawer->token_len = buff_i;  
                        // update token set A size
                        *A = *A + 1;
                    }
                    // Collision occur, check collision type
                    else{
                        // Token has occured before
                        if (is_valid_match(corres_drawer, buff, buff_i)){
                            // do nothing
                        } 
                        // New token (with same hash value)
                        else{
                            // open a new drawer at the end of list
                            drawer* temp = corres_drawer;
                            while(temp->next != NULL) temp = temp->next;
                            temp->next = createDrawer();
                            temp->next->yes = true;
                            temp->next->token_address = curr_token->str;
                            temp->next->token_len = buff_i;
                            // update token set A size
                            *A = *A + 1;
                        }
                    }
                    // Clear the buffer
                    for (int k = 0; k < buff_i; k++){
                        buff[k] = '\0';
                    } 
                    buff_i = 0;
                    // Clear hash
                    c_hash = 0;
                }
            }
        }
    }
}

void check_intersect(drawer** hash_table, mail current_mail, char flag, 
                     int* B, int* inter_size){ 

    // Buffer for token
    char buff[TOKEN_MAX_LEN];              // this is a guess
    int buff_i = 0;
    // Hash value
    int c_hash = 0; 

    // Subject or Content
    int max_size;
    char* source;
    if (flag == 's') {
        max_size = 256;
        source = current_mail.subject;
    }
    if (flag == 'c') {
        max_size = 100000;
        source = current_mail.content;
    }
    for (int i = 0; i < max_size; i++){
        // Initialization
        char c = source[i];
        // Keep reading until a token has formed
        if (isalpha(c) || isdigit(c)){
            // put in buffer
            buff[buff_i++] = c;
            // compute hash
            c_hash += (int) pow(hash_value(c), 2); 
        } 
        else{
            // Form a token only when there's item in buffer
            if (buff_i > 0){
                // Update set B size
                *B += 1;
                // check hash table
                if (hash_table[c_hash]->yes){
                    // check strings
                    bool check = is_valid_match(hash_table[c_hash], 
                                                buff, buff_i);
                    // if match, update size of intersection
                    if (check) *inter_size += 1;
                }
                // Clear the buffer
                for (int k = 0; k < buff_i; k++){
                    buff[k] = '\0';
                } 
                buff_i = 0;
                // Clear hash
                c_hash = 0;
            }
        }
    }
}
    
void _find_similar(mail* mails, int n_mails, int mid, 
                  double threshold, int* ans_array, int* ans_i){
    /* This function will output the mail IDs which exceed the 
    * threshold value in the ans_array in increasing order*/

    // Construct the hash tablle for mail[mid]
    // and record size of the token set
    int token_size_A;
    drawer* hash_table[HASH_SIZE];
    tokenList* tl = createTokenList();
    clock_t cons_start = clock();
    construct_hash_table(hash_table, tl, mails[mid], &token_size_A);
    clock_t cons_end = clock();
    printf("construct hash table cost %f s\n", 
            (double) (cons_end - cons_start) / CLOCKS_PER_SEC); 

    clock_t check_start = clock();
    // Check similarity from 0 to n_mails
    for (int i = 0; i < n_mails; i++){
        // Skip mid
        if (i == mid) continue;
        // Store size of sets
        int token_size_B = 0;
        int inter_size = 0;
        // Check subject
        check_intersect(hash_table, mails[i], 's', 
                        &token_size_B, &inter_size);
        // Check content
        check_intersect(hash_table, mails[i], 'c', 
                        &token_size_B, &inter_size);

        // Determine whether to put in answer array
        int union_size = token_size_A + token_size_B - inter_size;
        double score = (double) inter_size / union_size; 
        //printf("score: %f, threshold: %f\n", score, threshold);
        if (score > threshold){
            ans_array[*ans_i] = mails[i].id;
            *ans_i += 1;
        }
    }

    clock_t check_end = clock();
    printf("check intersection cost %f s\n", 
            (double) (check_end - check_start) / CLOCKS_PER_SEC); 
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
		else if(queries[i].type == find_similar){
            int ans_array[10000];
            int ans_i = 0;
            int mid = queries[i].data.find_similar_data.mid;
            double thresh = queries[i].data.find_similar_data.threshold;
            printf("mid: %d, thresh: %f\n", mid, thresh);
            _find_similar(mails, n_mails, mid, 
                          thresh, ans_array, &ans_i);
            api.answer(queries[i].id, ans_array, ans_i);
		}
		else if (queries[i].type == group_analyse)
		{
			int ans[10000] , len;
			Group_Analysis(queries[i].data.expression_match_data.len , queries[i].data.expression_match_data.mids);
		  	api.answer(queries[i].id, ans , 2);
		}
    return 0;
}