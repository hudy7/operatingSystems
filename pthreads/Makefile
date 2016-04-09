all: main.c hydrogen.c methane_threads.h
	gcc -Wno-cpp -std=c99 -o main main.c
	gcc -Wno-cpp -std=c99 -o hydrogen hydrogen.c
	gcc -Wno-cpp -std=c99 -o carbon carbon.c
	chmod 755 main

clean:
	-rm carbon hydrogen main
