// Make window always an odd number so that the tail/beak will display
// More precision
// Make only the edges show (idk how to do that or where to start tbh)
// Better/dedicated movement keys (also would be good to optimize moving the frame around by only a little bit)
// Make each draw fractal be called like 5 times for each zoom in/out to have motion and make it look smoother
// Add a feature where you can go to the exact zoomStep and origin so you can see cool structures.
// Need to make sure the initial coordinates are actually the origin bc truncation/rounding error stuff.

// Cool areas:
// -0.804918
// 0.168507
#include <ncurses.h>
#include <iostream>
#include <complex>

bool isInSet(std::complex<double> c, int zoomStep) {
    std::complex<double> z = c;
    std::complex<double> z1 = c;

    for (int i = 0; i < 25*zoomStep; i++) {
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

    void moveCursor(int ch);
    void moveLeft(int n);
    void moveRight(int n);
    void moveUp(int n);
    void moveDown(int n);

    void draw();
    void home();
    void zoomIn();
    void zoomOut();
    
    private:
    WINDOW *win;
    int screenHeight, screenWidth;
    int cursorColumn, cursorRow;
    double selectedX = -0.5;
    double selectedY = 0.0;
    char pixelChar = '#';
    double range = 3.0;
    double originY = 0.0;
    double originX = -0.5;
    int zoomStep = 1;
    float zoomScale = 0.5;
};

// Constructor of Fractal
Fractal::Fractal(WINDOW *win){
    this->win = win;
    getmaxyx(win, screenHeight, screenWidth);

    // Set intial cursor position to the middle of the window
    cursorColumn = screenHeight/2;
    cursorRow = screenWidth/2;
    wmove(win, cursorColumn, cursorRow);
}

// Moves cursor based on user input
void Fractal::moveCursor(int ch){
    switch(ch){
        case 'w':
        if(cursorColumn > 0){
            cursorColumn--;
            wmove(win, cursorColumn, cursorRow);
            wrefresh(win);
        }else{
            moveUp(1);
        }
        break;
    case 's':
        if(cursorColumn < screenHeight){
            cursorColumn++;
            wmove(win, cursorColumn, cursorRow);
            wrefresh(win);
        }else{
            moveDown(1);
        }
        break;
    case 'a':
        if(cursorRow > 0){
            cursorRow--;
            wmove(win, cursorColumn, cursorRow);
            wrefresh(win);
        }else{
            moveLeft(1);
        }
        break;
    case 'd':
        if(cursorRow < screenWidth){
            cursorRow++;
            wmove(win, cursorColumn, cursorRow);
            wrefresh(win);
        }else{
            moveRight(1);
        }
        break;
    }
}

// Translates the image up by n "pixels"
void Fractal::moveUp(int n){
    if(cursorColumn + n > screenHeight){
        cursorColumn = screenHeight;
    }else{
        cursorColumn += n;
    }
    wmove(win, cursorColumn, cursorRow);
    
    originY -= n * (range/screenHeight);
    draw();
}
// Translates the image down by n "pixels"
void Fractal::moveDown(int n){
    if(cursorColumn - n < 0){
        cursorColumn = 0;
    }else{
        cursorColumn -= n;
    }
    wmove(win, cursorColumn, cursorRow);

    originY += n * (range/screenHeight);
    draw();
}
// Translates the image left by n "pixels"
void Fractal::moveLeft(int n){
    if(cursorRow <= n){
        cursorRow = screenWidth;
    }else{
        cursorRow += n;
    }
    wmove(win, cursorColumn, cursorRow);

    originX -= n * (range/screenWidth);
    draw();
}
// Translates the image right by n "pixels"
void Fractal::moveRight(int n){
    if(cursorRow + n > screenWidth){
        cursorRow = screenWidth;
    }else{
        cursorRow -= n;
    }
    wmove(win, cursorColumn, cursorRow);

    originX += n * (range/screenWidth);
    draw();
}

// Zoom in one level based on the zoom factor
void Fractal::zoomIn(){
    zoomStep++;
    range *= zoomScale;
    getyx(win, cursorColumn, cursorRow);
    originX -= ((screenWidth/2)-cursorRow)*range/screenWidth; 
    originY -= ((screenHeight/2)-cursorColumn)*range/screenHeight;
    draw();
}
// Zoom out one level based on the zoom factor
void Fractal::zoomOut(){
    if(zoomStep > 1){
        zoomStep--;
    }
    getyx(win, cursorColumn, cursorRow);
    originX += ((screenWidth/2)-cursorRow)*range/screenWidth; 
    originY += ((screenHeight/2)-cursorColumn)*range/screenHeight;
    range = range/zoomScale;
    draw();
}

// Render initial fractal and cursor position
void Fractal::home(){
    zoomStep = 1;
    range = 3.0;
    originY = 0.0;
    originX = -0.5;
    cursorColumn = screenHeight/2;
    cursorRow = screenWidth/2;
    draw();
}

void Fractal::draw(){
    getyx(win, cursorColumn, cursorRow);
    werase(win);

    double real = 0;
    double imag = 0;
    std::complex<double> complex(real, imag);

    for(int i = 0; i < screenWidth; i++){
        for(int j = 0; j < screenHeight; j++){
            real = (originX - range/2) + i*(range/screenWidth);
            imag = (originY - range/2) + j*(range/screenHeight);
            complex.real(real);
            complex.imag(imag);
            if(isInSet(complex, zoomStep)){
                mvwaddch(win, j, i, pixelChar);
            }
        }
    }
    box(win, 0, 0);
    // mvwprintw(win, 1, 0, "X: %d\nY: %d\nreal: %lf\nimag: %lf", cursorRow, cursorColumn, selectedX, selectedY);
    wmove(win, cursorColumn, cursorRow);
    wrefresh(win);
}


int main() {

    int translateFactor = 5;

    // NCURSES START
    initscr();
    noecho();
    cbreak();

    // Get user's terminal size
    int screenHeight, screenWidth;
    getmaxyx(stdscr, screenHeight, screenWidth);
    if(screenHeight%2){
        screenHeight--;
    }

    // Create main window
    WINDOW *win = newwin(screenHeight, screenWidth, 0,0);
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

            // 'h': Bring user back to starting position
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
