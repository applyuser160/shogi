from enum import IntEnum, Enum

class Direction(IntEnum):
    UP = 0
    UP_LEFT = 1
    LEFT = 2
    DOWN_LEFT = 3
    DOWN = 4
    DOWN_RIGHT = 5
    RIGHT = 6
    UP_RIGHT = 7

class PieceName(IntEnum):
    NON = -1
    KING = 0
    ROOK = 1
    BICHOP = 2
    GOLDGENERAL = 3
    SILVERGENERAL = 4
    KNIGHT = 5
    LANCE = 6
    PAWN = 7
    PROMOTED_ROOK = 8
    PROMOTED_BICHOP = 9
    PROMOTED_SILVERGENERAL = 10
    PROMOTED_KNIGHT = 11
    PROMOTED_LANCE = 12
    PROMOTED_PAWN = 13

class Promotion(IntEnum):
    PROMOTABLE = True
    NOTPROMOTABLE = False

class Turn(IntEnum):
    FIRST = True
    SECOND = False

class TemplateMove(IntEnum):
    NEXTTO = 0
    UNLIMITED = 1
    HOP = 2
    NOT = 3

class SquareStatus(IntEnum):
    NOT_USED = False
    USED = True

class ConsoleColor:
    FIRST = '\x1b[36m'
    SECOND = '\x1b[91m'
    RESET = '\x1b[0m'