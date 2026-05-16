#include <ncurses.h>
#include <iostream>
#include <complex>

int isInSet(std::complex<double> c ) {
    int explode = 0;
    std::complex<double> z = c;
    std::complex<double> z1 = c;
    for (int i = 0; i < 50; i++) {
        z = z1;
        z1 = z*z + c;
        if(abs(z1) > 2){
            explode = 1;
        }
    }
    return explode;
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

    initscr();
    noecho();
    cbreak();
    int yMax, xMax;

    getmaxyx(stdscr, yMax, xMax);
    printw("%d %d", yMax, xMax);
    refresh();

    getch();
    getmaxyx(stdscr, yMax, xMax);
    printw("%d %d", yMax, xMax);
    refresh();

    getch();
    endwin();
}
