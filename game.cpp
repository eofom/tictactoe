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
bool Game::AI::rowHasWinnerMove(const Board &board, size_t row,
                                Board::Cell::State player,
                                std::tuple<size_t, size_t> & winner_move) {
  bool result = false;
  if (row < board.rowCount()) {
    bool has_enemy_cells = false;
    size_t cells_checked_by_player = 0;
    for (size_t column = 0; column < board.columnCount(); ++column) {
      Board::Cell::State state = board.getCellState(row, column);
      if (state == player) {
        ++cells_checked_by_player;
      } else if (state == board.otherPlayer(player)) {
        has_enemy_cells = true;
        break;
      } else {
        // Empty cell is candidate to be a winner move
        winner_move = { row, column };
      }
    }

    result = !has_enemy_cells
        && (board.columnCount() - 1 == cells_checked_by_player);
  }

  return result;
}

//----------------------------------------------------------------------------------------
bool Game::AI::columnHasWinnerMove(const Board &board, size_t column,
                                   Board::Cell::State player,
                                   std::tuple<size_t, size_t> &winner_move) {
  bool result = false;
  if (column < board.columnCount()) {
    bool has_enemy_cells = false;
    size_t cells_checked_by_player = 0;
    for (size_t row = 0; row < board.rowCount(); ++row) {
      Board::Cell::State state = board.getCellState(row, column);
      if (state == player) {
        ++cells_checked_by_player;
      } else if (state == board.otherPlayer(player)) {
        has_enemy_cells = true;
        break;
      } else {
        // Empty cell is candidate to be a winner move
        winner_move = { row, column };
      }
    }

    result = !has_enemy_cells
        && (board.rowCount() - 1 == cells_checked_by_player);
  }

  return result;
}

//----------------------------------------------------------------------------------------
bool Game::AI::leftTopDiagonalHasWinnerMove(const Board &board,
                                            Board::Cell::State player,
                                            std::tuple<size_t, size_t> &winner_move) {
  size_t cells_checked_by_player = 0;
  bool has_enemy_cells = false;
  for (size_t row = 0; row < board.rowCount(); ++row) {
    size_t column = row;
    Board::Cell::State state = board.getCellState(row, column);
    if (state == player) {
      ++cells_checked_by_player;
    } else if (state == board.otherPlayer(player)) {
      has_enemy_cells = true;
      break;
    } else {
      // Empty cell is candidate to be a winner move
      winner_move = { row, column };
    }
  }

  return !has_enemy_cells
      && (board.rowCount() - 1 == cells_checked_by_player);
}

//----------------------------------------------------------------------------------------
bool Game::AI::leftBottomDiagonalHasWinnerMove(const Board &board,
                                               Board::Cell::State player,
                                               std::tuple<size_t, size_t> &winner_move) {
  bool result = false;
  size_t cells_checked_by_player = 0;
  for (size_t column = 0; column < board.columnCount(); ++column) {
    size_t row = board.columnCount() - 1 - column;
    std::clog << "leftBottomDiagonalHasWinnerMove ["s << row << ", "s << column << "]"s << std::endl;
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
Board Game::AI::boardAfterMove(size_t row, size_t column, Board::Cell::State player,
                               const Board &board) {
  Board next_board(board);
  next_board.setCellState(row, column, player);

  return next_board;
}

//----------------------------------------------------------------------------------------
std::vector<std::tuple<size_t, size_t> > Game::AI::winnerMoves(const Board &board,
                                                               Board::Cell::State player) {
  std::vector<std::tuple<size_t, size_t> > winner_moves;
  std::tuple<size_t, size_t> cell;
  for (size_t row = 0; row < board.rowCount(); ++row) {
    if (rowHasWinnerMove(board, row, player, cell)) {
      winner_moves.push_back(cell);
    }
  }

  for (size_t column = 0; column < board.columnCount(); ++column) {
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

//----------------------------------------------------------------------------------------
std::vector<std::tuple<size_t, size_t> > Game::AI::loserMoves(const Board &board,
                                                              Board::Cell::State player) {
  return winnerMoves(board, board.otherPlayer(player));
}

//----------------------------------------------------------------------------------------
std::vector<std::tuple<size_t, size_t> > Game::AI::movesLeadingToTwoWinOpportunities(
      const Board &board, Board::Cell::State player) {
  std::vector<std::tuple<size_t, size_t> > brilliant_moves;
  for (const auto & move : board.possibleMoves()) {
    auto [row, column] = move;
    const Board next_position = boardAfterMove(row, column, player, board);
    std::clog << "Next position after move " << move << std::endl
        << next_position << std::endl;
    auto next_winner_moves = winnerMoves(next_position, player);
    std::clog << "next_winner_moves" << std::endl;
    for ( const auto & t : next_winner_moves) {
      std::clog << t << " ";
    }
    std::clog << std::endl;
    if (2u == next_winner_moves.size()) {
      brilliant_moves.push_back({row, column});
    }
  }

  return brilliant_moves;
}
