#include "cTUIframework.h"
#include <termios.h>
#include <stdio.h>
#include <unistd.h>


int main() {
    initTUI();
    //testing definitions
    struct TUI tuiStruct;
    struct tab tab1, tab2, tab3;
    char *test = "asdad";
    tab1.name = "testTab1";
    tab1.nameLen = 8;
    tab2.name = "testTab2";
    tab2.nameLen = 8;
    tab3.name = "testTab3";
    tab3.nameLen = 8;
    struct tab tabs[3] = {tab1, tab2, tab3};
    tuiStruct.tabs = 3;
    tuiStruct.tab = tabs;
    tuiStruct.floatingWidth = 0;
    tuiStruct.floatingHeight = 0;


    while (1) {
        updateTUI();
        renderTUI(tuiStruct);
        usleep(50000);
    }
    destroyTUI();
    return 0;
}
