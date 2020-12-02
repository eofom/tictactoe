#include <cassert>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

class Player {
public:

    enum class Type {
        USER,
        BOT
    };

    enum class Symbol {
        A = 'X',
        B = 'O'
    };

    Player() {
        PlayerInit();
    }

    Player(const Type& type): type_(type) {
        PlayerInit();
    }

    ~Player() {
        if(first_player_) {
            players_count_ = 0;
        }
    }

    void PlayerInit() {
        first_player_ = !players_count_++;
        name_ = "Player "s + static_cast<char>('A' + !first_player_);
    }

    Type GetType() const {
        return type_;
    }

    string GetName() const {
        return name_;
    }

    operator string() const {
        return string(1, static_cast<char>(first_player_ ? Symbol::A : Symbol::B));

    }

private:
    inline static int players_count_ = 0;
    bool first_player_ = true;
    string name_;
    Type type_ = Type::USER;
};

ostream& operator<<(ostream& out, const Player& player) {
    return out << static_cast<string>(player);
}

ostream& operator<<(ostream& out, const Player::Type& player_type) {
    return out << (player_type == Player::Type::USER ? "USER" : "BOT");
}

struct Move {
    int h_index = -1;
    int v_index = -1;

    Move() = default;

    Move(int v_index, int h_index)
            : v_index(v_index),
              h_index(h_index)  {
    }

    Move(const string& move)  {
        for (const char ch: move) {
            if ( ch >= 'a' && ch <= 'z') {
                v_index = ch - 'a';
            } else if (ch >= '0' && ch <= '9') {
                h_index = ch - '1';
            }
        }
    }

    operator string() {
        return string(1, static_cast<char>('a' + v_index)) + to_string((h_index + 1));
    }
};

ostream& operator<<(ostream& out, const Move& move) {
    return out << "(" << move.h_index << ", " << move.v_index << ")";
}

class Tictactoe {
public:

    Tictactoe() = default;

    Tictactoe(Player::Type player_a, Player::Type player_b, int grid_size = 3, int target = 0)
            : players_({player_a, player_b}),
              grid_size_(grid_size),
              target_(target ? target : grid_size) {
    }

    string Play() {
        PlayInit();
        Greeting();
        for (int i = 0; i < grid_size_ * grid_size_; ++i) {
            PrintGrid();
            if (CheckWin(DoMove(GetMove()))){
                PrintGrid();
                return players_[i % 2].GetName() + " is win!"s;
            }
        }
        PrintGrid();
        return "Draw"s;
    }


    string PlaySmulation(vector<vector<int>> moves)  {
        PlayInit();
        for (int i = 0; i < moves.size(); ++i) {
            Move move = {moves[i][1], moves[i][0]};
            if (!ValidMove(move)) {
                return "Not valid мove"s;
            }
            if (CheckWin(DoMove(move))){
                return moves.size() % 2 ? "A"s : "B"s;
            }
        }
        return moves.size() < grid_size_*grid_size_ ? "Pending"s : "Draw"s;
    }

private:
    int grid_size_ = 3;
    int moves_count_ = 0;
    int target_ = grid_size_;
    vector<vector<string>> grid_ = GetGrid();
    vector<Player> players_ = {Player::Type::USER, Player::Type::BOT};
    bool visible_ = players_[0].GetType() == Player::Type::USER || players_[1].GetType() == Player::Type::USER;

    void PlayInit() {
        moves_count_ = 0;
        grid_ = GetGrid();
    }

    void Greeting() const {
        Print("\rThe tic-tac-toe game has begun.\n"s);
    }

    bool CheckWin(const Move& lastMove) const {

        int sum_horizontal = 0, sum_vertical = 0, sum_diagonal_left = 0, sum_diagonal_right = 0;

        for (size_t i = 0; i < grid_size_; ++i) {
            sum_horizontal      += grid_[i][lastMove.v_index] == grid_[lastMove.h_index][lastMove.v_index];
            sum_vertical        += grid_[lastMove.h_index][i] == grid_[lastMove.h_index][lastMove.v_index];
            sum_diagonal_left   += lastMove.h_index == lastMove.v_index && grid_[i][i] == grid_[lastMove.h_index][lastMove.v_index];
            sum_diagonal_right  += lastMove.h_index == grid_size_ - lastMove.v_index - 1 &&  grid_[i][grid_size_ - i - 1] == grid_[lastMove.h_index][lastMove.v_index];
        }

        return  (sum_horizontal == target_ || sum_vertical == target_ || sum_diagonal_left == target_ || sum_diagonal_right == target_);
    }

