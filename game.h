#ifndef GAME_H
#define GAME_H

#include <tuple>

#include "board.h"

class Game
{
public:
  enum class Player {
    Human = 0,
    AI
  };

  Game(Player x, Player o);

  void play();

  void playHumanVsHuman();
  void playHumanVsAI(Board::Cell::State human_player);

  bool move(Board & board, Board::Cell::State player,
            const std::tuple<size_t, size_t> & coordinates);

  class AI {
  public:
    std::tuple<size_t, size_t> bestMove(const Board & board, Board::Cell::State player);
    std::set<std::tuple<size_t, size_t> > winnerMoves(const Board & board, Board::Cell::State player);
    std::set<std::tuple<size_t, size_t> > loserMoves(const Board & board, Board::Cell::State player);
    std::set<std::tuple<size_t, size_t> > movesLeadingToTwoWinOpportunities(const Board & board, Board::Cell::State player);
    std::vector<std::tuple<size_t, size_t>> goodMoves(const Board & board, Board::Cell::State player);

    bool rowHasCellOwnedByPlayer(const Board & board, size_t row, Board::Cell::State player);
    bool columnHasCellOwnedByPlayer(const Board & board, size_t column, Board::Cell::State player);
    bool leftTopDiagonalHasCellOwnedByPlayer(const Board & board, Board::Cell::State player);
    bool leftBottomDiagonalHasCellOwnedByPlayer(const Board & board, Board::Cell::State player);

    bool rowHasWinnerMove(const Board & board, size_t row, Board::Cell::State player, std::tuple<size_t, size_t> & winner_move);
    bool columnHasWinnerMove(const Board & board, size_t column, Board::Cell::State player, std::tuple<size_t, size_t> & winner_move);
    bool leftTopDiagonalHasWinnerMove(const Board & board, Board::Cell::State player, std::tuple<size_t, size_t> & winner_move);
    bool leftBottomDiagonalHasWinnerMove(const Board & board, Board::Cell::State player, std::tuple<size_t, size_t> & winner_move);

  private:
    Board boardAfterMove(size_t row, size_t column, Board::Cell::State player,
                         const Board & board);
  };

private:
  std::tuple<size_t, size_t> askForMove();
  std::tuple<size_t, size_t> numberToCellCoordinates(size_t number);

  Player player_x_;
  Player player_o_;
  Board board_;
};

#endif // GAME_H
