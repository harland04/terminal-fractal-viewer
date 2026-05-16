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
    // double real = 0;
    // double imag = 0;
    // std::complex<double> c(-0.3, 0.3);
    // int x = isInSet(c);
    // if (x == 0){
    //     std::cout << "In Mandelbrot Set" << x << " ";
    // } else if (x == 1){
    //     std::cout << "NOT In Mandelbrot Set"  << x << " ";
    // } else {
    //     std::cout << "ERROR" << x << " ";
    // }

    // NCURSES START
    initscr();
    noecho();
    cbreak();

    // Initialize Variables
    double range = 3;
    double xOrigin = -0.5;
    double yOrigin = 0;
    double zoomFactor = 0.5;
    char character = '#';

    // Get Terminal Size
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *win = newwin(yMax-2, xMax-2, 0,0);
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
                    s = std::to_string(mvinch(y,x));
                    wattron(win, A_NORMAL);
                    mvwprintw(win, y, x, s.c_str());
                    wattroff(win, A_NORMAL);

                    y--;
                    s = std::to_string(mvinch(y,x));

                    wattron(win, A_REVERSE);
                    mvwprintw(win, y, x, s.c_str());
                    wattroff(win, A_REVERSE);
                }
                refresh();
                break;
            case 115:
                if(y < yMax){
                    s = std::to_string(mvinch(y,x));
                    wattron(win, A_NORMAL);
                    mvwprintw(win, y, x, s.c_str());
                    wattroff(win, A_NORMAL);

                    y++;
                    s = std::to_string(mvinch(y,x));

                    wattron(win, A_REVERSE);
                    mvwprintw(win, y, x, s.c_str());
                    wattroff(win, A_REVERSE);
                }
                refresh();
                break;
            case 100:
                if(x < xMax){
                    s = std::to_string(mvinch(y,x));
                    wattron(win, A_NORMAL);
                    mvwprintw(win, y, x, s.c_str());
                    wattroff(win, A_NORMAL);

                    x++;
                    s = std::to_string(mvinch(y,x));

                    wattron(win, A_REVERSE);
                    mvwprintw(win, y, x, s.c_str());
                    wattroff(win, A_REVERSE);
                }
                refresh();
                break;
            case 97:
                if(x > 0){
                    s = std::to_string(mvinch(y,x));
                    wattron(win, A_NORMAL);
                    mvwprintw(win, y, x, s.c_str());
                    wattroff(win, A_NORMAL);

                    x--;
                    s = std::to_string(mvinch(y,x));

                    wattron(win, A_REVERSE);
                    mvwprintw(win, y, x, s.c_str());
                    wattroff(win, A_REVERSE);
                }
                refresh();
                break;
            default:
                break;
        }

        // Zoom in or zoom out based on input ('b' for zoom in, 'v' for zoom out)
        if(input == 98){
            yOrigin = yOrigin - range/2 + y*(range/yMax);
            xOrigin = xOrigin - range/2 + x*(range/xMax);
            range = range*zoomFactor;
            drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character);
            y = yMax/2;
            x = xMax/2;
            move(y, x);
            refresh();
        } else if(input == 118){
            getyx(win, y, x);
            yOrigin = yOrigin - range/2 + y * (range/yMax);
            xOrigin = xOrigin - range/2 + x * (range/xMax);
            range = range/zoomFactor;
            drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character);
            y = yMax/2;
            x = xMax/2;
            move(y, x);
            refresh();
        }        
        // mvwprintw(win, 1, 1, "hello");
        // move(y, x);
        // wrefresh(win);
        
    } while(input != 'x');


    endwin();
}
