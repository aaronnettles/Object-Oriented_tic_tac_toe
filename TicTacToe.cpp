/*

This is a basic object oriented Tic Tac Toe program.

Author: Aaron Nettles

*/

#include <stdexcept>
#include <iostream>
#include <vector>

//------------------------------------------------------------------------//

// game board
class Board {

  std::vector<std::vector<char>> slots { {'_', '_', '_'},
                                         {'_', '_', '_'},
                                         {'_', '_', '_'} };

  // the 'Three' functions below test for a filled diagonal, row, or column
  // 'O' for a win for 'O'
  // 'X' for a win for 'X'
  // '_' for incomplete

  char diagonalThree() {
    char diagonal = '_';

    if(slots[1][1] != '_') {
      if(slots[0][0] == slots[1][1] && slots[1][1] == slots[2][2]) {

        diagonal = slots[1][1];
      }
      if(slots[0][2] == slots[1][1] && slots[1][1] == slots[2][0]) {

        diagonal = slots[1][1];
      }
    }

    return diagonal;
  }

  char horizontalThree() {
    char horizontal = '_';

    for(size_t r = 0; r < 3; ++r) {
      if(slots[r][0] == slots[r][1] && slots[r][1] == slots[r][2]) {
        horizontal = slots[r][1];
        break;
      }
    }

    return horizontal;
  }

  char verticalThree() {
    char vertical = '_';

    for(size_t c = 0; c < 3; ++c) {
      if(slots[0][c] == slots[1][c] && slots[1][c] == slots[2][c]) {
        vertical = slots[1][c];
        break;
      }
    }

    return vertical;
  }

public:

  // basic constructor
  Board() {}

  // basic destructor
  ~Board() {}

  // element access operators for slots
  char& operator() (size_t r, size_t c) {
    if(r >= 3  || c >= 3) {
      throw std::out_of_range("index out of bounds");
    }
    return slots[r][c];
  }
  
  char operator() (size_t r, size_t c) const {
    if(r >= 3  || c >= 3) {
      throw std::out_of_range("index out of bounds");
    }
    return slots[r][c];
  }

  // copy the slot contents of another board
  void copySlots(const Board& B) {
    for(size_t r = 0; r < 3; ++r) {
      for(size_t c = 0; c < 3; ++c) {
        slots[r][c] = B(r, c);
      }
    }
  }

