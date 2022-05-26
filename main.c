#include "cTUIframework.h"
#include <termios.h>
#include <stdio.h>
#include <unistd.h>


int main() {
    initTUI();
    //testing definitions
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
    split1.type = 0;
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

    while (1) {
        updateTUI();
        renderTUI(tuiStruct);
        usleep(50000);
    }
    destroyTUI();
    return 0;
}