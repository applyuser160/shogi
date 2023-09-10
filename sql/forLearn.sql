/*重複しているレコードの探索　重複削除*/
SELECT
   nn.parentID AS newnode_parentID,
   nn.turnNumber AS newnode_turnNumber,
   nn.move AS newnode_move,
   nn.board AS newnode_board,
   SUM(nn.throughCount) AS sumThroughCount,
   SUM(nn.drawCount) AS sumDrawCount,
   SUM(nn.firstWinCount) AS sumFirstWinCount,
   SUM(nn.secondWinCount) AS sumSecondWinCount
FROM newnode nn
INNER JOIN (
   SELECT parentID, turnNumber, move, board
   FROM newnode
   WHERE turnNumber < 10
   GROUP BY parentID, turnNumber, move, board
   HAVING COUNT(*) > 1
   LIMIT 1000
) AS subquery
ON nn.parentID = subquery.parentID
   AND nn.turnNumber = subquery.turnNumber
   AND nn.move = subquery.move
   AND nn.board = subquery.board
WHERE nn.turnNumber < 10
GROUP BY nn.parentID, nn.turnNumber, nn.move, nn.board
HAVING COUNT(*) > 1
LIMIT 1000;


/*重複しているレコードの探索　レコード単位*/
SELECT
   nn."ID",
   nn."parentID",
   nn."turnNumber",
   nn.move,
   nn.board,
   nn."throughCount",
   nn."drawCount",
   nn."firstWinCount",
   nn."secondWinCount"
FROM newnode nn
INNER JOIN (
   SELECT parentID, turnNumber, move, board
   FROM newnode
   WHERE turnNumber < 10
   GROUP BY parentID, turnNumber, move, board
   HAVING COUNT(*) > 1
   LIMIT 1000
) AS subquery
ON nn.parentID = subquery.parentID
   AND nn.turnNumber = subquery.turnNumber
   AND nn.move = subquery.move
   AND nn.board = subquery.board;

/*重複しているレコードに紐づくレコードの探索*/
SELECT
   nn."ID",
   nn."parentID",
   nn."turnNumber",
   nn.move,
   nn.board,
   nn."throughCount",
   nn."drawCount",
   nn."firstWinCount",
   nn."secondWinCount"
FROM newnode nn
INNER JOIN (
   SELECT ID, parentID, turnNumber, move, board, "throughCount", "drawCount", "firstWinCount", "secondWinCount"
   FROM newnode
   WHERE turnNumber < 10
   GROUP BY ID, parentID, turnNumber, move, board, "throughCount", "drawCount", "firstWinCount", "secondWinCount"
   HAVING COUNT(*) > 1
   LIMIT 1000
) AS subquery
ON nn.parentID = subquery.parentID
   AND nn.turnNumber = subquery.turnNumber
   AND nn.move = subquery.move
   AND nn.board = subquery.board;