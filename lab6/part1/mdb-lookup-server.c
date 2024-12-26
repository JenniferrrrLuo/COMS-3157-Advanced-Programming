#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "mylist.h"
#include "mdb.h"

#define KeyMax 5

static void die(const char *message)
{
    perror(message);
    exit(1); 
}

int main(int argc, char **argv){
    /*
     * open the database file specified in the command line
     */

    // ignore SIGPIPE so that we don't terminate when we call
    // send() on a disconnected socket.
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) 
	die("signal() failed");

    if (argc != 3) {
        fprintf(stderr, "%s\n", "usage: mdb-lookup-server <database_file> <server-port>");
        exit(1);
    }

    unsigned short port = atoi(argv[2]);

    // Create a listening socket (also called server socket) 

    int servsock;
    if ((servsock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        die("socket failed");

    // Construct local address structure

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    // Bind to the local address

    if (bind(servsock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        die("bind failed");

    // Start listening for incoming connections

    if (listen(servsock, 5 /* queue size for connection requests */ ) < 0)
        die("listen failed");

    /*
     * lookup loop
     */

    char line[1000];
    char key[KeyMax + 1];
    char buf[4096];

    int clntsock;
    socklen_t clntlen;
    struct sockaddr_in clntaddr;

    // printf("lookup: ");
    // fflush(stdout);

    while(1){

        // Accept an incoming connection

        fprintf(stderr, "waiting for client ... ");

        clntlen = sizeof(clntaddr); // initialize the in-out parameter

        if ((clntsock = accept(servsock,
                        (struct sockaddr *) &clntaddr, &clntlen)) < 0)
            die("accept failed");

        
        char *filename = argv[1];
        FILE *fp = fopen(filename, "rb");
        if (fp == NULL) 
            die(filename);

        /*
        * read all records into memory
        */

        struct List list;
        initList(&list);

        int loaded = loadmdb(fp, &list);
        if (loaded < 0)
            die("loadmdb");
        
        fclose(fp);

        FILE *input = fdopen(clntsock, "r");
        if (input == NULL) {
            freemdb(&list);
            close(clntsock);
            die("fdopen failed");
        }

        while (fgets(line, sizeof(line), input) != NULL) {

            // must null-terminate the string manually after strncpy().
            strncpy(key, line, sizeof(key) - 1);
            key[sizeof(key) - 1] = '\0';

            // if newline is there, remove it.
            size_t last = strlen(key) - 1;
            if (key[last] == '\n')
                key[last] = '\0';

            // traverse the list, printing out the matching records
            struct Node *node = list.head;
            int recNo = 1;
            int len; 
            while (node) {
                struct MdbRec *rec = (struct MdbRec *)node->data;
                if (strstr(rec->name, key) || strstr(rec->msg, key)) {
                    len = snprintf(buf, sizeof(buf), "%4d: {%s} said {%s}\n", recNo, rec->name, rec->msg);
                    send(clntsock, buf, len, 0);
                }
                node = node->next;
                recNo++;
            }

            send(clntsock, "\n", 1, 0); 

            // printf("\nlookup: ");
            // fflush(stdout);

            // // see if fgets() produced error
            // if (ferror(stdin)) 
            //     die("stdin");

            // if (ferror(input)) 
            //     die("input");

        }

        fclose(input);
        freemdb(&list);
        close(clntsock);
    }
    close(servsock); 
    return 0;
}
