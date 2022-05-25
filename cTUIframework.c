#include "keyStrokes.h"
#include "cTUIframework.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>


int screenRows;
int screenCols;
int tab = 0;


int initTUI() {

    nonblock(1);
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


//rendering functions
void _contentRenderer(char *buffer, struct content contents, int x, int y, int height, int width) {
    setChar(buffer, x, y, '>');
    //writeTextBlock(buffer, x, y, height, width, contents.data);
}


void _splitRenderer(char *buffer, struct split split, int x, int y, int height, int width) {
    int type = split.type;
    int xOfset = x;
    int yOfset = y;
    for(int region = 0; region < split.splits; region++) {
        if (type) {
            if(split.split[region].type) {
                _splitRenderer(buffer, *split.split[region].split, xOfset, yOfset, height, split.size[region]);
            } else {
                _contentRenderer(buffer, *split.split[region].content, xOfset, yOfset, height, split.size[region]);
            }
            xOfset += split.size[region];
        } else {
            if(split.split[region].type) {
                _splitRenderer(buffer, *split.split[region].split, xOfset, yOfset, split.size[region], height);
            } else {
                _contentRenderer(buffer, *split.split[region].content, xOfset, yOfset, split.size[region], height);
            }
            yOfset += split.size[region];
        }
    }
}


int renderTUI(struct TUI tuiStruct) {
    //get screen size and initialize the screen string
    updateScreenSize();
    int totalChars = screenRows*(screenCols+1)-1;
    char screen[totalChars];
    for(int ch = 0; ch < totalChars; ch++) {screen[ch] = ' ';}
    for(int row = 1; row < screenRows+1; row++) {screen[(screenCols+1)*row-1] = '\n';}

    //tab bar rendering
    int barPos = 0;
    for(int barTab = 0; barTab < tuiStruct.tabs; ++barTab) {
        if(barTab == tab) {
            setChar(screen, barPos, 0, '>');
            barPos += 1;
        }
        writeLine(screen, barPos, 0, tuiStruct.tab[barTab].name, tuiStruct.tab[barTab].nameLen);
        barPos += tuiStruct.tab[barTab].nameLen+1;
    }

    //tab rendering
    struct tab currentTab = tuiStruct.tab[tab];
    if(currentTab.content->type) {
        _splitRenderer(screen, *currentTab.content->split, 0, 1, screenRows-4, screenCols);
    } else {
        _contentRenderer(screen, *currentTab.content->content, 0, 1, screenRows-4, screenCols);
    }

    //status bar
    writeHorizontalLine(screen, 0, screenRows-2, screenCols, '#');
    setChar(screen, screenCols-1, screenRows-1, getOldestInChar());

    //clear and render screen
    printf("\033[2J\033[H%s", screen);
    return 0;
}
