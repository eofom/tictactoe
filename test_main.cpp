#include <gtest/gtest.h>

#include "board.h"
#include "game.h"
using namespace std;


//------------------------------------------------------------------------------
class TestGameBoard : public ::testing::Test {
protected:
        void SetUp()
        { }
};

//------------------------------------------------------------------------------
//TEST_F(TestTicTacToe, test) {

//}

//------------------------------------------------------------------------------
TEST_F(TestGameBoard, testCreateEmptyBoard) {
  Board board;
  for (size_t row = 0; row < 3; ++row) {
    for (size_t column = 0; column < 3; ++column) {
      ASSERT_EQ(board.getCellState(row, column), Board::Cell::State::Open);
    }
  }
}

//------------------------------------------------------------------------------
TEST_F(TestGameBoard, testCreateBoardFromMoves) {
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
TEST_F(TestGameBoard, testXWinsInRow) {
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
TEST_F(TestGameBoard, testXWinsInColumn) {
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
TEST_F(TestGameBoard, testXWinsTopDiag) {
  Board board;
  board.setCellState(0, 0, Board::Cell::State::X);
  board.setCellState(1, 1, Board::Cell::State::X);
  board.setCellState(2, 2, Board::Cell::State::X);

  ASSERT_TRUE(board.isTopLeftDiagonalFilled(Board::Cell::State::X));
  ASSERT_TRUE(board.isWinnerX());
}

//------------------------------------------------------------------------------
TEST_F(TestGameBoard, testXWinsBottomDiag) {
  Board board;
  board.setCellState(2, 0, Board::Cell::State::X);
  board.setCellState(1, 1, Board::Cell::State::X);
  board.setCellState(0, 2, Board::Cell::State::X);

  ASSERT_TRUE(board.isBottomLeftDiagonalFilled(Board::Cell::State::X));
  ASSERT_TRUE(board.isWinnerX());
}

//------------------------------------------------------------------------------
TEST_F(TestGameBoard, testDraw) {
  Board board = Board::Create({{0,0},{1,1},{2,0},{1,0},{1,2},{2,1},{0,1},{0,2},{2,2}});
  ASSERT_TRUE(board.possibleMoves().empty());
}

//------------------------------------------------------------------------------
TEST_F(TestGameBoard, testPending) {
  Board board = Board::Create({{0,0},{1,1}});
  ASSERT_EQ(7u, board.possibleMoves().size());
}

//------------------------------------------------------------------------------
class TestGameAI : public ::testing::Test {
protected:
        void SetUp()
        { }
};
//------------------------------------------------------------------------------
TEST_F(TestGameAI, testRowHasWinnerMove) {
  Game::AI ai;

  // X | _ | _
  {
    Board board;
    board.setCellState(0, 0, Board::Cell::State::X);
    board.setCellState(0, 1, Board::Cell::State::Open);
    board.setCellState(0, 2, Board::Cell::State::Open);
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::O, move));
  }

  // X | X | _
  {
    Board board;
    board.setCellState(0, 0, Board::Cell::State::X);
    board.setCellState(0, 1, Board::Cell::State::X);
    board.setCellState(0, 2, Board::Cell::State::Open);
    std::tuple<size_t, size_t> move;
    ASSERT_TRUE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::X, move));
    auto [row, column] = move;
        ASSERT_EQ(0, row);
        ASSERT_EQ(2u, column);
    ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::O, move));
  }

  // X | _ | X
  {
    Board board;
    board.setCellState(0, 0, Board::Cell::State::X);
    board.setCellState(0, 1, Board::Cell::State::Open);
    board.setCellState(0, 2, Board::Cell::State::X);
    std::tuple<size_t, size_t> move;
    ASSERT_TRUE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::X, move));
    auto [row, column] = move;
        ASSERT_EQ(0, row);
        ASSERT_EQ(1u, column);
    ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::O, move));
  }

  // _ | X | X
  {
    Board board;
    board.setCellState(0, 0, Board::Cell::State::Open);
    board.setCellState(0, 1, Board::Cell::State::X);
    board.setCellState(0, 2, Board::Cell::State::X);
    std::tuple<size_t, size_t> move;
    ASSERT_TRUE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::X, move));
    auto [row, column] = move;
        ASSERT_EQ(0, row);
        ASSERT_EQ(0, column);
    ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::O, move));
  }

    // X | X | O
    {
      Board board;
      board.setCellState(0, 0, Board::Cell::State::X);
      board.setCellState(0, 1, Board::Cell::State::X);
      board.setCellState(0, 2, Board::Cell::State::O);
      std::tuple<size_t, size_t> move;
      ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::X, move));
      ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::O, move));
    }

    // X | O | X
    {
      Board board;
      board.setCellState(0, 0, Board::Cell::State::X);
      board.setCellState(0, 1, Board::Cell::State::O);
      board.setCellState(0, 2, Board::Cell::State::X);
      std::tuple<size_t, size_t> move;
      ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::X, move));
      ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::O, move));
    }

    // O | X | X
    {
      Board board;
      board.setCellState(0, 0, Board::Cell::State::O);
      board.setCellState(0, 1, Board::Cell::State::X);
      board.setCellState(0, 2, Board::Cell::State::X);
      std::tuple<size_t, size_t> move;
      ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::X, move));
      ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::O, move));
    }

  // X | O | _
  {
    Board board = Board::Create({ {0,0}, {0,1} });
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::O, move));
  }

  // X | _ | _
  {
    Board board = Board::Create({ {0,0} });
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::O, move));
  }

  // _ | _ | _
  {
    // TODO
    Board board;
    board.setCellState(0, 0, Board::Cell::State::Open);
    board.setCellState(0, 1, Board::Cell::State::Open);
    board.setCellState(0, 2, Board::Cell::State::Open);
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::O, move));
  }

  // X | X | X
  {
    // TODO
    Board board;
    board.setCellState(0, 0, Board::Cell::State::X);
    board.setCellState(0, 1, Board::Cell::State::X);
    board.setCellState(0, 2, Board::Cell::State::X);
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.rowHasWinnerMove(board, 0, Board::Cell::State::O, move));
  }
}

