#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#define TOKEN_MAX_LEN 80
#define HASH_SIZE 80000

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

int n_mails, n_queries;
mail *mails;
query *queries;

int main(){
	api.init(&n_mails, &n_queries, &mails, &queries);
    int i = 0, j = 0;
    for (int i = 0; i < n_queries; i++){
        if(queries[i].type == find_similar){
            int ans_array[10000];
            int ans_i = 0;
            int mid = queries[i].data.find_similar_data.mid;
            double thresh = queries[i].data.find_similar_data.threshold;
            printf("mid: %d, thresh: %f\n", mid, thresh);
            _find_similar(mails, n_mails, mid, 
                          thresh, ans_array, &ans_i);
            //printf("finish a query\n");
            //api.answer(queries[i].id, ans_array, ans_i);
            printf("4: ");
            for (int j = 0; j < ans_i; j++){
                printf("%d ", ans_array[j]);
            }
            return 0;
        }
    }
    return 0;
}
