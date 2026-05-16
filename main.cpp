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

    WINDOW *win = newwin(yMax, xMax, 0,0);
    box(win, 0, 0);
    keypad(stdscr, true);
    keypad(win, true);
    refresh();
    
    drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character);
    int input;
    int x = xMax/2;
    int y = yMax/2;
    std::string s;
    move(y, x);
    refresh();
    
    do{
        input = getch();
        //     if(input == 10){
        //         getyx(win, y, x);
        //         yOrigin = yOrigin - range + y * (range/yMax);
        //         xOrigin = xOrigin - range + x * (range/xMax);
        //         range = range*zoomFactor;
        //         werase(win);
        //         drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character);
        //         mvwprintw(win, 0, 0, "We are zooming");
        //     }
        // } 
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


        if(input == 98){
            getyx(win, y, x);
            yOrigin = yOrigin - range + y * (range/yMax);
            xOrigin = xOrigin - range + x * (range/xMax);
            range = range*zoomFactor;
            drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character);
            refresh();
        } else if(input == 118){
            getyx(win, y, x);
            yOrigin = yOrigin - range + y * (range/yMax);
            xOrigin = xOrigin - range + x * (range/xMax);
            range = range/zoomFactor;
            drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character);
            mvwprintw(win, 0, 0, "We are zooming out");
            refresh();
        }        

        mvwprintw(win, 1, 1, "X: %d\rY: %d", x, y);
        refresh();
        
    } while(input != 'x');


    endwin();
}
