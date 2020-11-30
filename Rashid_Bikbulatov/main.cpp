#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Solution {
public:
    Solution() = default;

    string tictactoe(vector<vector<int>>& moves) {
        for (size_t i = 0; i < moves.size(); ++i) {
            string tic;
            if (i % 2 == 0) {
                tic = 'X';
            } else {
                tic = '0';
            }

            grid[tic].push_back(moves[i][0]);
            grid[tic].push_back(moves[i][1]);
        }

        // check win player A
        if (checkPlayerWin("X")) {
            return "A";
        }

        // check win player B
        if (checkPlayerWin("0")) {
            return "B";
        }

        // check end move
        if (moves.size() == 9u) {
            return "Draw";
        }

        return "Pending";
    }

private:
    map<string, vector<int>> grid;

    bool checkPlayerWin(const string& player) {
        int array[3][3] = {0};
        bool check = false;

        check = false;
        int row = 0;
        for (const auto& i : grid[player]) {
            // iteration through +2
            if (check) {
                array[row][i] = 1;
                check = false;
                continue;
            }
            if (i == 0) {
                row = i;
            } else if (i == 1) {
                row = i;
            } else if (i == 2) {
                row = i;
            }
            check = true;
        }

        if ((array[0][0] && array[0][1] && array[0][2] == 1) || (array[0][0] && array[1][0] && array[2][0] == 1) ) {
            return true;
        } else if (array[1][0] && array[1][1] && array[1][2] == 1 || (array[0][1] && array[1][1] && array[2][1] == 1) ) {
            return true;
        } else if (array[2][0] && array[2][1] && array[2][2] == 1 || (array[0][2] && array[1][2] && array[2][2] == 1) ) {
            return true;
        }

        // check diagonal
        if (array[1][1] == 0) {
            return false;
        }

        return ((array[0][0] && array[2][2] > 0) ||
                (array[0][2] && array[2][0] > 0));
    }
};


int main() {
    Solution solution;
    vector<vector<int>> moves = {{0,0}, {1, 1}, {2, 0}, {1, 0}, {1, 2}, {2, 1}, {0, 1}, {0, 2}, {2, 2}};

    string tic = solution.tictactoe(moves);
    cout << tic << endl;

    return 0;
}
