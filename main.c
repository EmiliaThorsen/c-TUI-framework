#include "cTUIframework.h"
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include "drawHelpers.h"


int main() {
    initTUI();
    while (1) {
        updateTUI();
        renderTUI();
        usleep(50000);
    }
    destroyTUI();
    return 0;
}
