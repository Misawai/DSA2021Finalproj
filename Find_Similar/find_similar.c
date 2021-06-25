#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

/* Definition of 'drawer' type for hashing purpose */
typedef struct Drawer{
    bool yes;
    char* token_address;
}drawer;

int hash_value(char c){
    // TODO: return hash value
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

void construct_hash_table(drawer* hash_table, mail target_mail, int* A){
    // TODO:  
}

void check_intersect(drawer* hash_table, mail current_mail, char flag, 
                     int* B, int* inter_size){ 

    // Buffer for token
    char buff[80];              // this is a guess
    int buff_i = 0;

    // Subject
    int max_size;
    if (flag == 's') max_size = 256;
    if (flag == 'c') max_size = 100000;
    for (int i = 0; i < max_size; i++){
        // Initialization
        char c = current_mail.subject[i];
        int c_hash = 0; 
        // Keep reading until a token has formed
        if (isalpha(c) || isdigit(c)){
            // put in buffer
            buff[buff_i++] = c;
            // compute hash
            c_hash += hash_value(c); 
        } 
        else{
            // Form a token only when there's item in buffer
            if (buff_i > 0){
                // Update set B size
                B ++;
                // check hash table
                if (hash_table[c_hash].yes == 1){
                    // check string match or not
                    char* check_str = hash_table[c_hash].token_address;
                    bool check = true;
                    for (int k = 0; k < buff_i; k++){
                        char c = check_str[k];
                        // Size are different
                        if (!isdigit(c) && !isdigit(c)){
                            check = false;
                            break; 
                        }
                        // char are different
                        if (check_str[k] != buff[k]){
                            check = false;
                            break; 
                        } 
                    } 
                    // if match, update size of intersection
                    if (check) inter_size++;
                }
                // Clear the buffer
                for (int k = 0; k < buff_i; k++){
                    buff[k] = '\0';
                } 
                buff_i = 0;
            }
        }
    }
}
    
void _find_similar(mail* mails, int n_mails, int mid, 
                  int threshold, int* ans_array){
    /* This function will output the mail IDs which exceed the 
    * threshold value in the ans_array in increasing order*/

    // Construct the hash tablle for mail[mid]
    // and record size of the token set
    int h_size = 80000;
    int token_size_A;
    drawer hash_table[h_size];
    construct_hash_table(hash_table, mails[mid], &token_size_A);

    // Check similarity from 0 to n_mails
    int ans_i = 0;
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
        if ( (float) union_size/inter_size > threshold){
            ans_array[ans_i++] = mails[i].id;
        }
    }
}

int main(){
    return 0;
}