//------------------------------------------------------------------------------
TEST_F(TestGameAI, testColumnHasWinnerMove) {
  Game::AI ai;

  // X
  // _
  // _
  {
    Board board;
    board.setCellState(0, 0, Board::Cell::State::X);
    board.setCellState(1, 0, Board::Cell::State::Open);
    board.setCellState(2, 0, Board::Cell::State::Open);
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::O, move));
  }

  // X
  // X
  // _
  {
    Board board = Board::Create({ {0,0}, {0,1}, {1,0} });
    std::tuple<size_t, size_t> move;
    ASSERT_TRUE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::O, move));
    auto [row, column] = move;
    ASSERT_EQ(2u, row);
    ASSERT_EQ(0, column);
  }

    // X
    // _
    // X
    {
      // TODO
      Board board;
      board.setCellState(0, 0, Board::Cell::State::X);
      board.setCellState(1, 0, Board::Cell::State::Open);
      board.setCellState(2, 0, Board::Cell::State::X);
      std::tuple<size_t, size_t> move;
      ASSERT_TRUE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::X, move));
      ASSERT_FALSE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::O, move));
      auto [row, column] = move;
      ASSERT_EQ(1, row);
      ASSERT_EQ(0, column);
    }

    // _
    // X
    // X
    {
      // TODO
      Board board;
      board.setCellState(0, 0, Board::Cell::State::Open);
      board.setCellState(1, 0, Board::Cell::State::X);
      board.setCellState(2, 0, Board::Cell::State::X);
      std::tuple<size_t, size_t> move;
      ASSERT_TRUE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::X, move));
      ASSERT_FALSE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::O, move));
      auto [row, column] = move;
      ASSERT_EQ(0, row);
      ASSERT_EQ(0, column);
    }

    // X
    // X
    // O
    {
      // TODO
      Board board;
      board.setCellState(0, 0, Board::Cell::State::X);
      board.setCellState(1, 0, Board::Cell::State::X);
      board.setCellState(2, 0, Board::Cell::State::O);
      std::tuple<size_t, size_t> move;
      ASSERT_FALSE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::X, move));
      ASSERT_FALSE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::O, move));
    }

    // X
    // O
    // X
    {
      // TODO
      Board board;
      board.setCellState(0, 0, Board::Cell::State::X);
      board.setCellState(1, 0, Board::Cell::State::O);
      board.setCellState(2, 0, Board::Cell::State::X);
      std::tuple<size_t, size_t> move;
      ASSERT_FALSE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::X, move));
      ASSERT_FALSE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::O, move));
    }

    // O
    // X
    // X
    {
      // TODO
      Board board;
      board.setCellState(0, 0, Board::Cell::State::O);
      board.setCellState(1, 0, Board::Cell::State::X);
      board.setCellState(2, 0, Board::Cell::State::X);
      std::tuple<size_t, size_t> move;
      ASSERT_FALSE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::X, move));
      ASSERT_FALSE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::O, move));
    }

  // X
  // O
  // _
  {
    Board board = Board::Create({ {0,0}, {1,0} });
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::O, move));
  }

  // _
  // _
  // _
  {
    Board board;
    board.setCellState(0, 0, Board::Cell::State::Open);
    board.setCellState(1, 0, Board::Cell::State::Open);
    board.setCellState(2, 0, Board::Cell::State::Open);
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::O, move));
  }

  // X
  // X
  // X
  {
    Board board;
    board.setCellState(0, 0, Board::Cell::State::X);
    board.setCellState(1, 0, Board::Cell::State::X);
    board.setCellState(2, 0, Board::Cell::State::X);
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.columnHasWinnerMove(board, 0, Board::Cell::State::O, move));
  }
}

