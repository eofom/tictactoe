#include <gtest/gtest.h>

#include "board.h"

//------------------------------------------------------------------------------
class TestTicTacToe : public ::testing::Test {
protected:
        void SetUp()
        { }
};

//------------------------------------------------------------------------------
//TEST_F(TestTicTacToe, test) {

//}

//------------------------------------------------------------------------------
TEST_F(TestTicTacToe, testCreateEmptyBoard) {
  Board board;
  for (size_t row = 0; row < 3; ++row) {
    for (size_t column = 0; column < 3; ++column) {
      ASSERT_EQ(board.getCellState(row, column), Board::Cell::State::Open);
    }
  }
}

//------------------------------------------------------------------------------
TEST_F(TestTicTacToe, testCreateBoardFromMoves) {
  Board board = Board::Create( { {0,0},{2,0},{1,1},{2,1},{2,2} } );
  ASSERT_EQ(board.getCellState(0,0), Board::Cell::State::X);
  ASSERT_EQ(board.getCellState(0,1), Board::Cell::State::Open);
  ASSERT_EQ(board.getCellState(0,2), Board::Cell::State::Open);

  ASSERT_EQ(board.getCellState(2,0), Board::Cell::State::O);
  ASSERT_EQ(board.getCellState(2,1), Board::Cell::State::O);
  ASSERT_EQ(board.getCellState(2,2), Board::Cell::State::X);

  ASSERT_EQ(board.getCellState(1,0), Board::Cell::State::Open);
  ASSERT_EQ(board.getCellState(1,1), Board::Cell::State::X);
  ASSERT_EQ(board.getCellState(1,2), Board::Cell::State::Open);
}

//------------------------------------------------------------------------------
TEST_F(TestTicTacToe, testXWinsInRow) {
  // Win by top row
  {
    Board board;
    board.setCellState(0, 0, Board::Cell::State::X);
    board.setCellState(0, 1, Board::Cell::State::X);
    board.setCellState(0, 2, Board::Cell::State::X);

    ASSERT_TRUE(board.isRowFilled(0, Board::Cell::State::X));
    ASSERT_TRUE(board.isWinnerX());
  }

  // Win by middle row
  {
    Board board;
    board.setCellState(1, 0, Board::Cell::State::X);
    board.setCellState(1, 1, Board::Cell::State::X);
    board.setCellState(1, 2, Board::Cell::State::X);

    ASSERT_TRUE(board.isRowFilled(1, Board::Cell::State::X));
    ASSERT_TRUE(board.isWinnerX());
  }

  // Win by bottom row
  {
    Board board;
    board.setCellState(2, 0, Board::Cell::State::X);
    board.setCellState(2, 1, Board::Cell::State::X);
    board.setCellState(2, 2, Board::Cell::State::X);

    ASSERT_TRUE(board.isRowFilled(2, Board::Cell::State::X));
    ASSERT_TRUE(board.isWinnerX());
  }
}

//------------------------------------------------------------------------------
TEST_F(TestTicTacToe, testXWinsInColumn) {
  // Win in left column
  {
    Board board;
    board.setCellState(0, 0, Board::Cell::State::X);
    board.setCellState(1, 0, Board::Cell::State::X);
    board.setCellState(2, 0, Board::Cell::State::X);

    ASSERT_TRUE(board.isColumnFilled(0, Board::Cell::State::X));
    ASSERT_TRUE(board.isWinnerX());
  }

  // Win in middle column
  {
    Board board;
    board.setCellState(0, 1, Board::Cell::State::X);
    board.setCellState(1, 1, Board::Cell::State::X);
    board.setCellState(2, 1, Board::Cell::State::X);

    ASSERT_TRUE(board.isColumnFilled(1, Board::Cell::State::X));
    ASSERT_TRUE(board.isWinnerX());
  }

  // Win in right column
  {
    Board board;
    board.setCellState(0, 2, Board::Cell::State::X);
    board.setCellState(1, 2, Board::Cell::State::X);
    board.setCellState(2, 2, Board::Cell::State::X);

    ASSERT_TRUE(board.isColumnFilled(2, Board::Cell::State::X));
    ASSERT_TRUE(board.isWinnerX());
  }
}

//------------------------------------------------------------------------------
TEST_F(TestTicTacToe, testXWinsTopDiag) {
  Board board;
  board.setCellState(0, 0, Board::Cell::State::X);
  board.setCellState(1, 1, Board::Cell::State::X);
  board.setCellState(2, 2, Board::Cell::State::X);

  ASSERT_TRUE(board.isTopLeftDiagonalFilled(Board::Cell::State::X));
  ASSERT_TRUE(board.isWinnerX());
}

//------------------------------------------------------------------------------
TEST_F(TestTicTacToe, testXWinsBottomDiag) {
  Board board;
  board.setCellState(2, 0, Board::Cell::State::X);
  board.setCellState(1, 1, Board::Cell::State::X);
  board.setCellState(0, 2, Board::Cell::State::X);

  ASSERT_TRUE(board.isBottomLeftDiagonalFilled(Board::Cell::State::X));
  ASSERT_TRUE(board.isWinnerX());
}

//------------------------------------------------------------------------------
TEST_F(TestTicTacToe, testDraw) {
  Board board = Board::Create({{0,0},{1,1},{2,0},{1,0},{1,2},{2,1},{0,1},{0,2},{2,2}});
  ASSERT_TRUE(board.possibleMoves().empty());
}

//------------------------------------------------------------------------------
TEST_F(TestTicTacToe, testPending) {
  Board board = Board::Create({{0,0},{1,1}});
  ASSERT_EQ(7u, board.possibleMoves().size());
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
