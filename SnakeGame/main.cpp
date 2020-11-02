#include <iostream>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>

using namespace std;

class Snake{
private:
  int x;
  int y;
public:
  Snake(int a, int b){
    x = a;
    y = b;
  }
  int getX(){return x;}
  int getY(){return y;}
};

class Wall{
private:
  int x;
  int y;
public:
  Wall(int a, int b){
    x = a;
    y = b;
  }
  int getX(){return x;}
  int getY(){return y;}
};

int main() {
  WINDOW* GameBoard;  //게임이 진행되는 창
  WINDOW* ScoreBoard; //점수가 보여지는 창
  WINDOW* MissionBoard; //미션 창

  initscr();
  //resize_term(50, 50);
  noecho();
  curs_set(0);    //화면에 보이는 커서 설정, 0 : 커서 안보이게
  keypad(stdscr, true);
  timeout(200);
  srand((unsigned int)time(NULL));

  start_color();
  init_pair(1, COLOR_BLACK, COLOR_YELLOW);

  list<Snake> snakes;
  list<Snake>::iterator it;

  vector<Wall> wall;

  clock_t start, end;   //GrowItem과 PoisonItem 위치 이동시키기 위한 시간계산
  clock_t gameStartTime, gameEndTime;   //총 게임시간 측정
  bool quit = false;
  int point = 0; //게임이 끝났을 때 최종 점수(GrowItem - PoisonItem).
  int GrowItem = 0, PoisonItem = 0;
  int UsingGate = 0;  //게이트 몇 번 사용했는지 개수 셀 변수

  int LEVEL_num = 1;

  int dir = 2; // 처음에는 무조건 오른쪽으로 간다.
  int end_dir = 0; // tail방향의 방향키 입력 감지

  int foodX = rand() % 38 + 1;
  int foodY = rand() % 38 + 1; //최종도착지 좌표. 랜덤으로 정해진다.
  int poisonX = rand() % 38 + 1;
  int poisonY = rand() % 38 + 1;

  int gate1X = rand() % 38 + 1;    //1- 38 사이 랜덤값
  int gate1Y = rand() % 38 + 1;
  int gate2X = rand() % 38 + 1;    //1- 38 사이 랜덤값
  int gate2Y = rand() % 38 + 1;

  int MissionSnakesize = 10;     //미션 스네이크 사이즈
  int MissionGrowitem = 8;      //미션 그로우 아이템 얻기
  int MissionLimitedPoison = 8;  //포이즌 최대한계

  int ch; //방향키 입력 받기
  int Ssize = 3;
  int batchsec = 30000;  //GrowItem과 PoisonItem 위치 이동시키기 위한 시간계산
  int batchsec2 = 50000;
  int batchsec3 = 40000;
  int sec = 10;
  int sec3 = 10;

  int wallSize = 20;
  int wallSize2 = 15;

  for(int i = 0; i < Ssize; i++)
    snakes.push_front(Snake(Ssize+i, Ssize)); // 처음 snake의 좌표. 3개

  for(int i = 15; i < 22; i++){
    wall.push_back(Wall(i, wallSize));
  }
  for(int i = 17; i < 21; i++){
    wall.push_back(Wall(wallSize2, i));
  }

  end = clock();
  gameStartTime = clock();

  while(!quit && LEVEL_num != 5){ //LEVEL 4까지 스테이지 4개 하기
    ch = getch();
    switch (ch) {
      case KEY_LEFT:
        if(dir!= 2 ) dir = 1;
        else end_dir = 1;
        break;
      case KEY_RIGHT:
        if(dir != 1) dir = 2;
        else end_dir = 1;
        break;
      case KEY_UP:
        if(dir != 4) dir = 3;
        else end_dir = 1;
        break;
      case KEY_DOWN:
        if(dir!= 3) dir = 4;
        else end_dir = 1;
        break;
      case 'q':
        quit = true;
        break;
    } // dir값 지정하기 위한 switch-case문. 방향키를 입력받아 적용시키기 위함이다.

    if(end_dir == 1) quit = true;

    int x = 0, y = 0;
    Snake logic = snakes.front();
    x += logic.getX();
    y += logic.getY(); //head 부분 좌표

    if(dir == 1) x--;       //move to left
    else if(dir == 2) x++;  //move to right
    else if(dir == 3) y--;  //move to up
    else if(dir == 4) y++;  //move to down

    GameBoard = newwin(40, 40, 1, 1);
    wbkgd(GameBoard, COLOR_PAIR(1));
    wattron(GameBoard, COLOR_PAIR(1));
    wborder(GameBoard, '|', '|', '-', '-', '+', '+', '+', '+');

    for(it = snakes.begin(); it != snakes.end();it++){
      if(x == (*it).getX() && y == (*it).getY()) quit = true;
    }

    if(x == foodX && y == foodY){
      foodX = rand() % 38 + 1;
      foodY = rand() % 38 + 1;
      point++; // 목표 맞췄다면 다시 목표 설정하고 point 1점씩 증가.
      GrowItem++;
    }
    else if(x == poisonX && y == poisonY){
      poisonX = rand() % 38 + 1;
      poisonY = rand() % 38 + 1;
      snakes.pop_back();
      snakes.pop_back(); // poison 만나면 tail 잘린다.
      PoisonItem++;
      point--;
    }
    else if(x == gate1X && y == gate1Y){
        UsingGate++;
        if(gate2X == 0 || gate2X == 39 || gate2Y == 0 || gate2Y == 39){ // gate2도 가장자리.
        snakes.pop_back();
        x = gate2X;
        y = gate2Y;

        if (x == 0){
         dir = 2;
         x++;
        }
        else if (x == 39){
          dir = 1;
          x--;
        }
        else if (y == 0){
          dir = 4;
          y++;
        }
          else if (y == 39){
            dir = 3;
            y--;
          }
        }

        else{ // gate2가 가장자리 아닐때.
          if((gate1X == 15 && gate2Y == 20)){ //wall에 gate 2개 다 있을때
            if(dir == 2){ //오른쪽으로 진입했을때 아래로 내려가기
              snakes.pop_back();
              x = gate2X;
              y = gate2Y;
              y++;
              dir = 4;
            }
            else if(dir == 1){ //왼쪽으로 진입했을 때 위로 올라가기
              snakes.pop_back();
              x = gate2X;
              y = gate2Y;
              y--;
              dir = 3;
            }
            else if(dir == 4){ // 위에서 진입했을 때 아래로 내려가기
              snakes.pop_back();
              x = gate2X;
              y = gate2Y;
              y++;
            }
          }
          else if(gate2X == 15 && gate1Y == 20){
            if(dir == 4){ // 위에서 진입했을 때 왼쪽으로 가기
              snakes.pop_back();
              x = gate2X;
              y = gate2Y;
              x--;
              dir = 1;
            }
            else if(dir == 3){ //아래에서 진입했을 때 오른쪽으로 가기
              snakes.pop_back();
              x = gate2X;
              y = gate2Y;
              x++;
              dir = 2;
            }
          }

          else{
            snakes.pop_back();
            x = gate2X;
            y = gate2Y;

            if(dir == 1) x--;
            else if(dir == 2) x++;
            else if(dir == 3) y--;
            else if(dir == 4) y++;
          }
        }
    }
    else if(x == gate2X && y == gate2Y){
      UsingGate++;
        if(gate1X == 0 || gate1X == 39 || gate1Y == 0 || gate1Y == 39){
            snakes.pop_back();
            x = gate1X;
            y = gate1Y;

            if (x == 0){
            dir = 2;
            x++;
            }
            else if (x == 39){
              dir = 1;
              x--;
            }
            else if (y == 0){
              dir = 4;
              y++;
            }
            else if (y == 39){
              dir = 3;
              y--;
            }
        }
        else{ // gate1이 가장자리 아닐때.
          if(gate2X == 15 && gate1Y == 20){ //wall에 gate 2개 다 있을때
            if(dir == 2){ //오른쪽으로 진입했을때 아래로 내려가기
              snakes.pop_back();
              x = gate1X;
              y = gate1Y;
              y++;
              dir = 4;
            }
            else if(dir == 1){ //왼쪽으로 진입했을 때 위로 올라가기
              snakes.pop_back();
              x = gate1X;
              y = gate1Y;
              y--;
              dir = 3;
            }
            else if(dir == 4){ // 위에서 진입했을 때 아래로 내려가기
              snakes.pop_back();
              x = gate1X;
              y = gate1Y;
              y++;
            }
          }

          else if(gate1X == 15 && gate2Y == 20){
            if(dir == 4){ // 위에서 진입했을 때 왼쪽으로 가기
              snakes.pop_back();
              x = gate1X;
              y = gate1Y;
              x--;
              dir = 1;
            }
            else if(dir == 3){ //아래에서 진입했을 때 오른쪽으로 가기
              snakes.pop_back();
              x = gate1X;
              y = gate1Y;
              x++;
              dir = 2;
            }
          }

          else{
          snakes.pop_back();
          x = gate1X;
          y = gate1Y;

          if(dir == 1) x--;
          else if(dir == 2) x++;
          else if(dir == 3) y--;
          else if(dir == 4) y++;
          }
        }
    }
    else
      snakes.pop_back(); // 좌표다 못맞추었을 때 tail자름.

    for(int i = 0; i < 11; i++)
        mvwaddch(GameBoard, wall[i].getY(), wall[i].getX(), 'E');


    mvwaddch(GameBoard, gate1Y, gate1X, '@');
    mvwaddch(GameBoard, gate2Y, gate2X, '@');

    snakes.push_front(Snake(x, y)); // head 바꿔주기

    if(y > 38 || x > 38 || y < 1 || x < 1) quit = true; // GameBoard 창 크기 넘어가면 게임 끝
    else if(((x>=15)&&(x<22)&&(y==20))|| ((x==15)&&(y>=17)&&(y<21))) quit = true;
    //erase(); // snake의 개수 유지하고 출력해주도록 지워주기
    mvwaddch(GameBoard, foodY, foodX, 'O'); // food 지점 출력
    mvwaddch(GameBoard, poisonY, poisonX, 'X'); //poison 지점 출력
    wrefresh(GameBoard);
    start = clock();

    if((start - end) >= batchsec){ // 일정한 시간마다 아이템 위치 바꿈
      if((start - end) % batchsec >= 0 && (start - end) % batchsec <= 999){
        if(sec != ((start - end) / batchsec)){
          foodX = rand() % 38 + 1;
          foodY = rand() % 38 + 1;
          poisonX = rand() % 38 + 1;
          poisonY = rand() % 38 + 1;
          /* 아이템인줄 알고 가다가 실패하는게 더 재미있지 않을까?
          bool bola = false; // 바뀐 아이템 위치가 벽 위치와 일치할 경우..?
          while(bola){
          for(int i = 0; i < 11; i++){
            if(foodX == wall[i].getX() && foodY == wall[i].getY()){
              foodX = rand() % 38 + 1;
              foodY = rand() % 38 + 1;
              bola = true;
              }
            }
          }

          bola = false;
          while(bola){
            for(int i = 0; i < 11; i++){
              if(poisonX == wall[i].getX() && poisonY == wall[i].getY()){
                poisonX = rand() % 38 + 1;
                poisonY = rand() % 38 + 1;
                bola = true;
              }
            }
          }*/
          sec = (start - end) / batchsec;
        }
      }
    }

    if((start - end) >= batchsec2){ // 일정한 시간마다 벽 invisible
      if((start - end) % batchsec2 >= 0 && (start - end) % batchsec2 <= 30000){
          for(int i = 0; i < 11; i++){
            if(!(foodX == wall[i].getX() && foodY == wall[i].getY()) && !(poisonX == wall[i].getX() && poisonY == wall[i].getY())) //바뀐 아이템 위치가 벽 위를 덮게하기 위해
              mvwaddch(GameBoard, wall[i].getY(), wall[i].getX(), ' ');
          }
      }
    }

    if((start - end) >= batchsec3){ // 일정한 시간마다 gate 위치 조정
      if((start - end) % batchsec3 >= 0 && (start - end) % batchsec3 <= 999){
        if(sec3 != ((start - end) / batchsec3)){
          gate1X = rand() % 38 + 1;
          gate1Y = rand() % 38 + 1;
          gate2X = rand() % 38 + 1;
          gate2Y = rand() % 38 + 1;
          sec3 = (start - end) / batchsec3;
        }
      }
    }

    for(it = snakes.begin(); it != snakes.end();it++){
      mvwaddch(GameBoard, (*it).getY(), (*it).getX(), '*'); // snake 출력
      //if((*it).getY()==y && (*it).getX()==x && it!=snakes.begin()) quit= true;
    }
    wrefresh(GameBoard);
    
    //ScoreBoard 설정
    ScoreBoard = newwin(18, 34, 1, 45);
    wbkgd(ScoreBoard, COLOR_PAIR(1));
    wattron(ScoreBoard, COLOR_PAIR(1));
    wborder(ScoreBoard, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwprintw(ScoreBoard, 1, 1, "<< Score Board >>");
    mvwprintw(ScoreBoard, 3, 1, "SnakeSize : %i", snakes.size());
    mvwprintw(ScoreBoard, 5, 1, "GrowItem : %i", GrowItem);
    mvwprintw(ScoreBoard, 6, 1, "PoisonItem : %i", PoisonItem);
    mvwprintw(ScoreBoard, 7, 1, "UsingGate : %i", UsingGate);
    mvwprintw(ScoreBoard, 9, 1, "Your LEVEL : %i", LEVEL_num);
    mvwprintw(ScoreBoard, 15, 1, "Total_Point : %i", point);
    wrefresh(ScoreBoard);

    //MissionBoard 설정
    MissionBoard = newwin(19, 34, 22, 45);
    wbkgd(MissionBoard, COLOR_PAIR(1));
    wattron(MissionBoard, COLOR_PAIR(1));
    wborder(MissionBoard, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwprintw(MissionBoard, 1, 1, "<< Mission >>");
    mvwprintw(MissionBoard, 5, 1, "SnakeSize : %i / %i",snakes.size(), MissionSnakesize);
    mvwprintw(MissionBoard, 6, 1, "Must get GrowItem : %i / %i",GrowItem, MissionGrowitem);
    mvwprintw(MissionBoard, 9, 1, "Don't get PoisonItem : %i / %i",PoisonItem, MissionLimitedPoison);
    wrefresh(MissionBoard);

    if(snakes.size() < 3) quit = true;  //뱀 길이 3보다 작으면 종료
    if(PoisonItem == MissionLimitedPoison) quit = true;   //최대 한계 포이즌보다 포이즌 더 많이 먹으면 게임 끝

    if(GrowItem >= MissionGrowitem && snakes.size() >= MissionSnakesize && PoisonItem < MissionLimitedPoison && LEVEL_num<5){
      snakes.clear();
      for(int i = 0; i < Ssize; i++) snakes.push_front(Snake(Ssize+i, Ssize)); // 사이즈 3으로 초기화
      //좀 더 어렵게 만들기
      MissionSnakesize += 3;
      MissionGrowitem += 3;
      MissionLimitedPoison -=2;
      //기존 누적 초기화 하기
      GrowItem = 0;
      PoisonItem = 0;
      UsingGate = 0;
      //wall크기 늘리기
      wallSize += 3;
      wallSize2 += 3;
      //레벨 +1
      LEVEL_num +=1;
      getch();
    }
    if(LEVEL_num == 4){
      quit = true;
      //성공했다고 출력하기
    }
    mvprintw(42, 1, "<< If the level is 4, you will win in the game >>");
    mvprintw(43, 1, "<< If you clear the mission, your level goes up, And then it goes straight to the next level. >>");
    mvprintw(45, 1, "Your LEVLE : %i", LEVEL_num);
    refresh();
  }
  gameEndTime = clock();
  //TotalGameTime = gameEndTime - gameStartTime;  //총 게임 실행 계산
  //timeout(-1);
  erase();
  mvprintw(5, 1, "Your Total_Point : %i", point);  //최종 점수 출력
  mvprintw(1, 1, "You gained a total of %i GrowItem.", GrowItem);
  mvprintw(2, 1, "You gained a total of %i PoisonItem.", PoisonItem);
  //mvprintw(2, 1, "Game Run Time : %i\n", TotalGameTime);  //총 게임 실행시간 출력
  refresh();
  getch();
  delwin(GameBoard);
  delwin(ScoreBoard);
  delwin(MissionBoard);
  endwin();
  return 0;
}