#ifndef GAME_H
#define GAME_H

#include <tuple>

#include "board.h"
#include "marksline.h"

class Game
{
public:
  struct Player {
    enum class PlayerType {
      Human = 0,
      AI
    } type;

    enum class Mark {
      X = 0,
      O
    } mark;

    explicit Player(PlayerType type, Mark mark)
      : type(type),
        mark(mark)
    {}

    bool operator==(const Player& other) {
      return mark == other.mark;
    }
  };

  Game(Player::PlayerType x, Player::PlayerType o, size_t board_size = 3, size_t marks_to_win = 3);

  void play();

  class AI {
  public:
    std::pair<size_t, size_t> bestMove(const Board & board, Board::Cell::State player);
    std::set<std::pair<size_t, size_t> > winnerMoves(const Board & board, Board::Cell::State player);
    std::set<std::pair<size_t, size_t> > loserMoves(const Board & board, Board::Cell::State player);
    std::set<std::pair<size_t, size_t> > movesLeadingToTwoWinOpportunities(const Board & board, Board::Cell::State player);
    std::vector<std::pair<size_t, size_t>> goodMoves(const Board & board, Board::Cell::State player);

    bool rowHasCellOwnedByPlayer(const Board & board, size_t row, Board::Cell::State player);
    bool columnHasCellOwnedByPlayer(const Board & board, size_t column, Board::Cell::State player);
    bool leftTopDiagonalHasCellOwnedByPlayer(const Board & board, Board::Cell::State player);
    bool leftBottomDiagonalHasCellOwnedByPlayer(const Board & board, Board::Cell::State player);

    bool rowHasWinnerMove(const Board & board, size_t row, Board::Cell::State player, std::pair<size_t, size_t> & winner_move);
    bool columnHasWinnerMove(const Board & board, size_t column, Board::Cell::State player, std::pair<size_t, size_t> & winner_move);
    bool leftTopDiagonalHasWinnerMove(const Board & board, Board::Cell::State player, std::pair<size_t, size_t> & winner_move);
    bool leftBottomDiagonalHasWinnerMove(const Board & board, Board::Cell::State player, std::pair<size_t, size_t> & winner_move);

  private:
    Board boardAfterMove(size_t row, size_t column, Board::Cell::State player,
                         const Board & board);
  };

  size_t marksToWin() const;
  void setMarksToWin(size_t &marks_to_win);
  bool isWinner(const std::pair<size_t, size_t> & last_move) const;

private:
  const Player& otherPlayer(const Player& player);
  std::pair<size_t, size_t> askPlayerForMove(const Player &player);
  std::pair<size_t, size_t> askHumanForMove();
  std::pair<size_t, size_t> numberToCellCoordinates(size_t number);

  static Board::Cell::State cellStateForPlayer(const Player& player);

  const std::pair<Player, Player> players_;
  Board board_;
  size_t marks_to_win_;
};

#endif // GAME_H
