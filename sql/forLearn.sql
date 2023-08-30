/*重複しているレコードに紐づくレコードの探索*/ 
select c.* from (select b.* from jdbctestdb.Node as b join (select a.* from (select parentID, turnNumber, `row`, `column`, pieceName, pieceID, count(*) as count, sum(throughCount) as sumThroughCount, sum(drawCount) as sumDdawCount, sum(firstWinCount) as sumFirstWinCount, sum(secondWinCount) as sumSecondWinCount from jdbctestdb.Node group by parentID, turnNumber, `row`, `column`, pieceName, pieceID order by turnNumber asc) as a where a.count > 1 order by a.turnNumber limit 100) as a on a.parentID = b.parentID and a.turnNumber = b.turnNumber and a.`row` = b.`row` and a.`column` = b.`column` and a.pieceName = b.pieceName and a.pieceID = b.pieceID order by b.turnNumber) as d left join jdbctestdb.Node as c on d.ID = c.parentID;
/*重複しているレコードの探索　レコード単位*/ 
select b.* from jdbctestdb.Node as b join (select a.* from (select parentID, turnNumber, `row`, `column`, pieceName, pieceID, count(*) as count, sum(throughCount) as sumThroughCount, sum(drawCount) as sumDdawCount, sum(firstWinCount) as sumFirstWinCount, sum(secondWinCount) as sumSecondWinCount from jdbctestdb.Node group by parentID, turnNumber, `row`, `column`, pieceName, pieceID order by turnNumber asc) as a where a.count > 1 order by a.turnNumber limit 100) as a on a.parentID = b.parentID and a.turnNumber = b.turnNumber and a.`row` = b.`row` and a.`column` = b.`column` and a.pieceName = b.pieceName and a.pieceID = b.pieceID order by b.turnNumber;
/*重複しているレコードの探索　重複削除*/ 
select a.* from (select parentID, turnNumber, `row`, `column`, pieceName, pieceID, count(*) as count, sum(throughCount) as sumThroughCount, sum(drawCount) as sumDdawCount, sum(firstWinCount) as sumFirstWinCount, sum(secondWinCount) as sumSecondWinCount from jdbctestdb.Node group by parentID, turnNumber, `row`, `column`, pieceName, pieceID order by turnNumber asc) as a where a.count > 1 order by a.turnNumber limit 100;
