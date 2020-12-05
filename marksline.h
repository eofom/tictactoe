#ifndef MARKSLINE_H
#define MARKSLINE_H

#include "board.h"

class MarksLine
{
public:
  MarksLine(const Board& board, const std::pair<size_t, size_t>& cell);

  size_t size() const;
  Board::Cell::State player() const;

  virtual std::pair<size_t, size_t> stepBackward() const = 0;
  virtual std::pair<size_t, size_t> stepForward() const = 0;

private:
  const std::pair<size_t, size_t> line_cell_;
  const Board & board_;

  bool onLine(const std::pair<size_t, size_t>& cell) const;
  size_t countLineCells(const std::pair<size_t, size_t>& start_cell,
                        const std::pair<size_t, size_t>& step) const;
};

class MarksHorizontalLine : public MarksLine {
public:
  MarksHorizontalLine(const Board& board, const std::pair<size_t, size_t>& cell)
    : MarksLine(board, cell)
  {}

  std::pair<size_t, size_t> stepBackward() const override {
    // step back one column
    return {0, -1};
  }

  std::pair<size_t, size_t> stepForward() const override {
    // step forward one column
    return {0, 1};
  }
};

class MarksVerticalLine : public MarksLine {
public:
  MarksVerticalLine(const Board& board, const std::pair<size_t, size_t>& cell)
    : MarksLine(board, cell)
  {}

  std::pair<size_t, size_t> stepBackward() const override {
    // step back one row
    return {-1, 0};
  }

  std::pair<size_t, size_t> stepForward() const override {
    // step forward one row
    return {1, 0};
  }
};

class MarksLeftTopDiagonal : public MarksLine {
public:
  MarksLeftTopDiagonal(const Board& board, const std::pair<size_t, size_t>& cell)
    : MarksLine(board, cell)
  {}

  std::pair<size_t, size_t> stepBackward() const override {
    return {-1, -1};
  }

  std::pair<size_t, size_t> stepForward() const override {
    return {1, 1};
  }
};

class MarksLeftBottomDiagonal : public MarksLine {
public:
  MarksLeftBottomDiagonal(const Board& board, const std::pair<size_t, size_t>& cell)
    : MarksLine(board, cell)
  {}

  std::pair<size_t, size_t> stepBackward() const override {
    return {1, -1};
  }

  std::pair<size_t, size_t> stepForward() const override {
    return {-1, 1};
  }
};

#endif // MARKSLINE_H
