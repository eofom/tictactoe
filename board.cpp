#include "board.h"

#include <algorithm>
#include <iostream>

using namespace std;

//------------------------------------------------------------------------------
Board::Board()
{
  for (size_t row = 0; row < rowCount(); ++row) {
    for (size_t column = 0; column < columnCount(); ++column) {
      cells.push_back(Cell(row, column));
    }
  }
}

//------------------------------------------------------------------------------
Board::Cell::State Board::otherPlayer(Board::Cell::State player) {
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
/** FIXME Check dimensions? */
Board::Cell::State Board::getCellState(size_t row, size_t column) const {
  return cells[columnCount() * row + column].state;
}

//------------------------------------------------------------------------------
void Board::setCellState(size_t row, size_t column, Board::Cell::State state) {
  if (row < rowCount() && column < columnCount()) {
    cells[columnCount() * row + column].state = state;
  }
}

//------------------------------------------------------------------------------
bool Board::isRowFilled(size_t row, Board::Cell::State player) {
  // FIXME
  size_t start = columnCount() * row;
  auto owned_by_player = [player](const Cell & cell) {
    return cell.state == player;
  };

  return std::all_of(cells.begin() + start,
                     cells.begin() + start + columnCount(),
                     owned_by_player);
}

//------------------------------------------------------------------------------
bool Board::isColumnFilled(size_t column, Board::Cell::State player) {
  // FIXME
  bool ans = true;
  for (size_t row = 0; row < columnCount(); ++row) {
    ans &= getCellState(row, column) == player;
  }

  return ans;
}

//------------------------------------------------------------------------------
bool Board::isTopLeftDiagonalFilled(Board::Cell::State player) {
  // FIXME
  return getCellState(0,0) == player
      && getCellState(1,1) == player
      && getCellState(2,2) == player;
}

//------------------------------------------------------------------------------
bool Board::isBottomLeftDiagonalFilled(Board::Cell::State player) {
  // FIXME
  return getCellState(0,2) == player
      && getCellState(1,1) == player
      && getCellState(2,0) == player;
}

//------------------------------------------------------------------------------
bool Board::isWinner(Board::Cell::State player) {
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
bool Board::isWinnerX() {
  return isWinner(Cell::State::X);
}

//------------------------------------------------------------------------------
bool Board::isWinnerO() {
  return isWinner(Cell::State::O);
}

//------------------------------------------------------------------------------
std::set<std::tuple<size_t, size_t> > Board::possibleMoves() const {
  std::set<std::tuple<size_t, size_t> > moves;
  for (const auto & cell : cells) {
    if (Cell::State::Open == cell.state) {
      moves.emplace(cell.row, cell.column);
//      std::clog << "Possible move: " << std::tuple{cell.row, cell.column} << std::endl;
    }
  }

  return moves;
}

//------------------------------------------------------------------------------
bool Board::isPossibleMove(size_t row, size_t column) const
{
  bool ans = false;
  if (row < rowCount() && column < columnCount()) {
    ans = (Cell::State::Open == getCellState(row, column));
  }

  return ans;
}

//------------------------------------------------------------------------------
bool Board::isPossibleMove(std::tuple<size_t, size_t> coordinates) const
{
  auto [row, column] = coordinates;
  return isPossibleMove(row, column);
}

//------------------------------------------------------------------------------
Board::GameState Board::checkGameState() {
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
void Board::clear()
{
  for (size_t row = 0; row < rowCount(); ++row) {
    for (size_t column = 0; column < columnCount(); ++column) {
      setCellState(row, column, Cell::State::Open);
    }
  }
}

//------------------------------------------------------------------------------
Board Board::Create(const std::vector<Move> &moves) {
  Board board;
  Cell::State player = Cell::State::X;
  for (const Move & move : moves) {
    board.setCellState(move.row, move.column, player);
    player = board.otherPlayer(player);
  }

  return board;
}

//------------------------------------------------------------------------------
std::ostream & operator<<(std::ostream & os, const Board::Cell & cell) {
  char repr = ' ';
  switch (cell.state) {
  case Board::Cell::State::O:
    os << 'O';
    break;
  case Board::Cell::State::X:
    os << 'X';
    break;
  case Board::Cell::State::Open:
    os << (cell.row * BOARD_COLUMN_COUNT + cell.column + 1);
    break;
  }

  return os << repr;
}

//------------------------------------------------------------------------------
std::ostream & operator<<(std::ostream & os, const Board & board) {
    os << board.cells.at(0) << " | "s << board.cells.at(1) << " | "s << board.cells.at(2)
       << std::endl << std::string(13, '-') << std::endl
       << board.cells.at(3) << " | "s << board.cells.at(4) << " | "s << board.cells.at(5)
       << std::endl << std::string(13, '-') << std::endl
       << board.cells.at(6) << " | "s << board.cells.at(7) << " | "s << board.cells.at(8)
       << std::endl << std::string(13, '-') << std::endl;

    return os;
}

//------------------------------------------------------------------------------
bool operator ==(const Board::Move & lhs, const Board::Move rhs) {
  return lhs.row == rhs.row
      && lhs.column == rhs.column;
}



ostream &operator<<(ostream &os, const std::tuple<size_t, size_t> &t)
{
  auto [row, column] = t;
  return os << "["s << row << ","s << column << "]"s;
}
