#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <set>
#include <string>
#include <vector>

constexpr int g_grid_size = 3;
constexpr int g_corner_number = 4;
const std::vector<std::pair<int, int>> kCorners = {
    {0, 0}, {0, 2}, {2, 2}, {2, 0}};

enum GameMode { kBotVsHuman, kHumanVsBot, kBotVsBot };

enum class GameState { kWinA, kWinB, kPending, kDraw };

class Solution {
 public:
  void ClearGrid() {
    for (std::string &row : grid) {
      row = "   ";
    }
  }

  GameState tictactoe(std::vector<std::vector<int>> &moves) {
    // Filling playgrond with moves done
    for (int index = 0; index < moves.size(); ++index) {
      char sign;
      if (index % 2 == 0) {
        sign = player_1_sign;
      } else {
        sign = player_2_sign;
      }
      const std::vector<int> &move = moves[index];
      grid[move[0]][move[1]] = sign;
    }
    {
      // Printig playground
      std::cout << "   012\n";
      int line = 0;
      for (const std::string &row : grid) {
        std::cout << line++ << " |" << row << "|\n";
      }
      std::cout << std::endl;
    }
    // Starting to check result of last move
    int moves_number = moves.size();
    const int min_moves_number_to_win = 5;
    if (moves_number < min_moves_number_to_win) {
      return GameState::kPending;
    }

    for (const char player_sign : {player_1_sign, player_2_sign}) {
      if (playerHasWon(player_sign)) {
        return player_sign == player_1_sign ? GameState::kWinA
                                            : GameState::kWinB;
      }
    }

    if (moves_number == g_grid_size * g_grid_size) {
      return GameState::kDraw;
    }
    return GameState::kPending;
  }

 private:
  static const char player_1_sign = 'X';
  static const char player_2_sign = 'O';
  std::vector<std::string> grid = {"   ", "   ", "   "};

  bool StringIsComplete(const std::string &rank, char symbol) const {
    return all_of(rank.begin(), rank.end(),
                  [symbol](char c) { return c == symbol; });
  }

  bool playerHasWon(char player_sign) const {
    // Checking for win by rows
    for (const std::string &row : grid) {
      if (StringIsComplete(row, player_sign)) {
        return true;
      }
    }

    // Checking for win by columns
    for (int col = 0; col < g_grid_size; ++col) {
      std::string column;
      for (const std::string &row : grid) {
        column += row[col];
      }
      if (StringIsComplete(column, player_sign)) {
        return true;
      }
    }

    // Checking for win by diagonals
    if (grid[1][1] != player_sign) {
      return false;
    }

    return (grid[0][0] == player_sign && grid[2][2] == player_sign) ||
           (grid[2][0] == player_sign && grid[0][2] == player_sign);
  }
};

class TicTacToeBot {
 public:
  TicTacToeBot() { std::cout << "********** Tic Tac Toe game **********\n\n"; }

  void PlayGame() {
    GameInit();
    if (game_mode_ == GameMode::kBotVsBot) {
      Test();
      return;
    }
    GameState game_state;
    while (true) {
      game_state = GetPlayerMove();
      if (game_state != GameState::kPending) {
        break;
      }
      game_state = MakeBotMove("Bot");
      if (game_state != GameState::kPending) {
        break;
      }
    }
    PrintGameResult(game_state);
  }

 private:
  std::string player_1_name_;
  std::string player_2_name_;
  std::vector<std::vector<int>> moves_done_;
  Solution round_;
  GameMode game_mode_ = kBotVsHuman;
  bool (*CheckForEvenOddMovesPtr)(int) = nullptr;

  bool DigitIsCorrect(char ch) { return ch >= '0' && ch <= '2'; }

  bool InputIsCorrect(std::string &raw_input) {
    if (raw_input.empty() || raw_input.length() != 2) {
      return false;
    }
    return (DigitIsCorrect(raw_input[0]) && DigitIsCorrect(raw_input[1]));
  }

  void PrintGameResult(GameState game_state) {
    if (game_state == GameState::kDraw) {
      std::cout << "*** Draw ***\n";
    } else {
      std::cout << "*** "
                << (game_state == GameState::kWinA ? player_1_name_
                                                   : player_2_name_)
                << " won ***\n";
    }
  }

  void Test() {
    GameState game_state;
    while (true) {
      CheckForEvenOddMovesPtr = CheckForEvenMoves;
      game_state = MakeBotMove(player_1_name_);
      if (game_state != GameState::kPending) {
        break;
      }
      CheckForEvenOddMovesPtr = CheckForOddMoves;
      game_state = MakeBotMove(player_2_name_);
      if (game_state != GameState::kPending) {
        break;
      }
    }
    PrintGameResult(game_state);
  }

