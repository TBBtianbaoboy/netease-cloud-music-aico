#ifndef COLOR_H 
#define COLOR_H 
#include <curses.h>

#define GREEN 1
#define RED 3
#define CYAN 2
#define BLUE 4
#define YELLOW 5
#define WHITE 6

class Color final {
    public:
        void StartColor();
        void LoadAllColor();
        void OpenColor(int);
        void CloseColor(int);
        void WOpenColor(WINDOW*,int);
        void WCloseColor(WINDOW*,int);
};
#endif // COLOR_H
