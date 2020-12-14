#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <memory>

using namespace std;

class RandomizedBinarySearchTree {
public:
    struct Node {
        int key;
        int size;
        Node* left;
        Node* right;

        explicit Node(int key)
            : key(key),
            left(nullptr),
            right(nullptr),
            size(1) {

        }
    };

    explicit RandomizedBinarySearchTree(int seed)
        : root_(nullptr),
        generator_(seed) {

    }

    ~RandomizedBinarySearchTree() {
        Clear();
        delete root_;
    }

    [[nodiscard]] bool Find(int key) const {
        return Find(root_, key);
    }

    void Insert(int key) {
        root_ = Insert(root_, key);
    }

    void Remove(int key) {
        root_ = Remove(root_, key);
    }

    [[nodiscard]] Node* GetRoot() const {
        return root_;
    }

    void Clear() {
        Clear(root_);
    }

private:
    Node* root_ = nullptr;
    std::mt19937 generator_;

    Node* Find(Node* p, int key) const {
        if (!p) return nullptr;
        if (key == p->key) {
            return p;
        }
        if (key < p->key) {
            return Find(p->left, key);
        } else {
            return Find(p->right, key);
        }
    }

    static int GetSize(Node* p) {
        if (!p) return 0;
        return p->size;
    }

    static void FixSize(Node* p) {
        p->size = GetSize(p->left) + GetSize(p->right) + 1;
    }

    static Node* RotateRight(Node* p) {
        Node* q = p->left;
        if (!q) return p;
        p->left = q->right;
        q->right = p;
        q->size = p->size;
        FixSize(p);
        return q;
    }

    static Node* RotateLeft(Node* q) {
        Node* p = q->right;
        if (!p) return q;
        q->right = p->left;
        p->left = q;
        p->size = q->size;
        FixSize(q);
        return p;
    }

    Node* InsertRoot(Node* p, int key) {
        if (!p) return new Node(key);
        if (key < p->key) {
            p->left = InsertRoot(p->left, key);
            return RotateRight(p);
        } else {
            p->right = InsertRoot(p->right, key);
            return RotateLeft(p);
        }
    }

    Node* Insert(Node* p, int key) {
        if (!p) return new Node(key);
        if (uniform_int_distribution<int>(0, p->size + 1)(generator_) == 0) {
            return InsertRoot(p, key);
        }
        if (p->key > key) {
            p->left = Insert(p->left, key);
        } else {
            p->right = Insert(p->right, key);
        }
        FixSize(p);
        return p;
    }

    Node* Join(Node* p, Node* q) {
        if (!p) return q;
        if (!q) return p;
        if (uniform_int_distribution<int>(0, p->size + q->size)(generator_) < p->size) {
            p->right = Join(p->right, q);
            FixSize(p);
            return p;
        } else {
            q->left = Join(p, q->left);
            FixSize(q);
            return q;
        }
    }

    Node* Remove(Node* p, int key) {
        if (!p) return p;
        if (p->key == key) {
            Node* q = Join(p->left, p->right);
            delete p;
            return q;
        } else if (key < p->key) {
            p->left = Remove(p->left, key);
        } else {
            p->right = Remove(p->right, key);
        }
        return p;
    }

    void Clear(Node* p) {
        if (!p) return;
        if (p->left) Clear(p->left);
        if (p->right) Clear(p->right);
        if (p != root_) {
            delete p;
        } else {
            root_ = nullptr;
        }
    }
};

void PrintTree(RandomizedBinarySearchTree::Node* p, bool is_root = true) {
    if (!p) return;
    if (p->left)  PrintTree(p->left, false);
    cout << (is_root ? ("(" + std::to_string(p->key) + ")") : to_string(p->key)) << " ";
    if (p->right) PrintTree(p->right, false);
    if (is_root) cout << endl;
}

class IInterface {
public:
    virtual void SetVisible(bool) = 0;
    virtual void PrintGrid(const vector<vector<string>>&) const = 0;
    virtual void Print(const string&) const = 0;
    virtual void GetLine(string&) const = 0;

