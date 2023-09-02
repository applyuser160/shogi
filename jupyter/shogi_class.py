from typing import List, Self
import numpy as np
import copy
from shogi_enum import Direction, PieceName, Promotion, Turn, TemplateMove, SquareStatus, ConsoleColor
from shogi_const import NUMBER_OF_EDGE, NUMBER_OF_SQUARES, MAXIMUM_NUMBER_OF_DIRECTIONS_DUE_TO_POSITIONAL_MOVEMENT, NUMBER_OF_DIRECTIONS, NUMBER_OF_PIECES
from shogi_func import getPromotion, switchDirection, printb, getPromotedPieceName, getPieceNameBeforePromote

class ADirectionMove:
    direction: Direction
    maxLength: int

    def __init__(self, direction: Direction, maxLength: int):
        self.direction = direction
        self.maxLength = maxLength

class WayOfMove:
    through: bool
    adirectionMove: List[ADirectionMove]

    def __init__(self, templateMove: TemplateMove, direction: Direction):
        self.adirectionMove = [None] * MAXIMUM_NUMBER_OF_DIRECTIONS_DUE_TO_POSITIONAL_MOVEMENT
        if templateMove == TemplateMove.NEXTTO:
            self.through = False
            self.adirectionMove[0] = ADirectionMove(direction, 1)
            self.adirectionMove[1] = ADirectionMove(Direction.UP, 0)
        elif templateMove == TemplateMove.UNLIMITED:
            self.through = False
            self.adirectionMove[0] = ADirectionMove(direction, NUMBER_OF_EDGE - 1)
            self.adirectionMove[1] = ADirectionMove(Direction.UP, 0)
        elif templateMove == TemplateMove.HOP:
            self.through = True
            self.adirectionMove[0] = ADirectionMove(direction, 1)
            self.adirectionMove[1] = ADirectionMove(Direction.UP, 1)
        elif templateMove == TemplateMove.NOT:
            self.through = False
            self.adirectionMove[0] = ADirectionMove(direction, 0)
            self.adirectionMove[1] = ADirectionMove(Direction.UP, 0)

class Address:
    row: int
    column: int

    def __init__(self, row: int, column: int):
        self.row = row
        self.column = column

    def equal(self, target: Self) -> bool:
        return self.row == target.row and self.column == target.column
    
    def getAddressOfDirection(direction: Direction) -> Self:
        if direction == Direction.UP:
            return Address(-1, 0)
        elif direction == Direction.UP_LEFT:
            return Address(-1, 1)
        elif direction == Direction.LEFT:
            return Address(0, 1)
        elif direction == Direction.DOWN_LEFT:
            return Address(1, 1)
        elif direction == Direction.DOWN:
            return Address(1, 0)
        elif direction == Direction.DOWN_RIGHT:
            return Address(1, -1)
        elif direction == Direction.RIGHT:
            return Address(0, -1)
        elif direction == Direction.UP_RIGHT:
            return Address(-1, -1)
    
