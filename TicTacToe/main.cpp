#include <vector>
#include <string>
#include <array>
#include <cassert>
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;

class Solution {
public:
    enum class Player {
        A,
        B
    };

    Solution() : generator(chrono::system_clock::now().time_since_epoch().count()) {
        for (auto& row : play_field_) {
            row.fill(CellMark::EMPTY);
        }
    }

    string tictactoe(const vector<vector<int>>& moves) {
        GameCase game_result = GameCase::NOT_FINISHED;
        for (const auto& single_move : moves) {
            game_result = MakeStep(single_move);
            if (game_result != GameCase::NOT_FINISHED) {
                break;
            }
        }
        return DecodeGameCaseResult(game_result);
    }

    string tictactoe() {
        GameCase game_result = GameCase::NOT_FINISHED;
        while(1) {
            game_result = MakeStep(RandomMove());
            if (game_result != GameCase::NOT_FINISHED) {
                break;
            }
        }
        return DecodeGameCaseResult(game_result);
    }

    string tictactoe(Player human_player) {
        GameCase game_result = GameCase::NOT_FINISHED;
        while(1) {
            if (human_player == player_) {
                int move_x, move_y;
                while (1) {
                    cout << "Type your move (format X,Y)" << endl;
                    char comma;
                    cin >> move_x >> comma >> move_y;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Incorrect format. Try again" << endl;
                    }
                    else if (play_field_[move_x][move_y] == CellMark::EMPTY) {
                        break;
                    } else {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "This cell isn't empty. Try again" << endl;
                    }
                }
                game_result = MakeStep({move_x, move_y});
            } else {
                game_result = MakeStep(RandomMove());
            }

            if (game_result != GameCase::NOT_FINISHED) {
                break;
            }
        }
        return DecodeGameCaseResult(game_result);
    }

    void EnableStepPrinting() {
        print_steps_ = true;
    }

    void DisableStepPrinting() {
        print_steps_ = false;
    }

private:
    static constexpr int kFieldSquireSize = 3;
    
    enum class CellMark {
        EMPTY,
        A,
        B
    };

    struct LineMarksQuantity {
        int a_marks;
        int b_marks;
        LineMarksQuantity operator+=(const CellMark mark) {
            if (mark == CellMark::A) {
                ++this->a_marks;
            } else if (mark == CellMark::B) {
                ++this->b_marks;
            }
            return *this;
        }
    };

    enum class GameCase {
        NOT_FINISHED,
        DRAW,
        WINNER_A,
        WINNER_B
    };

    array<array<CellMark, kFieldSquireSize>, kFieldSquireSize> play_field_;
    Player player_ = Player::A;
    mt19937 generator;
    uniform_int_distribution<int> distribution;
    int move_number_ = 0;
    bool print_steps_ = true;

    string DecodeGameCaseResult(GameCase game_case) {
        string result_message = ""s;
        switch (game_case)
        {
        case GameCase::NOT_FINISHED:
            result_message = "Pending"s;
            break;
        case GameCase::DRAW:
            result_message = "Draw"s;
            break;
        case GameCase::WINNER_A:
            result_message = "A"s;
            break;
        case GameCase::WINNER_B:
            result_message = "B"s;
            break;
        default:
            break;
        }
        return result_message;
    }

    void PrintField() {
        if (!print_steps_) {
            return;
        }
        cout << "Move "s << move_number_ << endl;
        cout << "---"s << endl;
        //for (int j = 0; j < static_cast<int>(play_field_.size()); ++j) {
        //   for (int i=0; i < static_cast<int>(play_field_[j].size()); ++i) {  
        for (const auto& raw : play_field_) {
            for (const auto cell : raw) {     
                switch (cell) {
                case CellMark::EMPTY:
                    cout << " "s;
                    break;
                case CellMark::A:
                    cout << "X"s;
                    break;
                case CellMark::B:
                    cout << "O"s;
                    break;
                }
            }
            cout << endl;
        }
        cout << "---"s << endl;
    }

    int GetRandomToken() {
        return uniform_int_distribution<int>(0, kFieldSquireSize - 1)(generator);
    }

    vector<int> RandomMove() {
        //at least 1 empty cell required
        assert(CheckFinish() == GameCase::NOT_FINISHED);
        
        int move_x = 0;
        int move_y = 0;
        while(1) {
            //This is very simple bot. It doesn't analyze strategy and tactic. Only makes random move
            move_x = GetRandomToken();
            move_y = GetRandomToken();
            if (play_field_[move_x][move_y] == CellMark::EMPTY) {
                break;
            }
        }
        return {move_x, move_y};
    }

    GameCase MakeStep(const vector<int>& step_move) {    
        assert(step_move.size() == 2);
        auto& field_cell = play_field_[step_move[0]][step_move[1]];
        assert(field_cell == CellMark::EMPTY);

        field_cell = MarkCell(player_);
        player_ = OtherPlayer(player_);
        ++move_number_;
        PrintField();
        return CheckFinish();
    }

    static Player OtherPlayer(Player player) {
        return (player == Player::A) ? Player::B : Player::A;
    }

    static CellMark MarkCell(Player player) {
        switch (player) {
        case Player::A:
            return CellMark::A;
        case Player::B:
            return CellMark::B;
        }
        assert(false && "Unknown player");   
        return CellMark::EMPTY;
    }

    GameCase AnalyzeLine(const LineMarksQuantity& line_marks) {
        if (line_marks.a_marks == kFieldSquireSize) { //full line with "A" marks
            return GameCase::WINNER_A;
        } else if (line_marks.b_marks == kFieldSquireSize) { //full line with "B" marks
            return GameCase::WINNER_B;
        } else {
            return GameCase::NOT_FINISHED;
        }
    }

    GameCase CheckFinish() {
        //Check horizontal lines
        for (const auto& raw : play_field_) {
            LineMarksQuantity line_marks{};
            for (const auto cell : raw) {     
               line_marks += cell;
            }
            const GameCase line_result = AnalyzeLine(line_marks);
            if (line_result != GameCase::NOT_FINISHED) {
                return line_result;
            }
        }

        //Check vertical lines
        for (size_t i = 0; i < play_field_[i].size(); ++i) {
            LineMarksQuantity line_marks{};
            for (size_t j=0; j < play_field_.size(); ++j) {
                line_marks += play_field_[j][i];
            }
            const GameCase line_result = AnalyzeLine(line_marks);
            if (line_result != GameCase::NOT_FINISHED) {
                return line_result;
            }
        }

        //check diagonal lines
        {
            LineMarksQuantity line_marks{};
            for (size_t i=0; i < play_field_.size(); ++i) {
                line_marks += play_field_[i][i];
            }
            const GameCase line_result = AnalyzeLine(line_marks);
            if (line_result != GameCase::NOT_FINISHED) {
                return line_result;
            }
        }
        {
            LineMarksQuantity line_marks{};
            for (size_t i=0; i < play_field_.size(); ++i) {
                line_marks += play_field_[play_field_.size() - i - 1][i];
            }
            const GameCase line_result = AnalyzeLine(line_marks);
            if (line_result != GameCase::NOT_FINISHED) {
                return line_result;
            }
        }

        if (move_number_ < kFieldSquireSize * kFieldSquireSize) {
            return GameCase::NOT_FINISHED;
        } else {
            return GameCase::DRAW;
        }
    }
};

