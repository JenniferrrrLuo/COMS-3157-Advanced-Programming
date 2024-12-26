#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

static void die(const char *s) { perror(s); exit(1); }

static void send_response(int clntsock, char* status_code){
    char response[4096]; 
    int len = snprintf(response, sizeof(response),
            "HTTP/1.0 %s\r\n"
            "Content-Type: text/html\r\n"
            "\r\n"
            "<html><body><h1>%s</h1></body></html>\r\n",
            status_code, status_code);
    
    if (send(clntsock, response, len, 0) != len)
        perror("send failed");
}

static void send_good_response(int clntsock, char* status_code, const char *type, struct stat st){
    char response[4096]; 
    int len = snprintf(response, sizeof(response),
            "HTTP/1.0 %s\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %lld\r\n"
            "\r\n",
            status_code, type, (long long) st.st_size);

    if (send(clntsock, response, len, 0) != len)
        perror("send failed");
}

int connect_to_mdb_server(const char *host, unsigned short port) {

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0){
        die("socket failed");
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(host);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        perror("connect failed");

    return sock;
}
 
void handle_mdb_lookup(int clntsock, const char *requestURI, int mdb_fd) {
    const char *form =
        "<h1>mdb-lookup</h1>\n"
        "<p>\n"
        "<form method=GET action=/mdb-lookup>\n"
        "lookup: <input type=text name=key>\n"
        "<input type=submit>\n"
        "</form>\n"
        "<p>\n";

    const char *header = "HTTP/1.0 200 OK\r\n"
                         "Content-Type: text/html\r\n"
                         "\r\n";

    const char *html_opening = "<html><body>\n";
    const char *html_closing = "</body></html>\n";


    if (send(clntsock, header, strlen(header), 0) != strlen(header) ||
        send(clntsock, html_opening, strlen(html_opening), 0) != strlen(html_opening)) {
        perror("send header or HTML opening failed");
        return;
    }

    if (send(clntsock, form, strlen(form), 0) != strlen(form)) {
        perror("send form failed");
        return;
    }

    if (strncmp(requestURI, "/mdb-lookup?key=", 16) == 0) {
        const char *key = requestURI + 16;

        fprintf(stderr, "looking up [%s]: ", key);

        char query[4096];
        snprintf(query, sizeof(query), "%s\n", key);
        if (send(mdb_fd, query, strlen(query), 0) < 0) {
            perror("send to mdb-lookup-server failed");
            send_response(clntsock, "500 Internal Server Error");
            return;
        }

        const char *table_opening = "<table border=1 style=\"word-wrap:break-word;\">\n";
        if (send(clntsock, table_opening, strlen(table_opening), 0) != strlen(table_opening)) {
            perror("send table opening failed");
            return;
        }

        char buffer[8192];
        char line[8192];
        int line_index = 0;
        int recNo = 0;
        while (1) {
            ssize_t n = recv(mdb_fd, buffer, sizeof(buffer) - 1, 0);
            if (n <= 0) {
                if (n == 0) {
                    fprintf(stderr, "MDB server closed connection.\n");
                } else {
                    perror("recv from mdb-lookup-server failed");
                }
                break;
            }

            buffer[n] = '\0';

            for (size_t i = 0; i < n; i++) {
                if (buffer[i] == '\n') {
                    line[line_index] = '\0';
                    if (strlen(line) == 0) {
                        goto end_response;
                    }

                    char row[8192];
                    int len; 
                    if (recNo % 2 == 0) {
                        len = snprintf(row, sizeof(row), "<tr style=\"background-color:white;\"><td>%s</td></tr>\n", line);
                    } else {
                        len = snprintf(row, sizeof(row), "<tr style=\"background-color:yellow;\"><td>%s</td></tr>\n", line);
                    }

                    if (send(clntsock, row, len, 0) != len) {
                        perror("send row failed");
                        return;
                    }
                    line_index = 0;
                    recNo++;

                } else {
                    if (line_index < sizeof(line) - 1) {
                        line[line_index++] = buffer[i];
                    } else {
                        break;
                    }
                }
            }
        }

    end_response:
        const char *table_closing = "</table>\n";
        if (send(clntsock, table_closing, strlen(table_closing), 0) != strlen(table_closing)) {
            perror("send table closing failed");
            return;
        }
    }

    if (send(clntsock, html_closing, strlen(html_closing), 0) != strlen(html_closing)) {
        perror("send HTML closing failed");
        return;
    }
}