    virtual ~IInterface() = default;
};

class ConsoleInterface: public IInterface {
public:
    void SetVisible(bool visible) override {
        visible_ = visible;
    }

    void GetLine(string& command) const override {
        getline(cin, command);
    }

    void PrintGrid(const vector<vector<string>>& grid) const override {
        Print("\n"s);
        for (int i = 0; i < grid.size(); ++ i) {
            PrintRowAndVerticalMarks(grid, i);
            (i + 1 < grid.size()) ? PrintBorder(grid) : PrintHorizontalMarks(grid);
        }
    }

    void Print(const string& str) const override {
        if (visible_) {
            cout << str;
        }
    }

private:
    void PrintRowAndVerticalMarks(vector<vector<string>> grid, int row_index) const {
        Print(" "s);
        for (int j = 0; j < grid.size(); ++ j) {
            if (j != 0) {
                Print(" | "s);
            }
            Print(grid[row_index][j]);
        }
        Print(" "s + to_string(row_index + 1) + "\n"s);
    }

    void PrintBorder(const vector<vector<string>>& grid) const {
        Print(" "s);
        for (uint8_t j = 0u; j + 1u < grid.size() * 2u; ++j) {
            Print((j & 1u ? " + "s : "-"s));
        }
        Print("\n"s);
    }

    void PrintHorizontalMarks(const vector<vector<string>>& grid) const {
        Print(" "s);
        for (int j = 0; j < grid.size(); ++j) {
            Print(static_cast<char>('a' + j)  + "   "s);
        }
        Print("\n"s);
    }

    bool visible_ = true;
};

enum class PlayerType {
    USER,
    BOT,
    SMART_BOT
};

struct Player {
    const string name_;
    const string symbol_;
    const int index_;
};

struct Move {
    int h_index;
    int v_index;

    Move(int v_index, int h_index)
            : v_index(v_index),
              h_index(h_index)  {

    }

    explicit Move(const string& move)  {
        for (const char ch: move) {
            if ( ch >= 'a' && ch <= 'z') {
                v_index = ch - 'a';
            } else if (ch >= '0' && ch <= '9') {
                h_index = ch - '1';
            }
        }
    }

    [[nodiscard]] string ToString() const {
        return string(1, static_cast<char>('a' + v_index)) + to_string((h_index + 1));
    }
};

ostream& operator<<(ostream& out, const Move& move) {
    return out << "(" << move.h_index << ", " << move.v_index << ")";
}

class TicTacToe {
public:
    explicit TicTacToe(PlayerType player_type_a = PlayerType::BOT,
                       PlayerType player_type_b = PlayerType::BOT,
                       int grid_size = 3, int target = 0)
            : grid_size_(grid_size),
              target_(target ? target : grid_size),
              players_type_({player_type_a, player_type_b}),
              interface_(new ConsoleInterface()),
              valid_moves_indexes_(RandomizedBinarySearchTree(chrono::system_clock::now().time_since_epoch().count())) {
        interface_->SetVisible(player_type_a == PlayerType::USER || player_type_b == PlayerType::USER);
        assert(kIndexPlayerNone_ == 0);
        assert(kIndexPlayerFirst_ + kIndexPlayerSecond_ == kIndexPlayerNone_);
        assert(kIndexPlayerFirst_ * kIndexPlayerSecond_ != kIndexPlayerNone_);
        PlayersInit();
    }

    string Play() {
        PlayInit();
        Greeting();
        for (int i = 0; i < grid_size_ * grid_size_; ++i) {
            interface_->PrintGrid(grid_view_);
            Move move = GetMove();
            DoMove(move);
            bool is_win = CheckWin(move);
            if (is_win){
                interface_->PrintGrid(grid_view_);
                return GetPrevPlayer().name_ + " is win!"s;
            }
        }
        interface_->PrintGrid(grid_view_);
        return "Draw"s;
    }

