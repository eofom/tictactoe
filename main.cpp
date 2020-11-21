#include <iostream>
#include <vector>

struct Move {
  size_t row = 0;
  size_t column = 0;
};

//------------------------------------------------------------------------------
std::istream & ReadMove(std::istream & is, Move & move) {
  char bracket, comma;
  is >> bracket >> move.row >> comma >> move.column >> bracket;

  return is;
}

//------------------------------------------------------------------------------
std::istream & ReadAllMoves(std::istream & is, std::vector<Move> & moves) {
  char bracket, comma;
  std::cin >> bracket;
  do {
    Move move;
    ReadMove(std::cin, move);
    // FIXME Use emplace to avoid copying
    moves.push_back(move);
    std::cin >> comma;

    std::cout << "row = " << move.row
              << ", column = " << move.column
              << std::endl;
  } while (',' == comma);

  return is;
}

//------------------------------------------------------------------------------
int main() {
  std::vector<Move> moves;
  ReadAllMoves(std::cin, moves);

  std::cout << moves.size() << " moves were made" << std::endl;
}
