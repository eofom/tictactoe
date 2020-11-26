#include "game.h"

using namespace std;

//----------------------------------------------------------------------------------------
Game::Game()
{

}

//----------------------------------------------------------------------------------------
void Game::play() {
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

//----------------------------------------------------------------------------------------
std::tuple<size_t, size_t> Game::askForMove() {
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

//----------------------------------------------------------------------------------------
std::tuple<size_t, size_t> Game::numberToCellCoordinates(size_t number) {
  size_t row = (number-1) / 3;
  size_t column = (number-1) % 3;

  return { row, column };
}

//----------------------------------------------------------------------------------------
bool Game::AI::rowHasWinnerMove(const Board &board, size_t row, Board::Cell::State player, std::tuple<size_t, size_t> & winner_move)
{
  bool result = false;
  if (row < board.rowCount()) {
    size_t cells_checked_by_player = 0;
    for (size_t column = 0; column < board.columnCount(); ++column) {
      Board::Cell::State state = board.getCellState(row, column);
      if (state == player) {
        ++cells_checked_by_player;
      } else if (state == board.otherPlayer(player)) {
        break;
      } else {
        // Empty cell is candidate to be a winner move
        winner_move = { row, column };
      }
    }

    result = (board.columnCount() - 1 == cells_checked_by_player);
  }

  return result;
}

//----------------------------------------------------------------------------------------
bool Game::AI::columnHasWinnerMove(const Board &board, size_t column, Board::Cell::State player, std::tuple<size_t, size_t> &winner_move)
{
  bool result = false;
  if (column < board.columnCount()) {
    size_t cells_checked_by_player = 0;
    for (size_t row = 0; row < board.rowCount(); ++row) {
      Board::Cell::State state = board.getCellState(row, column);
      if (state == player) {
        ++cells_checked_by_player;
      } else if (state == board.otherPlayer(player)) {
        break;
      } else {
        // Empty cell is candidate to be a winner move
        winner_move = { row, column };
      }
    }

    result = (board.rowCount() - 1 == cells_checked_by_player);
  }

  return result;
}

//----------------------------------------------------------------------------------------
bool Game::AI::leftTopDiagonalHasWinnerMove(const Board &board, Board::Cell::State player, std::tuple<size_t, size_t> &winner_move)
{
  bool result = false;
  size_t cells_checked_by_player = 0;
  for (size_t row = 0; row < board.rowCount(); ++row) {
    size_t column = row;
    Board::Cell::State state = board.getCellState(row, column);
    if (state == player) {
      ++cells_checked_by_player;
    } else if (state == board.otherPlayer(player)) {
      break;
    } else {
      // Empty cell is candidate to be a winner move
      winner_move = { row, column };
    }

    result = (board.rowCount() - 1 == cells_checked_by_player);
  }

  return result;
}

//----------------------------------------------------------------------------------------
bool Game::AI::leftBottomDiagonalHasWinnerMove(const Board &board, Board::Cell::State player, std::tuple<size_t, size_t> &winner_move)
{
  bool result = false;
  size_t cells_checked_by_player = 0;
  for (size_t column = 0; column < board.columnCount(); ++column) {
    size_t row = board.columnCount() - 1 - column;
//    std::clog << "["s << row << ", "s << column << "]"s << std::endl;
    Board::Cell::State state = board.getCellState(row, column);
    if (state == player) {
      ++cells_checked_by_player;
    } else if (state == board.otherPlayer(player)) {
      break;
    } else {
      // Empty cell is candidate to be a winner move
      winner_move = { row, column };
    }

    result = (board.columnCount() - 1 == cells_checked_by_player);
  }

  return result;
}

//----------------------------------------------------------------------------------------
std::vector<std::tuple<size_t, size_t> > Game::AI::winnerMoves(const Board &board,
                                                               Board::Cell::State player) {
  std::vector<std::tuple<size_t, size_t> > winner_moves;
  std::tuple<size_t, size_t> cell;
  for (size_t row = 0; row < COLUMNLENGTH; ++row) {
    if (rowHasWinnerMove(board, row, player, cell)) {
      winner_moves.push_back(cell);
    }
  }

  for (size_t column = 0; column < ROWLENGTH; ++column) {
    if (columnHasWinnerMove(board, column, player, cell)) {
      winner_moves.push_back(cell);
    }
  }

  if (leftTopDiagonalHasWinnerMove(board, player, cell)) {
    winner_moves.push_back(cell);
  }

  if (leftBottomDiagonalHasWinnerMove(board, player, cell)) {
    winner_moves.push_back(cell);
  }

  return winner_moves;
}