    string MovesAnalysis(vector<vector<int>> moves)  {
        PlayInit();
        for (int i = 0; i < moves.size(); ++i) {
            Move move{moves[i][1], moves[i][0]};
            bool is_valid_move = IsValidMove(move);
            if (!is_valid_move) {
                return "Not valid move"s;
            }
            DoMove(move);
            bool is_win = CheckWin(move);
            if (is_win){
                return moves.size() & 1u ? "A"s : "B"s;
            }
        }
        return moves.size() < grid_size_*grid_size_ ? "Pending"s : "Draw"s;
    }

    void SwapPlayers() {
        iter_swap(players_type_.begin(), next(players_type_.begin()));
    }

private:
    inline static const int kIndexPlayerFirst_ = -1;
    inline static const int kIndexPlayerSecond_ = 1;
    inline static const int kIndexPlayerNone_ = 0;
    const string kSymbolPlayerNone_ = " "s;

    void PlayInit() {
        if (moves_count_) {
            moves_count_ = 0;
            SwapPlayers();
        }
        SetGrid();
        SetValidMoves();
     }

    void SetGrid() {
        grid_ = vector<vector<int>>(grid_size_, vector<int>(grid_size_, kIndexPlayerNone_));
        grid_view_ = vector<vector<string>>(grid_size_, vector<string>(grid_size_, kSymbolPlayerNone_));
    }

    void SetValidMoves() {
        valid_moves_indexes_.Clear();
        for (int i = 0; i < grid_size_ * grid_size_; ++i) {
            valid_moves_indexes_.Insert(i);
        }
    }

    void PlayersInit() {
        players_.emplace_back(Player{"Player A"s, "X"s, kIndexPlayerFirst_});
        players_.emplace_back(Player{"Player B"s, "O"s, kIndexPlayerSecond_});
    }

    void Greeting() const {
        interface_->Print("\rThe tic-tac-toe game has begun.\n"s);
    }

    [[nodiscard]] int CheckWin(const Move& move , bool forecast = false) const {
        vector<int> sums(4, 0);

        for(int i = 0; i < grid_size_; ++i){
            sums[0] += grid_[move.h_index][i];
            sums[1] += grid_[i][move.v_index];
            sums[2] += move.h_index == move.v_index ? grid_[i][i] : 0;
            sums[3] += move.h_index == grid_size_ - move.v_index - 1 ? grid_[i][grid_size_ - i - 1] : 0;
        }

        for (int sum: sums) {
            if (abs(sum) == abs((target_ - forecast) * GetCurrentPlayer().index_)) {
                return sum;
            }
        }

        return 0;
    }

    [[nodiscard]] Player GetCurrentPlayer() const {
        return players_[moves_count_ & 1u];
    }

    Player GetPrevPlayer() {
        return players_[!(moves_count_ & 1u)];
    }

    [[nodiscard]] PlayerType GetCurrentPlayerType() const  {
        return players_type_[moves_count_ & 1u];
    }

    [[nodiscard]] string ConvertPlayerIndexToSymbol(int index) const {
        if (index ==  players_[0].index_) {
            return players_[0].symbol_;
        } else if (index ==  players_[1].index_) {
            return players_[1].symbol_;
        }

        return kSymbolPlayerNone_;
    }

    [[nodiscard]] Move GetMove() const {
        Move move(-1, -1);

        if (GetCurrentPlayerType() == PlayerType::USER) {
            string command;
            do {
                if(!command.empty()) {
                    interface_->Print("\"" + command + "\" is not valid move. Please enter valid move.\n");
                }
                interface_->Print(GetCurrentPlayer().name_ + ": "s);
                interface_->GetLine(command);
                move = Move(command);
            } while (!IsValidMove(move));
        }  else {
            interface_->Print(GetCurrentPlayer().name_ + ": "s);
            do {
                move = GetBotMove();
            } while (!IsValidMove(move));
            interface_->Print(move.ToString() + "\n");
        }

        return move;
    }

