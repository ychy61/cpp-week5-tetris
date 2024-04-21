#include "game.h"

// 테트로미노가 다른 블록이나 바닥에 닿는지 검사하는 함수
bool Game::checkCollision(int x, int y) {
      for (int i = 0; i < currentTetromino.size(); i++) {
            for (int j = 0; j < currentTetromino.size(); j++) {
                  if (currentTetromino.check(i, j)) {
                  int newX = x + j;
                  int newY = y + i;

                  if (newX < 0 || newX >= BOARD_WIDTH || newY >= BOARD_HEIGHT || (newY >= 1 && board_[newX][newY])) {
                        return true;
                  }
                  }
            }
      }
      return false;
}

// 그림자의 Y 위치를 찾는 함수
int Game::findShadowY() {
      int shadowY = currentY;
      // 최대한 아래로 이동하며 충돌이 일어나는 위치를 찾습니다.
      while (!checkCollision(currentX, shadowY + 1)) {
            shadowY++;
      }
      return shadowY;
}

// 게임 화면을 그리는 함수
void Game::draw() {

      // 기본 맵 그리기
      console::drawBox(0, 0, BOARD_WIDTH, BOARD_HEIGHT);

      // NEXT 박스 그리기
      int boxsize = 6;
      int nextBoxMinX = BOARD_WIDTH + 1;
      int nextBoxMaxX = BOARD_WIDTH + boxsize;
      int nextTextX = nextBoxMinX + (boxsize - 4) / 2;
      int nextX = nextBoxMinX + (boxsize - sizeof(nextTetromino)) / 2;

      console::drawBox(nextBoxMinX, 0, nextBoxMaxX, 5);
      console::draw(nextTextX, 0, "NEXT");
      nextTetromino.drawAt(BLOCK_STRING, nextBoxMinX + 2, 2);

      // Hold 박스 그리기
      int holdBoxMinX = nextBoxMaxX + 1;
      int holdBoxMaxX = holdBoxMinX + boxsize - 1;
      int holdTextX = holdBoxMinX + (boxsize - 4) / 2;

      console::drawBox(holdBoxMinX, 0, holdBoxMaxX, 5);
      console::draw(holdTextX, 0, "HOLD");

      // 만약 홀드된 테트로미노가 있다면 그립니다.
      if (holdTetromino) {
            holdTetromino->original()->drawAt(BLOCK_STRING, holdBoxMinX + 2, 2);
      }

      // 남은 줄 표시
      std::string lineText = std::to_string(LINES) + " lines left";
      int lineTextX;
      if (lineText.length() > BOARD_WIDTH) {
            lineTextX = 0;
      } else {
            lineTextX = (BOARD_WIDTH - lineText.length()) / 2 + 1;
      }
      console::draw(lineTextX, BOARD_HEIGHT + 1, lineText);

      // 경과 시간 표시
      end = clock();
      double time = double(end - start) / CLOCKS_PER_SEC;
      int minutes = static_cast<int>(time / 60);
      int seconds = static_cast<int>(time) % 60;
      int milliseconds = static_cast<int>((time - static_cast<int>(time)) * 1000);
      std::ostringstream oss;
      oss << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setfill('0') << std::setw(2) << seconds << "."
            << std::setfill('0') << std::setw(2) << milliseconds;
      std::string timeString = oss.str();
      int timeTextX = (BOARD_WIDTH - timeString.length()) / 2 + 1;
      console::draw(timeTextX, BOARD_HEIGHT + 2, timeString);

      // 그림자 표시
      int shadowY = findShadowY();
      currentTetromino.drawAt(SHADOW_STRING, currentX, shadowY);

      // 테트로미노 그리기
      currentTetromino.drawAt(BLOCK_STRING, currentX, currentY);

      //이전에 기록된 테트로미노 그리기
      for (int x = 0; x < BOARD_WIDTH; x++) {
            for (int y = 0; y < BOARD_HEIGHT; y++) {
                  if (board_[x][y]) {
                  console::draw(x, y, BLOCK_STRING);
                  }
            }
      }

      if (win) {
            std::string winText = "You Win";
            console::draw((BOARD_WIDTH - winText.length()) / 2, BOARD_HEIGHT / 2, winText);
            double time = double(winTime - start) / CLOCKS_PER_SEC;
            int minutes = static_cast<int>(time / 60);
            int seconds = static_cast<int>(time) % 60;
            int milliseconds = static_cast<int>((time - static_cast<int>(time)) * 1000);
            std::ostringstream oss;
            oss << std::setfill('0') << std::setw(2) << minutes << ":"
                  << std::setfill('0') << std::setw(2) << seconds << "."
                  << std::setfill('0') << std::setw(2) << milliseconds;
            std::string timeString = oss.str();
            int timeTextX = (BOARD_WIDTH - timeString.length()) / 2 + 1;
            console::draw(timeTextX, BOARD_HEIGHT / 2 + 1, timeString);
      } else if (lose) {
            std::string loseText = "You Lose";
            console::draw((BOARD_WIDTH - loseText.length()) / 2, BOARD_HEIGHT / 2, loseText);
      }
}


