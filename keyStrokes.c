#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include "keyStrokes.h"


void nonblock(int state) {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    if(state) {
        ttystate.c_lflag &= ~ICANON;
        ttystate.c_cc[VMIN] = 0;
    } else {
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


struct inputKeys pollKeyStrokes() {
    char inChar[10];
    int ch = 0;
    while(1) {
        if (kbhit() != 0 && ch < 10) {
            inChar[ch] = fgetc(stdin);
            ch++;
        } else {break;}
    }
    struct inputKeys output;
    output.inputs = ch;
    output.inKeys = inChar;
    return output;
}