class AbstractPiece:
    name: PieceName
    promotion: Promotion
    wayOfMove: List[WayOfMove]

    def __init__(self, name: PieceName):
        self.name = name
        self.promotion = getPromotion(name)
        self.wayOfMove = [None] * NUMBER_OF_DIRECTIONS

        if name == PieceName.NON:
            self.wayOfMove[Direction.UP] = WayOfMove(TemplateMove.NOT, Direction.UP)
            self.wayOfMove[Direction.UP_LEFT] = WayOfMove(TemplateMove.NOT, Direction.UP_LEFT)
            self.wayOfMove[Direction.LEFT] = WayOfMove(TemplateMove.NOT, Direction.LEFT)
            self.wayOfMove[Direction.DOWN_LEFT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_LEFT)
            self.wayOfMove[Direction.DOWN] = WayOfMove(TemplateMove.NOT, Direction.DOWN)
            self.wayOfMove[Direction.DOWN_RIGHT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_RIGHT)
            self.wayOfMove[Direction.RIGHT] = WayOfMove(TemplateMove.NOT, Direction.RIGHT)
            self.wayOfMove[Direction.UP_RIGHT] = WayOfMove(TemplateMove.NOT, Direction.UP_RIGHT)
        elif name == PieceName.KING:
            self.wayOfMove[Direction.UP] = WayOfMove(TemplateMove.NEXTTO, Direction.UP)
            self.wayOfMove[Direction.UP_LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.UP_LEFT)
            self.wayOfMove[Direction.LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.LEFT)
            self.wayOfMove[Direction.DOWN_LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.DOWN_LEFT)
            self.wayOfMove[Direction.DOWN] = WayOfMove(TemplateMove.NEXTTO, Direction.DOWN)
            self.wayOfMove[Direction.DOWN_RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.DOWN_RIGHT)
            self.wayOfMove[Direction.RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.RIGHT)
            self.wayOfMove[Direction.UP_RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.UP_RIGHT)
        elif name == PieceName.ROOK:
            self.wayOfMove[Direction.UP] = WayOfMove(TemplateMove.UNLIMITED, Direction.UP)
            self.wayOfMove[Direction.UP_LEFT] = WayOfMove(TemplateMove.NOT, Direction.UP_LEFT)
            self.wayOfMove[Direction.LEFT] = WayOfMove(TemplateMove.UNLIMITED, Direction.LEFT)
            self.wayOfMove[Direction.DOWN_LEFT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_LEFT)
            self.wayOfMove[Direction.DOWN] = WayOfMove(TemplateMove.UNLIMITED, Direction.DOWN)
            self.wayOfMove[Direction.DOWN_RIGHT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_RIGHT)
            self.wayOfMove[Direction.RIGHT] = WayOfMove(TemplateMove.UNLIMITED, Direction.RIGHT)
            self.wayOfMove[Direction.UP_RIGHT] = WayOfMove(TemplateMove.NOT, Direction.UP_RIGHT)
        elif name == PieceName.BICHOP:
            self.wayOfMove[Direction.UP] = WayOfMove(TemplateMove.NOT, Direction.UP)
            self.wayOfMove[Direction.UP_LEFT] = WayOfMove(TemplateMove.UNLIMITED, Direction.UP_LEFT)
            self.wayOfMove[Direction.LEFT] = WayOfMove(TemplateMove.NOT, Direction.LEFT)
            self.wayOfMove[Direction.DOWN_LEFT] = WayOfMove(TemplateMove.UNLIMITED, Direction.DOWN_LEFT)
            self.wayOfMove[Direction.DOWN] = WayOfMove(TemplateMove.NOT, Direction.DOWN)
            self.wayOfMove[Direction.DOWN_RIGHT] = WayOfMove(TemplateMove.UNLIMITED, Direction.DOWN_RIGHT)
            self.wayOfMove[Direction.RIGHT] = WayOfMove(TemplateMove.NOT, Direction.RIGHT)
            self.wayOfMove[Direction.UP_RIGHT] = WayOfMove(TemplateMove.UNLIMITED, Direction.UP_RIGHT)
        elif name == PieceName.GOLDGENERAL:
            self.wayOfMove[Direction.UP] = WayOfMove(TemplateMove.NEXTTO, Direction.UP)
            self.wayOfMove[Direction.UP_LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.UP_LEFT)
            self.wayOfMove[Direction.LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.LEFT)
            self.wayOfMove[Direction.DOWN_LEFT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_LEFT)
            self.wayOfMove[Direction.DOWN] = WayOfMove(TemplateMove.NEXTTO, Direction.DOWN)
            self.wayOfMove[Direction.DOWN_RIGHT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_RIGHT)
            self.wayOfMove[Direction.RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.RIGHT)
            self.wayOfMove[Direction.UP_RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.UP_RIGHT)
        elif name == PieceName.SILVERGENERAL:
            self.wayOfMove[Direction.UP] = WayOfMove(TemplateMove.NEXTTO, Direction.UP)
            self.wayOfMove[Direction.UP_LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.UP_LEFT)
            self.wayOfMove[Direction.LEFT] = WayOfMove(TemplateMove.NOT, Direction.LEFT)
            self.wayOfMove[Direction.DOWN_LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.DOWN_LEFT)
            self.wayOfMove[Direction.DOWN] = WayOfMove(TemplateMove.NOT, Direction.DOWN)
            self.wayOfMove[Direction.DOWN_RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.DOWN_RIGHT)
            self.wayOfMove[Direction.RIGHT] = WayOfMove(TemplateMove.NOT, Direction.RIGHT)
            self.wayOfMove[Direction.UP_RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.UP_RIGHT)
        elif name == PieceName.KNIGHT:
            self.wayOfMove[Direction.UP] = WayOfMove(TemplateMove.NOT, Direction.UP)
            self.wayOfMove[Direction.UP_LEFT] = WayOfMove(TemplateMove.HOP, Direction.UP_LEFT)
            self.wayOfMove[Direction.LEFT] = WayOfMove(TemplateMove.NOT, Direction.LEFT)
            self.wayOfMove[Direction.DOWN_LEFT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_LEFT)
            self.wayOfMove[Direction.DOWN] = WayOfMove(TemplateMove.NOT, Direction.DOWN)
            self.wayOfMove[Direction.DOWN_RIGHT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_RIGHT)
            self.wayOfMove[Direction.RIGHT] = WayOfMove(TemplateMove.NOT, Direction.RIGHT)
            self.wayOfMove[Direction.UP_RIGHT] = WayOfMove(TemplateMove.HOP, Direction.UP_RIGHT)
        elif name == PieceName.LANCE:
            self.wayOfMove[Direction.UP] = WayOfMove(TemplateMove.UNLIMITED, Direction.UP)
            self.wayOfMove[Direction.UP_LEFT] = WayOfMove(TemplateMove.NOT, Direction.UP_LEFT)
            self.wayOfMove[Direction.LEFT] = WayOfMove(TemplateMove.NOT, Direction.LEFT)
            self.wayOfMove[Direction.DOWN_LEFT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_LEFT)
            self.wayOfMove[Direction.DOWN] = WayOfMove(TemplateMove.NOT, Direction.DOWN)
            self.wayOfMove[Direction.DOWN_RIGHT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_RIGHT)
            self.wayOfMove[Direction.RIGHT] = WayOfMove(TemplateMove.NOT, Direction.RIGHT)
            self.wayOfMove[Direction.UP_RIGHT] = WayOfMove(TemplateMove.NOT, Direction.UP_RIGHT)
        elif name == PieceName.PAWN:
            self.wayOfMove[Direction.UP] = WayOfMove(TemplateMove.NEXTTO, Direction.UP)
            self.wayOfMove[Direction.UP_LEFT] = WayOfMove(TemplateMove.NOT, Direction.UP_LEFT)
            self.wayOfMove[Direction.LEFT] = WayOfMove(TemplateMove.NOT, Direction.LEFT)
            self.wayOfMove[Direction.DOWN_LEFT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_LEFT)
            self.wayOfMove[Direction.DOWN] = WayOfMove(TemplateMove.NOT, Direction.DOWN)
            self.wayOfMove[Direction.DOWN_RIGHT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_RIGHT)
            self.wayOfMove[Direction.RIGHT] = WayOfMove(TemplateMove.NOT, Direction.RIGHT)
            self.wayOfMove[Direction.UP_RIGHT] = WayOfMove(TemplateMove.NOT, Direction.UP_RIGHT)
        elif name == PieceName.PROMOTED_ROOK:
            self.wayOfMove[Direction.UP] = WayOfMove(TemplateMove.UNLIMITED, Direction.UP)
            self.wayOfMove[Direction.UP_LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.UP_LEFT)
            self.wayOfMove[Direction.LEFT] = WayOfMove(TemplateMove.UNLIMITED, Direction.LEFT)
            self.wayOfMove[Direction.DOWN_LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.DOWN_LEFT)
            self.wayOfMove[Direction.DOWN] = WayOfMove(TemplateMove.UNLIMITED, Direction.DOWN)
            self.wayOfMove[Direction.DOWN_RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.DOWN_RIGHT)
            self.wayOfMove[Direction.RIGHT] = WayOfMove(TemplateMove.UNLIMITED, Direction.RIGHT)
            self.wayOfMove[Direction.UP_RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.UP_RIGHT)
        elif name == PieceName.PROMOTED_BICHOP:
            self.wayOfMove[Direction.UP] = WayOfMove(TemplateMove.NEXTTO, Direction.UP)
            self.wayOfMove[Direction.UP_LEFT] = WayOfMove(TemplateMove.UNLIMITED, Direction.UP_LEFT)
            self.wayOfMove[Direction.LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.LEFT)
            self.wayOfMove[Direction.DOWN_LEFT] = WayOfMove(TemplateMove.UNLIMITED, Direction.DOWN_LEFT)
            self.wayOfMove[Direction.DOWN] = WayOfMove(TemplateMove.NEXTTO, Direction.DOWN)
            self.wayOfMove[Direction.DOWN_RIGHT] = WayOfMove(TemplateMove.UNLIMITED, Direction.DOWN_RIGHT)
            self.wayOfMove[Direction.RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.RIGHT)
            self.wayOfMove[Direction.UP_RIGHT] = WayOfMove(TemplateMove.UNLIMITED, Direction.UP_RIGHT)
        elif name == PieceName.PROMOTED_SILVERGENERAL:
            self.wayOfMove[Direction.UP] = WayOfMove(TemplateMove.NEXTTO, Direction.UP)
            self.wayOfMove[Direction.UP_LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.UP_LEFT)
            self.wayOfMove[Direction.LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.LEFT)
            self.wayOfMove[Direction.DOWN_LEFT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_LEFT)
            self.wayOfMove[Direction.DOWN] = WayOfMove(TemplateMove.NEXTTO, Direction.DOWN)
            self.wayOfMove[Direction.DOWN_RIGHT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_RIGHT)
            self.wayOfMove[Direction.RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.RIGHT)
            self.wayOfMove[Direction.UP_RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.UP_RIGHT)
        elif name == PieceName.PROMOTED_KNIGHT:
            self.wayOfMove[Direction.UP] = WayOfMove(TemplateMove.NEXTTO, Direction.UP)
            self.wayOfMove[Direction.UP_LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.UP_LEFT)
            self.wayOfMove[Direction.LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.LEFT)
            self.wayOfMove[Direction.DOWN_LEFT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_LEFT)
            self.wayOfMove[Direction.DOWN] = WayOfMove(TemplateMove.NEXTTO, Direction.DOWN)
            self.wayOfMove[Direction.DOWN_RIGHT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_RIGHT)
            self.wayOfMove[Direction.RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.RIGHT)
            self.wayOfMove[Direction.UP_RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.UP_RIGHT)
        elif name == PieceName.PROMOTED_LANCE:
            self.wayOfMove[Direction.UP] = WayOfMove(TemplateMove.NEXTTO, Direction.UP)
            self.wayOfMove[Direction.UP_LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.UP_LEFT)
            self.wayOfMove[Direction.LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.LEFT)
            self.wayOfMove[Direction.DOWN_LEFT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_LEFT)
            self.wayOfMove[Direction.DOWN] = WayOfMove(TemplateMove.NEXTTO, Direction.DOWN)
            self.wayOfMove[Direction.DOWN_RIGHT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_RIGHT)
            self.wayOfMove[Direction.RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.RIGHT)
            self.wayOfMove[Direction.UP_RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.UP_RIGHT)
        elif name == PieceName.PROMOTED_PAWN:
            self.wayOfMove[Direction.UP] = WayOfMove(TemplateMove.NEXTTO, Direction.UP)
            self.wayOfMove[Direction.UP_LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.UP_LEFT)
            self.wayOfMove[Direction.LEFT] = WayOfMove(TemplateMove.NEXTTO, Direction.LEFT)
            self.wayOfMove[Direction.DOWN_LEFT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_LEFT)
            self.wayOfMove[Direction.DOWN] = WayOfMove(TemplateMove.NEXTTO, Direction.DOWN)
            self.wayOfMove[Direction.DOWN_RIGHT] = WayOfMove(TemplateMove.NOT, Direction.DOWN_RIGHT)
            self.wayOfMove[Direction.RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.RIGHT)
            self.wayOfMove[Direction.UP_RIGHT] = WayOfMove(TemplateMove.NEXTTO, Direction.UP_RIGHT)
    
    def pieceToString(self) -> str:
        if self.name == PieceName.NON:
            return " "
        elif self.name == PieceName.KING:
            return "k"
        elif self.name == PieceName.ROOK:
            return "r"
        elif self.name == PieceName.BICHOP:
            return "b"
        elif self.name == PieceName.GOLDGENERAL:
            return "g"
        elif self.name == PieceName.SILVERGENERAL:
            return "s"
        elif self.name == PieceName.KNIGHT:
            return "n"
        elif self.name == PieceName.LANCE:
            return "l"
        elif self.name == PieceName.PAWN:
            return "p"
        elif self.name == PieceName.PROMOTED_ROOK:
            return "D"
        elif self.name == PieceName.PROMOTED_BICHOP:
            return "H"
        elif self.name == PieceName.PROMOTED_SILVERGENERAL:
            return "S"
        elif self.name == PieceName.PROMOTED_KNIGHT:
            return "N"
        elif self.name == PieceName.PROMOTED_LANCE:
            return "L"
        elif self.name == PieceName.PROMOTED_PAWN:
            return "P"
        
