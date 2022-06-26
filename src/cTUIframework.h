//keystroke defining struct
struct keystroke {
    char key;
    int type;
    union {
        void (*function)();
        struct keystrokes *recursiveKeystroke;
    };
};


//main struct for holding keystroke definitions
struct keystrokes {
    int keystorkes;
    struct keystroke *keystrokeArray;
};


//holder for contents of a split or tab
struct container {
    int type; //the type of content
    union {
        struct split *split;
        struct content *content;
    };
};


//holder for stuff to be rendered to screen
struct content {
    char **data; //array of strings to write to screen
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
    int nameLen; //lenght of the tab's name
    struct container *content; //content of the tab, either content or a split
};


//main struct for holding all the content to be rendered and its layout
struct TUI {
    int tabs; //the amount of tabs
    struct tab *tab; //array of tabs
    int floatingWidth; //the width of the floating window (disabled if 0)
    int floatingHeight; //the height of the floating window (disabled if 0)
    struct container *floatingWindow; //container for the floating window
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
void renderTUI(struct TUI); //render the TUI struct
void setTab(int tab); //set witch tab to be rendered
void setKeystrokes(struct keystrokes); //set keystrokes to be handled
void setTheme(struct theme);
