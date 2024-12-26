#!/bin/bash
# - You need to make the script file executable using the "chmod +x" command.

on_ctrl_c() {
    echo "Ignoring Ctrl-C"
}
# Call on_ctrl_c() when the interrupt signal is received.
# The interrupt signal is sent when you press Ctrl-C.
trap  on_ctrl_c  INT

# - The script takes one parameter, port number, on which nc will listen.
port_num=$1

# - The script should create a named pipe named mypipe-<pid>, where <pid>
#   indicates the process ID of the shell running the script. 
# - Bash Reference Manual (http://www.gnu.org/software/bash/manual/bashref.html)
#   Section 3.4 tells you how to refer to the arguments and the process ID from
#   your script.
named_pipe="mypipe-$$"

mkfifo "$named_pipe"
cat "$named_pipe" | nc -l "$port_num" | /home/jae/cs3157-pub/bin/mdb-lookup-cs3157  > "$named_pipe"

# The named pipe should be removed at the end of the script.
echo "Removing FIFO"
rm -f "$named_pipe"

