/*******************************************************************************
 *
 * loneracer2.c:
 *  a quick lone racer game mockup which demonstrates some of the basic functionality
 *  and features of ncurses.
 *
 * Created: Saturday, Feb, 2021
 * Author: Conner Ohnesorge
 * Contact:
 *
 ******************************************************************************/
                                                                                                         
/*
//   _____                                 _
//  |_   _|                               | |
//    | |   _ __ ___   _ __    ___   _ __ | |_  ___
//    | |  | '_ ` _ \ | '_ \  / _ \ | '__|| __|/ __|
//   _| |_ | | | | | || |_) || (_) || |   | |_ \__ \
//  |_____||_| |_| |_|| .__/  \___/ |_|    \__||___/
//                    | |
//                    |_|
*/
                                                                                                         
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
                                                                                                         
/*
//    _____  _         _             _       _         __  _         _  _    _
//   / ____|| |       | |           | |     | |       / _|(_)       (_)| |  (_)
//  | |  __ | |  ___  | |__    __ _ | |   __| |  ___ | |_  _  _ __   _ | |_  _   ___   _ __   ___
//  | | |_ || | / _ \ | '_ \  / _` || |  / _` | / _ \|  _|| || '_ \ | || __|| | / _ \ | '_ \ / __|
//  | |__| || || (_) || |_) || (_| || | | (_| ||  __/| |  | || | | || || |_ | || (_) || | | |\__ \
//   \_____||_| \___/ |_.__/  \__,_||_|  \__,_| \___||_|  |_||_| |_||_| \__||_| \___/ |_| |_||___/
*/
                                                                                                         
#define NCARS 50  // number of random cars to generate
#define FUSE_LENGTH 4 //frames
bool move_bomb = false;
int bomb_cost = 10;
int max_speed = 10;
int tempThrowPoint = 0;
time_t start,end;
int energy = 50;
int randcutoff = 0;
int randdir = 0;
int bombCounter = 0;
int counter = 0;
int speed = 40;
/*
//    _____  _                       _
//   / ____|| |                     | |
//  | (___  | |_  _ __  _   _   ___ | |_  ___
//   \___ \ | __|| '__|| | | | / __|| __|/ __|
//   ____) || |_ | |   | |_| || (__ | |_ \__ \
//  |_____/  \__||_|    \__,_| \___| \__||___/
*/
                                                                                                         
typedef struct car {
    int x;
    int y;
    char c;
} car;
                                                                                                         
typedef struct bomb{
    int x;
    int y;
    int fuse;
} bomb;
                                                                                                         
typedef enum status {
    IN_PROGRESS = 0,
    QUIT,
    WON,
    LOST
} status;
                                                                                                         
/*
//   ______                    _    _
//  |  ____|                  | |  (_)
//  | |__  _   _  _ __    ___ | |_  _   ___   _ __   ___
//  |  __|| | | || '_ \  / __|| __|| | / _ \ | '_ \ / __|
//  | |   | |_| || | | || (__ | |_ | || (_) || | | |\__ \
//  |_|    \__,_||_| |_| \___| \__||_| \___/ |_| |_||___/
*/
void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
                                                                                                         
    // Storing start time
    clock_t start_time = clock();
                                                                                                         
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}
                                                                                                         
void initgame(){
    srand(time(0));
    initscr();
    timeout(50);
    nodelay(stdscr, TRUE);
    noecho();
    curs_set(false);
    start = time(0);
}
                                                                                                         
void addGameSpeed(){
    if(speed < max_speed){
        speed++;
    }
                                                                                                         
}
                                                                                                         
void printBorder(int x_max, int y_max){
    /* Print Border Around Sreen */
        border (' ', ' ', '-', '^', ' ', ' ', ' ', ' ');
        mvprintw (0, x_max / 2 - 15, "Lone Lane Racer");
        refresh();
                                                                                                         
}
                                                                                                         
                                                                                                         
/*
//   __  __         _
//  |  \/  |       (_)
//  | \  / |  __ _  _  _ __
//  | |\/| | / _` || || '_ \
//  | |  | || (_| || || | | |
//  |_|  |_| \__,_||_||_| |_|
*/
                                                                                                         
