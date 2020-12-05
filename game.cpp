#include "game.h"

#include <algorithm>
#include <limits>

using namespace std;

//----------------------------------------------------------------------------------------
Game::Game(Game::Player::PlayerType x, Game::Player::PlayerType o,
           size_t board_size, size_t marks_to_win)
  : players_({ Player(x, Player::Mark::X),
               Player(o, Player::Mark::O) }),
    board_(Board(board_size)),
    marks_to_win_(marks_to_win)
{}

//----------------------------------------------------------------------------------------
void Game::play()
{
  board_.clear();
  Player player = players_.first;
  bool is_winner = false;
  while (true) {
    std::cout << std::endl << board_ << std::endl<< std::endl;
    auto last_move = askPlayerForMove(player);
    auto [row, column] = last_move;
    board_.setCellState(row, column, cellStateForPlayer(player));
    is_winner = isWinner(last_move);
    if (is_winner || board_.possibleMoves().empty()) {
      break;
    }

    player = otherPlayer(player);
  }

  if (is_winner) {
    switch (player.mark) {
    case Player::Mark::X:
      std::cout << "X Wins." << std::endl;
      break;
    case Player::Mark::O:
      std::cout << "O Wins." << std::endl;
      break;
    }
  } else {
    std::cout << "Draw." << std::endl;
  }
}

//----------------------------------------------------------------------------------------
std::pair<size_t, size_t> Game::askPlayerForMove(const Player& player) {
  Game::AI ai;
  return (Player::PlayerType::Human == player.type)
      ? askHumanForMove()
      : ai.bestMove(board_, cellStateForPlayer(player));
}

