# SnakeGame
C++언어를 사용한 스네이크게임 제작 팀프로젝트

## 1 개요
### 1.1 프로젝트 소개
프로그래밍 언어는 C++을 사용하며, Ncurses 라이브러리를 활용하여 게임을 구현하였다.
이 게임은 일명 스네이크가 GROWTH ITEM과 POSION ITEM을 통해 길어지기도, 짧아지기도 하며 각 단계의 미션을 수행하는 게임이다.
단계는 총 4개로 구성되며 진행 될 수록 먹어서는 안되는 POSION 개수의 한계가 줄어들고 먹어야 할 GROW ITEM수와 뱀의 목표 길이가 길어진다.
중간중간 위치가 랜덤으로 바뀌는 GATE를 통해 맵의 이곳 저곳을 돌아다닐 수 있다.    

### 1.2 개발 방법
+ 개발 OS 환경 : Linux Ubuntu 16.04 LTS    
+ 개발 언어 : C++

### 1.3 사용 라이브러리
+ iostream
+ ncurses.h
+ stdlib.h
+ time.h
+ list
+ vector

## 2 개발내용및결과물
### 2.1 목표
1단계: Map의 구현    
+ Ncurses 라이브러리 함수들을 사용하여 총 3 개의 윈도우 창(Game Board, Score Board, Mission Board)를 만든다.
+ Game Board 에서는 실질적인 게임 화면이 나타난다 여기서 우리는 스네이크를 움직이며 게임을 한다.
+ Score Board 에는 내 점수와 먹은 Growth Item, Poison Item 의 개수 그리고 Gate 를 통과한 개수, 현재 LEVEL 이 표시된다. 이 윈도우 창을 보며 현재 게임 진행 상황을 알 수 있다.
+ Mission Board 에는 현재 레벨에 맞는 적당한 미션이 주어진다. 스네이크의 미션 길이와 먹어야 할 Growth Item 의 수, Poison Item 의 한계가 정해져 있고 실시간으로 내 상황과 함께 비교하여 출력된다. /(슬래시)로 구분되어 지며 슬래시 왼쪽은 현재 나의 상황, 슬래시 오른쪽은 미션 개수가 나타난다.    

2 단계: Snake 표현 및 조작
+ Snake의 처음 시작 길이는 3으로 한다.
+ Snake 의 처음 시작 방향은 오른쪽으로 설정한다.
+ Snake 의 진행 반대 방향의 키를 입력하면 게임이 종료되도록 한다.
+ 입력하는 방향키에 따라 Snake 가 움직이도록 한다
+ Snake 의 머리가 자신의 몸에 닿으면 게임이 종료되어야 한다.
+ Snake 의 머리가 벽에 닿으면 게임이 종료되어야 한다.
+ Snake 가 게임 윈도우 창 밖으로 가면 게임이 종료되어야 한다.
+ Snake 가 Growth Item 을 먹으면 길이가 1 늘어나고 점수가 증가.
+ Snake 가 Poison Item 을 먹으면 길이가 1 감소하고 점수가 감소한다.
+ 몸의 길이가 3 미만이 되면 게임이 종료되도록 설정한다.
+ Snake 는 Gate 를 통해 맵의 이곳저곳을 움직일 수 있도록 한다.

3단계: Item 요소의 구현
+ Growth Item 과 Poison Item 은 일정한 시간 간격으로 아이템 위치가 랜덤으로 바뀌도록 한다.
+ Growth Item 과 Poison Item 은 Snake 와 벽과 겹치지 않게 생성해야 한다.
+ Growth Item 과 Poison Item 은 동시에 3 개 이상 생성하지 않아야 한다.
+ Growth Item 과 Poison Item 의 역할
    + Growth – 몸의 길이가 1 증가 해야한다.
    + Poison – 몸의 길이가 1 감소 해야한다. (꼬리 부분이 감소한다)
+ 아이템을 획득하면 랜덤으로 위치가 다시 바뀌도록 한다.

4단계: Gate 요소의 구현
+ Gate는두개가한쌍이어야한다.
+ Gate 는 겹치지 않아야 한다.
+ Gate는map의임의의위치에서한쌍이생성되어야한다.
+ 각 Gate를 통해 Snake가 통과할 수 있도록 한다.
+ 일정 시간 간격으로 Gate의 위치가 바뀌는데, Snake가 통과 중일 땐 생성 혹은 사라지지 않도록 한다.

5단계: 점수 요소의 구현
+ Score Board 에는 현재까지 Snake 가 획득한 점수가 표시하도록 한다. ü B : 현재 Snake 의 길이 / 최대 길이
    + 게임 중 획득한 Growth Item 의 개수
    + 게임 중 획득한 Poison Item 의 개수
    + 게임 중 통과한 Gate 의 수
    + 게임 시간 (Seconds)
+ 주어진 Mission 을 달성해야 한다.
+ Mission 을 달성할 시 다음 map 스테이지로 넘어가도록 구현한다.
    + 스테이지는 최소 4 개로 구성하고 미션 난이도가 조금씩 상승한다.
    
## 3 결과물 내용
| 파일명 | 역할 |
| --- | --- | 
| snake.cpp | <snake.cpp> 파일 안에 모든 기능이 다 들어 있다. Snake 메소드 선언되어 있고 그 메소드를 구현했다. 뿐만 아니라 game 에 필요한 item, gate, mission 등 여러 요소들이 선언, 구현되어 있다. 3 개의 윈도우 화면을 띄우는 것, 일정 이상 진행되면 다음 stage 로 넘어가는 부분까지 모두 구현되어 있다. |
| Makefile | 컴파일을 하기 위한 makefile이다. 터미널에 ./snake 라고 입력하면 게임이 실행된다. |