  // show board in window
  void printSlots() {
    for(size_t r = 0; r < 3; ++r) {
      for(size_t c = 0; c < 3; ++c) {
        std::cout << slots[r][c] << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  // put a piece in a slot
  void addAPiece(char piece, size_t r, size_t c) {
    slots[r][c] = piece;
  }

  // determine state of board
  char stateOfSlots() {

    // number of empty slots
    size_t emptySlots = 0;

    // loop to test for a draw condition
    for(size_t r = 0; r < 3; ++r) {
      for(size_t c = 0; c < 3; ++c) {
        if(slots[r][c] == '_') {
          ++emptySlots;
        }
      }
    }

    // 'D' for a draw
    // 'O' for a win for 'O'
    // 'X' for a win for 'X'
    // '_' for incomplete

    if(emptySlots == 0) {

      return 'D';

    } else {

      char state = horizontalThree();
      if(state != '_') {return state;}

      state = verticalThree();
      if(state != '_') {return state;}

      state = diagonalThree();
      return state;

    }
  }

  // clear slots of board
  void clearSlots() {
    for(size_t r = 0; r < 3; ++r) {
      for(size_t c = 0; c < 3; ++c) {
        slots[r][c] = '_';
      }
    }
  }

};

//------------------------------------------------------------------------//

// computer player
class AI {

  // takes on 'X' or 'O' value
  char team;

  // for opposing team
  char other;

  // row choice
  size_t row;

  // col choice
  size_t col;

  // ranks stored as
  // 'W' for a winning slot and
  // 'B' for a potential blocking position
  std::vector<std::vector<char>> ranks { {'_', '_', '_'},
                                         {'_', '_', '_'},
                                         {'_', '_', '_'} };

  // place 'W' or 'B' where '_' is found as necessary
  void setDiagonalRanks() {

    // three potentially empty slots for diagonal from
    // the top left to the bottom right corner
    if((ranks[0][0] == '_' || ranks[0][0] == 'B') && ranks[1][1] == ranks[2][2]) {
      if(ranks[1][1] == team) {ranks[0][0] = 'W';}
      if(ranks[1][1] == other) {ranks[0][0] = 'B';}
    }
    if((ranks[1][1] == '_' || ranks[1][1] == 'B') && ranks[2][2] == ranks[0][0]) {
      if(ranks[2][2] == team) {ranks[1][1] = 'W';}
      if(ranks[2][2] == other) {ranks[1][1] = 'B';}
    }
    if((ranks[2][2] == '_' || ranks[2][2] == 'B') && ranks[0][0] == ranks[1][1]) {
      if(ranks[0][0] == team) {ranks[2][2] = 'W';}
      if(ranks[0][0] == other) {ranks[2][2] = 'B';}
    }

    // three potentially empty slots for diagonal from
    // the top right to the bottom left corner
    if((ranks[0][2] == '_' || ranks[0][2] == 'B') && ranks[1][1] == ranks[2][0]) {
      if(ranks[1][1] == team) {ranks[0][2] = 'W';}
      if(ranks[1][1] == other) {ranks[0][2] = 'B';}
    }
    if((ranks[1][1] == '_' || ranks[1][1] == 'B') && ranks[2][0] == ranks[0][2]) {
      if(ranks[2][0] == team) {ranks[1][1] = 'W';}
      if(ranks[2][0] == other) {ranks[1][1] = 'B';}
    }
    if((ranks[2][0] == '_' || ranks[2][0] == 'B') && ranks[0][2] == ranks[1][1]) {
      if(ranks[0][2] == team) {ranks[2][0] = 'W';}
      if(ranks[0][2] == other) {ranks[2][0] = 'B';}
    }
  }
  
  void setHorizontalRanks() {

    // loop from top to bottom and test from left to right
    for(size_t r = 0; r < 3; ++r) {
        
      // three potentially empty slots for horizontal
      if((ranks[r][0] == '_' || ranks[r][0] == 'B') && ranks[r][1] == ranks[r][2]) {
        if(ranks[r][1] == team) {ranks[r][0] = 'W';}
        if(ranks[r][1] == other) {ranks[r][0] = 'B';}
      }
      if((ranks[r][1] == '_' || ranks[r][1] == 'B') && ranks[r][2] == ranks[r][0]) {
        if(ranks[r][2] == team) {ranks[r][1] = 'W';}
        if(ranks[r][2] == other) {ranks[r][1] = 'B';}
      }
      if((ranks[r][2] == '_' || ranks[r][2] == 'B') && ranks[r][0] == ranks[r][1]) {
        if(ranks[r][0] == team) {ranks[r][2] = 'W';}
        if(ranks[r][0] == other) {ranks[r][2] = 'B';}
      }
    }
  }
  
  void setVerticalRanks() {

    // loop from top to bottom and test from left to right
    for(size_t c = 0; c < 3; ++c) {
        
      // three potentially empty slots for vertical
      if((ranks[0][c] == '_' || ranks[0][c] == 'B') && ranks[1][c] == ranks[2][c]) {
        if(ranks[1][c] == team) {ranks[0][c] = 'W';}
        if(ranks[1][c] == other) {ranks[0][c] = 'B';}
      }
      if((ranks[1][c] == '_' || ranks[1][c] == 'B') && ranks[2][c] == ranks[0][c]) {
        if(ranks[2][c] == team) {ranks[1][c] = 'W';}
        if(ranks[2][c] == other) {ranks[1][c] = 'B';}
      }
      if((ranks[2][c] == '_' || ranks[2][c] == 'B') && ranks[0][c] == ranks[1][c]) {
        if(ranks[0][c] == team) {ranks[2][c] = 'W';}
        if(ranks[0][c] == other) {ranks[2][c] = 'B';}
      }
    }
  }

  // set ranks based on diagonal, horizontal, and vertical doubles
  // use 'W', 'B' and 'E' for positions
  void setRanks(const Board& B) {

    // populate ranks with '_', 'X' and 'O'
    for(size_t r = 0; r < 3; ++r) {
      for(size_t c = 0; c < 3; ++c) {
        ranks[r][c] = B(r, c);
      }
    }

    // place 'W' or 'B' where necessary
    setDiagonalRanks();
    setHorizontalRanks();
    setVerticalRanks();
  }

public:

  // basic constructor
  AI() {}

  // basic destructor
  ~AI() {}

  // determine team
  void determineTeam(char t) {
    if(t == 'X') {
      team = 'X';
      other = 'O';
    } else {
      team = 'O';
      other = 'X';
    }
  }

  // make a slot choice
  void makeChoice(const Board& B) {

    // set ranks for slots
    setRanks(B);

    // choieMade variable
    bool choiceMade = false;

    // make choice for row and col
    for(size_t r = 0; r < 3; ++r) {
      for(size_t c = 0; c < 3; ++c) {
        if(ranks[r][c] == 'W') {
          row = r;
          col = c;
          choiceMade = true;
        }
      }
    }

    // if a winning position is not found...
    if(choiceMade == false) {
      for(size_t r = 0; r < 3; ++r) {
        for(size_t c = 0; c < 3; ++c) {
          if(ranks[r][c] == 'B') {
            row = r;
            col = c;
            choiceMade = true;
          }
        }
      }
    }

    // if a blocking position is not found...
    if(choiceMade == false) {
      if(ranks[1][1] == '_') {
        row = 1;
        col = 1;
        choiceMade = true;
      }
    }

    // if a center position is not found...
    if(choiceMade == false) {
      for(size_t r = 0; r < 3; r += 2) {
        for(size_t c = 0; c < 3; c += 2) {
          if(ranks[r][c] == '_') {
            row = r;
            col = c;
            choiceMade = true;
          }
        }
      }
    }
            
    // if a corner position is not found...
    if(choiceMade == false) {
      for(size_t r = 0; r < 3; ++r) {
        for(size_t c = 0; c < 3; ++c) {
          if(ranks[r][c] == '_') {
            row = r;
            col = c;
            choiceMade = true;
          }
        }
      }
    }
  }

  // show row choice
  size_t choiceRow() {return row;}

  // show col choice
  size_t choiceCol() {return col;}

};

//------------------------------------------------------------------------//

// game interface
class TicTacToe {

  // initial board object
  Board B;

  // initial computer opponent
  AI Computer;

  void playAGame(char humanTeam, char compTeam) {

    // clear board for a game
    B.clearSlots();

    // determine team for computer opponent
    Computer.determineTeam(compTeam);

    // choice variables
    size_t humanRow, humanCol, compRow, compCol;

    // loop while the game is in progress
    // a 'X' signifies a win for 'X',
    // a 'O' signifies a win for 'O' and
    // a 'D' signifies a draw

    char state = '_';
    while(state == '_') {

      if(humanTeam == 'X') {

        // loop for human choice
        while(true) {

          B.printSlots();

          std::cout << "Row Number: ";
          std::cin >> humanRow;
          std::cout << "Column Number: ";
          std::cin >> humanCol;
          std::cout << std::endl;

          // determine if slot is available
          if(humanRow > 2 || humanCol > 2) {

            std::cout << "That slot number does not exist." << std::endl;
          }
          else if(B(humanRow, humanCol) == '_') {

            B.addAPiece(humanTeam, humanRow, humanCol);
            break;

          } else {

            std::cout << "That slot has been taken." << std::endl;
          }
        }

        state = B.stateOfSlots();

        if(state == '_') {

          // cause computer to choose slot
          Computer.makeChoice(B);

          // get choice from computer
          compRow = Computer.choiceRow();
          compCol = Computer.choiceCol();
          
          // add a piece to the board
          B.addAPiece(compTeam, compRow, compCol);
          state = B.stateOfSlots();
        }
        
      } else {

        // cause computer to choose slot
        Computer.makeChoice(B);

        // get choice from computer
        compRow = Computer.choiceRow();
        compCol = Computer.choiceCol();
        
        // add a piece to the board
        B.addAPiece(compTeam, compRow, compCol);
        state = B.stateOfSlots();
        
        if(state == '_') {

          // loop for human choice
          while(true) {

            B.printSlots();

            std::cout << "Row Number: ";
            std::cin >> humanRow;
            std::cout << "Column Number: ";
            std::cin >> humanCol;
            std::cout << std::endl;

            // determine if slot is available
            if(humanRow > 2 || humanCol > 2) {

              std::cout << "That slot number does not exist." << std::endl;
            }
            else if(B(humanRow, humanCol) == '_') {

              B.addAPiece(humanTeam, humanRow, humanCol);
              break;

            } else {

              std::cout << "That slot has been taken." << std::endl;
            }
          }

          state = B.stateOfSlots();
        }
      }
    }

    B.printSlots();

    if(state == 'D') {
      std::cout << "There has been a draw." << std::endl;
    } else if(state == humanTeam) {
      std::cout << "Congratulations! You have won." << std::endl;
    } else if(state == compTeam) {
      std::cout << "I am sorry for you :-(. The computer has won." << std::endl;
    } else {
      std::cout << "There has been an error of some sort." << std::endl;
    }
    std::cout << std::endl;
  }

public:

  // basic constructor
  TicTacToe() {}

  // basic destructor
  ~TicTacToe() {}

  // main interface for game
  void interface() {

    // play variable for game
    char play;

    // human and computer team variables
    char humanTeam;
    char compTeam;

    // determine whether user desires to play a game
    while(true) {

      std::cout << "Do you wish to play a game of Tic Tac Toe? (y/n) ";
      std::cin >> play;
      std::cout << std::endl;

      if(play == 'n' || play == 'N') {

        break;

      } else {

        // determine team of user
        std::cout << "Do you wish to play with 'X' or 'O'? ";
        std::cin >> humanTeam;
        std::cout << std::endl;

        // finalize team of user and team of computer
        if(humanTeam == 'X' || humanTeam == 'x') {
          compTeam = 'O';
          humanTeam = 'X';
        } else if(humanTeam == 'O' || humanTeam == 'o') {
          compTeam = 'X';
          humanTeam = 'O';
        } else {
          humanTeam = 'X';
          compTeam = 'O';
        }

        // start a game of Tic Tac Toe
        playAGame(humanTeam, compTeam);
      }
    }
  }

};

//------------------------------------------------------------------------//

int main() {

  // initialize game object
  TicTacToe TTT;

  // run interface method for game
  TTT.interface();

  return 0;
}
