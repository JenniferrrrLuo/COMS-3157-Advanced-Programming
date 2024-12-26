#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
//reference: https://www.geeksforgeeks.org/qsort-function-in-c/

int compare_a(const void *a, const void *b){
    return (*(int*)a - *(int*)b);
}

int compare_d(const void *a, const void *b){
    return (*(int*)b - *(int*)a);
}

void sort_integer_array(int *begin, int *end, int ascending){
    if (ascending){
         qsort(begin, end-begin, sizeof(int), compare_a); 
    } else {
         qsort(begin, end-begin, sizeof(int), compare_d); 
    }
}

int main(){
    srandom(time(NULL)); 

    int n;
    scanf("%d", &n); 
    
    //original
    int *p_ori = malloc(sizeof(int) * n); 
    if (p_ori == NULL){
        perror("malloc returned NULL"); 
        exit(1); 
    }
    for (int i = 0; i < n; i++){
        p_ori[i] = random() % 100;  
    }
    
    //ascending
    int *p_asc = malloc(sizeof(int) * n);
    if (p_asc == NULL){
        perror("malloc returned NULL");
        free(p_ori); 
        exit(1);
    } 
    for (int j = 0; j < n; j++){
        p_asc[j] = p_ori[j];
    }
    sort_integer_array(p_asc, p_asc + n, 1); 

    //descending
    int *p_des = malloc(sizeof(int) * n);
    if (p_des == NULL){
        perror("malloc returned NULL");
        free(p_ori); 
        free(p_asc); 
        exit(1);
    }
    for(int k = 0; k < n; k++){
        p_des[k] = p_ori[k]; 
    }
    sort_integer_array(p_des, p_des + n, 0); 

    
    printf("    original:");
    for (int i = 0; i < n; i++){
        printf(" %d", p_ori[i]);
    }
    printf("\n");

    printf("    ascending:");
    for (int j = 0; j < n; j++){
        printf(" %d", p_asc[j]);
    }
    printf("\n");

    printf("    descending:");
    for(int k = 0; k < n; k++){
        printf(" %d", p_des[k]);
    }
    printf("\n");    
    
    
    free(p_ori); 
    free(p_asc); 
    free(p_des);

    return 0;  

}
