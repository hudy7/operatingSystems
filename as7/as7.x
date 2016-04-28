
struct data {
    int id;
    char message[2000];
};

program DISPLAY_PRG {
    version DISPLAY_VER {
        int get(int id) = 1;
	    int put(struct data) = 2;
    } = 1;
} = 0x20000001;