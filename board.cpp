#include "board.h"

#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;

//------------------------------------------------------------------------------
Board::Board(size_t size)
  : size(size)
{
  for (size_t row = 0; row < rowCount(); ++row) {
    for (size_t column = 0; column < columnCount(); ++column) {
      cells.push_back(Cell(row, column));
    }
  }
}

//------------------------------------------------------------------------------
size_t Board::rowCount() const {
  return size;
}

//------------------------------------------------------------------------------
size_t Board::columnCount() const {
  return size;
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
std::set<std::pair<size_t, size_t> > Board::possibleMoves() const {
  std::set<std::pair<size_t, size_t> > moves;
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
bool Board::isPossibleMove(std::pair<size_t, size_t> coordinates) const
{
  auto [row, column] = coordinates;
  return isPossibleMove(row, column);
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
Board Board::Create(size_t board_size, const std::vector<Move> &moves) {
  Board board(board_size);
  Cell::State player = Cell::State::X;
  for (const Move & move : moves) {
    board.setCellState(move.row, move.column, player);
    player = board.otherPlayer(player);
  }

  return board;
}

//------------------------------------------------------------------------------
std::ostream & operator<<(std::ostream & os, const Board & board) {
  for (size_t row = 0; row < board.rowCount(); ++row) {
    for (size_t column = 0; column < board.columnCount(); ++column) {
      os << "|" << setw(5);
      size_t cell_idx = board.columnCount() * row + column;
      const Board::Cell & cell = board.cells.at(cell_idx);
      switch (cell.state) {
      case Board::Cell::State::X:
        os << 'X';
        break;
      case Board::Cell::State::O:
        os << 'O';
        break;
      case Board::Cell::State::Open:
        os << cell_idx;
        break;

      }
    }
    os << "\n";
  }

  return os << std::endl;
//    os << board.cells.at(0) << " | "s << board.cells.at(1) << " | "s << board.cells.at(2)
//       << std::endl << std::string(13, '-') << std::endl
//       << board.cells.at(3) << " | "s << board.cells.at(4) << " | "s << board.cells.at(5)
//       << std::endl << std::string(13, '-') << std::endl
//       << board.cells.at(6) << " | "s << board.cells.at(7) << " | "s << board.cells.at(8)
//       << std::endl << std::string(13, '-') << std::endl;

//    return os;
}

//------------------------------------------------------------------------------
bool operator ==(const Board::Move & lhs, const Board::Move rhs) {
  return lhs.row == rhs.row
      && lhs.column == rhs.column;
}

//------------------------------------------------------------------------------
ostream &operator<<(ostream &os, const std::pair<size_t, size_t> &t)
{
  auto [row, column] = t;
  return os << "<"s << row << ","s << column << ">"s;
}
