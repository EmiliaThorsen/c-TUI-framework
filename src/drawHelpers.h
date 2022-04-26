void clearScreen();
void setChar(int x, int y, char ch);
void writeLine(int x, int y, char *str);
void writeTextBlock(int x, int y, int lines, char *str[]);
void writeVerticalLine(int x, int y, int length, char ch);

int getTermRows();
int getTermCols();
