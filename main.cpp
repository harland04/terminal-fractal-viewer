#include <ncurses.h>
#include <iostream>
#include <complex>

bool isInSet(std::complex<double> c ) {
    int explode = 0;
    std::complex<double> z = c;
    std::complex<double> z1 = c;
    for (int i = 0; i < 50; i++) {
        z = z1;
        z1 = z*z + c;
        if(abs(z1) > 2){
            return false;
        }
    }
    return true;
}

void drawFractal(WINDOW *win, int yMax, int xMax, double yOrigin, double xOrigin, double range, char character) {
    werase(win);
    double real = 0;
    double imag = 0;
    std::complex<double> complex(0, 0);

    for(int i = 0; i < xMax; i++){
        for(int j = 0; j < yMax; j++){
            real = (xOrigin - range/2) + i*(range/xMax);
            imag = (yOrigin - range/2) + j*(range/yMax);
            complex.real(real);
            complex.imag(imag);
            if(isInSet(complex)){
                mvwprintw(win, j, i, &character);
            }
        }
    }
    box(win, 0, 0);
    wrefresh(win);
}



int main() {

    // NCURSES START
    initscr();
    noecho();
    cbreak();

    // Initialize Variables
    double range = 3;
    double xOrigin = -0.5;
    double yOrigin = 0;
    double realCoord = -0.5;
    double imagCoord = 0;
    double zoomFactor = 0.5;
    char character = '#';

    // Get Terminal Size
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    xMax = 2*yMax;

    WINDOW *win = newwin(yMax, xMax, 0,0);
    refresh();
    
    drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character);

    int input;
    int x = xMax/2;
    int y = yMax/2;
    std::string s;
    move(y, x);
    refresh();
    // Main Program Loop
    do{
        // Get user input
        input = getch();

        // Check for cursor input and move cursor
        switch(input){
            case 119:
                if(y > 0){
                    y--;
                    move(y, x);
                    imagCoord = imagCoord + (range/yMax);
                }else{
                    yOrigin -= range/yMax;
                    drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character);
                }
                refresh();
                break;
            case 115:
                if(y < yMax){
                    y++;
                    move(y, x);
                    imagCoord = imagCoord - (range/yMax);
                }else{
                    yOrigin += range/yMax;
                    drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character);
                }
                refresh();
                break;
            case 100:
                if(x < xMax){
                    x++;
                    move(y, x);
                    realCoord = realCoord + (range/xMax);
                }else{
                    xOrigin += range/xMax;
                    drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character);
                    refresh();
                }
                refresh();
                break;
            case 97:
                if(x > 0){
                    x--;
                    move(y, x);
                    realCoord = realCoord - (range/xMax);
                }else{
                    xOrigin -= range/xMax;
                    drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character);
                }
                refresh();
                break;
            default:
                break;
        }

        // Zoom in or zoom out based on input ('b' for zoom in, 'v' for zoom out)
        if(input == 98){
            range = range*zoomFactor;
            xOrigin -= ((xMax/2)-x)*range/xMax; 
            yOrigin -= ((yMax/2)-y)*range/yMax;
            drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character);
            refresh();
        } else if(input == 118){
            xOrigin += ((xMax/2)-x)*range/xMax; 
            yOrigin += ((yMax/2)-y)*range/yMax;
            range = range/zoomFactor;
            drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character);
            refresh();
        }        
        mvwprintw(win, 1, 0, "X: %d\nY: %d\nxOr: %lf\nyOr: %lf", x, y, realCoord, imagCoord);
        move(y, x);
        wrefresh(win);
        
    } while(input != 'x');


    endwin();
}
