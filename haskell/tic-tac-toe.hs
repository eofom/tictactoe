isWinnerA :: [(Int,Int)] -> Bool
isWinnerA moves = isWinner $ getMovesA moves

getMovesA :: [(Int,Int)] -> [(Int,Int)]
getMovesA moves = map (\(i, move) -> move) $ filter isOddMove $ enumerateMoves moves
  where isOddMove (i, move) = odd i
        enumerateMoves moves = zip [1,2..] moves

isWinnerB :: [(Int,Int)] -> Bool
isWinnerB moves = isWinner $ getMovesB moves

getMovesB :: [(Int,Int)] -> [(Int,Int)]
getMovesB moves = map (\(i, move) -> move) $ filter isEvenMove $ enumerateMoves moves
  where isEvenMove (i, move) = even i
        enumerateMoves moves = zip [1,2..] moves

isWinner :: [(Int,Int)] -> Bool
isWinner moves
  | isRowFilled 0 moves = True
  | isRowFilled 1 moves = True
  | isRowFilled 2 moves = True
  | isColumnFilled 0 moves = True
  | isColumnFilled 1 moves = True
  | isColumnFilled 2 moves = True
  | isLeftTopDiagonalFilled moves = True
  | isLeftBottomDiagonalFilled moves = True
  | otherwise = False

getRowMoves :: Int -> [(Int,Int)] -> [(Int,Int)]
getRowMoves rownum moves = filter (\(row,_) -> row == rownum) moves

isRowFilled :: Int -> [(Int,Int)] -> Bool
isRowFilled rownum moves = 3 == (length (getRowMoves rownum moves))

getColumnMoves :: Int -> [(Int,Int)] -> [(Int,Int)]
getColumnMoves colnum moves = filter (\(_,column) -> column == colnum) moves

isColumnFilled :: Int -> [(Int,Int)] -> Bool
isColumnFilled colnum moves = 3 == (length (getColumnMoves colnum moves))

isLeftTopDiagonalFilled :: [(Int,Int)] -> Bool
isLeftTopDiagonalFilled moves = 3 == (length (getLeftTopDiagonalMoves moves))
  where getLeftTopDiagonalMoves moves = filter isLeftTopDiagonalMove moves
        isLeftTopDiagonalMove (row,column)
          | (row,column) == (0,0) = True
          | (row,column) == (1,1) = True
          | (row,column) == (2,2) = True
          | otherwise  = False

isLeftBottomDiagonalFilled :: [(Int,Int)] -> Bool
isLeftBottomDiagonalFilled moves = 3 == (length (getLeftBottomDiagonalMoves moves))
  where getLeftBottomDiagonalMoves moves = filter isLeftBottomDiagonalMove moves
        isLeftBottomDiagonalMove (row,column)
          | (row,column) == (2,0) = True
          | (row,column) == (1,1) = True
          | (row,column) == (0,2) = True
          | otherwise = False

main = do
  let left_top_diag_a_moves = [(0,0),(2,0),(1,1),(2,1),(2,2)]
  print(isWinnerA left_top_diag_a_moves)

  let top_row_a_moves = [(0,0),(0,1),(0,1),(1,1),(0,2)]
  print(isWinnerA top_row_a_moves)

  let middle_column_a_moves = [(0,1),(0,0),(1,1),(1,0),(2,1)]
  print(isWinnerA middle_column_a_moves)

  let left_bottom_diag_a_moves = [(0,2),(0,1),(1,1),(0,2),(2,0)]
  print(isWinnerA left_bottom_diag_a_moves)

  let middle_row_b_moves = [(0,0),(1,0),(0,2),(1,1),(2,0),(1,2)]
  print(isWinnerA middle_row_b_moves)
  print(isWinnerB middle_row_b_moves)
