#define ARROW_UP 0
#define ARROW_LEFT 1
#define ARROW_RIGHT 2
#define ARROW_DOWN 3

void clearTerminal();
void placeCursor(int, int);

char getChar();
int readArrow();

int readChar(char *);
int readInt(int *);
int readString(int, char *);
int readStringSilent(int, char *);
int readStringMask(int, char *, char);
