#include <iostream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <sstream>
#include <functional>
#include <algorithm>

using namespace std;

enum class GameItem {
   EMPTY_SYMBOL,
   PLAYER_1_SYMBOL,
   PLAYER_2_SYMBOL,
};

enum class GameResult {
   PENDING,
   DRAW,
   PLAYER_1_WIN,
   PLAYER_2_WIN
};

string GameResultToString( const GameResult result )
{
   string resultString;
   switch (result) {
      case GameResult::PENDING :
         resultString = "Pending"s;
         break;
      case GameResult::DRAW :
         resultString = "Draw"s;
         break;
      case GameResult::PLAYER_1_WIN :
         resultString = "Player 1 win"s;
         break;
      case GameResult::PLAYER_2_WIN :
         resultString = "Player 2 win"s;
         break;
   }

   return resultString;
}

char GameItemToChar( const GameItem item )
{
   char ret = ' ';
   switch (item) {
      case GameItem::EMPTY_SYMBOL :
         ret = '.';
         break;

      case GameItem::PLAYER_1_SYMBOL :
         ret = 'X';
         break;

      case GameItem::PLAYER_2_SYMBOL :
         ret = 'O';
         break;
   }

   return ret;
}

class GameField {
private :
   vector<vector<GameItem>> matrix_;
   uint32_t rowCount_;
   uint32_t colCount_;

public :
   GameField(uint32_t fieldSize = 3)
      : matrix_(fieldSize, vector<GameItem>(fieldSize, GameItem::EMPTY_SYMBOL)),
        rowCount_( fieldSize ),
        colCount_( fieldSize )
   {
   }

   bool isSquare() const { return rowCount_==colCount_; }
   uint32_t getRowCount() const { return rowCount_; }
   uint32_t getColCount() const { return colCount_; }

   void show()
   {
      printf("Game field content :\n");
      for ( uint32_t rowIdx = 0; rowIdx < rowCount_; ++rowIdx) {
         bool isFirstItemInRow = true;

         for ( uint32_t colIdx = 0; colIdx < colCount_; ++colIdx ) {
            if ( isFirstItemInRow ) {
               isFirstItemInRow = !isFirstItemInRow;
               cout << GameItemToChar(matrix_[rowIdx][colIdx]);
            } else {
               cout << " " << GameItemToChar(matrix_[rowIdx][colIdx]);
            }
         }
         cout << endl;
      }
   }

   GameItem& operator () (const uint32_t rowPosition, const uint32_t colPosition)
   {
      checkMatrixRange( rowPosition, colPosition );
      return matrix_[rowPosition][colPosition];
   }

   const GameItem& operator () (const uint32_t rowPosition, const uint32_t colPosition) const
   {
      checkMatrixRange( rowPosition, colPosition );
      return matrix_[rowPosition][colPosition];
   }


private :
   void checkMatrixRange( const uint32_t rowPosition, const uint32_t colPosition ) const
   {
      if (( rowPosition > (rowCount_ - 1) ) || ( colPosition > (colCount_ - 1))) {
         stringstream ss;
         ss << "matrix out of range access request (" << rowPosition << "," << colPosition << ")"
            << " available (" << rowCount_ - 1 << "," << colCount_-1 << ")";
         throw out_of_range(ss.str());
      }
   }
};

class GameFieldChecker
{
public :
   static bool isEmptyItemsLeft(const GameField& field )
   {
      for ( uint32_t rowIdx = 0; rowIdx < field.getRowCount(); ++rowIdx) {
         for ( uint32_t colIdx = 0; colIdx < field.getColCount(); ++colIdx ) {
            if (field(rowIdx,colIdx) == GameItem::EMPTY_SYMBOL) {
               return true;
            }
         }
      }

      return false;
   }

   static bool isAnyColumnMatchSymbol(const GameField& field, const GameItem gameItem)
   {
      for ( uint32_t columnIdx = 0; columnIdx < field.getColCount(); columnIdx++ ) {
         if ( true == isColumnMatchSymbol(field, gameItem, columnIdx) ) {
            return true;
         }
      }

      return false;
   }