// 테트로미노 고정 함수
void Game::fixTetromino() {
      for (int i = 0; i < currentTetromino.size(); i++) {
            for (int j = 0; j < currentTetromino.size(); j++) {
                  if (currentTetromino.check(i, j)) {
                        int newX = currentX + j;
                        int newY = currentY + i;
                        if (newX >= 0 && newX < BOARD_WIDTH && newY < BOARD_HEIGHT) {
                              board_[newX][newY] = true; // 보드에 테트로미노 고정
                        }
                  }
            }
      }
}


// 랜덤 테트로미노 생성 함수
Tetromino Game::getRandomTetromino() {
      // 중앙에서 시작하도록 위치 초기화
      currentX = (BOARD_WIDTH - currentTetromino.size()) / 2;
      currentY = 1;
      checkhold = false;
      // 0부터 6 사이의 난수 생성
      int randomIndex = rand() % 7;
      // 난수에 따라 테트로미노 선택
      if (randomIndex == 0) {
            return Tetromino::I;
      } else if (randomIndex == 1) {
            return Tetromino::O;
      } else if (randomIndex == 2) {
            return Tetromino::T;
      } else if (randomIndex == 3) {
            return Tetromino::S;
      } else if (randomIndex == 4) {
            return Tetromino::Z;
      } else if (randomIndex == 5) {
            return Tetromino::J;
      } else {
            return Tetromino::L;
      }
}

Game::Game(){
      win = false; // 승리 여부
      lose = false; //패배 여부
      totalLines = LINES; // 총 줄 수
      dropTimer = DROP_DELAY;
      currentTetromino = getRandomTetromino(); //현재 테트로미노(랜덤 테트로미노로 초기화)
      nextTetromino = getRandomTetromino(); //다음 테트로 미노
      currentX = (BOARD_WIDTH - currentTetromino.size())/ 2+1; // 시작 위치 X
      currentY = 1; // 시작 위치 Y
      shadowY = 20; // 그림자 Y
      checkhold = false; //홀드 여부
      // checkfix = false; //테트로미노 고정 여부

      // 게임 보드 초기화
      for(int i = 0; i < BOARD_WIDTH; i++){
            for(int j = 0; j < BOARD_HEIGHT; j++){
                  board_[i][j] = false;
            }
      }
      srand((unsigned int)time(nullptr));

      start = clock(); //게임 시작 시간
}

// 라인 클리어 함수
void Game::clearLines() {
      for (int i = 0; i < BOARD_HEIGHT; ++i) {
            int check = 0; // 한 줄이 다 찼는지 확인하는 변수
            // 현재 줄이 다 찼는지 확인
            for (int j = 0; j < BOARD_WIDTH; ++j) {
                  if (board_[j][i] == false) { // 하나라도 비어있으면
                  check = 0; // 체크 초기화
                  break;
                  } else if (board_[j][i] == true) { // 블록이 채워져 있으면
                  check++; // 체크 증가
                  }
            }
            // 한 줄이 다 찼을 경우
            if (check == BOARD_WIDTH-2) {
                  // 현재 줄을 삭제하고 한 칸씩 위로 당김
                  for (int y = i; y > 0; --y) {
                  for (int x = 0; x < BOARD_WIDTH; ++x) {
                        board_[x][y] = board_[x][y - 1];
                  }
                  }
                  // 맨 윗줄은 비움
                  for (int x = 0; x < BOARD_WIDTH; ++x) {
                  board_[x][0] = false;
                  }
            }
      }
}



// 테트로미노를 주어진 방향으로 이동하는 함수
void Game::moveTetromino(int direaction) {
      bool canMove = true; // 이동 가능 여부 플래그

      // 테트로미노의 모든 블록에 대해 이동 가능 여부 확인
      for (int i = 0; i < currentTetromino.size(); ++i) {
            for (int j = 0; j < currentTetromino.size(); ++j) {
                  if (currentTetromino.check(i, j)) {
                  int newX = currentX + j + direaction; // 새로운 x 좌표 계산
                  int newY = currentY + i; // 현재 y 좌표 사용

                  // 보드를 벗어나거나 다른 블록과 충돌하는지 여부 확인
                  if (newX < 1 || newX >= BOARD_WIDTH || (newY >= 0 && board_[newX][newY])) {
                        canMove = false; // 이동 불가능한 경우 플래그 설정
                        break;
                  }
                  }
            }
            if (!canMove) { // 이동 불가능한 경우 반복문 종료
                  break;
            }
      }

      // 이동 가능한 경우 테트로미노를 이동시킴
      if (canMove) {
            currentX += direaction; // 주어진 방향으로 이동
      }
}