int main(int argc, char **argv){

    if (argc != 5) {
        fprintf(stderr, "usage: %s <server_port> <web_root> <mdb-lookup-host> <mdb-lookup-port>\n", argv[0]);
        exit(1);
    }

    unsigned short server_port = atoi(argv[1]);
    const char *web_root = argv[2];
    const char *mdb_lookup_host = argv[3];
    unsigned short mdb_lookup_port = atoi(argv[4]);

    unsigned short mdb_fd = connect_to_mdb_server(mdb_lookup_host, mdb_lookup_port);

    int servsock;
    if ((servsock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        die("socket failed");

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(server_port);

    if (bind(servsock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        die("bind failed");

    if (listen(servsock, 5 /* queue size for connection requests */ ) < 0)
        die("listen failed");

    int clntsock;
    socklen_t clntlen;
    struct sockaddr_in clntaddr;

    while (1) {

        clntlen = sizeof(clntaddr);

        if ((clntsock = accept(servsock,(struct sockaddr *) &clntaddr, &clntlen)) < 0)
            perror("accept failed");

        const char *client_ip = inet_ntoa(clntaddr.sin_addr);

        char buf[4096];
        char* end;
        ssize_t cnt = 0;

        while ((end = memchr(buf, '\n', cnt))==NULL){
            int r = recv(clntsock, buf + cnt, sizeof(buf)-cnt-1, 0);
            if (r < 0) {
                perror("recv failed");
                close(clntsock);
                continue;
            }
            cnt += r;
            buf[r] = '\0'; 
        }

        char *end_line = strstr(buf, "\r\n");
        if (!end_line) {
            send_response(clntsock, "400 Bad Request");
            close(clntsock);
            continue;
        }

        *end_line = '\0';

        char requestLine[4096];
        strncpy(requestLine, buf, sizeof(requestLine)-1);
        requestLine[sizeof(requestLine)-1] = '\0';

        char *token_separators = "\t \r\n";
        char *method = strtok(requestLine, token_separators);
        char *requestURI = strtok(NULL, token_separators);
        char *httpVersion = strtok(NULL, token_separators);

        if (!method || !requestURI || !httpVersion) {
            send_response(clntsock, "400 Bad Request");
            fprintf(stderr, "%s \"%s\" 400 Bad Request\n", inet_ntoa(clntaddr.sin_addr), requestLine);
            close(clntsock);
            continue;
        }

        if (strcmp(method, "GET") != 0) {
            send_response(clntsock, "501 Not Implemented");
            fprintf(stderr, "%s \"%s\" 501 Not Implemented\n", inet_ntoa(clntaddr.sin_addr), requestLine);
            close(clntsock);
            continue;
        }

        if (strcmp(method, "GET") == 0){
            if (strncmp(requestURI, "/mdb-lookup", 11) == 0) {
                handle_mdb_lookup(clntsock, requestURI, mdb_fd);
                fprintf(stderr, "%s \"%s %s %s\" 200 OK\n", inet_ntoa(clntaddr.sin_addr), method, requestURI, httpVersion);
            } else {

                if (strcmp(httpVersion, "HTTP/1.0") != 0 && strcmp(httpVersion, "HTTP/1.1") != 0) {
                    send_response(clntsock, "501 Not Implemented");
                    fprintf(stderr, "%s \"%s\" 501 Not Implemented\n", inet_ntoa(clntaddr.sin_addr), requestLine);
                    close(clntsock);
                    continue;
                }

                if (requestURI[0] != '/') {
                    send_response(clntsock, "400 Bad Request");
                    fprintf(stderr, "%s \"%s\" 400 Bad Request\n", inet_ntoa(clntaddr.sin_addr), requestLine);
                    close(clntsock);
                    continue;
                }

                if (strstr(requestURI, "/../") != NULL || strcmp(requestURI+strlen(requestURI)-3, "/..") == 0) {
                    send_response(clntsock, "400 Bad Request");
                    fprintf(stderr, "%s \"%s\" 400 Bad Request\n", inet_ntoa(clntaddr.sin_addr), requestLine);
                    close(clntsock);
                    continue;
                }

                char editedURI[4096]; 
                if (requestURI[strlen(requestURI)-1] == '/') {
                    snprintf(editedURI, sizeof(editedURI), "%s%s", requestURI, "index.html");
                } else {
                    snprintf(editedURI, sizeof(editedURI), "%s", requestURI);
                }

                char path[4096]; 
                snprintf(path, sizeof(path), "%s%s", web_root, editedURI);

                struct stat st;
                if (stat(path, &st) < 0) {
                    send_response(clntsock, "404 Not Found");
                    fprintf(stderr, "%s \"%s\" 404 Not Found\n", client_ip, requestLine);
                    close(clntsock);
                    continue;
                }

                if (S_ISDIR(st.st_mode)) {

                    if (editedURI[strlen(editedURI)-1] != '/') {
                        send_response(clntsock, "403 Forbidden");
                        fprintf(stderr, "%s \"%s\" 403 Forbidden\n", client_ip, requestLine);
                        close(clntsock);
                        continue;
                    }

                    if (S_ISDIR(st.st_mode)) {
                        send_response(clntsock, "403 Forbidden");
                        fprintf(stderr, "%s \"%s\" 403 Forbidden\n", client_ip, requestLine);
                        close(clntsock);
                        continue;
                    }
                }

                char *final = strrchr(path, '.');
                final++;
                const char *type = "application/octet-stream";
                if (strcmp(final, "html")==0){
                    type = "text/html";
                }
                if (strcmp(final, "jpg")==0){
                    type = "image/jpeg";
                }

                send_good_response(clntsock, "200 OK", type, st);

                char file_buf[4096];
                ssize_t len;
                int send_failed = 0;

                int fd = open(path, O_RDONLY);
                while ((len = read(fd, file_buf, sizeof(file_buf))) >= 1) {
                    if(send(clntsock, file_buf, len, 0) != len){
                        break;
                    }
                }

                if (!send_failed) {
                    fprintf(stderr, "%s \"%s %s\" 200 OK\n", client_ip, requestLine, editedURI);
                }
            }
        }

        close(clntsock);
    }

    close(mdb_fd);
    close(servsock);
    return 0;

}
