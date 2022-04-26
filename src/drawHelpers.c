#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

void clearScreen() {
    printf("\033[2J");
    return;
}


void setChar(int x, int y, char ch) {
    printf("\033[%i;%iH%c", y, x, ch);
    return;
}


void writeLine(int x, int y, char *str) {
    printf("\033[%i;%iH%s", y, x, str);
    return;
}


void writeTextBlock(int x, int y, int lines, char *str[]) {
    for(int line = 0; line < lines; line++) {
        writeLine(x, y+line,str[line]);
    }
    return;
}


void writeVerticalLine(int x, int y, int length, char ch) {
    for(int line = 0; line < length; line++) {
        setChar(x, y+line, ch);
    }
    return;
}


int getTermRows() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
}


int getTermCols() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}
