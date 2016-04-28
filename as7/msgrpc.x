struct data {
  char id[32];
  char msg[80];
};

program DISPLAY_PRG {
  version DISPLAY_VER {
    int get(char id) = 1;
    int put(struct data) = 2;
  } = 1;
} = 0x20000777;
