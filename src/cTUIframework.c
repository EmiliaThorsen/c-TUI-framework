#include "keyStrokes.h"
#include "cTUIframework.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>


int screenRows;
int screenCols;
int tab = 0;
char latestKeyStrokes[10];
int keystrokeDisplayLength = 0;
struct keystrokes baseKeyStrokes;
struct keystrokes currentKeyStrokes;
struct theme theme;



void initTUI() {
    nonblock(1);
}


void destroyTUI() {
    nonblock(0);
}


void setTab(int tab) {
    tab = tab;
}


void setKeystrokes(struct keystrokes keyStrokes) {
    baseKeyStrokes = keyStrokes;
}


void updateTUIKeystrokes() {
    struct inputKeys input = pollKeyStrokes();
    for(int key = 0; key < input.inputs; key++) {
        char inKey = input.inKeys[key];
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
void setChar(char *buffer, int x, int y, char ch) {
    buffer[x+y*(screenCols+1)] = ch;
}


void writeLine(char *buffer, int x, int y, char *str) {
    int strLength = strlen(str);
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


void writeHorizontalLine(char *buffer, int x, int y, int length, char ch) {
    for (int col = 0; col < length; col++) {
        buffer[x+col+y*(screenCols+1)] = ch;
    }
}


void writeVerticalLine(char *buffer, int x, int y, int length, char ch) {
    for (int row = 0; row < length; row++) {
        buffer[x+(y+row)*(screenCols+1)] = ch;
    }
}


//rendering functions
void _contentRenderer(char *buffer, struct content contents, int x, int y, int height, int width) {
    setChar(buffer, x, y, '>');
    //writeTextBlock(buffer, x, y, height, width, contents.data);
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
                _contentRenderer(buffer, *split.split[region].content, xOfset, yOfset, height, split.size[region]);
            }
            xOfset += split.size[region];
        } else {
            if(split.split[region].type) {
                _splitRenderer(buffer, *split.split[region].split, xOfset, yOfset, split.size[region], height, theme.splitBorders, sideBorder);
            } else {
                _contentRenderer(buffer, *split.split[region].content, xOfset, yOfset, split.size[region], height);
            }
            yOfset += split.size[region];
        }
        if(theme.splitBorders && region != split.splits - 1) {
            if(type) {
                writeVerticalLine(buffer, xOfset, yOfset, height, '|');
                if(topBorder) {
                    if(yOfset > 1) setChar(buffer, xOfset, yOfset - 1, '+');
                    if(yOfset + height > screenRows-4) setChar(buffer, xOfset, yOfset + height, '+');
                }
                xOfset++;
            } else {
                writeHorizontalLine(buffer, xOfset, yOfset, width - 1, '-');
                if(sideBorder) {
                    if(xOfset > 0) setChar(buffer, xOfset - 1, yOfset, '+');
                    if(xOfset + width < screenCols) setChar(buffer, xOfset + width - 1, yOfset, '+');
                }
                yOfset++;
            }
        }
    }
}


void renderTUI(struct TUI tuiStruct) {
    //get screen size and initialize the screen string
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
        writeLine(screen, barPos, 0, tuiStruct.tab[barTab].name);
        barPos += strlen(tuiStruct.tab[barTab].name) + 1;
    }
    int tabBar = 1;
    if(theme.topBarBorder) {
        writeHorizontalLine(screen, 0, 1, screenCols, '-');
        tabBar++;
    }

    //status bar
    int rightLen = strlen(tuiStruct.barRight);
    writeLine(screen, 0, screenRows - 2, tuiStruct.barLeft);
    writeLine(screen, screenCols - rightLen, screenRows - 2, tuiStruct.barRight);
    int tabSize = screenRows - 4;
    if(theme.bottomBarBorder) {
        writeHorizontalLine(screen, 0, screenRows - 3, screenCols, '-');
        tabSize--;
    }

    //tab rendering
    int EdgeMargin = 0;
    if(theme.sideEdgeBorder) {
        writeVerticalLine(screen, 0, tabBar, tabSize, '|');
        writeVerticalLine(screen, screenCols - 1, tabBar, tabSize, '|');
        if(theme.topBarBorder) {
            setChar(screen, 0, tabBar - 1, '+');
            setChar(screen, screenCols - 1, tabBar - 1, '+');
        }
        if(theme.bottomBarBorder) {
            setChar(screen, 0, screenRows- 3, '+');
            setChar(screen, screenCols - 1, screenRows - 3, '+');
        }
        EdgeMargin++;
    }
    struct tab currentTab = tuiStruct.tab[tab];
    if(currentTab.content->type) {
        _splitRenderer(screen, *currentTab.content->split, EdgeMargin, tabBar, tabSize, screenCols-EdgeMargin, theme.bottomBarBorder + theme.topBarBorder, theme.sideEdgeBorder);
    } else {
        _contentRenderer(screen, *currentTab.content->content, EdgeMargin, tabBar, tabSize, screenCols-EdgeMargin);
    }

    //floating window rendering
    if(tuiStruct.floatingWindow->height && tuiStruct.floatingWindow->width) {
        int floatingHeight = tuiStruct.floatingWindow->height;
        int floatingWidth = tuiStruct.floatingWindow->width;
        int floatingX = screenCols/2 - floatingWidth/2;
        int floatingY = screenRows/2 - floatingHeight/2;
        if(theme.floatingWindowBorders) {
            writeHorizontalLine(screen, floatingX, floatingY, floatingWidth, '-');
            writeHorizontalLine(screen, floatingX, floatingY + floatingHeight, floatingWidth, '-');
            writeVerticalLine(screen, floatingX, floatingY, floatingHeight, '|');
            writeVerticalLine(screen, floatingX + floatingWidth, floatingY, floatingHeight, '|');
            setChar(screen, floatingX, floatingY, '+');
            setChar(screen, floatingX, floatingY + floatingHeight, '+');
            setChar(screen, floatingX + floatingWidth, floatingY, '+');
            setChar(screen, floatingX + floatingWidth, floatingY + floatingHeight, '+');
            floatingX++;
            floatingY++;
            floatingHeight -= 2;
            floatingWidth -= 2;
        }

        struct container floatingWindow = tuiStruct.floatingWindow->window;
        if(floatingWindow.type) {
            _splitRenderer(screen, *floatingWindow.split, floatingX, floatingY, floatingWidth, floatingHeight, theme.floatingWindowBorders, theme.floatingWindowBorders);
        } else {
            _contentRenderer(screen, *floatingWindow.content, floatingX, floatingY, floatingWidth, floatingHeight);
        }
    }

    //keystroke indicator
    int keystrokeIndicatorLen = strlen(latestKeyStrokes);
    if(keystrokeDisplayLength) {
        for(int ch = keystrokeDisplayLength; ch; ch--) {
            setChar(screen, screenCols-ch, screenRows-1, latestKeyStrokes[ch]);
        }
    }

    //clear and render screen
    printf("\033[2J\033[H%s", screen);
}
