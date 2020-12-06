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
        Node(int key): key(key), left(0), right(0), size(1) {

        }
    };

    explicit RandomizedBinarySearchTree(int seed): root_(0), generator_(std::mt19937(seed)) {

    }

    ~RandomizedBinarySearchTree() {
        Clear();
        delete root_;
    }

    bool Find(int key) const {
        return Find(root_, key);
    }

    void Insert(int key) {
        root_ = Insert(root_, key);
    }

    void Remove(int key) {
        root_ = Remove(root_, key);
    }

    Node* GetRoot() const {
        return root_;
    }

    void Clear() {
        Clear(root_);
    }

private:
    Node* root_ = 0;
    std::mt19937 generator_;

    Node* Find(Node* p, int key) const {
        if (!p) return 0;
        if (key == p->key) {
            return p;
        }
        if (key < p->key) {
            return Find(p->left, key);
        } else {
            return Find(p->right, key);
        }
    }

    int GetSize(Node* p) {
        if (!p) return 0;
        return p->size;
    }

    void FixSize(Node* p) {
        p->size = GetSize(p->left) + GetSize(p->right) + 1;
    }

    Node* RotateRight(Node* p) {
        Node* q = p->left;
        if (!q) return p;
        p->left = q->right;
        q->right = p;
        q->size = p->size;
        FixSize(p);
        return q;
    }

    Node* RotateLeft(Node* q) {
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
            root_ =0;
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

class Player {
public:
    enum class Type {
        USER,
        BOT,
        SMART_BOT
    };

    explicit Player(const string& name, const string& simbol, int index)
        : name_(name),
        simbol_(simbol),
        index_(index) {
    }

    string GetName() const {
        return name_;
    }

    string GetSimbol() const {
        return simbol_;
    }

    int GetIndex() const {
        return index_;
    }

private:
    string name_;
    string simbol_;
    int index_;
};

ostream& operator<<(ostream& out, const Player::Type& player_type) {
    switch (player_type) {
        case Player::Type::USER:
            out << "USER"s;
        case Player::Type::BOT:
            out << "BOT"s;
        case Player::Type::SMART_BOT:
            out << "SMART_BOT"s;
    }
    return out;
}

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

    string ToString() {
        return string(1, static_cast<char>('a' + v_index)) + to_string((h_index + 1));
    }
};

ostream& operator<<(ostream& out, const Move& move) {
    return out << "(" << move.h_index << ", " << move.v_index << ")";
}

class IInterface {
public:
    virtual void SetVisible(bool) = 0;
    virtual void PrintGrid(const vector<vector<string>>&) const = 0;
    virtual void Print(const string&) const = 0;
    virtual void GetLine(string&) const = 0;

    virtual ~IInterface() {};
};

class ConsolInterface: public IInterface {
public:
    void SetVisible(bool visible) {
        visible_ = visible;
    }

    void GetLine(string& comand) const {
        getline(cin, comand);
    }

    void PrintGrid(const vector<vector<string>>& grid) const {
        Print("\n"s);
        for (int i = 0; i < grid.size(); ++ i) {
            PrintRowAndVerticalMarks(grid, i);
            (i + 1 < grid.size()) ? PrintBorder(grid) : PrintHorizontalMarks(grid);
        }
    }

    void Print(const string& str) const {
        if (visible_) {
            cout << str;
        }
    }

private:
    bool visible_ = true;

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

    void PrintBorder(vector<vector<string>> grid) const {
        Print(" "s);
        for (int j = 0; j + 1 < grid.size() * 2; ++ j) {
            Print((j % 2 == 0 ?  "-"s : " + "s));
        }
        Print("\n"s);
    }

    void PrintHorizontalMarks(vector<vector<string>> grid) const {
        Print(" "s);
        for (int j = 0; j < grid.size(); ++ j) {
            Print(static_cast<char>('a' + j)  + "   "s);
        }
        Print("\n"s);
    }
};

