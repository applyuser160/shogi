create index turnNumber_index on shogi.newnode(turnNumber);
create index parentID_index on shogi.newnode(parentID);
create index unique_index on shogi.newnode(parentID, turnNumber, `move`, `board`);