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
    tab2.name = "testTab2";
    tab3.name = "testTab3";
    struct container tabContent;
    tabContent.type = split;
    struct split split1;
    split1.splits = 2;
    split1.type = split;
    int sizes[2] = {10, 10};
    split1.size = sizes;
    struct content testcontent;
    struct container asd;
    asd.type = content;
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
    tuiStruct.barLeft = "left owo";
    tuiStruct.barRight = "right UwU";
    struct floatingWindow floatingWindow;
    floatingWindow.height = 10;
    floatingWindow.width = 20;
    floatingWindow.window = asd;
    tuiStruct.floatingWindow = &floatingWindow;

    struct keystrokes keystroke;
    keystroke.keystorkes = 2;
    struct keystroke teststroke[2];
    teststroke[0].key = 't';
    teststroke[0].type = final;
    teststroke[0].function = keytestidk;
    struct keystrokes recursiveTest;
    recursiveTest.keystorkes = 1;
    struct keystroke recursiveArray[1];
    recursiveArray[0].key = 'a';
    recursiveArray[0].type = final;
    recursiveArray[0].function = quit;
    recursiveTest.keystrokeArray = recursiveArray;


    teststroke[1].key = 'q';
    teststroke[1].type = recursive;
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