class Tictactoe {
public:
    Tictactoe(Player::Type player_type_a = Player::Type::BOT,
              Player::Type player_type_b = Player::Type::BOT,
              int grid_size = 3, int target = 0)
            : grid_size_(grid_size),
              target_(target ? target : grid_size),
              players_type_({player_type_a, player_type_b}),
              interface_(new ConsolInterface()),
              index_valid_moves_(RandomizedBinarySearchTree(chrono::system_clock::now().time_since_epoch().count())) {
        interface_->SetVisible(player_type_a == Player::Type::USER || player_type_b == Player::Type::USER);
        assert(kIndexPlayerNone_ == 0);
        assert(kIndexPlayerFirst_ + kIndexPlayerSecond_ == kIndexPlayerNone_);
        assert(kIndexPlayerFirst_ * kIndexPlayerSecond_ != kIndexPlayerNone_);
    }

    string Play() {
        PlayInit();
        Greeting();
        for (int i = 0; i < grid_size_ * grid_size_; ++i) {
            interface_->PrintGrid(grid_wiev_);
            if (CheckWin(DoMove(GetMove()))){
                interface_->PrintGrid(grid_wiev_);
                return GetPrevPlayer().GetName() + " is win!"s;
            }
        }
        interface_->PrintGrid(grid_wiev_);
        return "Draw"s;
    }

    string Play(vector<vector<int>> moves)  {
        PlayInit();
        for (int i = 0; i < moves.size(); ++i) {
            Move move = {moves[i][1], moves[i][0]};
            if (!IsValidMove(move)) {
                return "Not valid мove"s;
            }
            if (CheckWin(DoMove(move))){
                return moves.size() % 2 ? "A"s : "B"s;
            }
        }
        return moves.size() < grid_size_*grid_size_ ? "Pending"s : "Draw"s;
    }

    void SwapPlayers() {
        iter_swap(players_type_.begin(), players_type_.begin() + 1);
    }

private:
    int grid_size_ = 3;
    int moves_count_ = 0;
    int target_ = grid_size_;
    vector<Player> players_;
    vector<vector<int>> grid_;
    vector<vector<string>> grid_wiev_;
    vector<Player::Type> players_type_;
    RandomizedBinarySearchTree index_valid_moves_;
    shared_ptr<IInterface> interface_;

    inline static const int kIndexPlayerFirst_ = -1;
    inline static const int kIndexPlayerSecond_ = 1;
    inline static const int kIndexPlayerNone_ = 0;
    inline static const string kSimbolPlayerNone_ = " "s;

    void PlayInit() {
        moves_count_ = 0;
        SetGrid();
        SetValidMovs();
        PlaerInit();
    }

    void SetGrid() {
        grid_ = vector<vector<int>>(grid_size_, vector<int>(grid_size_, kIndexPlayerNone_));
        grid_wiev_ = vector<vector<string>>(grid_size_, vector<string>(grid_size_, kSimbolPlayerNone_));
    }

    void SetValidMovs() {
        index_valid_moves_.Clear();
        for (int i = 0; i < grid_size_; ++i) {
            for (int j = 0; j < grid_size_; ++j) {
                if (grid_[i][j] == kIndexPlayerNone_) {
                    index_valid_moves_.Insert(grid_size_ * i + j);
                }
            }
        }
    }

    void PlaerInit() {
        players_.emplace_back(Player("Player A"s, "X", kIndexPlayerFirst_));
        players_.emplace_back(Player("Player B"s, "O", kIndexPlayerSecond_));
    }

    void Greeting() const {
        interface_->Print("\rThe tic-tac-toe game has begun.\n"s);
    }

    int CheckWin(const Move& move , bool forecast = false) const {
        vector<int> sums(4, 0);

        for(int i = 0; i < grid_size_; ++i){
            sums[0] += grid_[move.h_index][i];
            sums[1] += grid_[i][move.v_index];
            sums[2] += move.h_index == move.v_index ? grid_[i][i] : 0;
            sums[3] += move.h_index == grid_size_ - move.v_index - 1 ? grid_[i][grid_size_ - i - 1] : 0;
        }

        for (int sum: sums) {
            if (abs(sum) == abs((target_ - forecast) * GetCurrentPlayer().GetIndex())) {
                return sum;
            }
        }

        return 0;
    }

    Player GetCurrentPlayer() const {
        return players_[moves_count_ % 2];
    }

