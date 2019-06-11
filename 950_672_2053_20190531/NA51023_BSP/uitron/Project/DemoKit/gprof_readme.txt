under path [BSP]\uitron\Project\DemoKit\DemoKit_Data\Release\
>[delete all files except \ObjectCode and \ObjectLds]

under path [BSP]\uitron\Project\DemoKit
>make release LDS_MAP="ON"

pack youf f/w bin
>make pack

[copy new f/w bin to sdcard]

[reboot with new f/w bin]

[run until profile timing]

> gprof start
gprof open
Profile from 0x00901f74..0x01272c23 in 618699 buckets of size 16
> TFTPD [1a7fde0]: port 69
gprof stop
gprof pause
> gprof dump
gprof dump result
=> [A:\gmon.out] write ok, file size = 1237451


copy A:\gmon.out to [BSP]\uitron\Project\DemoKit\DemoKit_Data\Release\gmon.out


under path A:\gmon.out to [BSP]\uitron\Project\DemoKit\
start NVT Make "dos box"
switch to path [BSP]\NT96510_BSP\uitron\Project\DemoKit\
>gprof_r
Processing file "gmon.out"
Step-1: convert by gprof...
Step-2: convert by symutil...
Ok Please check result in file "gprof.txt.out"
請按任意鍵繼續 . . .


see result to gprof.txt and gprof.txt.out