  void GameInit() {
    round_.ClearGrid();
    moves_done_.clear();
    std::cout << "\nSelect the game mode:\n"
              << "    1: Bot vs player\n"
              << "    2: Player vs Bot\n"
              << "    3: Test: Bot vs bot\n\n";
    int game_mode = 0;
    do {
      std::cout << "Enter your choice: ";
      std::cin >> game_mode;
      if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "You must enter a digit, repeat input.\n";
        continue;
      }
      if (game_mode < 1 || game_mode > 3) {
        std::cout << "Digit must be within a range of 1...3, repeat input.\n";
      }
    } while (game_mode < 1 || game_mode > 3);
    game_mode_ = static_cast<GameMode>(game_mode - 1);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    switch (game_mode_) {
      case kBotVsHuman:
        std::cout << "\nYou play with \"O\" sign\n\n";
        player_1_name_ = "Bot";
        player_2_name_ = "Player";
        CheckForEvenOddMovesPtr = CheckForEvenMoves;
        MakeBotMove("Bot");
        break;
      case kHumanVsBot:
        std::cout << "\nYou play with \"X\" sign\n\n";
        player_1_name_ = "Player";
        player_2_name_ = "Bot";
        CheckForEvenOddMovesPtr = CheckForOddMoves;
        round_.tictactoe(moves_done_);
        break;
      case kBotVsBot:
        std::cout << "\nBot 1 plays with \"X\" sign, Bot 2 with \"O\" sign\n\n";
        player_1_name_ = "Bot 1";
        player_2_name_ = "Bot 2";
        break;
      default:
        break;
    }
  }

  GameState GetPlayerMove() {
    do {
      int row = 0;
      int column = 0;
      std::string input;
      std::cout << "Enter you move, (row 0...2 column 0...2 (e.g. 12)) or type "
                   "\"stop\" to exit: ";
      std::getline(std::cin, input);
      if (input == "stop") {
        throw -1;
      }
      if (InputIsCorrect(input)) {
        row = std::stoi(input.substr(0, 1));
        column = std::stoi(input.substr(1, 1));
      } else {
        std::cout << input << " is invalid input! Enter again\n";
        continue;
      }

      if (MoveIsCorrect(row, column)) {
        AddMove(row, column);
        break;
      }
      std::cout << "Invalid move! Cell (" << row << ", " << column
                << ") is already occupied. Enter again!\n";
    } while (true);
    return round_.tictactoe(moves_done_);
  }

  bool MoveIsCorrect(int row, int column) {
    if (moves_done_.empty()) {
      return true;
    }
    for (const auto &move : moves_done_) {
      if (move[0] == row && move[1] == column) {
        return false;
      }
    }
    return true;
  }

  void AddMove(int new_row, int new_column) {
    std::vector<int> new_move = {new_row, new_column};
    moves_done_.push_back(new_move);
  }

  std::vector<int> GetRandomMove() {
    std::vector<std::set<int>> moves_table(g_grid_size);
    for (const auto &move : moves_done_) {
      moves_table[move[0]].insert(move[1]);
    }
    std::vector<std::vector<int>> free_moves;
    for (int row = 0; row < g_grid_size; ++row) {
      for (int column = 0; column < g_grid_size; ++column) {
        if (moves_table[row].count(column) == 0) {
          free_moves.push_back({row, column});
        }
      }
    }
    if (free_moves.size() == 1) {
      return free_moves[0];
    }
    std::random_device random_device;
    std::mt19937 gen_random(random_device());
    static const double fraction =
        1.0 / (static_cast<double>(gen_random.max()) + 1.0);
    int random_index =
        static_cast<int>(gen_random() * fraction * free_moves.size());
    if (random_index >= free_moves.size()) {
      --random_index;
    }
    return free_moves[random_index];
  }

  GameState MakeBotMove(const std::string bot_name) {
    std::vector<int> bot_move = ComputeMove();
    if (!bot_move.empty()) {
      std::cout << "*ai* ";
    } else {
      bot_move = GetRandomMove();
    }
    int bot_row = bot_move[0];
    int bot_column = bot_move[1];
    AddMove(bot_row, bot_column);
    std::cout << bot_name << " move: (" << bot_row << ", " << bot_column << ")"
              << '\n';
    return round_.tictactoe(moves_done_);
  }

  // Checking if diagonally opposite cell is empty
  std::vector<int> CheckDiagonal(
      const std::vector<std::set<int>> &player_1_table,
      const std::vector<std::set<int>> &player_2_table, int corner) {
    int opposite_corner = corner + 2;
    if (opposite_corner > 3) {
      opposite_corner -= 4;
    }
    int row = kCorners[corner].first;
    int column = kCorners[corner].second;
    int opposite_row = kCorners[opposite_corner].first;
    int opposite_column = kCorners[opposite_corner].second;
    std::vector<int> diagonal_move;
    if (player_1_table[row].count(column) != 0 &&
        player_2_table[opposite_row].count(opposite_column) == 0) {
      diagonal_move = {opposite_row, opposite_column};
    }
    return diagonal_move;
  }

  std::vector<int> SearchForCellToSeize(
      const std::vector<std::set<int>> &player_1_table,
      const std::vector<std::set<int>> &player_2_table, bool center_seized) {
    std::vector<int> cell_found;

    // Searching for cell by diagonal
    if (center_seized) {
      for (int corner = 0; corner < g_corner_number; ++corner) {
        cell_found = CheckDiagonal(player_1_table, player_2_table, corner);
        if (!cell_found.empty()) {
          return cell_found;
        }
      }
    }

    for (int index = 0; index < g_grid_size; ++index) {
      // Searching for cell by rows
      if (player_1_table[index].size() == 2 && player_2_table[index].empty()) {
        int cell_score = std::accumulate(player_1_table[index].begin(),
                                         player_1_table[index].end(), 0);
        cell_found.push_back(index);
        cell_found.push_back(g_grid_size - cell_score);
        return cell_found;
      }

      // Searching for cell by columns
      int bot_score = 0;
      int player_score = 0;
      int player_empty_row = 0;
      for (const std::set<int> &line : player_2_table) {
        bot_score += line.count(index);
      }
      for (const std::set<int> &line : player_1_table) {
        player_score += line.count(index);
      }

      if (player_score == 2 && bot_score == 0) {
        for (int row = 0; row < g_grid_size; ++row) {
          if (player_1_table[row].count(index) == 0) {
            player_empty_row = row;
            break;
          }
        }
        cell_found.push_back(player_empty_row);
        cell_found.push_back(index);
        return cell_found;
      }
    }
    return cell_found;
  }

  static bool CheckForEvenMoves(int sw) { return sw % 2 == 0; }

  static bool CheckForOddMoves(int sw) { return sw % 2 != 0; }

  std::vector<int> ComputeMove() {
    try {
      std::vector<std::set<int>> player_1_table;
      std::vector<std::set<int>> player_2_table;
      bool player_1_seized_center = false;
      bool player_2_seized_center = false;

      // Distributing players's moves data to corresponding tables
      for (int row = 0; row < g_grid_size; ++row) {
        int move_counter = 0;

        std::set<int> player_1_row;
        std::set<int> player_2_row;
        for (const std::vector<int> &move : moves_done_) {
          if (move[0] == row) {
            if (CheckForEvenOddMovesPtr(move_counter)) {
              player_1_row.insert(move[1]);
              if (move[0] == 1 && move[1] == 1) {
                player_1_seized_center = true;
              }
            } else {
              player_2_row.insert(move[1]);
              if (move[0] == 1 && move[1] == 1) {
                player_2_seized_center = true;
              }
            }
          }
          ++move_counter;
        }
        player_1_table.push_back(player_1_row);
        player_2_table.push_back(player_2_row);
      }

      std::vector<int> computed_move;
      // Checking if win is possible
      computed_move = SearchForCellToSeize(player_1_table, player_2_table,
                                           player_1_seized_center);
      if (!computed_move.empty()) {
        return computed_move;
      }
      // Checking if win by capturing the center cell is possible
      if (!player_1_seized_center && !player_2_seized_center) {
        if ((player_1_table[0].count(0) != 0 &&
             player_1_table[2].count(2) != 0) ||
            (player_1_table[0].count(2) != 0 &&
             player_1_table[2].count(0) != 0)) {
          return {1, 1};
        }
      }
      // Attempt to prevent loss if opponent is about to win
      computed_move = SearchForCellToSeize(player_2_table, player_1_table,
                                           player_2_seized_center);
      return computed_move;
    } catch (...) {
      std::cout << "Something went wrong, ComputeMove function failed.\n";
      exit;
    }
  }
};

int main() {
  TicTacToeBot bot;
  try {
    while (true) {
      bot.PlayGame();
      std::cout << "\nOne more game? (y/n): ";
      std::string answer;
      std::getline(std::cin, answer);
      if (answer == "n" || answer == "N" || answer == "stop") {
        break;
      }
    }

  } catch (int) {
    // Game has been discontinued by player
  }
  std::cout << "\nGame over!\n\n";

  return EXIT_SUCCESS;
}