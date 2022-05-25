struct container {
    int type; //the type of content
    union {
        struct split *split;
        struct content *content;
    };
};


struct content {
    char **data; //array of strings to write to screen
};


struct split {
    int splits; //amout of splits
    int type; //vertical or horizontal splits
    int *size; //the size of the splits in characters
    struct container *split; //contents of the splits
};


struct tab {
    char *name; //name of the tab
    int nameLen; //lenght of the tab's name
    struct container *content; //content of the tab, either content or a split
};


//main struct for the TUI
struct TUI {
    int tabs; //the amount of tabs
    struct tab *tab; //array of tabs
    int floatingWidth; //the width of the floating window (disabled if 0)
    int floatingHeight; //the height of the floating window (disabled if 0)
    struct container *floatingWindow; //container for the floating window
    char *barRight; //text on the right side of the status bar
    char *barLeft; //text on the left side of the status bar
};


int initTUI (); //initiate cTUI
int destroyTUI(); //destroy cTUI
int updateTUI(); //update keystroke handler
int renderTUI(struct TUI); //render the TUI struct
