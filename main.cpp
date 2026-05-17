// Make window always an odd number so that the tail/beak will display
// More precision
// Make only the edges show (idk how to do that or where to start tbh)
// Better/dedicated movement keys (also would be good to optimize moving the frame around by only a little bit)
// Make each draw fractal be called like 5 times for each zoom in/out to have motion and make it look smoother
// Add a feature where you can go to the exact zoomLevel and origin so you can see cool structures.
// Need to make sure the initial coordinates are actually the origin bc truncation/rounding error stuff.

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
    void moveCursor(int ch);

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

void Fractal::moveCursor(int ch){
    switch(ch){
        case 'w':
        if(cursorY > 0){
            cursorY--;
            wmove(win, cursorY, cursorX);
            wrefresh(win);
        }else{
            moveUp(1);
        }
        break;
    case 's':
        if(cursorY < yMax){
            cursorY++;
            wmove(win, cursorY, cursorX);
            wrefresh(win);
        }else{
            moveDown(1);
        }
        break;
    case 'a':
        if(cursorX > 0){
            cursorX--;
            wmove(win, cursorY, cursorX);
            wrefresh(win);
        }else{
            moveUp(1);
        }
        break;
    case 'd':
        if(cursorX < xMax){
            cursorX++;
            wmove(win, cursorY, cursorX);
            wrefresh(win);
        }else{
            moveUp(1);
        }
        break;
    }
}

void Fractal::moveUp(int n){
    if(cursorY <= n){
        cursorY = 0;
    }else{
        cursorY += n;
    }
    wmove(win, cursorY, cursorX);
    
    yOrigin -= n * (range/yMax);
    draw();
}
void Fractal::moveDown(int n){
    if(cursorY + n > yMax){
        cursorY = yMax;
    }else{
        cursorY -= n;
    }
    wmove(win, cursorY, cursorX);

    yOrigin += n * (range/yMax);
    draw();
}
void Fractal::moveLeft(int n){
    if(cursorX <= n){
        cursorX = xMax;
    }else{
        cursorX += n;
    }
    wmove(win, cursorY, cursorX);

    xOrigin -= n * (range/xMax);
    draw();
}
void Fractal::moveRight(int n){
    if(cursorX + n > xMax){
        cursorX = xMax;
    }else{
        cursorX -= n;
    }
    wmove(win, cursorY, cursorX);

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


int main() {

    int translateFactor = 5;

    // NCURSES START
    initscr();
    noecho();
    cbreak();

    // Get user's terminal size
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    if(yMax%2){
        yMax--;
    }

    // Create main window
    WINDOW *win = newwin(yMax, xMax, 0,0);
    // keypad(win, true);
    keypad(stdscr, true);
    refresh();

    // Initialize fractal class and draw initial scene
    Fractal fractal(win);
    fractal.draw();

    int input;

    // Main Program Loop
    do{
        // Get user input
        input = getch();

        // Check whether user input is a valid command
        switch(input){

            // 'wasd' controls the terminals cursor. 'w': 199. 's': 115. 'a': 97. 'd': 100.
            case 119:
                fractal.moveCursor(119);
                break;
            case 115:
                fractal.moveCursor(115);
                break;
            case 97:
                fractal.moveCursor(97);
                break;
            case 100:
                fractal.moveCursor(100);
                break;

            // Translate the image in a given direction
            case KEY_UP:
                fractal.moveUp(translateFactor);
                break;
            case KEY_DOWN:
                fractal.moveDown(translateFactor);
                break;
            case KEY_LEFT:
                fractal.moveLeft(translateFactor);
                break;
            case KEY_RIGHT:
                fractal.moveRight(translateFactor);
                break;

            // 'h'
            case 104:
                fractal.home();
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

        wrefresh(win);

    } while(input != 'x');


    endwin();
}