int main() {
                                                                                                         
    int x_max, x_cur, y_max, y_cur;
    char c;
    car *cars[NCARS];
    bomb *bomb;
                                                                                                         
    bomb = malloc(sizeof(bomb));
    bomb->fuse = 0;
    bomb->x = x_max + 1;
    bomb->y = y_max + 1;
    status win_condition;
    initgame();
    getmaxyx (stdscr, y_max, x_max);
                                                                                                         
    do  {
                                                                                                         
        printBorder(x_max,y_max);
                                                                                                         
        /* (Re)set win condition */
        win_condition = IN_PROGRESS;
                                                                                                         
        /* Create random cars */
        for (int i = 0; i < NCARS; i++) {
            cars[i]= malloc(sizeof(car));
            cars[i]->x = rand() % 200 + (x_max);
            cars[i]->y = rand() % (y_max - 2) + 1;
            cars[i]->c = 'A' + (rand() % 52);
        }
                                                                                                         
        /* Set starting postion of car */
        y_cur = y_max /2;
        x_cur = x_max / 2 - 50;
        move (y_max - 5, x_max / 2);
        int animationbomb_width = 7;
        bool anibomb = false;
        /* Begin game loop */
        int tempx;
        int tempy;
        while (!win_condition) {
            printBorder(x_max,y_max);
            if((bomb->fuse == 1)&&(bomb->x < x_max)){
                bomb->x -= animationbomb_width/2;
                bomb->fuse = 0;
                mvprintw(bomb->y-2,bomb->x,"\\  |  /");
                mvprintw(bomb->y-1,bomb->x," \\ | / ");
                mvprintw(bomb->y,bomb->x,  ":--O--:");
                mvprintw(bomb->y+1,bomb->x," / | \\ ");
                mvprintw(bomb->y+2,bomb->x,"/  |  \\");
                tempx = bomb->x;
                tempy = bomb->y;
                delay(1000);
                mvprintw(bomb->y-2,bomb->x,"        ");
                mvprintw(bomb->y-1,bomb->x,"        ");
                mvprintw(bomb->y,bomb->x,  "        ");
                mvprintw(bomb->y+1,bomb->x,"        ");
                mvprintw(bomb->y+2,bomb->x,"        ");
                                                                                                         
                                                                                                         
            }
                                                                                                         
            //speed function applier
            timeout(speed);
                                                                                                         
            mvprintw (0, x_max - 22, "Time:");
            mvprintw (0, x_max - 14, "%.1f", difftime(time(0),start));
            mvprintw (0, 2, "[");
            for(int i = 1; i<=20; i++){
                if(i < energy){
                mvprintw (0, 2+i, "*");
                }else{
                mvprintw (0, 2+i, " ");
                }
            }
            mvprintw (0, 22, "]");
            c = getch();
                                                                                                         
                                                                                                         
            mvprintw(y_cur -1,x_cur,"         ");
            mvprintw(y_cur,x_cur,"          ");
            switch (c) {
            case (27):
                win_condition = QUIT;
                break;
            case 'w':
                if(y_cur>2){
                move (--y_cur, x_cur);
                }else{
                                                                                                         
                }
                break;
            case 'a':
                if(x_cur > 1){
                move (y_cur, --x_cur);
                }
                break;
            case 's':
                if(y_cur<y_max-2){
                    move (++y_cur, x_cur);
                }else{}
                                                                                                         
                break;
            case 'd':
                                                                                                         
                move (y_cur, ++x_cur);
                break;
            case 'q':
                win_condition = QUIT;
                break;
            case 'e':
                if(energy >= 1){
                if(bomb->x != x_max+1){
                if(!bomb->fuse){
                    bomb->x = x_cur;
                    bomb->y = y_cur;
                    bomb->fuse = FUSE_LENGTH;
                }}
                energy -= 1;
                                                                                                         
                move_bomb=true;
                }
                break;
            }
            if(c != 'e'){
                move_bomb=false;
            }
            for(int i = 0; i < NCARS; i++){
                if(c == cars[i]->c){
                    mvprintw(cars[i]->y, cars[i]->x," ");
                    cars[i]->x = x_max;
                    energy++;
                    addGameSpeed();
                }
            }
            mvprintw(y_cur -1,x_cur,".-'--`-._");
            mvprintw(y_cur,x_cur,"'-O---O--'");
                                                                                                         
            if (x_cur == x_max-8) {
                win_condition = WON;
                break;
            }
                                                                                                         
            /* Move cars */
                                                                                                         
                                                                                                         
            for (int i = 0; i < NCARS; i++) {
                                                                                                         
                if (cars[i]->x <= 3) {
                                                                                                         
                mvprintw(cars[i]->y, cars[i]->x," ");
                cars[i]->x = x_max;
                }
                if (cars[i]->y <= 1) {
                mvprintw(cars[i]->y, cars[i]->x," ");
                cars[i]->y = y_max-2;
                }
                randcutoff = rand() % NCARS;
                                                                                                         
                mvprintw(cars[i]->y, cars[i]->x,"  ");
                                                                                                         
                                                                                                         
                                                                                                         
                                                                                                         
                if(i == randcutoff){
                                                                                                         
                    mvaddch (cars[i]->y - 1 , cars[i]->x-1, cars[i]->c);
                                                                                                         
                    cars[i]->y--;
                    cars[i]->c = 'A' + (rand() % 26);
                    cars[i]->x -= 1;
                                                                                                         
                                                                                                         
                }else{
                                                                                                         
                    mvaddch (cars[i]->y, cars[i]->x-1, cars[i]->c);
                    cars[i]->x--;
                }
                if (y_cur == cars[i]->y && ((x_cur == cars[i]->x) || (x_cur == cars[i]->x-1) || (x_cur == cars[i]->x-2) || (x_cur == cars[i]->x-3) || (x_cur == cars[i]->x-4) || (x_cur == cars[i]->x-4)|| (x_cur == cars[i]->x-5)|| (x_cur == cars[i]->x-6)|| (x_cur == cars[i]->x-7)|| (x_cur == cars[i]->x-8))){
                                                                                                         
                     win_condition = LOST;
                }
                                                                                                         
                if((cars[i]->x <= tempx+13)&&(cars[i]->x >= tempx - 9)){
                    mvprintw(cars[i]->y, cars[i]->x," ");
                    cars[i]->x += 300;
                                                                                                         
                                                                                                         
                                                                                                         
                                                                                                         
                }
            }
                                                                                                         
            mvprintw(bomb->y,bomb->x,"@");
            if(bomb->fuse == FUSE_LENGTH){
                                                                                                         
            }
            if(move_bomb){
                                                                                                         
                mvprintw(bomb->y,bomb->x,"-");
                bomb->x += 1;
                mvprintw(bomb->y,bomb->x-1," ");
                mvprintw(bomb->y - 2,bomb->x-1," ");
                bomb->x += 1;
                mvprintw(bomb->y,bomb->x-1," ");
                mvprintw(bomb->y - 2,bomb->x-1," ");
            }
                                                                                                         
            mvprintw(bomb->y - 2,bomb->x,"%d",bomb->fuse);
                                                                                                         
            if(counter < 20){
                counter++;
            }else{
                counter = 0;
                if((bomb->fuse)){
                bomb->fuse--;
                }
            }
                                                                                                         
                                                                                                         
            refresh();  // update screen
            tempx = x_max + 100;
            tempy = y_max + 30;
        }
                                                                                                         
        /* Print Win Condition */
        if (win_condition == WON){ move(y_max/2,x_max/2);  printw ("You won!");}
        else if (win_condition == LOST){move(y_max/2,x_max/2); printw ("You lost!");}
        refresh();
        sleep (1);
        clear();  // reset screen
                                                                                                         
    } while (win_condition != QUIT);
    for(int i = 0;i<NCARS;i++){
        free(cars[i]);
    }
    free(bomb);
                                                                                                         
    endwin();
                                                                                                         
    return 0;
}
