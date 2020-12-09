#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;

class Solution {
public:
    //TODO Свяжи в коде эту A с литералами "A", которые встречаются ниже и с сущностью CellMark. 
    // Видимо, понадобится структура PlayingSide или вроде того. 
    // Для выражения идеи о том, что клетки бывают пустыми, можно использовать std::optional
    enum class Player {
        A,
        B
    };

    enum class GameType {
        BOTS,
        PLAYER_BOT,
        PLAYERS
    };

    Solution(int board_size = 3, int cells_to_win = 3) :           
            kFieldSquireSize_(board_size),
            kCellsToWin_(cells_to_win),
            generator(chrono::system_clock::now().time_since_epoch().count())
    {
        assert(board_size > 0 && "Board size should be more than 0");
        assert(cells_to_win > 0 && "Number of cells for win should be more than 0");
        assert(cells_to_win <= board_size && "Number of cells for win can't be more than board size");
        play_field_.resize(kFieldSquireSize_);
        for (auto& row : play_field_) {
            row.resize(kFieldSquireSize_, CellMark::EMPTY);
        }
    }

    string tictactoe(const vector<vector<int>>& moves) {
        //TODO можно не создавать эту переменную, а возвращать результат как только он получен
        GameCase game_result = GameCase::NOT_FINISHED;
        for (const auto& single_move : moves) {
            assert(single_move.size() == 2);
            Move move{single_move[0], single_move[1]};
            game_result = MakeStep(move);
            if (game_result != GameCase::NOT_FINISHED) {
                break;
            }
        }
        return DecodeGameCaseResult(game_result);
    }

    string tictactoe(GameType game_type, Player human_player = Player::A) {
        GameCase game_result = GameCase::NOT_FINISHED;
        while (game_result == GameCase::NOT_FINISHED) {
            if (game_type == GameType::BOTS) {
                game_result = MakeStep(RandomMove());
            } else if (game_type == GameType::PLAYER_BOT) {
                if (human_player == player_) {
                    game_result = MakeStep(HumanMove());
                } else {
                    game_result = MakeStep(RandomMove());
                }
            } else {
                game_result = MakeStep(HumanMove());
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
    const int kFieldSquireSize_;
    const int kCellsToWin_;
    
    struct Move {
        int x = 0;
        int y = 0;
    };

    enum class CellMark {
        EMPTY,
        A,
        B
    };

    //TODO Хватает считать только одни marks, те, что в последнем ходе. В итоге, когда ты 
    // еще и запакуешь обработку разных линий в один метод, эта структура потеряет смысл
    struct LineMarksQuantity {
        int a_marks;
        int b_marks;
        LineMarksQuantity operator+=(const CellMark mark) {
            if (mark == CellMark::A) {
                ++a_marks;
            } else if (mark == CellMark::B) {
                ++b_marks;
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

    vector<vector<CellMark>> play_field_;
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
        return uniform_int_distribution<int>(0, kFieldSquireSize_ - 1)(generator);
    }

    Move RandomMove() {
        //at least 1 empty cell required
        assert(CheckFinish() == GameCase::NOT_FINISHED);
        Move move{};
        while(true) {
            //TODO попробуй реализовать выбор поля за одно разыгрывание, без цикла
            //This is very simple bot. It doesn't analyze strategy and tactic. Only makes random move
            move.x = GetRandomToken();
            move.y = GetRandomToken();
            if (play_field_[move.x][move.y] == CellMark::EMPTY) {
                break;
            }
        }
        return move;
    }

    Move HumanMove() {
        //at least 1 empty cell required
        assert(CheckFinish() == GameCase::NOT_FINISHED);
        Move move{};
        while (true) {
            cout << "Type your move (format X,Y)" << endl;
            char comma;
            cin >> move.x >> comma >> move.y;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Incorrect format. Try again" << endl;
            } else if (move.x < 0 || move.x > kFieldSquireSize_ - 1
                    || move.y < 0 || move.y > kFieldSquireSize_ - 1) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "This cell is out of board. Try again" << endl;
            }
            else if (play_field_[move.x][move.y] == CellMark::EMPTY) {
                break;
            } else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "This cell isn't empty. Try again" << endl;
            }
        }
        return move;
    }

    GameCase MakeStep(const Move& move) {    
        assert(move.x >= 0 && move.x < kFieldSquireSize_);
        assert(move.y >= 0 && move.y < kFieldSquireSize_);
        auto& field_cell = play_field_[move.x][move.y];
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
        if (line_marks.a_marks == kCellsToWin_) { 
            return GameCase::WINNER_A;
        } else if (line_marks.b_marks == kCellsToWin_) {
            return GameCase::WINNER_B;
        } else {
            return GameCase::NOT_FINISHED;
        }
    }

    GameCase CheckFinish() {
        //TODO Тут много дублированного кода. Запакуй эти три проверки в один метод, принимающий начало линии и шаг.
        // А здесь только вызывай их, проходясь по возможным положениям начала и возможным шагам.
        //TODO Проверяй только линии, содержащие последний ход
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

        if (move_number_ < kFieldSquireSize_ * kFieldSquireSize_) {
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

int InputValueInRange(const int min_value, const int max_value) {
    int value;
    while (true) {
        cin >> value;
        if (cin.fail() || value < min_value || value > max_value) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Incorrect value, range " << min_value << "..." << max_value << ". Try again" << endl;
        } else {
            break;
        }
    }
    return value;
}

Solution::Player InputPlayer() {
    cout << "Select your side: A or B" << endl;
    while (true) {
        string side;
        cin >> side;
        transform(side.begin(), side.end(),side.begin(), ::toupper);
        if (side == "A"s) {
            return Solution::Player::A;
        } else if (side == "B"s) {
            return Solution::Player::B;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Incorrect side. Try again" << endl;
        }
    }        
}

void PlayGame() {
    cout << "Enter board size" << endl;
    const int board_size = InputValueInRange(1, numeric_limits<int>::max());
    cout << "Enter number of cells to win" << endl;
    const int cells_to_win = InputValueInRange(1, board_size);
    while(true) {
        Solution tic_tac(board_size, cells_to_win);
        cout << "Type mode: bots, player-bot, players or exit" << endl;
        string mode_str;
        cin >> mode_str;
        transform(mode_str.begin(), mode_str.end(),mode_str.begin(), ::toupper);
        if (mode_str == "BOTS") {
            cout << "Game result: "s + tic_tac.tictactoe(Solution::GameType::BOTS) << endl;
        } else if (mode_str == "PLAYERS") {
            cout << "Game result: "s + tic_tac.tictactoe(Solution::GameType::PLAYERS) << endl;
        } else if (mode_str == "PLAYER-BOT") {
            const Solution::Player human_player = InputPlayer();
            cout << "Game result: "s + tic_tac.tictactoe(Solution::GameType::PLAYER_BOT, human_player) << endl;
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
    {
        vector<vector<int>> moves = {{0,0},{2,0},{1,1},{2,1},{2,2}, {3,0}, {5,5}};
        Solution tic_tac(6, 4);
        tic_tac.DisableStepPrinting();
        assert(tic_tac.tictactoe(moves) == "A");
    }
    cout << __FUNCTION__ << " tests done"s << endl;
}

int main() {
    UnitTests();
    PlayGame();
}