    void DoMove(const Move& move) {
        grid_[move.h_index][move.v_index] = GetCurrentPlayer().index_;
        grid_view_[move.h_index][move.v_index] = ConvertPlayerIndexToSymbol(grid_[move.h_index][move.v_index]);
        valid_moves_indexes_.Remove(grid_size_ * move.h_index + move.v_index);
        ++moves_count_;
    }

    [[nodiscard]] bool IsValidMove(const Move& move) const {
        return  move.h_index >= 0 && move.h_index < grid_size_ && move.v_index >= 0 && move.v_index < grid_size_ && valid_moves_indexes_.Find(grid_size_ * move.h_index + move.v_index);
    }

    [[nodiscard]] Move IndexToMove(int move_index) const {
        return Move(move_index % grid_size_, move_index / grid_size_);
    }

    [[nodiscard]] Move GetBotMove() const {
        if (GetCurrentPlayerType() == PlayerType::SMART_BOT) {
            return GetSmartBotMove(valid_moves_indexes_.GetRoot());
        }

        return IndexToMove(valid_moves_indexes_.GetRoot()->key);
    }

    Move GetSmartBotMove(RandomizedBinarySearchTree::Node* root) const {
        auto win_move_index = SmartMove(root);
        return win_move_index ? IndexToMove(win_move_index->key) : IndexToMove(root->key);
    }

    RandomizedBinarySearchTree::Node* SmartMove(RandomizedBinarySearchTree::Node* p) const {
        RandomizedBinarySearchTree::Node* tmp_result = nullptr;

        int chance_to_win_for_current_player = CheckWin(IndexToMove(p->key), true) / GetCurrentPlayer().index_;
        if (chance_to_win_for_current_player > 0) {
            return p;
        } else if (chance_to_win_for_current_player < 0) {
            tmp_result = p;
        }

        if (p->left ) {
            auto left = SmartMove(p->left);
            if (left) return left;
        }

        if (p->right ) {
            auto right = SmartMove(p->right);
            if (right) return right;
        }

        return tmp_result;
    }

    const int grid_size_ = 3;
    uint8_t moves_count_ = 0u;
    int target_ = grid_size_;
    vector<Player> players_;
    vector<vector<int>> grid_;
    vector<vector<string>> grid_view_;
    vector<PlayerType> players_type_;
    RandomizedBinarySearchTree valid_moves_indexes_;
    shared_ptr<IInterface> interface_;
};

void Tests() {
    TicTacToe game;
    assert(game.MovesAnalysis({{0, 0}, {2, 0}, {0, 1}, {2, 1}, {0, 2}}) == "A"s);
    assert(game.MovesAnalysis({{1, 2}, {2, 0}, {0, 1}, {1, 0}, {2, 2}, {0, 0}}) == "B"s);
    assert(game.MovesAnalysis({{0, 0}, {1, 1}, {0, 1}, {0, 2}, {1, 0}, {2, 0}}) == "B"s);
    assert(game.MovesAnalysis({{0, 0}, {2, 0}, {1, 1}, {2, 1}, {2, 2}}) == "A"s);
    assert(game.MovesAnalysis({{0, 0}, {1, 1}, {0, 1}, {0, 2}, {1, 0}, {2, 0}}) == "B"s);
    assert(game.MovesAnalysis({{0, 0}, {1, 1}, {2, 0}, {1, 0}, {1, 2}, {2, 1}, {0, 1}, {0, 2}, {2, 2}}) == "Draw"s);
    assert(game.MovesAnalysis({{0, 0}, {1, 1}}) == "Pending"s);
    assert(game.MovesAnalysis({{0, 0}, {1, 1}, {1, 1}}) == "Not valid move"s);

    cout << "Tests Ok" << endl;
}

int main() {
    Tests();

    TicTacToe game(PlayerType::USER, PlayerType::BOT);

    while (true) {
        auto result = game.Play();
        cout << result << endl;
        cout << endl << "Play again? y/n: "s;
        string answer;
        getline(cin, answer);
        if (answer == "n"s) {
            break;
        }
    }

    return 0;
}
