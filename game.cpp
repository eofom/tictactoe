#include "game.h"

Game::Game()
{

}

void Game::play()
{
  Board::Cell::State player = Board::Cell::State::X;
  while ( !board_.isWinnerX()
          && !board_.isWinnerO()
          && !board_.possibleMoves().empty() ) {
    std::cout << std::endl << board_ << std::endl<< std::endl;
    auto [row, column] = askForMove();
    board_.setCellState(row, column, player);
    player = board_.otherPlayer(player);
  }
  std::cout << std::endl << board_ << std::endl<< std::endl;

  if (board_.isWinnerX()) {
    std::cout << "X WINS!!!" << std::endl;
  } else if (board_.isWinnerO()) {
    std::cout << "O WINS!!!" << std::endl;
  } else if (board_.possibleMoves().empty()) {
    std::cout << "Draw... Try again." << std::endl;
  }
}

std::tuple<size_t, size_t> Game::askForMove()
{
  std::tuple<size_t, size_t> coordinates;
  while (true) {
    size_t cell_number = 0;
    std::cout << "Make a move [1-9]: ";
    std::cin >> cell_number;

    auto [ row, column ] = numberToCellCoordinates(cell_number);
    if (row < COLUMNLENGTH && column < ROWLENGTH) {
      if (board_.getCellState(row, column) == Board::Cell::State::Open) {
        coordinates = { row, column };
        break;
      } else {
        std::cerr << "Cell " << cell_number << " is occupied" << std::endl;
      }
    } else {
      std::cerr << "Cell " << cell_number << " is out of range (must be 1-9)" << std::endl;
    }
  }

  return coordinates;
}

std::tuple<size_t, size_t> Game::numberToCellCoordinates(size_t number)
{
  size_t row = (number-1) / 3;
  size_t column = (number-1) % 3;

  return { row, column };
}
