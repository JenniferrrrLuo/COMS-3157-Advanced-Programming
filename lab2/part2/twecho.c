/*
 * twecho
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char **duplicateArgs(int argc, char **argv){
    char **copy = malloc(sizeof(char *) * (argc + 1)); 
    if (copy == NULL){
        perror("malloc returned NULL"); 
        exit(1); 
    }
    for (int i = 0; i < argc; i++){
        int length = strlen(argv[i]); 
        copy[i] = malloc(sizeof(char) * (length + 1)); 
        if (copy[i] == NULL){
            perror("malloc returned NULL");
            for (int j = 0; j < i; j++){
               free(copy[j]); 
            }
            free(copy);  
            exit(1); 
        }
        for (int j = 0; j < length; j++){
            copy[i][j] = toupper(argv[i][j]); 
        }
        copy[i][length] = '\0';
    }
    copy[argc] = NULL; 
    return copy; 
}

static void freeDuplicatedArgs(char **copy){
    if (copy == NULL){
        return; 
    }
    int i = 0; 
    while (copy[i] != NULL){
        free(copy[i++]); 
    }
    free(copy); 
}


/*
 * DO NOT MODIFY main().
 */
int main(int argc, char **argv)
{
    if (argc <= 1)
        return 1;

    char **copy = duplicateArgs(argc, argv);
    char **p = copy;

    argv++;
    p++;
    while (*argv) {
        printf("%s %s\n", *argv++, *p++);
    }

    freeDuplicatedArgs(copy);

    return 0;
}
