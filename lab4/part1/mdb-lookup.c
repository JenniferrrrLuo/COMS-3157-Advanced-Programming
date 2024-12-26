#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "mdb.h"
#include "mylist.h"

void removeAllNodes_mdb(struct List *list)
{
    while (!isEmptyList(list)){
        void *data = popFront(list);
        free(data); 
    }
}

int main(int argc, char **argv){

    if (argc != 2){
        fprintf(stderr, "%s\n", "usage: mdb-lookup <database_file>"); 
        exit(1); 
    }

    FILE *fp = fopen(argv[1],"rb");
    if (fp == NULL){
        perror("Unable to open file."); 
        exit(1); 
    }

    struct List list;
    initList(&list);
    struct Node *node = NULL;

    while (1) {
        struct MdbRec *memory_record = (struct MdbRec *)malloc(sizeof(struct MdbRec));
        if (!memory_record){
            fclose(fp);
            perror("malloc failed"); 
            exit(1); 
        }

        size_t res = fread(memory_record, sizeof(struct MdbRec), 1, fp); 
        if (res != 1) {
            if (feof(fp)){
                free(memory_record); 
                break; 
            }     
            free(memory_record); 
            exit(1);    
        }

        if (node == NULL){
            node = addFront(&list, memory_record); 
        } else {
            node = addAfter(&list, node, memory_record); 
        }
    }
    fclose(fp); 

    char line[1000]; 
    printf("lookup: "); 

    while (fgets(line, sizeof(line), stdin) != NULL){

        if (line[strlen(line) -1] == '\n') {
            line[strlen(line) -1] = '\0'; 
        }

        char search_string[6]; 
        if (strlen(line) > 5){
            strncpy(search_string, line, 5); 
            search_string[5] = '\0'; 
        } else {
            strncpy(search_string, line, strlen(line)); 
            search_string[strlen(line)] = '\0'; 
        }
        // printf("search_string = %s\n", search_string); 

        int record_number = 1; 
        struct Node *node = list.head;
        while (node) {
            // printf("%d\n", record_number); 
            struct MdbRec *record = (struct MdbRec *)node->data;
            // printf("%s, %s\n", record->name, record->msg);
            if (strstr((const char *)record->name, (const char *)search_string) || strstr((const char *)record->msg, (const char *)search_string)){
                printf("%3d: {%s} said {%s}\n", record_number, record->name, record->msg); 
            }
            node = node->next; 
            record_number++; 
        }
        printf("lookup: "); 

    }
    removeAllNodes_mdb(&list);
    return 0; 
}
