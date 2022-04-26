#include "cTUIframework.h"
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include "drawHelpers.h"


int main() {
    int rows = getTermRows();
    int cols = getTermCols();
    clearScreen();
    setChar(10,10,'A');
    writeLine(15, 10, "test123");
    char *block[3];
    block[0] = "block test!";
    block[1] = "uwu";
    block[2] = "cheese";
    writeTextBlock(1,5,3,block);
    writeVerticalLine(4,0,rows,'I');


    initTUI();
    while (1) {
        updateTUI();
        usleep(50000);
    }
    destroyTUI();
    return 0;
}