// 게임 업데이트 함수
void Game::update() {
      if (!win && !lose) {
            dropTimer--; // 드롭 타이머 감소
            if (dropTimer == 0) {
                  dropTimer = DROP_DELAY; // 드롭 딜레이 초기화
                  // 충돌이 발생하지 않으면 아래로 한 칸 이동
                  if (!checkCollision(currentX, currentY + 1)) {
                        currentY++;
                  } else {
                        fixTetromino(); // 충돌 발생 시 테트로미노 고정
                        currentTetromino = nextTetromino; // 다음 테트로미노를 현재로 설정
                        nextTetromino = getRandomTetromino(); // 새로운 다음 테트로미노 생성
                        clearLines(); // 라인 클리어
                  }
            }
            // 이동 키 입력 처리
            if (key(console::K_LEFT)) {
                  moveTetromino(-1);
            }
            if (key(console::K_RIGHT)) {
                  moveTetromino(1);
            }
            if (key(console::K_UP)) {
                  // 하드드롭
                  currentY = findShadowY(); // 그림자 위치로 이동
                  fixTetromino(); // 테트로미노 고정

                  currentTetromino = nextTetromino; // 다음 테트로미노를 현재로 설정
                  nextTetromino = getRandomTetromino(); // 새로운 다음 테트로미노 생성
                  clearLines(); // 라인 클리어
            }
            if (key(console::K_DOWN)) {
                  currentY++; // 소프트 드롭
            }
            // 홀드 키 입력 처리
            if (key(console::K_SPACE) && !checkhold) {
                  if (!holdTetromino) { // 아직 홀드된 테트로미노가 없는 경우
                        holdTetromino = new Tetromino(currentTetromino); // 현재 테트로미노를 홀드
                        currentTetromino = nextTetromino; // 다음 테트로미노를 현재로 설정
                        nextTetromino = getRandomTetromino(); // 새 테트로미노 생성
                  } else {
                        // 홀드된 테트로미노가 있는 경우
                        Tetromino temp = *holdTetromino->original(); // 홀드된 테트로미노 임시 저장
                        *holdTetromino = currentTetromino; // 현재 테트로미노를 홀드 위치에 저장
                        currentTetromino = temp; // 홀드된 테트로미노를 현재 테트로미노로 설정
                  }
                  checkhold = true;
            }
            // 회전 키 입력 처리
            if (key(console::K_X)) {
                  Tetromino original = currentTetromino;
                  currentTetromino = currentTetromino.rotatedCW();
                  if (checkCollision(currentX, currentY)) {
                        currentTetromino = original;
                  }
            }
            if (key(console::K_Z)) {
                  Tetromino original = currentTetromino;
                  currentTetromino = currentTetromino.rotatedCCW();
                  if (checkCollision(currentX, currentY)) {
                        currentTetromino = original;
                  }
            }
            // 승리 여부 확인
            if (totalLines == 0) {
                  win = true;
                  winTime = clock();
                  std::string loseText = "You Win";
                  console::draw(1, BOARD_HEIGHT/2, loseText);
                  double time = double(winTime - start) / CLOCKS_PER_SEC;
                  int minutes = static_cast<int>(time / 60);
                  int seconds = static_cast<int>(time) % 60;
                  int milliseconds = static_cast<int>((time - static_cast<int>(time)) * 1000);
                  std::ostringstream oss;
                  oss << std::setfill('0') << std::setw(2) << minutes << ":"
                        << std::setfill('0') << std::setw(2) << seconds << "."
                        << std::setfill('0') << std::setw(2) << milliseconds;
                  std::string timeString = oss.str();
                  int timeTextX = (BOARD_WIDTH - timeString.length()) / 2 + 1;
                  console::draw(timeTextX, BOARD_HEIGHT/2+1, timeString);
            }

            for (int x = 0; x < BOARD_WIDTH; ++x) {
                  if (board_[x][1]) {
                        lose = true;
                        std::string loseText = "You Lose";
                        console::draw(1, BOARD_HEIGHT/2, loseText);
                  }
            }
      }
}

// 종료 여부 확인 함수
bool Game::shouldExit() {
      // ESC 키 입력 시 종료
      if (key(console::K_ESC)|| win || lose) {
            return true;
      }
      return false;
}