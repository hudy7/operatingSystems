struct data {
  int id;
  char msg[80];
};

program DISPLAY_PRG {
  version DISPLAY_VER {
    int get(int id) = 1;
    int put(struct data) = 2;
  } = 1;
} = 0x20000001;	
