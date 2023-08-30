#ifndef PIECE_H
#define PIECE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "util.h"

#define NUMBER_OF_EDGE 9
#define NUMBER_OF_SQUARES 81
#define MAXIMUM_NUMBER_OF_DIRECTIONS_DUE_TO_POSITIONAL_MOVEMENT 2
#define NUMBER_OF_DIRECTIONS 8
#define NUMBER_OF_PIECES 40

typedef enum Direction
{
    UP = 0,
    UP_LEFT = 1,
    LEFT = 2,
    DOWN_LEFT = 3,
    DOWN = 4,
    DOWN_RIGHT = 5,
    RIGHT = 6,
    UP_RIGHT = 7,
} Direction;

typedef enum PieceName
{
    NON = -1,
    KING = 0,
    ROOK = 1,
    BICHOP = 2,
    GOLDGENERAL = 3,
    SILVERGENERAL = 4,
    KNIGHT = 5,
    LANCE = 6,
    PAWN = 7,
    PROMOTED_ROOK = 8,
    PROMOTED_BICHOP = 9,
    PROMOTED_SILVERGENERAL = 10,
    PROMOTED_KNIGHT = 11,
    PROMOTED_LANCE = 12,
    PROMOTED_PAWN = 13,
} PieceName;

typedef enum Promotion
{
    PROMOTABLE = true,
    NOTPROMOTABLE = false,
} Promotion;

typedef enum Turn
{
    FIRST = true,
    SECOND = false,
} Turn;

typedef enum TemplateMove
{
    NEXTTO = 0,
    UNLIMITED = 1,
    HOP = 2,
    NOT = 3,
} TemplateMove;

typedef struct ADirectionMove
{
    Direction direction;
    int maxLength;
} ADirectionMove;

typedef struct WayOfMove
{
    bool through;
    ADirectionMove adirectionMove[MAXIMUM_NUMBER_OF_DIRECTIONS_DUE_TO_POSITIONAL_MOVEMENT];
} WayOfMove;

typedef struct AbstractPiece
{
    PieceName name;
    Promotion promotion;
    WayOfMove wayOfMove[NUMBER_OF_DIRECTIONS];
} AbstractPiece;

typedef struct Piece
{
    AbstractPiece piece;
    int index;
    Turn turn;
} Piece;

// 駒の名前により成ることができるかを返す
Promotion getPromotion(PieceName name);

// 駒の動きのテンプレートを返す
WayOfMove getTemplateMove(TemplateMove move, Direction direction);

// 駒（抽象）を生成し、返す
AbstractPiece generateAbstractPiece(PieceName name);

// 駒（実体）を生成し、返す
Piece generatePiece(PieceName name, int index, Turn turn);

// 駒の表示用の文字列を返す
char pieceToString(AbstractPiece piece);

// 方向の逆を取得し、返す
Direction switchDirection(Direction direction);

// 指定の方向に移動するためのR1C1の加減を返す
void getAddressOfDirection(Direction direction, int *vectorRow, int *vectorColumn);

// 成った後の駒の名前を取得し、返す
PieceName getPromotedPieceName(PieceName name);

// 成る前の駒の名前を取得し、返す
PieceName getPieceNameBeforePromote(PieceName name);

// 駒の情報を11bitにする　
int getBinaryFromPiece(Piece piece);

#endif