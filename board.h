#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>

static const size_t ROWLENGTH = 3;
static const size_t COLUMNLENGTH = 3;

struct Board {
  struct Move {
    Move(size_t row, size_t column)
      : row(row),
        column(column)
    {}

    size_t row = 0;
    size_t column = 0;
  };

  struct Cell {
    enum class State {
      Open = 0,
      X,
      O
    };

    Cell(size_t row, size_t column, State state = State::Open)
      : row(row),
        column(column),
        state(state)
    {}

    size_t row = 0;
    size_t column = 0;
    State state = State::Open;
  };

  Board();

  size_t rowCount() const {
    return COLUMNLENGTH;
  }

  size_t columnCount() const {
    return ROWLENGTH;
  }

  Cell::State getCellState(size_t row, size_t column) const;

  void setCellState(size_t row, size_t column, Cell::State state);

  bool isRowFilled(size_t row, Cell::State player);

  bool isColumnFilled(size_t column, Cell::State player);

  bool isTopLeftDiagonalFilled(Cell::State player);

  bool isBottomLeftDiagonalFilled(Cell::State player);

  bool isWinner(Cell::State player);

  bool isWinnerX();

  bool isWinnerO();

  std::vector<Cell> possibleMoves();

  enum class GameState {
    Pending = 0,
    winX,
    winO,
    Draw
  };

  GameState checkGameState();

  static Board Create(const std::vector<Move> & moves);

  static Cell::State otherPlayer(Cell::State player);


  std::vector<Cell> cells;
};


std::ostream & operator<<(std::ostream & os, const Board & board);


#endif // BOARD_H
