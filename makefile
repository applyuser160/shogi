CC = gcc
OBJECT_FILES = mcts.o db_midiate.o board.o node.o piece.o tree.o util.o
LIB_MYSQL = -L "./mysql-8.1.0-winx64/lib" -llibmysql
CLEAN = del -f *.o

all: main
	$(CC) -o main main.o $(OBJECT_FILES) $(LIB_MYSQL)
	$(CLEAN)
learning: learn
	$(CC) -o learn learn.o $(OBJECT_FILES) $(LIB_MYSQL)
	$(CLEAN)
merging: merge
	$(CC) -o merge merge.o $(OBJECT_FILES) $(LIB_MYSQL)
	$(CLEAN)
learn: mcts
	$(CC) -Iinclude -c -o learn.o learn.c
merge: mcts
	$(CC) -Iinclude -c -o merge.o merge.c
main: mcts
	$(CC) -Iinclude -c -o main.o main.c
db_midiate: node
	$(CC) -c db_midiate.c -I include 
mcts: node tree board db_midiate
	$(CC) -Iinclude -c -o mcts.o mcts.c
board: piece
	$(CC) -Iinclude -c -o board.o board.c
game: board util
	$(CC) -Iinclude -c -o game.o game.c
node: board util
	$(CC) -Iinclude -c -o node.o node.c
piece: util
	$(CC) -c -o piece.o piece.c
tree: board node util
	$(CC) -Iinclude -c -o tree.o tree.c
util:
	$(CC) -Iinclude -c -o util.o util.c