//------------------------------------------------------------------------------
TEST_F(TestGameAI, testLeftTopDiagonalHasWinnerMove) {
  Game::AI ai;

  // _ | _ | _
  // _ | _ | _
  // _ | _ | _
  {
    Board board;
    board.setCellState(0,0, Board::Cell::State::Open);
    board.setCellState(1,1, Board::Cell::State::Open);
    board.setCellState(2,2, Board::Cell::State::Open);
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::O, move));
  }

  // X | _ | _
  // _ | X | _
  // _ | _ | X
  {
    Board board;
    board.setCellState(0,0, Board::Cell::State::X);
    board.setCellState(1,1, Board::Cell::State::X);
    board.setCellState(2,2, Board::Cell::State::X);
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::O, move));
  }

  // X | _ | _
  // _ | _ | _
  // _ | _ | _
  {
    Board board;
    board.setCellState(0,0, Board::Cell::State::X);
    board.setCellState(1,1, Board::Cell::State::Open);
    board.setCellState(2,2, Board::Cell::State::Open);
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::O, move));
  }


  // X | O | _
  // _ | X | _
  // _ | _ | _
  {
    Board board = Board::Create({ {0,0}, {0,1}, {1,1} });
    std::tuple<size_t, size_t> move;
    ASSERT_TRUE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
    auto [row, column] = move;
    ASSERT_EQ(2u, row);
    ASSERT_EQ(2u, column);
    ASSERT_FALSE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::O, move));
  }

    // X | _ | _
    // _ | _ | _
    // _ | _ | X
    {
      // TODO
      Board board;
      board.setCellState(0,0, Board::Cell::State::X);
      board.setCellState(1,1, Board::Cell::State::Open);
      board.setCellState(2,2, Board::Cell::State::X);
      std::tuple<size_t, size_t> move;
      ASSERT_TRUE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
      auto [row, column] = move;
      ASSERT_EQ(1u, row);
      ASSERT_EQ(1u, column);
      ASSERT_FALSE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::O, move));
    }

    // _ | _ | _
    // _ | X | _
    // _ | _ | X
    {
      // TODO
      Board board;
      board.setCellState(0,0, Board::Cell::State::Open);
      board.setCellState(1,1, Board::Cell::State::X);
      board.setCellState(2,2, Board::Cell::State::X);
      std::tuple<size_t, size_t> move;
      ASSERT_TRUE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
      auto [row, column] = move;
      ASSERT_EQ(0, row);
      ASSERT_EQ(0, column);
      ASSERT_FALSE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::O, move));

    }

    // X | _ | _
    // _ | X | _
    // _ | _ | O
    {
      // TODO
      Board board;
      board.setCellState(0,0, Board::Cell::State::X);
      board.setCellState(1,1, Board::Cell::State::X);
      board.setCellState(2,2, Board::Cell::State::O);
      std::tuple<size_t, size_t> move;
      ASSERT_FALSE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
      ASSERT_FALSE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::O, move));
    }

    // X | _ | _
    // _ | O | _
    // _ | _ | X
    {
      // TODO
      Board board;
      board.setCellState(0,0, Board::Cell::State::X);
      board.setCellState(1,1, Board::Cell::State::O);
      board.setCellState(2,2, Board::Cell::State::X);
      std::tuple<size_t, size_t> move;
      ASSERT_FALSE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
      ASSERT_FALSE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::O, move));
    }

    // O | _ | _
    // _ | X | _
    // _ | _ | X
    {
      // TODO
      Board board;
      board.setCellState(0,0, Board::Cell::State::O);
      board.setCellState(1,1, Board::Cell::State::X);
      board.setCellState(2,2, Board::Cell::State::X);
      std::tuple<size_t, size_t> move;
      ASSERT_FALSE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
      ASSERT_FALSE(ai.leftTopDiagonalHasWinnerMove(board, Board::Cell::State::O, move));
    }
}

