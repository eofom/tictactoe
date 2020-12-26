#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "array"

using namespace std;

class Solution {
public:
    string tictactoe(const vector<vector<int>>& moves) {
        for (size_t i = 0; i < moves.size(); ++i) {
            string tic;
            if (i % 2 == 0) {
                tic = 'X';
            } else {
                tic = '0';
            }

            vector<int> move = moves[i];
            grid[tic].push_back(move.front());
            grid[tic].push_back(move.back());
        }

        Player_A playerA;
        if (checkPlayerWin(playerA.char_move)) {
            return playerA.player;
        }

        Player_B playerB;
        if (checkPlayerWin(playerB.char_move)) {
            return playerB.player;
        }

        // check end move
        if (moves.size() == max_moves) {
            return "Draw";
        }

        return "Pending";
    }

private:
    map<string, vector<int>> grid;
    const size_t max_moves = 9;

    const struct {
        string player;
        string char_move;
    } Player_A = {"A", "X"};

    const struct {
        string player;
        string char_move;
    } Player_B = {"B", "0"};

    bool checkDiagonal(const array<array<int, 3>, 3>& arr) {
        if (arr[1][1] == 0) {
            return false;
    }

        return ((arr[0][0] && arr[2][2] > 0) ||
                (arr[0][2] && arr[2][0] > 0));
    }

    bool checkRowsAndCols(const array<array<int, 3>, 3>& arr) {
        if ((arr[0][0] && arr[0][1] && arr[0][2] == 1) || (arr[0][0] && arr[1][0] && arr[2][0] == 1) ) {
            return true;
        } else if (arr[1][0] && arr[1][1] && arr[1][2] == 1 || (arr[0][1] && arr[1][1] && arr[2][1] == 1) ) {
            return true;
        } else if (arr[2][0] && arr[2][1] && arr[2][2] == 1 || (arr[0][2] && arr[1][2] && arr[2][2] == 1) ) {
            return true;
        }

        return false;
    }

    bool checkPlayerWin(const string& player) {
        array<array<int, 3>, 3> arr{};
        bool check = false;

        check = false;
        int row = 0;
        for (const auto& i : grid[player]) {
            // iteration through +2
            if (check) {
                arr[row][i] = 1;
                check = false;
                continue;
            }

            row = i;
            check = true;
        }

        return checkRowsAndCols(arr) || checkDiagonal(arr);
    }
};


int main() {
    Solution solution;
    vector<vector<int>> moves = {{0,0}, {1, 1}, {2, 0}, {1, 0}, {1, 2}, {2, 1}, {0, 1}, {0, 2}, {2, 2}};

    string result = solution.tictactoe(moves);
    cout << result << endl;

    return 0;
}
