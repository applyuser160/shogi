create table shogi.Node (
  ID varchar(100) not null
  , parentID varchar(100) null
  , turnNumber int(10) not null 
  , `row` int(10) not null 
  , `column` int(10) not null 
  , pieceName int(10) not null 
  , pieceID int(10) not null 
  , throughCount int(10) not null
  , drawCount int(10) not null 
  , firstWinCount int(10) not null 
  , secondWinCount int(10) not null 
  , constraint Node_PKC primary key (ID)
) comment 'Node' ;

ALTER TABLE shogi.Node MODIFY COLUMN ID VARCHAR(100);
ALTER TABLE shogi.Node MODIFY COLUMN parentID VARCHAR(100) NULL;