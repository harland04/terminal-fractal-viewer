// Make window always an odd number so that the tail/beak will display
// More precision
// Make only the edges show (idk how to do that or where to start tbh)
// Better/dedicated movement keys (also would be good to optimize moving the frame around by only a little bit)
// Make each draw fractal be called like 5 times for each zoom in/out to have motion and make it look smoother
// Add a feature where you can go to the exact zoomLevel and origin so you can see cool structures.

// Cool areas:
// -0.804918
// 0.168507
#include <ncurses.h>
#include <iostream>
#include <complex>

bool isInSet(std::complex<double> c, int zoomLevel) {
    int explode = 0;
    std::complex<double> z = c;
    std::complex<double> z1 = c;

    for (int i = 0; i < 25*zoomLevel; i++) {
        z = z1;
        z1 = z*z + c;
        if(abs(z1) > 2){
            return false;
        }
    }
    return true;
}

void drawFractal(WINDOW *win, int yMax, int xMax, double yOrigin, double xOrigin, double range, char character, int zoomLevel) {
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
            if(isInSet(complex, zoomLevel)){
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

    // Change to zoomLevel
    double zoomFactor = 0.5;
    int zoomLevel = 1;
    char character = '#';

    // Get Terminal Size
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *win = newwin(yMax, xMax, 0,0);
    keypad(win, true);
    keypad(stdscr, true);
    refresh();
    
    drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character, zoomLevel);

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
                    drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character, zoomLevel);
                    move(y, x);
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
                    drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character, zoomLevel);
                    move(y, x);
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
                    drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character, zoomLevel);
                    move(y, x);
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
                    drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character,zoomLevel);
                    move(y, x);
                }
                refresh();
                break;
            // These might sort of break the coord variables.
            case KEY_UP:
                yOrigin -= range/yMax;
                drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character,zoomLevel);
                move(y, x);
                break;
            case KEY_DOWN:
                yOrigin += range/yMax;
                drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character,zoomLevel);
                move(y, x);
                break;
            case KEY_LEFT:
                xOrigin -= range/xMax;
                drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character,zoomLevel);
                move(y, x);
                break;
            case KEY_RIGHT:
                xOrigin += range/xMax;
                drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character,zoomLevel);
                move(y, x);
            default:
                break;
        }

        // Zoom in or zoom out based on input ('b' for zoom in, 'v' for zoom out)
        if(input == 98){
            zoomLevel++;
            range = range*zoomFactor;
            xOrigin -= ((xMax/2)-x)*range/xMax; 
            yOrigin -= ((yMax/2)-y)*range/yMax;
            drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character, zoomLevel);
            refresh();
        } else if(input == 118){
            if(zoomLevel > 1){
                zoomLevel--;
            }
            xOrigin += ((xMax/2)-x)*range/xMax; 
            yOrigin += ((yMax/2)-y)*range/yMax;
            range = range/zoomFactor;
            drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character, zoomLevel);
            refresh();
        }        
        mvwprintw(win, 1, 0, "X: %d\nY: %d\nreal: %lf\nimag: %lf", x, y, realCoord, imagCoord);
        move(y, x);
        wrefresh(win);
        
    } while(input != 'x');


    endwin();
}