    Move GetMove() const {
        Move move;
        if (players_[moves_count_ % 2].GetType() == Player::Type::USER) {
            string comand;
            do {
                if(!comand.empty()) {
                    Print("\"" + comand + "\" is not valid move. Please enter valid move.\n");
                }
                Print( players_[moves_count_ % 2].GetName() + ": "s);
                getline(cin, comand);
                move = comand;
            } while (!ValidMove(move));
        } else {
            Print(players_[moves_count_ % 2].GetName() + ": "s);
            do {
                move = GetBotMove();
            } while (!ValidMove(move));
            Print(string(move) + "\n");
        }
        return move;
    }

    Move GetBotMove() const {
        vector<Move> valid_moves;
        for (int v_index = 0; v_index < grid_size_; ++v_index) {
            for (int h_index = 0; h_index < grid_size_; ++h_index) {
                if (grid_[h_index][v_index] == " "s) {
                    valid_moves.push_back({v_index,h_index});
                }
            }
        }
        random_device random_device;
        srand(random_device() + time(0));
        return valid_moves[rand() % (valid_moves.size())];
    }

    const Move& DoMove(const Move& move) {
        if (ValidMove(move)) {
            ++moves_count_;
            grid_[move.h_index][move.v_index] = players_[!(moves_count_ % 2)];
        }
        return move;
    }

    bool ValidMove(const Move& move) const {
        return  move.h_index >= 0 && move.h_index < grid_size_ && move.v_index >= 0 && move.v_index < grid_size_ && grid_[move.h_index][move.v_index] == " "s;
    }

    vector<vector<string>> GetGrid() const {
        return vector<vector<string>>(grid_size_, vector<string>(grid_size_, " "s));
    }

    void PrintGrid() const {
        Print("\n"s);
        for (int i = 0; i < grid_size_; ++ i) {
            PrintRowAndVerticalMarks(i);
            (i + 1 < grid_size_) ? PrintBorder() : PrintHorizontalMarks();
        }
    }

    void PrintRowAndVerticalMarks(int row_index) const {
        Print(" "s);
        for (int j = 0; j < grid_size_; ++ j) {
            if (j != 0) {
                Print(" | "s);
            }
            Print(grid_[row_index][j]);
        }
        Print(" "s + to_string(row_index + 1) + "\n"s);
    }

    void PrintBorder() const {
        Print(" "s);
        for (int j = 0; j + 1 < grid_size_ * 2; ++ j) {
            Print((j % 2 == 0 ?  "-"s : " + "s));
        }
        Print("\n"s);
    }

    void PrintHorizontalMarks() const {
        Print(" "s);
        for (int j = 0; j < grid_size_; ++ j) {
            Print(static_cast<char>('a' + j)  + "   "s);
        }
        Print("\n"s);
    }

    void Print(const string& str) const {
        if (visible_) {
            cout << str;
        }
    }
};

void Tests() {
    Tictactoe game;
    assert(game.PlaySmulation({{0, 0} ,{2, 0}, {0, 1}, {2, 1}, {0, 2}}) == "A"s);
    assert(game.PlaySmulation({{1, 2} ,{2, 0}, {0, 1}, {1, 0}, {2, 2} , {0, 0}}) == "B"s);
    assert(game.PlaySmulation({{0, 0}, {1, 1}, {0, 1}, {0, 2}, {1, 0}, {2, 0}}) == "B"s);
    assert(game.PlaySmulation({{0, 0} ,{2, 0}, {1, 1}, {2, 1}, {2, 2}}) == "A"s);
    assert(game.PlaySmulation({{0, 0}, {1, 1}, {0, 1}, {0, 2}, {1, 0}, {2, 0}}) == "B"s);
    assert(game.PlaySmulation({{0, 0}, {1, 1}, {2, 0}, {1, 0}, {1, 2}, {2, 1}, {0, 1}, {0, 2}, {2, 2}}) == "Draw"s);
    assert(game.PlaySmulation({{0, 0}, {1, 1}}) == "Pending"s);
    assert(game.PlaySmulation({{0, 0}, {1, 1}, {1, 1}}) == "Not valid мove"s);

    cout << "tests Ok" << endl;
}

int main() {
    Tests();

    Tictactoe game(Player::Type::USER, Player::Type::BOT);

    while(true) {
        cout << game.Play() << endl;
        cout << endl << "Play again? y/n: "s;
        string answer;
        getline(cin, answer);
        if (answer == "n"s) {
            break;
        }
    }

    return 0;
}
