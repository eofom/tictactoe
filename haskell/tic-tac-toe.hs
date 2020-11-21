isWinnerA :: [(Int,Int)] -> Bool
isWinnerA moves = isWinner $ getMovesA moves

getMovesA :: [(Int,Int)] -> [(Int,Int)]
getMovesA moves = map (\(i, move) -> move) $ filter isOddMove $ enumerateMoves moves
  where isOddMove (i, move) = odd i
        enumerateMoves moves = zip [1,2..] moves

isWinner :: [(Int,Int)] -> Bool
isWinner moves = isLeftTopDiagonalFilled moves

isLeftTopDiagonalFilled :: [(Int,Int)] -> Bool
isLeftTopDiagonalFilled moves = 3 == (length (getLeftTopDiagonalMoves moves))
  where getLeftTopDiagonalMoves moves = filter isLeftTopDiagonalMove moves
        isLeftTopDiagonalMove (row,column)
          | (row,column) == (0,0) = True
          | (row,column) == (1,1) = True
          | (row,column) == (2,2) = True
          | otherwise  = False

main = do
  let moves = [(0,0),(2,0),(1,1),(2,1),(2,2)]
  print(isWinnerA moves)
  
