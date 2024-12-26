#include <stdio.h>
#include <string.h>
#include "../part1/mylist.h"

int compar(const void *a, const void *b){
    return strcmp((const char *)a, (const char *)b);
}

void print(void *data) {
    printf("%s\n", (char *)data);
}

int main(int argc, char **argv){
    if (argc <= 1){
        return 1;
    }

    // initialize list
    struct List list;
    initList(&list);

    for (int i = 1; i < argc; i++){
        addFront(&list, argv[i]); 
    }

    traverseList(&list, print);
    printf("\n");  
    struct Node *dude = findNode(&list, "dude", (int (*)(const void *, const void *))compar); 
    if (dude){
        printf("dude found\n");
    } else{
        printf("dude not found\n");
    }

    removeAllNodes(&list);
    return 0;  

}

