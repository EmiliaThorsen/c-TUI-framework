#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>


void nonblock(int state) {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    if (state==1) {
        ttystate.c_lflag &= ~ICANON;
        ttystate.c_cc[VMIN] = 0;
    } else if (state==0) {
        ttystate.c_lflag |= ICANON;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}


int kbhit() {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}


int initTUI() {
    nonblock(1);
    return 0;
}


int destroyTUI() {
    nonblock(0);
    return 0;
}


int updateTUI() {
    char c;
    if (kbhit() != 0) {
        c = fgetc(stdin); 
        printf("user hit: %c\n", c);
    }
    printf("asd\n");
    return 0;
}