//------------------------------------------------------------------------------
TEST_F(TestGameAI, testLeftBottomDiagonalHasWinnerMove) {
  Game::AI ai;

  // _ | _ | _
  // _ | _ | _
  // _ | _ | _
  {
    Board board;
    board.setCellState(0, 2, Board::Cell::State::Open);
    board.setCellState(1, 1, Board::Cell::State::Open);
    board.setCellState(2, 0, Board::Cell::State::Open);
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::O, move));
  }

  // _ | _ | X
  // _ | X | _
  // X | _ | _
  {
    // TODO
    Board board;
    board.setCellState(0, 2, Board::Cell::State::X);
    board.setCellState(1, 1, Board::Cell::State::X);
    board.setCellState(2, 0, Board::Cell::State::X);
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::O, move));
  }

  // _ | _ | X
  // _ | _ | _
  // _ | _ | _
  {
    Board board;
    board.setCellState(0, 2, Board::Cell::State::X);
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::O, move));
  }

  // _ | O | X
  // _ | X | _
  // _ | _ | _
  {
    Board board = Board::Create({ {0,2}, {0,1}, {1,1} });
    std::tuple<size_t, size_t> move;
    ASSERT_TRUE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
    auto [ row, column ] = move;
    ASSERT_EQ(2u, row);
    ASSERT_EQ(0, column);
    ASSERT_FALSE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::O, move));
  }

  // _ | _ | X
  // _ | _ | _
  // X | _ | _
  {
    // TODO
    Board board;
    board.setCellState(0, 2, Board::Cell::State::X);
    board.setCellState(1, 1, Board::Cell::State::Open);
    board.setCellState(2, 0, Board::Cell::State::X);
    std::tuple<size_t, size_t> move;
    ASSERT_TRUE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
    auto [ row, column ] = move;
    ASSERT_EQ(1u, row);
    ASSERT_EQ(1u, column);
    ASSERT_FALSE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::O, move));
  }

  // _ | _ | _
  // _ | X | _
  // X | _ | _
  {
    // TODO
    Board board;
    board.setCellState(0, 2, Board::Cell::State::Open);
    board.setCellState(1, 1, Board::Cell::State::X);
    board.setCellState(2, 0, Board::Cell::State::X);
    std::tuple<size_t, size_t> move;
    ASSERT_TRUE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
    auto [ row, column ] = move;
    ASSERT_EQ(0, row);
    ASSERT_EQ(2u, column);
    ASSERT_FALSE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::O, move));
  }

  // _ | _ | X
  // _ | X | _
  // O | _ | _
  {
    // TODO
    Board board;
    board.setCellState(0, 2, Board::Cell::State::X);
    board.setCellState(1, 1, Board::Cell::State::X);
    board.setCellState(2, 0, Board::Cell::State::O);
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::O, move));
  }

  // _ | _ | X
  // _ | O | _
  // X | _ | _
  {
    // TODO
    Board board;
    board.setCellState(0, 2, Board::Cell::State::X);
    board.setCellState(1, 1, Board::Cell::State::O);
    board.setCellState(2, 0, Board::Cell::State::X);
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::O, move));
  }

  // _ | _ | O
  // _ | X | _
  // X | _ | _
  {
    // TODO
    Board board;
    board.setCellState(0, 2, Board::Cell::State::O);
    board.setCellState(1, 1, Board::Cell::State::X);
    board.setCellState(2, 0, Board::Cell::State::X);
    std::tuple<size_t, size_t> move;
    ASSERT_FALSE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::X, move));
    ASSERT_FALSE(ai.leftBottomDiagonalHasWinnerMove(board, Board::Cell::State::O, move));
  }
}

