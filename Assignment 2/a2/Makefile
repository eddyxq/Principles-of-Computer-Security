all: overflow format1 format2 format3 env tocttou

sploits: sploit_overflow sploit_format1 sploit_format2 sploit_format3 sploit_env sploit_tocttou

sploit_overflow: sploit_overflow.c
	gcc -o sploit_overflow sploit_overflow.c

sploit_format1: sploit_format1.c
	gcc -o sploit_format1 sploit_format1.c

sploit_format2: sploit_format2.c
	gcc -o sploit_format2 sploit_format2.c

sploit_format3: sploit_format3.c
	gcc -o sploit_format3 sploit_format3.c

sploit_env: sploit_env.c
	gcc -o sploit_env sploit_env.c

sploit_tocttou: sploit_tocttou.c
	gcc -o sploit_tocttou sploit_tocttou.c

overflow: overflow.c
	gcc -fno-stack-protector -ggdb -z execstack -o overflow overflow.c
	/bin/chgrp2 ./overflow

format1: format1.c
	gcc -Wno-format-security -ggdb -o format1 format1.c
	/bin/chgrp2 ./format1

format2: format2.c
	gcc -Wno-format-security -ggdb -o format2 format2.c
	/bin/chgrp2 ./format2

format3: format3.c
	gcc -Wno-format-security -ggdb -o format3 format3.c
	/bin/chgrp2 ./format3

env: env.c
	gcc -ggdb -o env env.c
	/bin/chgrp2 ./env

tocttou: tocttou.c
	gcc -ggdb -o tocttou tocttou.c
	/bin/chgrp2 ./tocttou

clean:
	rm -f overflow format1 format2 format3 env tocttou sploit_overflow sploit_format1 sploit_format2 sploit_format3 sploit_env sploit_tocttou
