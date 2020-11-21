#include <algorithm>
#include <iostream>
#include <vector>

static const size_t ROWLENGTH = 3;
static const size_t COLUMNLENGTH = 3;

struct Move {
  Move(size_t row, size_t column)
    : row(row),
      column(column)
  {}

  size_t row = 0;
  size_t column = 0;
};

//------------------------------------------------------------------------------
bool operator ==(const Move & lhs, const Move rhs) {
  return lhs.row == rhs.row
      && lhs.column == rhs.column;
}

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
    Move move(0,0);
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
std::vector<Move> getOddMoves(const std::vector<Move> & moves) {
  std::vector<Move> odd_moves;
  for (size_t i = 0; i < moves.size(); ++i) {
    if (i % 2 == 0) {
      odd_moves.push_back(moves.at(i));
    }
  }

  return odd_moves;
}

//------------------------------------------------------------------------------
std::vector<Move> getEvenMoves(const std::vector<Move> & moves) {
  std::vector<Move> even_moves;
  for (size_t i = 0; i < moves.size(); ++i) {
    if (i % 2) {
      even_moves.push_back(moves.at(i));
    }
  }

  return even_moves;
}

//------------------------------------------------------------------------------
bool isRowFilled(size_t rownum, const std::vector<Move> & moves) {
  std::vector<Move> row_items;
  for (const Move & cell : moves) {
    if (cell.row == rownum) {
      row_items.push_back(cell);
    }
  }

  return row_items.size() == ROWLENGTH;
}

//------------------------------------------------------------------------------
bool isColumnFilled(size_t colnum, const std::vector<Move> & moves) {
  std::vector<Move> column_items;
  for (const Move & cell : moves) {
    if (cell.row == colnum) {
      column_items.push_back(cell);
    }
  }

  return column_items.size() == COLUMNLENGTH;
}

//------------------------------------------------------------------------------
bool isTopLeftDiagonalFilled(const std::vector<Move> & moves) {
  return std::find(moves.begin(), moves.end(), Move(0,0)) != moves.end()
      && std::find(moves.begin(), moves.end(), Move(1,1)) != moves.end()
      && std::find(moves.begin(), moves.end(), Move(2,2)) != moves.end();
}

//------------------------------------------------------------------------------
bool isBottomLeftDiagonalFilled(const std::vector<Move> & moves) {
  return std::find(moves.begin(), moves.end(), Move(2,0)) != moves.end()
      && std::find(moves.begin(), moves.end(), Move(1,1)) != moves.end()
      && std::find(moves.begin(), moves.end(), Move(0,2)) != moves.end();
}

//------------------------------------------------------------------------------
bool allBoardFilled(const std::vector<Move> & moves) {
  return moves.size() == ROWLENGTH * COLUMNLENGTH;
}

//------------------------------------------------------------------------------
bool isWinner(const std::vector<Move> & single_player_moves) {
  return isRowFilled(0, single_player_moves)
      || isRowFilled(1, single_player_moves)
      || isRowFilled(2, single_player_moves)
      || isColumnFilled(0, single_player_moves)
      || isColumnFilled(1, single_player_moves)
      || isColumnFilled(2, single_player_moves)
      || isTopLeftDiagonalFilled(single_player_moves)
      || isBottomLeftDiagonalFilled(single_player_moves);
}

//------------------------------------------------------------------------------
bool isWinnerA(const std::vector<Move> & all_moves) {
  return isWinner(getOddMoves(all_moves));
}

//------------------------------------------------------------------------------
bool isWinnerB(const std::vector<Move> & all_moves) {
  return isWinner(getEvenMoves(all_moves));
}

//------------------------------------------------------------------------------
int main() {
  std::vector<Move> moves;
  ReadAllMoves(std::cin, moves);

  std::clog << moves.size() << " moves were made" << std::endl;

  if (isWinnerA(moves)) {
    std::cout << "A" << std::endl;
  } else if (isWinnerB(moves)) {
    std::cout << "B" << std::endl;
  } else if (allBoardFilled(moves)) {
    std::cout << "Draw" << std::endl;
  } else {
    std::cout << "Pending" << std::endl;
  }
}
