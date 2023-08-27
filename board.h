#ifndef BOARD_H
#define BOARD_H

#include "piece.h"

typedef enum SquareStatus
{
    NOT_USED = false,
    USED = true,
} SquareStatus;

typedef struct Address
{
    int row;
    int column;
} Address;

typedef struct Square
{
    Address address;
    SquareStatus isUsed;
    Piece piece;
} Square;

typedef struct Board
{
    Square squares[NUMBER_OF_SQUARES];
} Board;

typedef struct Condition
{
    Board board;
    Piece notOnBoard[NUMBER_OF_PIECES];
    Turn turn;
    int turnNumber;
} Condition;

typedef struct Move
{
    Address address;
    Piece piece;
} Move;

// 駒を盤面に配置する
void deployAPiece(Board *board, Piece piece, int row, int column);

// 初期配置を返す
Board initBoard();

// 盤面・持ち駒の表示
void displayCondition(Condition condition);

// 持ち駒の初期化
void initNotOnBoard(Piece notOnBoard[NUMBER_OF_PIECES]);

// 状況の初期化
Condition initCondition();

// 任意の駒が盤面のどこに位置するか調べる　
// 存在しない場合は、0,0を返す
Address serchPieceFromBoard(Board board, Piece piece);

// 任意の駒が持ち駒の中のどこに位置するか調べる 
// 存在しない場合は、-1を返す
int serchPieceFromNotOnBoard(Piece notOnBoard[NUMBER_OF_PIECES], Piece piece);

// 盤面の任意の場所に駒を設置する
void deployPieceOnBoard(Board *board, Address address, Piece piece);

// 持ち駒の任意の場所に駒を設置する
void deployPieceNotOnBoard(Piece notOnBoard[NUMBER_OF_PIECES], int index, Piece piece);

// 任意の位置にある駒が動くことが出来るか
bool ableMove(Turn turn, PieceName name, Address address);

// 指定の駒が成ることが出来るか
bool ableBe(Address oldAddress, Address newAddress, Piece piece);

// 指定の方向に移動する
Square moveBoard(Board board, Piece piece, int row, int column, int length, Direction direction);

// 特定の方向にnマス移動した場所を返す
Square getSquareWhenMoved(Board board, Piece piece, int row, int column, int length, Direction direction);

// 桂馬の動く先
Square getSquareWhenMovedKnight(Board board, Piece piece, int row, int column, Direction direction);

// 二歩を検知
bool isTwoSteps(Board board, Piece piece, Address address);

// 手を追加
void addMoves(Move **moves, int *count, Address address, Piece piece);

// その場所に指すことができる手を追加する
void serchAndAddMoves(Move **pointableHands, int *count, Square fromSquare, Square targetSquare, Turn turn);

// 特定のアドレスから任意の場所に移動し、指すことが出来る手を追加する
void executeAddMoves(Move **pointableHands, int *count, Square fromSquare, Board board, Direction direction, Turn turn);

// 動かせる手を探索
int serchPointableHands(Condition condition, Move **pointableHands);

// 手を指し、状況を更新
void executeMove(Condition *condition, Move move);

// 終了判定
bool isEnd(Condition condition, Turn *winner);

void getBinaryFromBoard(char *out, Board board);

void getBinaryFromNotOnBoard(char *out, Piece notOnBoard[NUMBER_OF_PIECES]);

void getBinaryFromCondition(char *out, Condition condition);

#endif