#include <iostream>
#include "console.h"
#include <conio.h>
#include <ctime>
using namespace std;
TConsole console;
 
enum Dir{
    sdUP,
    sdDOWN,
    sdRIGHT,
    sdLEFT
};
 
enum Key{
    kNone,
    kUp,
    kDown,
    kLeft, 
    kRight,
    kEsc,
    kSpace,
    kEnter
};
 
struct Field{
    int width;
    int height;
    int coordsX;
    int coordsY;
};
 
struct Food{
    int x;
    int y;            
};
 
struct Score{
    int x;
    int y;
    int score;
};
 
enum state{
    gsPlay,
    gsPause,
    gsGameover
};
 
struct State{
};
 
struct Point{
 
    int x;
    int y;
};
 
struct Snake{
    Point body[1000];
    Dir dir;
    int length;
    int x;
    int y;
};
 
struct Game{
    Snake mysnake;
    Food myfood;
    Score myscore;
    State mystate;
    Field myfield;
};
 
Key getkey(){
    if(console.KeyPressed()){
        unsigned char k = getch();    
        switch(k){
            case 0x20: return kSpace;
            case 0x0D: return kEnter;
            case 0x1B: return kEsc;
            case 0xE0: k = getch();
                switch(k){    
                    case 0x48: return kUp;
                    case 0x50: return kDown;
                    case 0x4b: return kLeft;
                    case 0x4d: return kRight;
                }
        }    
    }
    return kNone;
}
 
void initField(Field &f){
    f.width = 72; 
    f.height = 20;
    f.coordsX = 1;
    f.coordsY = 1;
}
 //	f.width = 20; 
//	f.height = 20;
//	f.coordsX = 6;
//	f.coordsY = 2;
void initScore(Score &sco, Field &f){
    sco.x = 5;
    sco.y =  -1;
    sco.score = 0;
}
 
void initSnake(Snake &s, Field &f){
    s.dir = sdLEFT;
    s.length = 4;
    Point start;
    start.x = (f.width - s.length) / 2;
    start.y = f.height / 2;
    for(int i = 0; i < s.length; i++){
        s.body[i].x = start.x + i;
        s.body[i].y = start.y;
    }
}
 
void initFood(Food &k){
    int x; 
    int y;
}
 
void initGame(Game &g){
    g.myscore.x = 0;
}
 
void ShowSnake(Snake &s, Field &f){
    console.TextColor (COLOR_BROWN);
    for(int i = 0; i < s.length; i++){
        console.GotoXY(s.body[i].x + f.coordsX + 1, s.body[i].y + f.coordsY + 1);
        cout << "*";
    }
}
 
void moveSnake(Snake &s){
 
    for(int i = s.length - 1; i > 0; i--){
        s.body[i].x = s.body[i-1].x;
        s.body[i].y = s.body[i-1].y; 
    }     
    switch(s.dir){
        case sdUP: s.body[0].y--;
        break;
        
        case sdRIGHT: s.body[0].x++;
        break;
        
        case sdDOWN: s.body[0].y++;
        break;
        
        case sdLEFT: s.body[0].x--;
        break;
    }
}
 
void generateCoord(Food &k, Field &f){
    k.x = rand() % (f.width);
    k.y = rand() % (f.height); 
}
 
bool isRightCoord(Food &k, Snake &s, Field &f){
    for(int i = 0; i < s.length; i++){
        if(k.x == s.body[i].x && k.y == s.body[i].y ){
            return false;
        }
    }
    return true;
}
 
void createFood(Food &k, Snake &s, Field &f){
     do{
        generateCoord(k, f);
    }while(not isRightCoord(k, s, f));
}
 
void ShowFood(Food &k, Snake &s, Field &f){
    console.TextColor(COLOR_WHITE);
    console.GotoXY(k.x + f.coordsX + 1, k.y + f.coordsY + 1);
    cout << "$";
}
 
void HideSnake(Snake &s, Field &f){
    console.GotoXY(s.body[s.length-1].x + f.coordsX + 1, s.body[s.length-1].y + f.coordsY + 1);
    cout << " ";
}
 
void TurnSnake(Snake &s, Dir dir){
    switch(dir){
        case sdLEFT: if(s.dir == sdRIGHT) return;
        break;
        case sdRIGHT: if(s.dir == sdLEFT) return;
        break;
        case sdUP:     if(s.dir == sdDOWN) return;
        break;
        case sdDOWN: if(s.dir == sdUP) return;
        break;
    } 
    s.dir = dir;
}
 
void Play(Field &f){
    console.TextColor(COLOR_CYAN);
    console.GotoXY(f.coordsX + 3, f.coordsY + f.height + 2);
    cout << "Play ";
}
 