string GetStringByPlayer(const Solution::Player player) {
    switch (player) {
    case Solution::Player::A:
        return "A"s;
    case Solution::Player::B: 
        return "B"s;
    }
    return ""s;
}

void PlayWithPlayer(Solution::Player player) {
    Solution tic_tac;
    string result = tic_tac.tictactoe(player);
    cout << "Game result: "s << result << "."s;
    if (result == GetStringByPlayer(player)) {
        cout << " You win!"s;
    }
    cout << endl;
}

void PlayGame() {
    while(1) {
        cout << "Type mode: bots, player or exit" << endl;
        string mode_str;
        cin >> mode_str;
        transform(mode_str.begin(), mode_str.end(),mode_str.begin(), ::toupper);
        if (mode_str == "BOTS") {
            Solution tic_tac;
            cout << "Game result: "s + tic_tac.tictactoe() << endl;
        } else if (mode_str == "PLAYER") {
            cout << "Select your side: A or B" << endl;
            string side;
            cin >> side;
            transform(side.begin(), side.end(),side.begin(), ::toupper);
            Solution tic_tac;
            if (side == "A"s) {
                PlayWithPlayer(Solution::Player::A);
            } else if (side == "B"s) {
                PlayWithPlayer(Solution::Player::B);
            } else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Incorrect side" << endl;
            }
        } else if (mode_str == "EXIT") {
            break;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Incorrect mode, type again" << endl;
        }
    }
}

void UnitTests() {
    {
        vector<vector<int>> moves = {{0,0},{2,0},{1,1},{2,1},{2,2}};
        Solution tic_tac;
        tic_tac.DisableStepPrinting();
        assert(tic_tac.tictactoe(moves) == "A");
    }
    {
        vector<vector<int>> moves = {{0,0},{1,1},{0,1},{0,2},{1,0},{2,0}};
        Solution tic_tac;
        tic_tac.DisableStepPrinting();
        assert(tic_tac.tictactoe(moves) == "B");
    }
    {
        vector<vector<int>> moves = {{0,0},{1,1},{2,0},{1,0},{1,2},{2,1},{0,1},{0,2},{2,2}};
        Solution tic_tac;
        tic_tac.DisableStepPrinting();
        assert(tic_tac.tictactoe(moves) == "Draw"); 
    }
    {
        vector<vector<int>> moves = {{0,0},{1,1}};
        Solution tic_tac;
        tic_tac.DisableStepPrinting();
        assert(tic_tac.tictactoe(moves) == "Pending"); 
    }
    cout << __FUNCTION__ << " tests done"s << endl;
}

int main() {
    UnitTests();
    PlayGame();
}