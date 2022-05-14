#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>


char inChar;
char oldestInChar;

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


void pollKeyStrokes() {
    if (kbhit() != 0) {
        inChar = fgetc(stdin);
        oldestInChar = inChar;
    }
    return;
}


char getOldestInChar() {
    return oldestInChar;
}
