#include <iostream>
#include <vector>

using namespace std;

#include "board.h"
#include "game.h"


//------------------------------------------------------------------------------
std::istream & ReadMove(std::istream & is, Board::Move & move) {
  char bracket, comma;
  is >> bracket >> move.row >> comma >> move.column >> bracket;

  return is;
}

//------------------------------------------------------------------------------
std::istream & ReadAllMoves(std::istream & is, std::vector<Board::Move> & moves) {
  char bracket, comma;
  std::cin >> bracket;
  do {
    Board::Move move(0,0);
    ReadMove(std::cin, move);
    // FIXME Use emplace to avoid copying
    moves.push_back(move);
    std::cin >> comma;

    std::clog << "row = " << move.row
              << ", column = " << move.column
              << std::endl;
  } while (',' == comma);

  return is;
}

//------------------------------------------------------------------------------
int main() {
  std::cout << "Who would you like to play? [X,O]? ";
  char me = ' ';
  while ( 'x' != me && 'o' != me ) {
    std::cin >> me;
    me = std::tolower(me);
  }

  std::cout << "Would you like to play vs. Human or AI? [H,A]? ";
  char buddy = ' ';
  while ( 'h' != buddy && 'a' != buddy ) {
    std::cin >> buddy;
    buddy = std::tolower(buddy);
  }

  Game::Player::Player::PlayerType player_x = Game::Player::PlayerType::Human;
  Game::Player::Player::PlayerType player_o = Game::Player::PlayerType::Human;
  if ('a' == buddy) {
    if ('x' == me) {
      player_o = Game::Player::PlayerType::AI;
    } else {
      player_x = Game::Player::PlayerType::AI;
    }
  }

  Game game(player_x, player_o);
  game.play();

  return 0;
}