class Piece:
    piece: AbstractPiece
    index: int
    turn: Turn

    def __init__(self, name: PieceName, index: int, turn: Turn):
        self.piece = AbstractPiece(name)
        self.index = index
        self.turn = turn

    def getBinaryFromPiece(self) -> int:
        result: int = 0
        result += (self.index << 5)
        result += ((self.piece.name + 1) << 1)
        result += 1 if self.turn else 0
        return result
    
    def equal(self, target: Self) -> bool:
        return self.index == target.index
    
    def ableMove(self, address: Address) -> bool:
        if self.turn == Turn.FIRST:
            if self.piece.name == PieceName.KNIGHT and address.row < 3:
                return False
            elif (self.piece.name == PieceName.LANCE or self.piece.name == PieceName.PAWN) and address.row < 2:
                return False
        elif self.turn == Turn.SECOND:
            if self.piece.name == PieceName.KNIGHT and address.row > 7:
                return False
            elif (self.piece.name == PieceName.LANCE or self.piece.name == PieceName.PAWN) and address.row > 8:
                return False
        return True
    
    def ablePromote(self, oldAddress: Address, newAddress: Address) -> bool:
        if self.piece.promotion:
            if self.turn == Turn.FIRST:
                return oldAddress.row < 4 or newAddress.row < 4
            else:
                return oldAddress.row > 6 or newAddress.row > 6
        return False
    
    def toString(self) -> str:
        name: str = self.piece.pieceToString()
        index: str = f"{self.index:0=2}"
        turn: str = f"{self.turn}"
        direction: str = \
            f"u0:{self.piece.wayOfMove[0].adirectionMove[0].direction}:{self.piece.wayOfMove[0].adirectionMove[0].maxLength}," \
            f"u1:{self.piece.wayOfMove[0].adirectionMove[1].direction}:{self.piece.wayOfMove[0].adirectionMove[1].maxLength}," \
            f"ul0:{self.piece.wayOfMove[1].adirectionMove[0].direction}:{self.piece.wayOfMove[1].adirectionMove[0].maxLength}," \
            f"ul1:{self.piece.wayOfMove[1].adirectionMove[1].direction}:{self.piece.wayOfMove[1].adirectionMove[1].maxLength}," \
            f"l0:{self.piece.wayOfMove[2].adirectionMove[0].direction}:{self.piece.wayOfMove[2].adirectionMove[0].maxLength}," \
            f"l1:{self.piece.wayOfMove[2].adirectionMove[1].direction}:{self.piece.wayOfMove[2].adirectionMove[1].maxLength}," \
            f"dl0:{self.piece.wayOfMove[3].adirectionMove[0].direction}:{self.piece.wayOfMove[3].adirectionMove[0].maxLength}," \
            f"dl1:{self.piece.wayOfMove[3].adirectionMove[1].direction}:{self.piece.wayOfMove[3].adirectionMove[1].maxLength}," \
            f"d0:{self.piece.wayOfMove[4].adirectionMove[0].direction}:{self.piece.wayOfMove[4].adirectionMove[0].maxLength}," \
            f"d1:{self.piece.wayOfMove[4].adirectionMove[1].direction}:{self.piece.wayOfMove[4].adirectionMove[1].maxLength}," \
            f"dr0:{self.piece.wayOfMove[5].adirectionMove[0].direction}:{self.piece.wayOfMove[5].adirectionMove[0].maxLength}," \
            f"dr1:{self.piece.wayOfMove[5].adirectionMove[1].direction}:{self.piece.wayOfMove[5].adirectionMove[1].maxLength}," \
            f"r0:{self.piece.wayOfMove[6].adirectionMove[0].direction}:{self.piece.wayOfMove[6].adirectionMove[0].maxLength}," \
            f"r1:{self.piece.wayOfMove[6].adirectionMove[1].direction}:{self.piece.wayOfMove[6].adirectionMove[1].maxLength}," \
            f"ur0:{self.piece.wayOfMove[7].adirectionMove[0].direction}:{self.piece.wayOfMove[7].adirectionMove[0].maxLength}," \
            f"ur1:{self.piece.wayOfMove[7].adirectionMove[1].direction}:{self.piece.wayOfMove[7].adirectionMove[1].maxLength},"
        result: str = f"name:{name},index:{index},turn:{turn},direction:{direction}"
        return result
    
