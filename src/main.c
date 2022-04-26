#include "cTUIframework.h"
#include <termios.h>
#include <stdio.h>
#include <unistd.h>


int main() {
    initTUI();
    while (1) {
        updateTUI();
        usleep(50000);
    }
    destroyTUI();
    return 0;
}
