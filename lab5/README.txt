- Jennifer Luo
- dl3572
- lab5
- This lab works exactly as specified. 


0       1       1       1 ?             -1 Ss       0  35:41 /sbin/init
      1    2163    2163    2163 ?             -1 Ss       0   2:58 sshd: /usr/sbin/sshd -D [listener] 1 of 10-100 startups
   2163 1687865 1687865 1687865 ?             -1 Ss       0   0:00  \_ sshd: dl3572 [priv]
1687865 1687884 1687865 1687865 ?             -1 S     2344   0:00  |   \_ sshd: dl3572@pts/39
1687884 1687885 1687885 1687885 pts/39   1693806 Ss    2344   0:00  |       \_ -bash
1687885 1693806 1693806 1687885 pts/39   1693806 S+    2344   0:00  |           \_ ./mdb-lookup-server-nc-1 10344
1693806 1693807 1693806 1687885 pts/39   1693806 S+    2344   0:00  |               \_ /bin/bash ./mdb-lookup-server-nc.sh 10344
1693807 1693809 1693806 1687885 pts/39   1693806 S+    2344   0:00  |                   \_ cat mypipe-1693807
1693807 1693810 1693806 1687885 pts/39   1693806 S+    2344   0:00  |                   \_ nc -l 10344
1693807 1693811 1693806 1687885 pts/39   1693806 S+    2344   0:00  |                   \_ /bin/sh /home/jae/cs3157-pub/bin/mdb-lookup-cs3157
1693811 1693812 1693806 1687885 pts/39   1693806 S+    2344   0:00  |                       \_ /home/jae/cs3157-pub/bin/mdb-lookup /home/jae/cs3157-pub/bin/mdb-cs3157

shell scripts: 
mdb-lookup-server-nc.sh
mdb-lookup-cs3157
