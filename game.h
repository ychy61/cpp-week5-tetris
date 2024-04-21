#include "console/console.h"
#ifndef GAME_H
#define GAME_H

#include "tetromino.h"
#include <sstream>  // std::ostringstream를 사용하기 위해 포함
#include <iomanip>  // std::setfill과 std::setw를 사용하기 위해 포함

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 40

#define DROP_DELAY 60

class Game {
private:
  // 게임 판을 나타내는 배열
  // board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을 의미한다
  bool board_[BOARD_WIDTH][BOARD_HEIGHT];

  int lineTextX = 0;
  clock_t start;
  clock_t end;
  clock_t winTime;

  bool win = false;
  bool lose = false;
  int totalLines = LINES;
  int dropTimer = DROP_DELAY;
  int shadowY;
  int currentX, currentY;
  Tetromino currentTetromino = Tetromino::I;
  Tetromino nextTetromino = Tetromino::I;
  Tetromino *holdTetromino;
  Tetromino getRandomTetromino();
  //   void fixTetromino();
  //   bool checkMove(int newX, int newY, const Tetromino &tetromino);
  bool checkCollision(int x, int y);
  int findShadowY();
  bool checkhold;
  bool checkfix;
  void fixTetromino();
  void clearLines();
  // void moveRight();
  // void moveLeft();
  void moveTetromino(int direaction);

public:
  // 게임의 한 프레임을 처리한다.
  void update();

  // 게임 화면을 그린다.
  void draw();

  // 게임 루프가 종료되어야 하는지 여부를 반환한다.
  bool shouldExit();

  Game();
};
#endif