   static bool isAnyRowMatchSymbol(const GameField& field, const GameItem gameItem)
   {
      for ( uint32_t rowIdx = 0; rowIdx < field.getRowCount(); rowIdx++ ) {
         if ( true == isRowMatchSymbol(field, gameItem, rowIdx) ) {
            return true;
         }
      }

      return false;
   }

   static bool isDiagonalMatchSymbol(const GameField& field, const GameItem gameItem)
   {
      if ( !field.isSquare() ) {
         throw logic_error("Diagonal exists for square fields only");
      }

      if ( isMainDiagonalMatchSymbol( field, gameItem ) ) {
         return true;
      }

      return isReverseDiagonalMatchSymbol( field, gameItem );
   }

private :
   static bool isReverseDiagonalMatchSymbol(const GameField& field, const GameItem gameItem)
   {
      const auto fieldSize = field.getColCount();
      for ( uint32_t idx = fieldSize; idx > 0 ; --idx ) {
         if ( field(fieldSize - idx, idx - 1) != gameItem ) {
            return false;
         }
      }

      return true;
   }

   static bool isMainDiagonalMatchSymbol(const GameField& field, const GameItem gameItem)
   {
      for ( uint32_t idx = 0; idx < field.getColCount(); ++idx ) {
         if ( field(idx, idx) != gameItem ) {
            return false;
         }
      }

      return true;
   }

   static bool isColumnMatchSymbol(const GameField& field, const GameItem gameItem, const uint32_t columnId)
   {
      for ( uint32_t rowIdx = 0; rowIdx < field.getRowCount(); rowIdx++ ) {
         if (field(rowIdx,columnId) != gameItem) {
            return false;
         }
      }

      return true;
   }

   static bool isRowMatchSymbol(const GameField& field, const GameItem gameItem, const uint32_t rowId)
   {
      for ( uint32_t columnIdx = 0; columnIdx < field.getColCount(); columnIdx++ ) {
         if (field(rowId,columnIdx) != gameItem) {
            return false;
         }
      }

      return true;
   }
};

class GameResultChecker
{
public :
   using CheckerType = function<bool (const GameField& field, const GameItem gameItem)>;

   static GameResult getGameResult(const GameField& field )
   {
      if (isPlayerWin(field, GameItem::PLAYER_1_SYMBOL)) {
         return GameResult::PLAYER_1_WIN;
      }

      if (isPlayerWin(field, GameItem::PLAYER_2_SYMBOL)) {
         return GameResult::PLAYER_2_WIN;
      }

      return GameFieldChecker::isEmptyItemsLeft(field) ? GameResult::PENDING : GameResult::DRAW;
   }

   static bool isPlayerWin(const GameField& field, const GameItem gameItem )
   {
      vector<CheckerType> checkers;

      checkers.push_back( bind(GameFieldChecker::isAnyRowMatchSymbol, placeholders::_1, placeholders::_2) );
      checkers.push_back( bind(GameFieldChecker::isAnyColumnMatchSymbol, placeholders::_1, placeholders::_2) );
      checkers.push_back( bind(GameFieldChecker::isDiagonalMatchSymbol, placeholders::_1, placeholders::_2) );

      return any_of(checkers.begin(), checkers.end(), [field, gameItem](CheckerType checker) {
         return checker(field, gameItem);
      });
   }
};

class GameLoader {
public :
   static void loadData( const vector<vector<uint32_t>>& moves, GameField& field )
   {
      for ( size_t moveIdx = 0; moveIdx < moves.size(); moveIdx++ ) {
         auto fieldCoordinated = moves[moveIdx];
         if ( 0 == moveIdx % 2 ) {
            field(fieldCoordinated[0], fieldCoordinated[1]) = GameItem::PLAYER_1_SYMBOL;
         } else {
            field(fieldCoordinated[0], fieldCoordinated[1]) = GameItem::PLAYER_2_SYMBOL;
         }
      }
   }
};

