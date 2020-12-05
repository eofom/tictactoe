#include "marksline.h"


MarksLine::MarksLine(const Board &board, const std::pair<size_t, size_t> &cell)
  : line_cell_(cell),
    board_(board)
{}

// FIXME embed in class CellCoordinates
std::pair<size_t, size_t> operator+(const std::pair<size_t, size_t>& cell,
                                     const std::pair<size_t, size_t>& step) {
  auto [row, column] = cell;
  auto [rowstep, colstep] = step;
  return std::pair{ row + rowstep, column + colstep };
}

size_t MarksLine::size() const {
  size_t sum = countLineCells(line_cell_, stepBackward()) +
      countLineCells(line_cell_, stepForward()) + 1;

  return sum;
}

Board::Cell::State MarksLine::player() const {
  auto [row, column] = line_cell_;
      return board_.getCellState(row, column);
}

bool MarksLine::onLine(const std::pair<size_t, size_t> &cell) const {
  auto [row, column] = cell;
  return row < board_.rowCount()
      && column < board_.columnCount()
      && board_.getCellState(row, column) == player();
  }

/** Counts cells marked by same player as start_cell. Do not counts start_cell itself */
size_t MarksLine::countLineCells(const std::pair<size_t, size_t> &start_cell,
      const std::pair<size_t, size_t> &step) const {
  size_t sum = 0;
  for (auto c = start_cell + step; onLine(c); c = c + step) {
    ++sum;
  }

  return sum;
}