    Player GetPrevPlayer() {
        return players_[!(moves_count_ % 2)];
    }

    Player::Type GetCurrentPlayerType() const  {
        return players_type_[moves_count_ % 2];
    }

    string ConvertPlayerIndexToSimbol(int index) const {
        if (index ==  players_[0].GetIndex()) {
            return players_[0].GetSimbol();
        } else if (index ==  players_[1].GetIndex()) {
            return players_[1].GetSimbol();
        }

        return kSimbolPlayerNone_;
    }

    Move GetMove() const {
        Move move(-1, -1);

        if (GetCurrentPlayerType() == Player::Type::USER) {
            string comand;
            do {
                if(!comand.empty()) {
                    interface_->Print("\"" + comand + "\" is not valid move. Please enter valid move.\n");
                }
                interface_->Print(GetCurrentPlayer().GetName() + ": "s);
                interface_->GetLine(comand);
                move = Move(comand);
            } while (!IsValidMove(move));
        }  else {
            interface_->Print(GetCurrentPlayer().GetName() + ": "s);
            do {
                move = GetBotMove();
            } while (!IsValidMove(move));
            interface_->Print(move.ToString() + "\n");
        }

        return move;
    }

    const Move& DoMove(const Move& move) {
        grid_[move.h_index][move.v_index] = GetCurrentPlayer().GetIndex();
        grid_wiev_[move.h_index][move.v_index] = ConvertPlayerIndexToSimbol(grid_[move.h_index][move.v_index]);
        index_valid_moves_.Remove(grid_size_ * move.h_index + move.v_index);
        ++moves_count_;
        return move;
    }

    bool IsValidMove(const Move& move) const {
        return  move.h_index >= 0 && move.h_index < grid_size_ && move.v_index >= 0 && move.v_index < grid_size_ && index_valid_moves_.Find(grid_size_ * move.h_index + move.v_index);
    }

    Move IndexToMove(int move_index) const {
        return Move(move_index % grid_size_, move_index / grid_size_);
    }

    Move GetBotMove() const {
        if (GetCurrentPlayerType() == Player::Type::SMART_BOT) {
            return GetSmartBotMove(index_valid_moves_.GetRoot());
        }

        return IndexToMove(index_valid_moves_.GetRoot()->key);
    }

    Move GetSmartBotMove(RandomizedBinarySearchTree::Node* root) const {
        auto win_move_index = SmartMove(root);
        return win_move_index ? IndexToMove(win_move_index->key) : IndexToMove(root->key);
    }

    RandomizedBinarySearchTree::Node* SmartMove(RandomizedBinarySearchTree::Node* p) const {
        RandomizedBinarySearchTree::Node* tmp_result = 0;

        int chance_to_win_for_current_player = CheckWin(IndexToMove(p->key), true) / GetCurrentPlayer().GetIndex();
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
};

void Tests() {
    Tictactoe game;
    assert(game.Play({{0, 0} ,{2, 0}, {0, 1}, {2, 1}, {0, 2}}) == "A"s);
    assert(game.Play({{1, 2} ,{2, 0}, {0, 1}, {1, 0}, {2, 2} , {0, 0}}) == "B"s);
    assert(game.Play({{0, 0}, {1, 1}, {0, 1}, {0, 2}, {1, 0}, {2, 0}}) == "B"s);
    assert(game.Play({{0, 0} ,{2, 0}, {1, 1}, {2, 1}, {2, 2}}) == "A"s);
    assert(game.Play({{0, 0}, {1, 1}, {0, 1}, {0, 2}, {1, 0}, {2, 0}}) == "B"s);
    assert(game.Play({{0, 0}, {1, 1}, {2, 0}, {1, 0}, {1, 2}, {2, 1}, {0, 1}, {0, 2}, {2, 2}}) == "Draw"s);
    assert(game.Play({{0, 0}, {1, 1}}) == "Pending"s);
    assert(game.Play({{0, 0}, {1, 1}, {1, 1}}) == "Not valid мove"s);

    cout << "Tests Ok" << endl;
}

int main() {
    Tests();

    Tictactoe game(Player::Type::USER, Player::Type::BOT);

    while (true) {
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
