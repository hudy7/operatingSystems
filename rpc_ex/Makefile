
all: east.c west.c main.c bridge.h
	gcc -Wno-cpp -std=c99 -o east east.c
	gcc -Wno-cpp -std=c99 -o west west.c
	gcc -Wno-cpp -std=c99 -g -o main main.c
	chmod 755 main

clean:
	-rm east west main
	-rm -r main.dSYM
