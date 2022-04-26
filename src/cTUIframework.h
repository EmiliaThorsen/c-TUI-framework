int initTUI (); //initiate cTUI
int destroyTUI(); //destroy cTUI
int updateTUI(); //update keystroke handler
int renderTUI(struct TUI tui); //render the TUI struct


union container {
    struct split *split;
    struct content *content;
};


struct content {
    char **data; //array of strings to write to screen
};


struct split {
    int *type; //array of the type of the splits
    int splits; //amout of splits
    union container *split; //contents of the splits
};


struct tab {
    char *name; //name of the tab
    int type; //the type of content
    union container *content; //content of the tab, either content or a split
};


//main struct for the TUI
struct TUI {
    int tabs; //the amount of tabs
    struct tab *tab; //array of tabs
    char *barRight; //text on the right side of the status bar
    char *barLeft; //text on the left side of the status bar
};
