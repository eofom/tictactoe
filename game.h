#ifndef GAME_H
#define GAME_H

#include <tuple>

#include "board.h"

class Game
{
public:
  Game();

  void play();

  class AI {
  public:
    std::tuple<size_t, size_t> move(const Board & board, Board::Cell::State player);

    std::vector<std::tuple<size_t, size_t>> winnerMoves(const Board & board, Board::Cell::State player);
    std::vector<std::tuple<size_t, size_t>> loserMoves(const Board & board, Board::Cell::State player);
    std::vector<std::tuple<size_t, size_t>> movesLeadingToTwoWinOpportunities(const Board & board, Board::Cell::State player);
    std::vector<std::tuple<size_t, size_t>> goodMoves(const Board & board, Board::Cell::State player);

    bool rowHasWinnerMove(const Board & board, size_t row, Board::Cell::State player, std::tuple<size_t, size_t> & winner_move);
    bool columnHasWinnerMove(const Board & board, size_t column, Board::Cell::State player, std::tuple<size_t, size_t> & coords);
    bool leftTopDiagonalHasWinnerMove(const Board & board, Board::Cell::State player, std::tuple<size_t, size_t> & coords);
    bool leftBottomDiagonalHasWinnerMove(const Board & board, Board::Cell::State player, std::tuple<size_t, size_t> & coords);
  private:

  };

private:
  std::tuple<size_t, size_t> askForMove();
  std::tuple<size_t, size_t> numberToCellCoordinates(size_t number);

  Board board_;
};

#endif // GAME_H
