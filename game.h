#ifndef GAME_H
#define GAME_H

#include <tuple>

#include "board.h"

class Game
{
public:
  Game();

  void play();

private:
  std::tuple<size_t, size_t> askForMove();
  std::tuple<size_t, size_t> numberToCellCoordinates(size_t number);

  Board board_;
};

#endif // GAME_H