void Pause(Field &f){
    console.TextColor(COLOR_CYAN);
    console.GotoXY(f.coordsX + 3, f.coordsY + f.height + 2);
    cout << "Pause";
    getch();
    Play(f);
}
 
bool IsSelfSnake(Snake &s){
    for(int i = 3; i < s.length; i++){
        if(s.body[i].x == s.body[0].x && s.body[i].y == s.body[0].y) {
            return true;
        }
    }
    return false;
}
 
bool IsCrashWall(Snake &s, Field &f){
    return (s.body[0].x == f.width + f.coordsX -1 or s.body[0].y == f.height + f.coordsY - 1 or s.body[0].x == f.coordsX -2 or s.body[0].y == f.coordsY - 2);
}
 
bool IsGameOver(Game &g){
    return(IsSelfSnake(g.mysnake) or IsCrashWall(g.mysnake, g.myfield));
}
 
bool IsEatFood(Snake &s, Food &k){
    return (s.body[0].x == k.x && s.body[0].y == k.y);
}
 
void Increaselength(Snake &s){
    s.length++;
    s.body[s.length-1].x = s.body[s.length - 2].x;
    s.body[s.length-1].y = s.body[s.length - 2].y;
}
 
void ShowField(Field &f){
    int n = 0;
    for(int i = f.width; n <= i; n++){
        console.GotoXY(n + f.coordsX, f.height+1+f.coordsY);
        console.TextColor(COLOR_LIGHTRED);
        cout << "+";
    }
    n = 0;
    for(int i = f.height; n <=i+1; n++){
        console.GotoXY(f.width+1+f.coordsX,n+f.coordsY);
        cout << "+";
    }    
    n = 0;
    for(int i = f.width; n <=i; n++){
        console.GotoXY(n+f.coordsX,f.coordsY);
        cout << "+";
    }
    n = 0;
    for(int i = f.height; n <=i; n++){
        console.GotoXY(f.coordsX,n+f.coordsY);
        cout << "+";
    }    
}
 
void ShowScore(Score &sco, Snake &s, Field &f){
    console.GotoXY(f.coordsX + sco.x, f.coordsY + sco.y);
    console.TextColor(COLOR_GREEN);
    cout << "Score: " <<  sco.score++ ;
}
 
void Exit(Field &f, Snake &s){
    console.TextColor(COLOR_RED);
    console.GotoXY(f.width / 2 - 5, f.height / 2);
    cout << "Game Over";
    console.GotoXY(f.coordsX + 3, f.coordsY + f.height + 2);
    cout << "    ";
    console.TextColor (COLOR_LIGHTGRAY);
    for(int i = 0; i < s.length; i++){
        console.GotoXY(s.body[i].x + f.coordsX + 1, s.body[i].y + f.coordsY + 1);
        cout << "*";
    }
    ShowField(f);
}
 
int main(){
   cout << "snake";
    srand(time(0));
    Game mygame;
    
    initField(mygame.myfield);    
    initSnake(mygame.mysnake, mygame.myfield);    
    initScore(mygame.myscore, mygame.myfield);
    
    createFood(mygame.myfood, mygame.mysnake, mygame.myfield);
    
    ShowScore(mygame.myscore, mygame.mysnake, mygame.myfield);
    ShowField(mygame.myfield);
    ShowFood(mygame.myfood, mygame.mysnake, mygame.myfield);
    Play(mygame.myfield);
    getch();
    while(not IsGameOver(mygame)){
        Key key = getkey();
        switch(key){
            case kUp: TurnSnake(mygame.mysnake, sdUP);
            break;
            case kDown: TurnSnake(mygame.mysnake, sdDOWN);
            break;
            case kRight: TurnSnake(mygame.mysnake, sdRIGHT);
            break;
            case kLeft: TurnSnake(mygame.mysnake, sdLEFT);
            break;
            case kSpace: ShowSnake(mygame.mysnake, mygame.myfield); Pause(mygame.myfield); HideSnake(mygame.mysnake, mygame.myfield);
            break;
        }
        moveSnake(mygame.mysnake);
        if(IsEatFood(mygame.mysnake, mygame.myfood)){
            Increaselength(mygame.mysnake);
            createFood(mygame.myfood, mygame.mysnake, mygame.myfield);
            ShowFood(mygame.myfood, mygame.mysnake, mygame.myfield);
            ShowScore(mygame.myscore, mygame.mysnake, mygame.myfield);
        }
        ShowSnake(mygame.mysnake, mygame.myfield);
        Sleep(100);
        HideSnake(mygame.mysnake, mygame.myfield);
        }
    Exit(mygame.myfield, mygame.mysnake);
    getch();    
}
 
