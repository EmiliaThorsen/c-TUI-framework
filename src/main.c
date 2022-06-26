#include "cTUIframework.h"
#include <termios.h>
#include <stdio.h>
#include <unistd.h>


int running = 1;
int testThing = 10;


void keytestidk() {
    testThing += 10;
}


void quit() {
    running = 0;
}

int main() {
    initTUI();
    //testing definitions

    struct theme theme;
    theme.splitBorders = 1;
    theme.floatingWindowBorders = 1;
    theme.topBarBorder = 1;
    theme.sideEdgeBorder = 1;
    theme.bottomBarBorder = 1;

    setTheme(theme);
    struct TUI tuiStruct;
    struct tab tab1, tab2, tab3;
    tab1.name = "testTab1";
    tab1.nameLen = 8;
    tab2.name = "testTab2";
    tab2.nameLen = 8;
    tab3.name = "testTab3";
    tab3.nameLen = 8;
    struct container tabContent;
    tabContent.type = 1;
    struct split split1;
    split1.splits = 2;
    split1.type = 1;
    int sizes[2] = {10, 10};
    split1.size = sizes;
    struct content testcontent;
    struct container asd;
    asd.type = 0;
    asd.content = &testcontent;
    struct container asdarr[2];
    asdarr[0] = asd;
    asdarr[1] = asd;
    split1.split = asdarr;
    tabContent.split = &split1;
    tab1.content = &tabContent;
    struct tab tabs[3] = {tab1, tab2, tab3};
    tuiStruct.tabs = 3;
    tuiStruct.tab = tabs;
    tuiStruct.floatingWidth = 0;
    tuiStruct.floatingHeight = 0;
    tuiStruct.barLeft = "left owo";
    tuiStruct.barRight = "right UwU";
    tuiStruct.floatingHeight = 10;
    tuiStruct.floatingWidth = 20;
    tuiStruct.floatingWindow = &asd;
    
    struct keystrokes keystroke;
    keystroke.keystorkes = 2;
    struct keystroke teststroke[2];
    teststroke[0].key = 't';
    teststroke[0].type = 0;
    teststroke[0].function = keytestidk;
    struct keystrokes recursiveTest;
    recursiveTest.keystorkes = 1;
    struct keystroke recursive[1];
    recursive[0].key = 'a';
    recursive[0].type = 0;
    recursive[0].function = quit;
    recursiveTest.keystrokeArray = recursive;


    teststroke[1].key = 'q';
    teststroke[1].type = 1;
    teststroke[1].recursiveKeystroke = &recursiveTest;
    keystroke.keystrokeArray = teststroke;
    
    setKeystrokes(keystroke);
    while (running) {
        tuiStruct.tab[0].content->split->size[0] = testThing;
        updateTUIKeystrokes();
        updateScreenSize();
        renderTUI(tuiStruct);
        usleep(50000);
    }
    destroyTUI();
    return 0;
}
