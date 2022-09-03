#include "cTUIFramework.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <termios.h>
#include "stdlib.h"


int screenRows;
int screenCols;
int tab = 0;
char latestKeyStrokes[10];
int keystrokeDisplayLength = 0;
struct keystrokes baseKeyStrokes;
struct keystrokes currentKeyStrokes;
struct theme theme;


void initTUI() {
    //sets the terminal to non blocking mode
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_cc[VMIN] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}


void destroyTUI() {
    //unsets non blocking mode
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag |= ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);}


void setTab(int newTab) {
    tab = newTab;
}


void setKeystrokes(struct keystrokes keyStrokes) {
    baseKeyStrokes = keyStrokes;
}


//checks if there are chars left in the buffer from user inputs
int kbhit() {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}


int pollKeyStrokes(char *array) {
    int ch = 0;
    while(1) {
        if (kbhit() != 0 && ch < 10) {
            array[ch] = fgetc(stdin);
            ch++;
        } else {break;}
    }
    return ch;
}


void updateTUIKeystrokes() {
    char inKeys[10];
    int inputs = pollKeyStrokes(inKeys);
    for(int key = 0; key < inputs; key++) {
        char inKey = inKeys[key];
        int failed = 1;
        for(int keyStroke = 0; keyStroke < currentKeyStrokes.keystorkes; keyStroke++) {
            if(inKey == currentKeyStrokes.keystrokeArray[keyStroke].key) {
                if(currentKeyStrokes.keystrokeArray[keyStroke].type) {
                    currentKeyStrokes = *currentKeyStrokes.keystrokeArray[keyStroke].recursiveKeystroke;
                    keystrokeDisplayLength++;
                    latestKeyStrokes[keystrokeDisplayLength] = inKey;
                } else {
                    currentKeyStrokes.keystrokeArray[keyStroke].function();
                    keystrokeDisplayLength = 0;
                }
                failed = 0;
                break;
            }
        }
        if(failed) {
            currentKeyStrokes = baseKeyStrokes;
            keystrokeDisplayLength = 0;
        }
    }
}


void updateScreenSize() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    screenRows = w.ws_row-1;
    screenCols = w.ws_col;
}


void setTheme(struct theme newTheme) {
    theme = newTheme;
}


int getScreenWidth() {
    return screenCols;
}


int getScreenHeight() {
    return screenRows;
}


//draw functions


void _setChar(char *buffer, int x, int y, char ch) {
    buffer[x + y * (screenCols + 1)] = ch;
}


void _writeLine(char *buffer, int x, int y, char *str) {
    int strLength = strlen(str);
    for(int ch = 0; ch < strLength; ch++) {
        buffer[x + ch + y * (screenCols + 1)] = str[ch];
    }
}


void _writeTextBlock(char *buffer, int x, int y, int lines, int cols, char **str) {
    for(int line = 0; line < lines; line++) {
        for(int col = 0; col < cols; col++) {
            buffer[x + col + (y + line) * (screenCols + 1)] = str[line][col];
        }
    }
}


void _writeHorizontalLine(char *buffer, int x, int y, int length, char ch) {
    for(int col = 0; col < length; col++) {
        buffer[x + col + y * (screenCols + 1)] = ch;
    }
}


void _writeVerticalLine(char *buffer, int x, int y, int length, char ch) {
    for(int row = 0; row < length; row++) {
        buffer[x + (y + row) * (screenCols + 1)] = ch;
    }
}


//rendering functions


void _contentRenderer(char *buffer, char **(*func)(int, int, int), int x, int y, int width, int height, int id) {
    char **content = func(width, height, id);
    _writeTextBlock(buffer, x, y, width, height, content);
    free(content);
}


char **initWindowContent(int width, int height, char background) {
    char **content = (char **)malloc(sizeof(char *) * width + sizeof(char) * width * height);
    char *ptr = (char *)(content + width);
    for(int i = 0; i < width + 1; i++) content[i] = (ptr + height * i);
    for (int line = 0; line < width; line++) {
        for (int col = 0; col < height; col++) {
            content[line][col] = background;
        }
    }
    return content;
}


void _splitRenderer(char *buffer, struct split split, int x, int y, int height, int width, int topBorder, int sideBorder) {
    int type = split.type;
    int xOfset = x;
    int yOfset = y;
    for(int region = 0; region < split.splits; region++) {
        if(type) {
            if(split.split[region].type) {
                _splitRenderer(buffer, *split.split[region].split, xOfset, yOfset, height, split.size[region], topBorder, theme.splitBorders);
            } else {
                _contentRenderer(buffer, *split.split[region].content, xOfset, yOfset, height, split.size[region], split.split[region].id);
            }
            xOfset += split.size[region];
        } else {
            if(split.split[region].type) {
                _splitRenderer(buffer, *split.split[region].split, xOfset, yOfset, split.size[region], height, theme.splitBorders, sideBorder);
            } else {
                _contentRenderer(buffer, *split.split[region].content, xOfset, yOfset, split.size[region], height, split.split[region].id);
            }
            yOfset += split.size[region];
        }
        if(theme.splitBorders && region != split.splits - 1) {
            if(type) {
                _writeVerticalLine(buffer, xOfset, yOfset, height, '|');
                if(topBorder) {
                    if(yOfset > 1) _setChar(buffer, xOfset, yOfset - 1, '+');
                    if(yOfset + height > screenRows - 4) _setChar(buffer, xOfset, yOfset + height, '+');
                }
                xOfset++;
            } else {
                _writeHorizontalLine(buffer, xOfset, yOfset, width - 1, '-');
                if(sideBorder) {
                    if(xOfset > 0) _setChar(buffer, xOfset - 1, yOfset, '+');
                    if(xOfset + width < screenCols) _setChar(buffer, xOfset + width - 1, yOfset, '+');
                }
                yOfset++;
            }
        }
    }
}


