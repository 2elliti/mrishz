source := src
dest := build


${dest}/mrishz : ${source}/main.c
	gcc ${source}/main.c -o ${dest}/mrishz