void TestSuite()
{
   {
      GameField data;

      for ( uint32_t rowIdx = 0; rowIdx < data.getRowCount(); ++rowIdx) {
         for ( uint32_t colIdx = 0; colIdx < data.getColCount(); ++colIdx ) {
            data(rowIdx,colIdx) = GameItem::PLAYER_1_SYMBOL;
         }
      }

      if ( GameFieldChecker::isEmptyItemsLeft(data) == true ) {
         throw logic_error("isEmptyItemsLeft failure");
      }
   }

   {
      GameField data;
      data(0, 0) = GameItem::PLAYER_1_SYMBOL;
      data(1, 0) = GameItem::PLAYER_1_SYMBOL;
      data(2, 0) = GameItem::PLAYER_1_SYMBOL;

      if ( false == GameFieldChecker::isAnyColumnMatchSymbol(data, GameItem::PLAYER_1_SYMBOL) ) {
         data.show();

         throw logic_error("column 0 not detected");
      }

      if ( false == GameResultChecker::isPlayerWin(data, GameItem::PLAYER_1_SYMBOL) ) {
         throw logic_error("player 1 win not detected (column 0)");
      }
   }

   {
      GameField data;
      data(0, 1) = GameItem::PLAYER_1_SYMBOL;
      data(1, 1) = GameItem::PLAYER_1_SYMBOL;
      data(2, 1) = GameItem::PLAYER_1_SYMBOL;

      if ( false == GameFieldChecker::isAnyColumnMatchSymbol(data, GameItem::PLAYER_1_SYMBOL) ) {
         data.show();

         throw logic_error("column 1 not detected");
      }

      if ( false == GameResultChecker::isPlayerWin(data, GameItem::PLAYER_1_SYMBOL) ) {
         throw logic_error("player 1 win not detected (column 1)");
      }
   }

   {
      GameField data;
      data(0, 2) = GameItem::PLAYER_1_SYMBOL;
      data(1, 2) = GameItem::PLAYER_1_SYMBOL;
      data(2, 2) = GameItem::PLAYER_1_SYMBOL;

      if ( false == GameFieldChecker::isAnyColumnMatchSymbol(data, GameItem::PLAYER_1_SYMBOL) ) {
         data.show();

         throw logic_error("column 2 not detected");
      }

      if ( false == GameResultChecker::isPlayerWin(data, GameItem::PLAYER_1_SYMBOL) ) {
         throw logic_error("player 1 win not detected (column 2)");
      }
   }

   {
      GameField data;

      if ( true == GameFieldChecker::isAnyColumnMatchSymbol(data, GameItem::PLAYER_1_SYMBOL) ) {
         data.show();

         throw logic_error("column false detection");
      }

      if ( true == GameResultChecker::isPlayerWin(data, GameItem::PLAYER_1_SYMBOL) ) {
         throw logic_error("player 1 win false detected (column false)");
      }

   }

   {
      GameField data;
      data(0, 0) = GameItem::PLAYER_1_SYMBOL;
      data(0, 1) = GameItem::PLAYER_1_SYMBOL;
      data(0, 2) = GameItem::PLAYER_1_SYMBOL;

      if ( false == GameFieldChecker::isAnyRowMatchSymbol(data, GameItem::PLAYER_1_SYMBOL) ) {
         data.show();

         throw logic_error("row 0 not detected");
      }
   }

   {
      GameField data;
      data(1, 0) = GameItem::PLAYER_1_SYMBOL;
      data(1, 1) = GameItem::PLAYER_1_SYMBOL;
      data(1, 2) = GameItem::PLAYER_1_SYMBOL;

      if ( false == GameFieldChecker::isAnyRowMatchSymbol(data, GameItem::PLAYER_1_SYMBOL) ) {
         data.show();

         throw logic_error("row 1 not detected");
      }

      if ( false == GameResultChecker::isPlayerWin(data, GameItem::PLAYER_1_SYMBOL) ) {
         throw logic_error("player 1 win not detected (row 1)");
      }

   }

   {
      GameField data;
      data(2, 0) = GameItem::PLAYER_1_SYMBOL;
      data(2, 1) = GameItem::PLAYER_1_SYMBOL;
      data(2, 2) = GameItem::PLAYER_1_SYMBOL;

      if ( false == GameFieldChecker::isAnyRowMatchSymbol(data, GameItem::PLAYER_1_SYMBOL) ) {
         data.show();

         throw logic_error("row 2 not detected");
      }

      if ( false == GameResultChecker::isPlayerWin(data, GameItem::PLAYER_1_SYMBOL) ) {
         throw logic_error("player 1 win not detected (row 2)");
      }
   }

   {
      GameField data;

      if ( true == GameFieldChecker::isAnyRowMatchSymbol(data, GameItem::PLAYER_1_SYMBOL) ) {
         data.show();

         throw logic_error("row false detection");
      }

      if ( true == GameResultChecker::isPlayerWin(data, GameItem::PLAYER_1_SYMBOL) ) {
         throw logic_error("player 1 win false detection (row false)");
      }
   }

   {
      GameField data;
      data(0, 0) = GameItem::PLAYER_1_SYMBOL;
      data(1, 0) = GameItem::PLAYER_1_SYMBOL;
      data(2, 0) = GameItem::PLAYER_1_SYMBOL;
      data(2, 1) = GameItem::PLAYER_1_SYMBOL;
      data(2, 2) = GameItem::PLAYER_1_SYMBOL;

      if ( false == GameFieldChecker::isAnyRowMatchSymbol(data, GameItem::PLAYER_1_SYMBOL) ) {
         data.show();

         throw logic_error("row & column not detected");
      }

      if ( false == GameFieldChecker::isAnyColumnMatchSymbol(data, GameItem::PLAYER_1_SYMBOL) ) {
         data.show();

         throw logic_error("row & column not detected");
      }

      if ( false == GameResultChecker::isPlayerWin(data, GameItem::PLAYER_1_SYMBOL) ) {
         throw logic_error("player 1 win not detected (row & column)");
      }
   }

   {
      GameField data;
      data(0,0) = GameItem::PLAYER_1_SYMBOL;
      data(1,1) = GameItem::PLAYER_1_SYMBOL;
      data(2,2) = GameItem::PLAYER_1_SYMBOL;

      if ( false == GameFieldChecker::isDiagonalMatchSymbol(data, GameItem::PLAYER_1_SYMBOL) ) {
         data.show();

         throw logic_error("main diagonal not detected");
      }

      if ( false == GameResultChecker::isPlayerWin(data, GameItem::PLAYER_1_SYMBOL) ) {
         throw logic_error("player 1 win not detected (main diagonal)");
      }
   }

   {
      GameField data;
      data(0,2) = GameItem::PLAYER_1_SYMBOL;
      data(1,1) = GameItem::PLAYER_1_SYMBOL;
      data(2,0) = GameItem::PLAYER_1_SYMBOL;

      if ( false == GameFieldChecker::isDiagonalMatchSymbol(data, GameItem::PLAYER_1_SYMBOL) ) {
         data.show();

         throw logic_error("reverse diagonal not detected");
      }

      if ( false == GameResultChecker::isPlayerWin(data, GameItem::PLAYER_1_SYMBOL) ) {
         throw logic_error("player 1 win not detected (reverse diagonal)");
      }
   }

   cout << "TestSuite finished ok" << endl;
}

int main()
{
   try {
      TestSuite();

      GameField data;
      GameLoader::loadData({ {0,0}, {0,1}, {1,0}, {0,2}, {2,0}, {1, 1}}, data);
      data.show();
      cout << "Result : " << GameResultToString(GameResultChecker::getGameResult(data)) << endl;

   } catch( const out_of_range& e ) {
      cout << e.what() << endl;
   }

   return 0;
}