class Square:
    address: Address
    isUsed: SquareStatus
    piece: Piece

    def __init__(self, address: Address, isUsed: SquareStatus, piece: Piece):
        self.address = address
        self.isUsed = isUsed
        self.piece = piece

    def deployPiece(self, piece: Piece):
        self.isUsed = SquareStatus.USED
        self.piece = piece

class Board:
    squares: List[Square] = [None] * NUMBER_OF_SQUARES

    def deployPiece(self, piece: Piece, row: int, column: int):
        self.squares[9 * (row - 1) + (column - 1)].deployPiece(piece)

    def __init__(self):
        for i in range(NUMBER_OF_SQUARES):
            address = Address(i // 9 + 1, i % 9 + 1)
            piece = Piece(PieceName.NON, 0, Turn.FIRST)
            square = Square(address, SquareStatus.NOT_USED, piece)
            self.squares[i] = square
        
        self.deployPiece(Piece(PieceName.LANCE, 1, Turn.FIRST), 9, 1)

        # self.deployPiece(Piece(PieceName.KING, 1, Turn.FIRST), 9, 5)
        # self.deployPiece(Piece(PieceName.ROOK, 2, Turn.FIRST), 8, 2)
        # self.deployPiece(Piece(PieceName.BICHOP, 3, Turn.FIRST), 8, 8)
        # self.deployPiece(Piece(PieceName.GOLDGENERAL, 4, Turn.FIRST), 9, 4)
        # self.deployPiece(Piece(PieceName.GOLDGENERAL, 5, Turn.FIRST), 9, 6)
        # self.deployPiece(Piece(PieceName.SILVERGENERAL, 6, Turn.FIRST), 9, 3)
        # self.deployPiece(Piece(PieceName.SILVERGENERAL, 7, Turn.FIRST), 9, 7)
        # self.deployPiece(Piece(PieceName.KNIGHT, 8, Turn.FIRST), 9, 2)
        # self.deployPiece(Piece(PieceName.KNIGHT, 9, Turn.FIRST), 9, 8)
        # self.deployPiece(Piece(PieceName.LANCE, 10, Turn.FIRST), 9, 1)
        # self.deployPiece(Piece(PieceName.LANCE, 11, Turn.FIRST), 9, 9)
        # self.deployPiece(Piece(PieceName.PAWN, 12, Turn.FIRST), 7, 1)
        # self.deployPiece(Piece(PieceName.PAWN, 13, Turn.FIRST), 7, 2)
        # self.deployPiece(Piece(PieceName.PAWN, 14, Turn.FIRST), 7, 3)
        # self.deployPiece(Piece(PieceName.PAWN, 15, Turn.FIRST), 7, 4)
        # self.deployPiece(Piece(PieceName.PAWN, 16, Turn.FIRST), 7, 5)
        # self.deployPiece(Piece(PieceName.PAWN, 17, Turn.FIRST), 7, 6)
        # self.deployPiece(Piece(PieceName.PAWN, 18, Turn.FIRST), 7, 7)
        # self.deployPiece(Piece(PieceName.PAWN, 19, Turn.FIRST), 7, 8)
        # self.deployPiece(Piece(PieceName.PAWN, 20, Turn.FIRST), 7, 9)

        # self.deployPiece(Piece(PieceName.KING, 21, Turn.SECOND), 1, 5)
        # self.deployPiece(Piece(PieceName.ROOK, 22, Turn.SECOND), 2, 8)
        # self.deployPiece(Piece(PieceName.BICHOP, 23, Turn.SECOND), 2, 2)
        # self.deployPiece(Piece(PieceName.GOLDGENERAL, 24, Turn.SECOND), 1, 4)
        # self.deployPiece(Piece(PieceName.GOLDGENERAL, 25, Turn.SECOND), 1, 6)
        # self.deployPiece(Piece(PieceName.SILVERGENERAL, 26, Turn.SECOND), 1, 3)
        # self.deployPiece(Piece(PieceName.SILVERGENERAL, 27, Turn.SECOND), 1, 7)
        # self.deployPiece(Piece(PieceName.KNIGHT, 28, Turn.SECOND), 1, 2)
        # self.deployPiece(Piece(PieceName.KNIGHT, 29, Turn.SECOND), 1, 8)
        # self.deployPiece(Piece(PieceName.LANCE, 30, Turn.SECOND), 1, 1)
        # self.deployPiece(Piece(PieceName.LANCE, 31, Turn.SECOND), 1, 9)
        # self.deployPiece(Piece(PieceName.PAWN, 32, Turn.SECOND), 3, 1)
        # self.deployPiece(Piece(PieceName.PAWN, 33, Turn.SECOND), 3, 2)
        # self.deployPiece(Piece(PieceName.PAWN, 34, Turn.SECOND), 3, 3)
        # self.deployPiece(Piece(PieceName.PAWN, 35, Turn.SECOND), 3, 4)
        # self.deployPiece(Piece(PieceName.PAWN, 36, Turn.SECOND), 3, 5)
        # self.deployPiece(Piece(PieceName.PAWN, 37, Turn.SECOND), 3, 6)
        # self.deployPiece(Piece(PieceName.PAWN, 38, Turn.SECOND), 3, 7)
        # self.deployPiece(Piece(PieceName.PAWN, 39, Turn.SECOND), 3, 8)
        # self.deployPiece(Piece(PieceName.PAWN, 40, Turn.SECOND), 3, 9)
    
    def serchPiece(self, piece: Piece) -> Address:
        for i in self.squares:
            if i.piece.equal(piece):
                return i.address
            
    def getSquareWhenMoved(self, piece: Piece, address: Address, length: int, direction: Direction) -> Square:
        dcAddress: Address = copy.deepcopy(address)
        d = [Direction] * MAXIMUM_NUMBER_OF_DIRECTIONS_DUE_TO_POSITIONAL_MOVEMENT
        l = [int] * MAXIMUM_NUMBER_OF_DIRECTIONS_DUE_TO_POSITIONAL_MOVEMENT

        for index, value in enumerate(piece.piece.wayOfMove[direction].adirectionMove):
            if piece.turn == Turn.FIRST:
                d[index] = value.direction
            else:
                d[index] = switchDirection(value.direction)
            l[index] = value.maxLength

        if l[0] > length: l[0] = length

        print(f"l[0]:{l[0]}")
        
        vector = [Address] * 2
        vector[0] = Address.getAddressOfDirection(d[0])
        vector[1] = Address.getAddressOfDirection(d[1])

        dcAddress.row += vector[0].row * l[0] + vector[1].row * l[1]
        dcAddress.column += vector[0].column * l[0] + vector[1].column * l[1]

        for i in self.squares:
            if i.address.equal(dcAddress):
                result = copy.deepcopy(i)
                return result
        
        return Square(Address(0, 0), SquareStatus.NOT_USED, Piece(PieceName.NON, 0, Turn.FIRST))

    def isTwoSteps(self, piece: Piece, address: Address) -> bool:
        if piece.piece.name == PieceName.PAWN:
            for i in self.squares:
                if i.address.column == address.column:
                    if i.piece.piece.name == PieceName.PAWN and i.piece.turn == piece.turn:
                        return True
            return False
        else:
            return False
        
    def getBinary(self) -> str:
        result: str
        for i in self.squares:
            num: int = i.piece.getBinaryFromPiece()
            index: int = (i.address.row - 1) * NUMBER_OF_EDGE + (i.address.column - 1)
            result += printb(11, num)
        return result
    
class Move:
    address: Address
    piece: Piece

    def __init__(self, address: Address, piece: Piece):
        self.address = address
        self.piece = piece

class Condition:
    board: Board
    notOnBoard: List[Piece] = [None] * NUMBER_OF_PIECES
    turn: Turn
    turnNumber: int

    def __init__(self):
        self.board = Board()
        self.turnNumber = 1
        self.turn = Turn.FIRST
        self.initNotOnBoard()

    def initNotOnBoard(self):
        for i in range(NUMBER_OF_PIECES):
            self.notOnBoard[i] = Piece(PieceName.NON, 0, Turn.FIRST)

    def serchPieceFromNotOnBoard(self, piece: Piece) -> int:
        for i in range(NUMBER_OF_PIECES):
            if self.notOnBoard[i].equal(piece):
                return i
            
    def deployPieceNotOnBoard(self, index: int, piece: Piece):
        self.notOnBoard[index] = piece

    def getBinaryFromNotOnBoard(self) -> str:
        result: str
        for i in self.notOnBoard:
            num: int = i.getBinaryFromPiece()
            result += printb(11, num)
        return result

    def getBinaryFromCondition(self) -> str:
        result: str = self.board.getBinary()
        result += self.getBinaryFromNotOnBoard()
        result += printb(9, self.turnNumber)
        result += "1" if self.turn else "0"

    def serchAddMoves(self, pointableHands: List[Move], fromSquare: Square, targetSquare: Square) -> List[Move]:
        if fromSquare.piece.ablePromote(fromSquare.address, targetSquare.address):
            if fromSquare.piece.ableMove(targetSquare.address):
                pointableHands.append(Move(targetSquare.address, fromSquare.piece))
                promotedPiece: Piece = copy.deepcopy(fromSquare.piece)
                promotedPiece.piece.name = getPromotedPieceName(fromSquare.piece.piece.name)
                promotedPiece.piece.promotion = Promotion.NOTPROMOTABLE
                pointableHands.append(Move(targetSquare.address, promotedPiece))
            else:
                promotedPiece: Piece = copy.deepcopy(fromSquare.piece)
                promotedPiece.piece.name = getPromotedPieceName(fromSquare.piece.piece.name)
                promotedPiece.piece.promotion = Promotion.NOTPROMOTABLE
                pointableHands.append(Move(targetSquare.address, promotedPiece))
        else:
            pointableHands.append(Move(targetSquare.address, fromSquare.piece))
        return pointableHands

    def executeAddMoves(self, pointableHands: List[Move], fromSquare: Square, board: Board, direction: Direction, turn: Turn) -> List[Move]:
        wayOfMove: WayOfMove = fromSquare.piece.piece.wayOfMove[direction]
        if wayOfMove.adirectionMove[0].maxLength == NUMBER_OF_EDGE - 1:
            print("unlimited")
            for j in range(1, NUMBER_OF_EDGE):
                targetSquare: Square = board.getSquareWhenMoved(fromSquare.piece, fromSquare.address, j, direction)
                if targetSquare.piece.piece.name != PieceName.NON:
                    if targetSquare.piece.turn != turn:
                        pointableHands = self.serchAddMoves(pointableHands, fromSquare, targetSquare)
                elif targetSquare.address.row == 0 and targetSquare.address.column == 0:
                    return pointableHands
                else:
                    pointableHands = self.serchAddMoves(pointableHands, fromSquare, targetSquare)
        elif wayOfMove.adirectionMove[1].maxLength == 1:
            targetSquare: Square = board.getSquareWhenMoved(fromSquare.piece, fromSquare.address, 1, direction)
            if targetSquare.piece.piece.name != PieceName.NON:
                if targetSquare.piece.turn != turn:
                    pointableHands = self.serchAddMoves(pointableHands, fromSquare, targetSquare)
            elif targetSquare.address.row == 0 and targetSquare.address.column == 0:
                return pointableHands
            else:
                pointableHands = self.serchAddMoves(pointableHands, fromSquare, targetSquare)
        elif wayOfMove.adirectionMove[0].maxLength == 1:
            targetSquare: Square = board.getSquareWhenMoved(fromSquare.piece, fromSquare.address, 1, direction)
            if targetSquare.piece.piece.name != PieceName.NON:
                if targetSquare.piece.turn != turn:
                    pointableHands = self.serchAddMoves(pointableHands, fromSquare, targetSquare)
            elif targetSquare.address.row == 0 and targetSquare.address.column == 0:
                return pointableHands
            else:
                pointableHands = self.serchAddMoves(pointableHands, fromSquare, targetSquare)
        return pointableHands

    def serchPointableHands(self) -> List[Move]:
        print("serch")
        result: List[Move] = []
        copied: Condition = copy.deepcopy(self)
        for i in copied.board.squares:
            piece: Piece = i.piece
            if piece.piece.name != PieceName.NON and piece.turn == copied.turn:
                for d in range(NUMBER_OF_DIRECTIONS):
                    result = self.executeAddMoves(result, i, copied.board, d, copied.turn)
        
        for i in copied.notOnBoard:
            if i.piece.name != PieceName.NON and i.turn == copied.turn:
                for j in range(NUMBER_OF_SQUARES):
                    address: Address = Address(j // 9 + 1, j % 9 + 1)
                    if copied.board.squares[j].piece.piece.name == PieceName.NON:
                        if not i.ableMove(address) or copied.board.isTwoSteps(i, address):
                            continue
                        result.append(Move(address, i))
        return result
    
    def executeMove(self, move: Move) -> Self:
        pieceNon: Piece = Piece(PieceName.NON, 0, Turn.FIRST)
        address: Address = self.board.serchPiece(move.piece)
        if address.row != 0 and address.column != 0:
            self.board.deployPiece(pieceNon, address.row, address.column)
        
        notObBoardIndex: int = self.serchPieceFromNotOnBoard(move.piece)
        if notObBoardIndex != -1:
            self.deployPieceNotOnBoard(notObBoardIndex, pieceNon)
        
        for square in self.board.squares:
            if square.address.equal(move.address):
                if square.piece.piece.name != PieceName.NON:
                    ind: int = self.serchPieceFromNotOnBoard(pieceNon)
                    square.piece.turn = not square.piece.turn
                    square.piece.piece.name = getPieceNameBeforePromote(square.piece.piece.name)
                    square.piece.piece.promotion = getPromotion(square.piece.piece.name)
                    self.deployPieceNotOnBoard(ind, square.piece)
                self.board.deployPiece(move.piece, move.address.row, move.address.column)

    def isEnd(self) -> bool:
        for i in self.notOnBoard:
            if i.piece.name == PieceName.KING:
                return True
        return False

    def whichWin(self) -> Turn:
        if self.isEnd(self):
            for i in self.notOnBoard:
                if i.piece.name == PieceName.KING:
                    return i.turn
                
    def displayCondition(self):
        print("|9|8|7|6|5|4|3|2|1|")
        for i in self.board.squares:
            if i.address.column == 1: print("|", end="")
            if i.piece.piece.name == PieceName.NON:
                print(f"{i.piece.piece.pieceToString()}|", end="")
            elif i.piece.turn == Turn.FIRST:
                print(ConsoleColor.FIRST + f"{i.piece.piece.pieceToString()}" + ConsoleColor.RESET + "|", end="")
            elif i.piece.turn == Turn.SECOND:
                print(ConsoleColor.SECOND + f"{i.piece.piece.pieceToString()}" + ConsoleColor.RESET + "|", end="")
            if i.address.column == NUMBER_OF_EDGE: print(f"{i.address.row}")
        
        print("NotOnBoard:")
        for i in self.notOnBoard:
            if i.piece.name == PieceName.NON: continue
            if i.turn == Turn.FIRST:
                print(ConsoleColor.FIRST + f"{i.piece.piece.pieceToString()}" + ConsoleColor.RESET + ",", end="")
            elif i.piece.turn == Turn.SECOND:
                print(ConsoleColor.SECOND + f"{i.piece.piece.pieceToString()}" + ConsoleColor.RESET + ",", end="")
        print("")