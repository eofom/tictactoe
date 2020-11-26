#include <iostream>
#include <vector>

using namespace std;

#include "board.h"


//------------------------------------------------------------------------------
std::istream & ReadMove(std::istream & is, Board::Move & move) {
  char bracket, comma;
  is >> bracket >> move.row >> comma >> move.column >> bracket;

  return is;
}

//------------------------------------------------------------------------------
std::istream & ReadAllMoves(std::istream & is, std::vector<Board::Move> & moves) {
  char bracket, comma;
  std::cin >> bracket;
  do {
    Board::Move move(0,0);
    ReadMove(std::cin, move);
    // FIXME Use emplace to avoid copying
    moves.push_back(move);
    std::cin >> comma;

    std::clog << "row = " << move.row
              << ", column = " << move.column
              << std::endl;
  } while (',' == comma);

  return is;
}

//------------------------------------------------------------------------------
int main() {
  std::vector<Board::Move> moves;
  ReadAllMoves(std::cin, moves);

//  std::clog << moves.size() << " moves were made" << std::endl;
  Board board = Board::Create(moves);
  std::cout << board << std::endl;

  if (board.isWinnerX()) {
    std::cout << "A" << std::endl;
  } else if (board.isWinnerO()) {
    std::cout << "B" << std::endl;
  } else if (board.possibleMoves().empty()) {
    std::cout << "Draw" << std::endl;
  } else {
    std::cout << "Pending" << std::endl;
  }
}
