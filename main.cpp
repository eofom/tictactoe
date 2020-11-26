#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

static const size_t ROWLENGTH = 3;
static const size_t COLUMNLENGTH = 3;

//------------------------------------------------------------------------------
struct Move {
  Move(size_t row, size_t column)
    : row(row),
      column(column)
  {}

  size_t row = 0;
  size_t column = 0;
};

//------------------------------------------------------------------------------
struct Board {
  struct Cell {
    enum class State {
      Open = 0,
      X,
      O
    } state = State::Open;
  };

  //------------------------------------------------------------------------------
  Cell::State otherPlayer(Cell::State player) {
    Cell::State other = Cell::State::Open;
    switch (player) {
    case Cell::State::X:
      other = Cell::State::O;
      break;
    case Cell::State::O:
      other = Cell::State::X;
      break;
    }

    return other;
  }

  //------------------------------------------------------------------------------
  Cell::State getCellState(size_t row, size_t column) {
    return cells[ROWLENGTH * row + column].state;
  }

  //------------------------------------------------------------------------------
  // FIXME Check dimensions?
  void setCellState(size_t row, size_t column, Cell::State state) {
    cells[ROWLENGTH * row + column].state = state;
  }

  //------------------------------------------------------------------------------
  bool isRowFilled(size_t row, Cell::State player) {
    // FIXME
    size_t start = ROWLENGTH * row;
    auto owned_by_player = [player](const Cell & cell) {
      return cell.state == player;
    };

    return std::all_of(cells.begin() + start,
                       cells.begin() + start + ROWLENGTH,
                       owned_by_player);
  }

  //------------------------------------------------------------------------------
  bool isColumnFilled(size_t column, Cell::State player) {
    // FIXME
    bool ans = true;
    for (size_t row = 0; row < ROWLENGTH; ++row) {
      ans &= getCellState(row, column) == player;
    }

    return ans;
  }

  //------------------------------------------------------------------------------
  bool isTopLeftDiagonalFilled(Cell::State player) {
    // FIXME
    return getCellState(0,0) == player
        && getCellState(1,1) == player
        && getCellState(2,2) == player;
  }

  //------------------------------------------------------------------------------
  bool isBottomLeftDiagonalFilled(Cell::State player) {
    // FIXME
    return getCellState(0,2) == player
        && getCellState(1,1) == player
        && getCellState(2,0) == player;
  }

  //------------------------------------------------------------------------------
  bool isWinner(Cell::State player) {
    return isRowFilled(0, player)
        || isRowFilled(1, player)
        || isRowFilled(2, player)
        || isColumnFilled(0, player)
        || isColumnFilled(1, player)
        || isColumnFilled(2, player)
        || isTopLeftDiagonalFilled(player)
        || isBottomLeftDiagonalFilled(player);
  }

  //------------------------------------------------------------------------------
  bool isWinnerX() {
    return isWinner(Cell::State::X);
  }

  //------------------------------------------------------------------------------
  bool isWinnerO() {
    return isWinner(Cell::State::O);
  }

  //------------------------------------------------------------------------------
  std::vector<Cell> possibleMoves() {
    // FIXME
    return {};
  }

  //------------------------------------------------------------------------------
  enum class GameState {
    Pending = 0,
    winX,
    winO,
    Draw
  };

  //------------------------------------------------------------------------------
  GameState checkGameState() {
    GameState ans = GameState::Pending;
    if (isWinnerX()) {
      ans = GameState::winX;
    } else if (isWinnerO()) {
      ans = GameState::winO;
    } else if (possibleMoves().empty()) {
      ans = GameState::Draw;
    } else {
      ans = GameState::Pending;
    }

    return ans;
  }

  //------------------------------------------------------------------------------
  static Board Create(const std::vector<Move> & moves) {
    Board board;
    Cell::State player = Cell::State::X;
    for (const Move & move : moves) {
      board.setCellState(move.row, move.column, player);
      player = board.otherPlayer(player);
    }

    return board;
  }

  //------------------------------------------------------------------------------

  std::vector<Cell> cells = std::vector<Cell>(9);
};

//------------------------------------------------------------------------------
std::ostream & operator<<(std::ostream & os, const Board::Cell & cell) {
  char repr = ' ';
  switch (cell.state) {
  case Board::Cell::State::O:
    repr = 'O';
    break;
  case Board::Cell::State::X:
    repr = 'X';
    break;
  case Board::Cell::State::Open:
    repr = ' ';
    break;
  }

  return os << repr;
}

//------------------------------------------------------------------------------
std::ostream & operator<<(std::ostream & os, const Board & board) {
    os << board.cells.at(0) << " | "s << board.cells.at(1) << " | "s << board.cells.at(2)
       << std::endl << std::string(9, '-') << std::endl
       << board.cells.at(3) << " | "s << board.cells.at(4) << " | "s << board.cells.at(5)
       << std::endl << std::string(9, '-') << std::endl
       << board.cells.at(6) << " | "s << board.cells.at(7) << " | "s << board.cells.at(8)
       << std::endl << std::string(9, '-') << std::endl;

    return os;
}

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
int main() {
  std::vector<Move> moves;
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
