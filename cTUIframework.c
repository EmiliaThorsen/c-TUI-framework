#include "keyStrokes.h"
#include "cTUIframework.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>


struct TUI tuiStruct;
int screenRows;
int screenCols;
int tab;
int test;


int initTUI() {
    nonblock(1);
    test = 0;
    return 0;
}


int destroyTUI() {
    nonblock(0);
    return 0;
}


int updateTUI() {
    pollKeyStrokes();
    return 0;
}



void updateScreenSize() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    screenRows = w.ws_row-1;
    screenCols = w.ws_col;
}


//draw functions
void setChar(char *buffer, int x, int y, char ch) {
    buffer[x+y*(screenCols+1)] = ch;
}


void writeLine(char *buffer, int x, int y, char *str, int strLength) {
    for (int ch = 0; ch < strLength; ch++) {
        buffer[x+ch+y*(screenCols+1)] = str[ch];
    }
}


void writeTextBlock(char *buffer, int x, int y, int lines, int cols, char *str[]) {
    for(int line = 0; line < lines; line++) {
        for (int ch = 0; ch < cols; ch++) {
            buffer[x+ch+(y+line)*(screenCols+1)] = str[line][ch];
        }
    }
}


void writeHorizontalLine(char *buffer, int x, int y, int lenght, char ch) {
    for (int col = 0; col < lenght; col++) {
        buffer[x+col+y*(screenCols+1)] = ch;
    }
}


void writeVerticalLine(char *buffer, int x, int y, int lenght, char ch) {
    for (int row = 0; row < lenght; row++) {
        buffer[x+(y+row)*(screenCols+1)] = ch;
    }
}


int renderTUI() {
    //get screen size and initialize the screen string
    updateScreenSize();
    int totalChars = screenRows*(screenCols+1)-1;
    char screen[totalChars];
    for(int ch = 0; ch < totalChars; ch++) {screen[ch] = ' ';}
    for (int row = 1; row < screenRows+1; row++) {screen[(screenCols+1)*row-1] = '\n';}

    setChar(screen, 1, 0, 'a');
    writeLine(screen, 0, 3, "im gay", 6);
    char *block[5] = {"uwu ", "abcd", "gay ", "boop", "boob"};
    writeTextBlock(screen, 5, 5, 5, 4, block);
    writeHorizontalLine(screen, 0, screenRows-15, screenCols, '#');
    writeVerticalLine(screen, 10, 0, 30, 'O');
    setChar(screen, screenCols-1, 30, getOldestInChar());

    //test animation
    test++;
    if (test == 10) {
       test = 0; 
    }
    setChar(screen, 10+test, 10, 'O');

    //clear and render screen
    printf("\033[2J\033[H%s", screen);
    return 0;
}