//----------------------------------------------------------------------------------------
std::pair<size_t, size_t> Game::askHumanForMove() {
  std::pair<size_t, size_t> coordinates;
  while (true) {
    size_t cell_number = 0;
    std::cout << "Make a move [1-9]: ";
    std::cin >> cell_number;

    auto [ row, column ] = numberToCellCoordinates(cell_number);
    if (row < board_.rowCount() && column < board_.columnCount()) {
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
std::pair<size_t, size_t> Game::numberToCellCoordinates(size_t number) {
  size_t row = (number-1) / 3;
  size_t column = (number-1) % 3;

  return { row, column };
}

//----------------------------------------------------------------------------------------
Board::Cell::State Game::cellStateForPlayer(const Game::Player &player) {
  return (player.mark == Player::Mark::X)
      ? Board::Cell::State::X
      : Board::Cell::State::O;
}

//----------------------------------------------------------------------------------------
bool Game::isWinner(const std::pair<size_t, size_t> &last_move) const {
  MarksHorizontalLine horizontal(board_, last_move);
  MarksVerticalLine vertical(board_, last_move);
  MarksLeftTopDiagonal top_diagonal(board_, last_move);
  MarksLeftBottomDiagonal bottom_diagonal(board_, last_move);

  return horizontal.size() == marks_to_win_
      || vertical.size() == marks_to_win_
      || top_diagonal.size() == marks_to_win_
      || bottom_diagonal.size() == marks_to_win_;
}

//----------------------------------------------------------------------------------------
const Game::Player &Game::otherPlayer(const Game::Player &player) {
  auto [x, o] = players_;
  return (player.mark == x.mark)
      ? players_.second : x;
}

//----------------------------------------------------------------------------------------
size_t Game::marksToWin() const {
  return marks_to_win_;
}

//----------------------------------------------------------------------------------------
void Game::setMarksToWin(size_t &marks_to_win) {
  marks_to_win_ = marks_to_win;
}

//----------------------------------------------------------------------------------------
bool Game::AI::rowHasWinnerMove(const Board &board, size_t row,
                                Board::Cell::State player,
                                std::pair<size_t, size_t> & winner_move) {
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
                                   std::pair<size_t, size_t> &winner_move) {
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
                                            std::pair<size_t, size_t> &winner_move) {
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
                                               std::pair<size_t, size_t> &winner_move) {
  size_t cells_checked_by_player = 0;
  bool has_enemy_cells = false;
  for (size_t column = 0; column < board.columnCount(); ++column) {
    size_t row = board.columnCount() - 1 - column;
//    std::clog << "leftBottomDiagonalHasWinnerMove ["s << row << ", "s << column << "]"s << std::endl;
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
      && (board.columnCount() - 1 == cells_checked_by_player);
}

//----------------------------------------------------------------------------------------
/** FIXME Check if move is vald */
Board Game::AI::boardAfterMove(size_t row, size_t column, Board::Cell::State player,
                               const Board &board) {
  Board next_board(board);
  next_board.setCellState(row, column, player);

  return next_board;
}

//----------------------------------------------------------------------------------------
std::pair<size_t, size_t> Game::AI::bestMove(const Board &board,
                                              Board::Cell::State player) {
  auto winner_moves = winnerMoves(board, player);
  if (!winner_moves.empty()) {
    return (*winner_moves.begin());
  }

  auto loser_moves = loserMoves(board, player);
  if (!loser_moves.empty()) {
    return (*loser_moves.begin());
  }

  auto brilliant_moves = movesLeadingToTwoWinOpportunities(board, player);
  if (!brilliant_moves.empty()) {
//    std::clog << "Brilliant moves: ";
//    for (const auto & t : brilliant_moves) {
//      std::clog << t << " ";
//    }
//    std::clog << std::endl;
    return(*brilliant_moves.begin());
  }

  std::pair<size_t, size_t> center_move = {1u,1u};
  if (board.isPossibleMove(center_move)) {
    return center_move;
  }

  // FIXME Prefer to continue lines which doesn't have enemy marks yet
  /*
   * In this position
   *
   * _ | _ | _
   * _ | X | _
   * _ | _ | O
   *
   * prefer diagonal moves {2,0}, {0,2} rather than {0,0} */

  auto possible_moves = board.possibleMoves();
  auto pred_diagonal_move = [&board](const std::pair<size_t, size_t> move) {
    auto [row, column] = move;
    return row == column
        || row == board.columnCount() - 1 - column;
  };
  const auto lookup = std::find_if(possible_moves.begin(), possible_moves.end(),
        pred_diagonal_move);
  if (possible_moves.end() != lookup) {
      return (*lookup);
  }

  // FIXME otherwise return invalid move
  return { std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max() };
}

//----------------------------------------------------------------------------------------
std::set<std::pair<size_t, size_t> > Game::AI::winnerMoves(const Board &board,
                                                               Board::Cell::State player) {
  std::set<std::pair<size_t, size_t> > winner_moves;
  std::pair<size_t, size_t> cell;
  for (size_t row = 0; row < board.rowCount(); ++row) {
    if (rowHasWinnerMove(board, row, player, cell)) {
      winner_moves.insert(cell);
    }
  }

  for (size_t column = 0; column < board.columnCount(); ++column) {
    if (columnHasWinnerMove(board, column, player, cell)) {
      winner_moves.insert(cell);
    }
  }

  if (leftTopDiagonalHasWinnerMove(board, player, cell)) {
    winner_moves.insert(cell);
  }

  if (leftBottomDiagonalHasWinnerMove(board, player, cell)) {
    winner_moves.insert(cell);
  }

  return winner_moves;
}

//----------------------------------------------------------------------------------------
std::set<std::pair<size_t, size_t> > Game::AI::loserMoves(const Board &board,
                                                              Board::Cell::State player) {
  return winnerMoves(board, board.otherPlayer(player));
}

//----------------------------------------------------------------------------------------
std::set<std::pair<size_t, size_t> > Game::AI::movesLeadingToTwoWinOpportunities(
      const Board &board, Board::Cell::State player) {
  std::set<std::pair<size_t, size_t> > brilliant_moves;
//  std::clog << "Current board:" << std::endl << board << std::endl;
  for (const auto & move : board.possibleMoves()) {
    auto [row, column] = move;
    const Board next_position = boardAfterMove(row, column, player, board);
//    std::clog << "Next position after move " << move << std::endl
//        << next_position << std::endl;
    auto next_winner_moves = winnerMoves(next_position, player);
//    std::clog << "next_winner_moves" << std::endl;
//    for ( const auto & t : next_winner_moves) {
//      std::clog << t << " ";
//    }
//    std::clog << std::endl;
    if (2u == next_winner_moves.size()) {
      brilliant_moves.emplace(row, column);
    }
  }

  return brilliant_moves;
}

//----------------------------------------------------------------------------------------
bool Game::AI::rowHasCellOwnedByPlayer(const Board &board, size_t row,
                                       Board::Cell::State player) {
  bool ans = false;
  if (row < board.rowCount()) {
    for (size_t column = 0; column < board.columnCount(); ++column) {
      if (board.getCellState(row, column) == player) {
        ans = true;
        break;
      }
    }
  }

  return ans;
}

//----------------------------------------------------------------------------------------
bool Game::AI::columnHasCellOwnedByPlayer(const Board &board, size_t column,
                                          Board::Cell::State player) {
  bool ans = false;
  if (column < board.columnCount()) {
    for (size_t row = 0; row < board.rowCount(); ++row) {
      if (board.getCellState(row, column) == player) {
        ans = true;
        break;
      }
    }
  }

  return ans;
}

//----------------------------------------------------------------------------------------
bool Game::AI::leftTopDiagonalHasCellOwnedByPlayer(const Board &board,
                                                   Board::Cell::State player) {
  bool ans = false;
  for (size_t row = 0; row < board.rowCount(); ++row) {
    for (size_t column = 0; column < board.columnCount(); ++column) {
      if (board.getCellState(row, column) == player) {
        ans = true;
        break;
      }
    }
  }

  return ans;
}

//----------------------------------------------------------------------------------------
bool Game::AI::leftBottomDiagonalHasCellOwnedByPlayer(const Board &board,
                                                      Board::Cell::State player) {
  bool ans = false;
  for (size_t column = 0; column < board.columnCount(); ++column) {
    size_t row = board.columnCount() - 1 - column;
    if (board.getCellState(row, column) == player) {
      ans = true;
      break;
    }
  }

  return ans;
}
