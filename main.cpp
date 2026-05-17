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

class Fractal{
    public:
    Fractal(WINDOW *win);
    ~Fractal();
    void draw();
    void moveLeft(int n);
    void moveRight(int n);
    void moveUp(int n);
    void moveDown(int n);
    void zoomIn();
    void zoomOut();
    void home();

    private:
    WINDOW *win;
    int yMax, xMax;
    int cursorY, cursorX;
    double xCoord = -0.5;
    double yCoord = 0.0;
    char character = '#';
    double range = 3.0;
    double yOrigin = 0.0;
    double xOrigin = -0.5;
    int zoomLevel = 1;
    float zoomFactor = 0.5;
};

// Constructor of Fractal
Fractal::Fractal(WINDOW *win){
    this->win = win;
    getmaxyx(win, yMax, xMax);
    cursorY = yMax/2;
    cursorX = xMax/2;
    wmove(win, cursorY, cursorX);
}
Fractal::~Fractal(){}

void Fractal::moveUp(int n){
    yOrigin -= n * (range/yMax);
    draw();
}
void Fractal::moveDown(int n){
    yOrigin += n * (range/yMax);
    draw();
}
void Fractal::moveLeft(int n){
    xOrigin -= n * (range/xMax);
    draw();
}
void Fractal::moveRight(int n){
    xOrigin += n * (range/xMax);
    draw();
}

void Fractal::zoomIn(){
    zoomLevel++;
    range *= zoomFactor;
    getyx(win, cursorY, cursorX);
    xOrigin -= ((xMax/2)-cursorX)*range/xMax; 
    yOrigin -= ((yMax/2)-cursorY)*range/yMax;
    draw();
}
void Fractal::zoomOut(){
    if(zoomLevel > 1){
        zoomLevel--;
    }
    getyx(win, cursorY, cursorX);
    xOrigin += ((xMax/2)-cursorX)*range/xMax; 
    yOrigin += ((yMax/2)-cursorY)*range/yMax;
    range = range/zoomFactor;
    draw();
}

void Fractal::home(){
    zoomLevel = 1;
    range = 3.0;
    yOrigin = 0.0;
    xOrigin = -0.5;
    draw();
}

void Fractal::draw(){
    getyx(win, cursorY, cursorX);
    // mvwprintw(win, 1, 0, "X: %d\nY: %d\nreal: %lf\nimag: %lf", cursorX, cursorY, xCoord, yCoord);
    // wmove(win, cursorY, cursorX);
    // wrefresh(win);
    // getch();
    werase(win);

    double real = 0;
    double imag = 0;
    std::complex<double> complex(real, imag);

    for(int i = 0; i < xMax; i++){
        for(int j = 0; j < yMax; j++){
            real = (xOrigin - range/2) + i*(range/xMax);
            imag = (yOrigin - range/2) + j*(range/yMax);
            complex.real(real);
            complex.imag(imag);
            if(isInSet(complex, zoomLevel)){
                mvwaddch(win, j, i, character);
            }
        }
    }
    box(win, 0, 0);
    mvwprintw(win, 1, 0, "X: %d\nY: %d\nreal: %lf\nimag: %lf", cursorX, cursorY, xCoord, yCoord);
    wmove(win, cursorY, cursorX);
    wrefresh(win);
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
    if(yMax%2){
        yMax--;
    }

    WINDOW *win = newwin(yMax, xMax, 0,0);
    keypad(win, true);
    keypad(stdscr, true);
    refresh();

    // Initialize fractal class
    Fractal fractal(win);

    
    fractal.draw();

    int input, x, y;


    // Main Program Loop
    do{
        // Get user input
        input = getch();
        getyx(win, y, x);

        // Check for cursor input and move cursor
        switch(input){
            // 'w'
            case 119:
                if(y > 0){
                    wmove(win, y--, x);
                    wrefresh(win);
                }else{
                    fractal.moveUp(1);
                }
                break;
            // 's'
            case 115:
                if(y < yMax){
                    wmove(win, y++, x);
                    wrefresh(win);
                }else{
                    fractal.moveDown(1);
                }
                break;
            // 'd'
            case 100:
                if(x < xMax){
                    wmove(win, y, x++);
                    wrefresh(win);
                }else{
                    fractal.moveRight(1);
                }
                break;
            // 'a'
            case 97:
                if(x > 0){
                    wmove(win, y, x--);
                    wrefresh(win);
                }else{
                    fractal.moveLeft(1);
                }
                break;
            // These might sort of break the coord variables.
            case KEY_UP:
                fractal.moveUp(1);
                break;
            case KEY_DOWN:
                fractal.moveDown(1);
                break;
            case KEY_LEFT:
                fractal.moveLeft(1);
                break;
            case KEY_RIGHT:
                fractal.moveRight(1);
                break;
            // 'h'
            case 104:
                // Make these variables global? or have a global version? if I do then i can probably make other fractals easy too.
                fractal.home();
                // yOrigin = 0;
                // xOrigin = -0.5;
                // range = 3;
                // drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character,zoomLevel);
                break;
            // Zoom in or zoom out based on input ('b' for zoom in, 'v' for zoom out)
            case 98:
                fractal.zoomIn();
                break;
            case 118:
                fractal.zoomOut();
                break;
            default:
                break;
        }

        // Zoom in or zoom out based on input ('b' for zoom in, 'v' for zoom out)
        // if(input == 98){
        //     fractal.zoomIn();
        //     zoomLevel++;
        //     range = range*zoomFactor;
        //     xOrigin -= ((xMax/2)-x)*range/xMax; 
        //     yOrigin -= ((yMax/2)-y)*range/yMax;
        //     drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character, zoomLevel);
        //     refresh();
        // } else if(input == 118){
        //     fractal.zoomOut();
        //     if(zoomLevel > 1){
        //         zoomLevel--;
        //     }
        //     xOrigin += ((xMax/2)-x)*range/xMax; 
        //     yOrigin += ((yMax/2)-y)*range/yMax;
        //     range = range/zoomFactor;
        //     drawFractal(win, yMax, xMax, yOrigin, xOrigin, range, character, zoomLevel);
        //     refresh();
        // }        
        // mvwprintw(win, 1, 0, "X: %d\nY: %d\nreal: %lf\nimag: %lf", x, y, realCoord, imagCoord);
        wrefresh(win);
        
    } while(input != 'x');


    endwin();
}
