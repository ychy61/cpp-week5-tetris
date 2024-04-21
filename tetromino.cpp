#include "tetromino.h"

Tetromino::Tetromino(std::string name, int size, std::string shape){
      name_ = name;
      size_ = size;

      for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
            shape_[row][col] = (shape[row * size + col] == 'O');
            }
      }

      if (name == "I") {
      original_ = &Tetromino::I;
      } else if (name == "O") {
      original_ = &Tetromino::O;
      } else if (name == "T") {
      original_ = &Tetromino::T;
      } else if (name == "S") {
      original_ = &Tetromino::S;
      } else if (name == "Z") {
      original_ = &Tetromino::Z;
      } else if (name == "J") {
      original_ = &Tetromino::J;
      } else if (name == "L") {
      original_ = &Tetromino::L;
      }


      // original_;

}

Tetromino Tetromino::rotatedCW(){
  std::string newShape(size_ * size_, 'X');
      for (int y = 0; y < size_; ++y) {
            for (int x = 0; x < size_; ++x) {
                  int newX = size_ - y - 1;
                  int newY = x;
                  newShape[newY * size_ + newX] = (shape_[y][x] ? 'O' : 'X');
            }
      }
      Tetromino rotated(name_, size_, newShape);
      return rotated;
}

Tetromino Tetromino::rotatedCCW(){
  std::string newShape(size_ * size_, 'X');
      for (int y = 0; y < size_; ++y) {
            for (int x = 0; x < size_; ++x) {
                  // 회전 공식 적용
                  int newX = size_ - y - 1;
                  int newY = x;
                  newShape[newY * size_ + newX] = (shape_[y][x] ? 'O' : 'X');
            }
      }
      return Tetromino(name_, size_, newShape);
}

// 화면의 x, y 위치에 s 문자열로  테트로미노를 그린다
void Tetromino::drawAt(std::string s, int x, int y){
      for (int i = 0; i < size_; i++)
      {
            for (int j = 0; j < size_; j++)
            {
                  if (shape_[i][j] == true)
                  console::draw(x + j, y + i, s);
            }
      }
}

// 각 테트로미노 종류에 대한 선언
Tetromino Tetromino::T("T", 3, "XOXOOOXXX");
Tetromino Tetromino::I("I", 4, "XXXXOOOOXXXXXXXX");
Tetromino Tetromino::O("O", 2, "OOOO");
Tetromino Tetromino::S("S", 3, "XOOOOXXXX");
Tetromino Tetromino::Z("Z", 3, "OOXXOOXXX");
Tetromino Tetromino::J("J", 3, "OXXOOOXXX");
Tetromino Tetromino::L("L", 3, "XXOOOOXXX");
