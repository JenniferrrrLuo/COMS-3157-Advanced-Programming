// Write an improved version, mdb-lookup-server-nc-2.c.

// mdb-lookup-server-nc-1.c from (b) forked once, exec'd the script, waited until
// it's done, and then exited.

// mdb-lookup-server-nc-2.c will do the following:

// - It has a loop in which it displays a prompt, "port number: ".  It reads the
//   port number typed in by the user and then fork/exec the script from (a) using
//   that port number.  It also prints out a message stating that an instance of
//   mdb-lookup-server has started.  The message should include the child's process
//   ID and the port number on which it's listening.

// - Hitting ENTER on the prompt should simply display another prompt.

// - On every iteration, before it displays a prompt, it should check if any of the
//   child processes have terminated.  It should display the process IDs of all
//   mdb-lookup-server processes that have terminated since the last prompt was
//   displayed, along with messages saying that they have terminated.  For this,
//   you need to use the non-blocking version of waitpid() system call.  Here is
//   how you use it:

//   	pid = waitpid( (pid_t) -1, NULL, WNOHANG);

// - Use Ctrl-C to quit.

/*
 * mdb-lookup-server-nc-2.c
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

static void die(const char *s) { perror(s); exit(1); }

int main() {
    char	buf[100];
    pid_t	pid;

    while(1){
        // check if any of the child processes have terminated 
        while ((pid = waitpid( (pid_t) -1, NULL, WNOHANG))> 0) {
            fprintf(stderr, "[pid=%d] ", (int)pid);
            fprintf(stderr, "mdb-lookup-server terminated\n");
        }

        printf("port number: ");

        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            if (feof(stdin)) {
                break;
            } else {
                perror("fgets failed");
                continue;
            }
        }

        if (buf[strlen(buf) - 1] == '\n') {
            buf[strlen(buf) - 1] = 0; // replace newline with '\0'
        }

        if (strlen(buf) == 0){
            continue; 
        }

        pid = fork();
        if (pid < 0) {
            die("fork error");
        } else if (pid == 0) { // child process
            fprintf(stderr, "[pid=%d] ", (int)getpid());
            fprintf(stderr, "mdb-lookup-server started on port %s\n", buf);
            execl("./mdb-lookup-server-nc.sh", "mdb-lookup-server-nc.sh", buf, (char *)0);
            die("execl failed");
        } else { // parent process
           // if (waitpid(pid, NULL,0) != pid){
           //      die("waitpid failed");
           // }
            fprintf(stderr, "[pid=%d] ", (int)pid);
            fprintf(stderr, "mdb-lookup-server started on port %s\n", buf);
        }
    }
}
