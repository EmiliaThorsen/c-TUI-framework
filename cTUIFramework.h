enum {
    final,
    recursive
};


//keystroke defining struct
struct keystroke {
    char key;
    int type;
    int id;
    union {
        void (*function)(int);
        struct keystrokes *recursiveKeystroke;
    };
};


//main struct for holding keystroke definitions
struct keystrokes {
    int keystorkes;
    struct keystroke *keystrokeArray;
};


enum {
    content,
    split
};


//holder for contents of a split or tab
struct container {
    int type; //the type of content
    int id; //id of the window or split, used to identify what window is being rendered in your program
    union {
        struct split *split;
        char **(*content)(int, int, int);
    };
};


//struct defining splits in your tui
struct split {
    int splits; //amout of splits
    int type; //vertical or horizontal splits
    int *size; //the size of the splits in characters
    struct container *split; //contents of the splits
};


//struct defining tabs in your tui
struct tab {
    char *name; //name of the tab
    struct container *content; //content of the tab, either content or a split
};


//struct defining the floating window
struct floatingWindow {
    int height; //if height or width is 0 will the floating window be disabled
    int width;
    struct container window;
};


//main struct for holding all the content to be rendered and its layout
struct TUI {
    int tabs; //the amount of tabs
    struct tab *tab; //array of tabs
    struct floatingWindow *floatingWindow; //struct defining the centered floating window
    char *barRight; //text on the right side of the status bar
    char *barLeft; //text on the left side of the status bar
};


//struct defining estetical changes
struct theme {
    int splitBorders;
    int floatingWindowBorders;
    int topBarBorder;
    int sideEdgeBorder;
    int bottomBarBorder;
};


void initTUI (); //initiate TUI
void destroyTUI(); //destroy TUI
void updateTUIKeystrokes(); //update keystroke handler
void updateScreenSize();
char **initWindowContent(int, int, char); //mallocs a 2d array of chars for window content
void renderTUI(struct TUI); //render the TUI struct
void setTab(int tab); //set witch tab to be rendered
void setKeystrokes(struct keystrokes); //set keystrokes to be handled
void setTheme(struct theme);
int getScreenWidth();
int getScreenHeight();