void renderTUI(struct TUI tuiStruct) {
    //get screen size and initialize the screen string
    int totalChars = screenRows * (screenCols + 1) - 1;
    char screen[totalChars];
    for(int ch = 0; ch < totalChars; ch++) {screen[ch] = ' ';}
    for(int row = 1; row < screenRows + 1; row++) {screen[(screenCols + 1) * row - 1] = '\n';}

    //tab bar rendering
    int barPos = 0;
    for(int barTab = 0; barTab < tuiStruct.tabs; ++barTab) {
        if(barTab == tab) {
            _setChar(screen, barPos, 0, '>');
            barPos += 1;
        }
        _writeLine(screen, barPos, 0, tuiStruct.tab[barTab].name);
        barPos += strlen(tuiStruct.tab[barTab].name) + 1;
    }
    int tabBar = 1;
    if(theme.topBarBorder) {
        _writeHorizontalLine(screen, 0, 1, screenCols, '-');
        tabBar++;
    }

    //status bar
    int rightLen = strlen(tuiStruct.barRight);
    _writeLine(screen, 0, screenRows - 2, tuiStruct.barLeft);
    _writeLine(screen, screenCols - rightLen, screenRows - 2, tuiStruct.barRight);
    int tabSize = screenRows - 4;
    if(theme.bottomBarBorder) {
        _writeHorizontalLine(screen, 0, screenRows - 3, screenCols, '-');
        tabSize--;
    }

    //tab rendering
    int EdgeMargin = 0;
    if(theme.sideEdgeBorder) {
        _writeVerticalLine(screen, 0, tabBar, tabSize, '|');
        _writeVerticalLine(screen, screenCols - 1, tabBar, tabSize, '|');
        if(theme.topBarBorder) {
            _setChar(screen, 0, tabBar - 1, '+');
            _setChar(screen, screenCols - 1, tabBar - 1, '+');
        }
        if(theme.bottomBarBorder) {
            _setChar(screen, 0, screenRows - 3, '+');
            _setChar(screen, screenCols - 1, screenRows - 3, '+');
        }
        EdgeMargin++;
    }
    struct tab currentTab = tuiStruct.tab[tab];
    if(currentTab.content->type) {
        _splitRenderer(screen, *currentTab.content->split, EdgeMargin, tabBar, tabSize, screenCols - EdgeMargin, theme.bottomBarBorder + theme.topBarBorder, theme.sideEdgeBorder);
    } else {
        _contentRenderer(screen, *currentTab.content->content, EdgeMargin, tabBar, tabSize, screenCols - EdgeMargin, currentTab.content->id);
    }

    //floating window rendering
    if(tuiStruct.floatingWindow->height && tuiStruct.floatingWindow->width) {
        int floatingHeight = tuiStruct.floatingWindow->height;
        int floatingWidth = tuiStruct.floatingWindow->width;
        int floatingX = screenCols / 2 - floatingWidth / 2;
        int floatingY = screenRows / 2 - floatingHeight / 2;
        if(theme.floatingWindowBorders) {
            _writeHorizontalLine(screen, floatingX, floatingY, floatingHeight, '-');
            _writeHorizontalLine(screen, floatingX, floatingY + floatingWidth, floatingHeight, '-');
            _writeVerticalLine(screen, floatingX, floatingY, floatingWidth, '|');
            _writeVerticalLine(screen, floatingX + floatingHeight, floatingY, floatingWidth, '|');
            _setChar(screen, floatingX, floatingY, '+');
            _setChar(screen, floatingX, floatingY + floatingWidth, '+');
            _setChar(screen, floatingX + floatingHeight, floatingY, '+');
            _setChar(screen, floatingX + floatingHeight, floatingY + floatingWidth, '+');
            floatingX++;
            floatingY++;
            floatingHeight -= 1;
            floatingWidth -= 1;
        }

        struct container floatingWindow = tuiStruct.floatingWindow->window;
        if(floatingWindow.type) {
            _splitRenderer(screen, *floatingWindow.split, floatingX, floatingY, floatingWidth, floatingHeight, theme.floatingWindowBorders, theme.floatingWindowBorders);
        } else {
            _contentRenderer(screen, *floatingWindow.content, floatingX, floatingY, floatingWidth, floatingHeight, floatingWindow.id);
        }
    }

    //keystroke indicator
    if(keystrokeDisplayLength) {
        for(int ch = keystrokeDisplayLength; ch; ch--) {
            _setChar(screen, screenCols - ch, screenRows - 1, latestKeyStrokes[ch]);
        }
    }

    //clear and print screen
    printf("\033[2J\033[H%s", screen);
}