//------------------------------------------------------------------------------
TEST_F(TestGameAI, testWinnerMoves) {
  Game::AI ai;

  // X | _ \ _
  // x | x | _
  // _ | x | _
  {
    Board board;
    board.setCellState(0, 0, Board::Cell::State::X);
    board.setCellState(1, 0, Board::Cell::State::X);
    board.setCellState(1, 1, Board::Cell::State::X);
    board.setCellState(2, 1, Board::Cell::State::X);
    auto cells = ai.winnerMoves(board, Board::Cell::State::X);
    ASSERT_EQ(4u, cells.size());
    ASSERT_NE(cells.end(), std::find(cells.begin(), cells.end(), std::tuple{2,0}));
    ASSERT_NE(cells.end(), std::find(cells.begin(), cells.end(), std::tuple{0,1}));
    ASSERT_NE(cells.end(), std::find(cells.begin(), cells.end(), std::tuple{1,2}));
    ASSERT_NE(cells.end(), std::find(cells.begin(), cells.end(), std::tuple{2,2}));
  }
}

//------------------------------------------------------------------------------
TEST_F(TestGameAI, testMovesLeadingToTwoWinOpportunities) {
  Game::AI ai;

  // _ | O | O
  // _ | X | _
  // X | _ | _
  {
    Board board = Board::Create({ {1,1}, {0,1}, {2,0}, {0,2} });
//    std::clog << board << std::endl;
    auto moves = ai.movesLeadingToTwoWinOpportunities(board, Board::Cell::State::X);
//    std::clog << "testMovesLeadingToTwoWinOpportunities:\n"
//              << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
//    for (auto [row, column] : moves) {
//      std::clog << "["s << row << ","s << column << "]"s << std::endl;
//    }
    ASSERT_EQ(3u, moves.size());
    ASSERT_NE(moves.end(), std::find(moves.begin(), moves.end(), std::tuple{0,0}));
    ASSERT_NE(moves.end(), std::find(moves.begin(), moves.end(), std::tuple{1,0}));
    ASSERT_NE(moves.end(), std::find(moves.begin(), moves.end(), std::tuple{2,2}));
  }
}

//------------------------------------------------------------------------------
TEST_F(TestGameAI, testBestMove) {
  Game::AI ai;

  // O | _ | _
  // _ | X | _
  // X | _ | _
  {
    Board board = Board::Create({ {1,1}, {0,0}, {2,0} });
    ASSERT_EQ(ai.bestMove(board, Board::Cell::State::O), (std::tuple{0,2u}));
  }

  // _ | O | O
  // _ | X | _
  // X | _ | _
  {
    Board board = Board::Create({ {1,1}, {0,1}, {2,0}, {0,2} });
    ASSERT_EQ(ai.bestMove(board, Board::Cell::State::X), (std::tuple{0,0}));
  }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
