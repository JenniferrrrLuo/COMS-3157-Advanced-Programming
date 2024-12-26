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
#include <netdb.h>


static void die(const char *s) { perror(s); exit(1); }

int main(int argc, char **argv)
{
    if (argc != 4) {
        fprintf(stderr, "usage: %s <host> <port-number> <file-path> \n", argv[0]);
        exit(1);
    }

    const char* host = argv[1];
    unsigned short port_number = atoi(argv[2]);
    const char* file_path = argv[3];

    // Create a socket for TCP connection

    int sock; // socket descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        die("socket failed");

    struct hostent *he;
    // get server ip from server name
    if ((he = gethostbyname(host)) == NULL) {
        die("gethostbyname failed");
    }
    char *serverIP = inet_ntoa(*(struct in_addr *)he->h_addr);

    // Construct a server address structure

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr)); // must zero out the structure
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(serverIP);
    servaddr.sin_port        = htons(port_number); // must be in network byte order

    // Establish a TCP connection to the server

    if (connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        die("connect failed");
    char buf[4096];
    int len = snprintf(buf, sizeof(buf), 
                       "GET %s HTTP/1.0\r\n"
                       "Host: %s:%d\r\n"
                       "\r\n", 
                       file_path, host, port_number);
    if (send(sock, buf, len, 0) != len)
        die("send failed");

    FILE *input = fdopen(sock, "r");
    if (input == NULL) {
        close(sock);
        die("fdopen failed");
    }
    
    char line[1000];
    fgets(line, sizeof(line), input); 
    char version[100]; 
    int status; 
    sscanf(line, "%99s %d", version, &status); 

    if ((strcmp(version, "HTTP/1.0") != 0 && strcmp(version, "HTTP/1.1") != 0) || status != 200) {
        fprintf(stderr, "%s", line);
        fclose(input);
        exit(1); 
    }
    
    while (fgets(line, sizeof(line), input) != NULL) { 
        if (strcmp(line, "\r\n") == 0){
            break; 
        }
    }
    const char *filename = strrchr(file_path, '/');
    if(filename == NULL){
        filename = file_path;
    }
    else{
        filename++;
    }
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        fclose(input);
        die("fopen");
    }

    char buf_2[4096];
    unsigned int n;

    while ((n = fread(buf_2, 1, sizeof(buf_2), input)) > 0) {
        fwrite(buf_2, 1, n, fp);
    }

    fclose(fp); 
    fclose(input); 
    return 0;

}
