import os
import sys
from datetime import datetime
from dotenv import load_dotenv
import cshogi
from cshogi import CSA, Board
import random
import numpy as np
import pandas as pd
import uuid
import sqlalchemy as sa
from sqlalchemy.engine.url import URL
from typing import List, Self
from sqlalchemy import ForeignKey, insert, func, select, text, update
from sqlalchemy.schema import Column
from sqlalchemy.orm import *
from sqlalchemy.types import Integer, String
from sqlalchemy.ext.declarative import declarative_base
from sql_setting import *
import shogi_learn as sl
from enum import IntEnum
import glob
import shutil

class Winner(IntEnum):
    DRAW = 1
    FIRST = 2
    SECOND = 3

class Base(DeclarativeBase):
    pass

class Node(Base):
    __tablename__ = "newnode"
    id = Column('ID', String(100), primary_key=True)
    parentId = Column('parentID', String(100))
    turnNumber = Column('turnNumber', Integer)
    move = Column('move', Integer)
    board = Column('board', String(150))
    throughCount = Column('throughCount', Integer)
    drawCount = Column('drawCount', Integer)
    firstWinCount = Column('firstWinCount', Integer)
    secondWinCount = Column('secondWinCount', Integer)

    def __init__(self, parent: str, turnNumber: int, move: int, board: str, through: int, draw: int, first: int, second: int):
        self.id = str(uuid.uuid4())
        self.parentId = parent
        self.turnNumber = turnNumber
        self.move = move
        self.board = board
        self.throughCount = through
        self.drawCount = draw
        self.firstWinCount = first
        self.secondWinCount = second

    def full_name(self) -> str:
        return f"id[{self.id},pid[{self.parentId}],tn[{self.turnNumber}],tc[{self.throughCount}],dc[{self.drawCount}],fc[{self.firstWinCount}],sc[{self.secondWinCount}]"

def bulkinsert(nodes: List[Node]):
    session.bulk_save_objects(nodes)
    session.commit()

def selectCount():
    return session.query(Node).count()

def fight() -> Winner:
    winner: Winner
    shogi_board = cshogi.Board()
    nodes: List[Node] = []
    for i in range(500):
        moves = list(shogi_board.legal_moves)
        x = random.randint(0, len(moves) - 1)
        shogi_board.push(moves[x])
        parentId = ''
        if i != 0:
            parentId = nodes[i - 1].id
        else:
            parentId = '7fc22d27-9439-4f05-bbd7-444be019c788'
        node = Node(
            parentId, 
            shogi_board.move_number,
            moves[x],
            shogi_board.sfen(),
            1, 0, 0, 0
        )
        nodes.append(node)

        if shogi_board.is_game_over():
            winner = Winner.FIRST if not shogi_board.turn else Winner.SECOND
            for j in nodes:
                if not shogi_board.turn:
                    j.firstWinCount += 1
                else:
                    j.secondWinCount += 1
            break
        elif shogi_board.move_number == 500:
            winner = Winner.DRAW
            for j in nodes:
                j.drawCount += 1
            break
    bulkinsert(nodes)
    return winner

def loop(laps: int):
    results = [0, 0, 0]
    start = datetime.now()
    for i in range(laps):
        result = fight()
        results[result - 1] += 1
        now = datetime.now()
        lap = (now - start) / (i + 1)
        eta = now + (laps - (i + 1)) * lap
        print(f"\rdraw:{results[0]:0>5},"\
            f"first:{results[1]:0>5},"\
            f"second:{results[2]:0>5},"\
            f"total:{(results[0]+results[1]+results[2]):0>5},"\
            f"eta:{eta}", 
            end="")

def getQueryNodeGroup():
    subquery = session.query(
        Node.parentId,
        Node.turnNumber,
        Node.move,
        Node.board,
        func.count().label("count"),
        func.sum(Node.throughCount).label("sumThroughCount"),
        func.sum(Node.drawCount).label("sumDrawCount"),
        func.sum(Node.firstWinCount).label("sumFirstWinCount"),
        func.sum(Node.secondWinCount).label("sumSecondWinCount")
    ).where(Node.turnNumber < 10).group_by(Node.parentId, Node.turnNumber, Node.move, Node.board).subquery("sub")
    return select(subquery).where(subquery.c.count > 1).limit(1000)

def getNodeGroup():
    query = getQueryNodeGroup()
    return list(session.execute(query))

def getQueryGroupedNode():
    group = getQueryNodeGroup().subquery("a")
    query = select(Node).join(
        group,
        (Node.parentId == group.c.parentId) &
        (Node.turnNumber == group.c.turnNumber) &
        (Node.move == group.c.move) &
        (Node.board == group.c.board)
    ).order_by(Node.turnNumber)
    return query

def getGroupedNode():
    query = getQueryGroupedNode()
    return session.query(Node).from_statement(query).all()

def getQueryGroupedNodeChild():
    d = getQueryGroupedNode().subquery("d")
    query = select(Node).join(
        d,
        Node.parentId == d.c.ID
    )
    return query

def getGroupedNodeChild():
    query = getQueryGroupedNodeChild()
    return session.query(Node).from_statement(query).all()

def merge():
    grouped = getNodeGroup()
    print(f"get grouped {len(grouped)} records")
    if len(grouped) == 0:
        print("grouped non")
        return
    deleteList = getGroupedNode()
    print(f"get deleteList {len(deleteList)} records")
    updateList = getGroupedNodeChild()
    print(f"get updateList {len(updateList)} records")

    insertList: List[Node] = []
    print("generate insertList")
    for ind, ele in enumerate(grouped):
        node = Node(
            ele.parentId,
            ele.turnNumber,
            ele.move,
            ele.board,
            ele.sumThroughCount,
            ele.sumDrawCount,
            ele.sumFirstWinCount,
            ele.sumSecondWinCount
        )
        insertList.append(node)
        print(f"\r{((ind + 1) / len(grouped)):.2%}", end="")
    print("")

    print("update updateList")
    for ind, ele in enumerate(deleteList):
        newID = ''
        for node in insertList:
            if node.turnNumber == ele.turnNumber\
                and node.move == ele.move\
                and node.board == ele.board:
                newID = node.id
        for node in updateList:
            if ele.id == node.parentId:
                node.parentId = newID
        print(f"\r{((ind + 1) / len(deleteList)):.2%}", end="")
    print("")

    print("delete execute")
    for ind, ele in enumerate(deleteList):
        session.delete(ele)
        print(f"\r{((ind + 1) / len(deleteList)):.2%}", end="")
    print("")
    print("update execute")
    session.bulk_save_objects(updateList)
    print("insert execute")
    session.bulk_save_objects(insertList)
    print("commit execute")
    session.commit()
    print("finished")

def updateTopNode():
    topnode = session.query(Node).where(Node.turnNumber == 1).one()
    secondnode = session.query(
        func.sum(Node.throughCount).label("sumThroughCount"),
        func.sum(Node.drawCount).label("sumDrawCount"),
        func.sum(Node.firstWinCount).label("sumFirstWinCount"),
        func.sum(Node.secondWinCount).label("sumSecondWinCount")
    ).where(Node.turnNumber == 2).one()
    topnode.throughCount = secondnode.sumThroughCount
    topnode.drawCount = secondnode.sumDrawCount
    topnode.firstWinCount = secondnode.sumFirstWinCount
    topnode.secondWinCount = secondnode.sumSecondWinCount
    session.commit()

def serchNotInputFileName() -> List[str]:
    return glob.glob('data/notInput/*.csa')

def moveInputedFiles():
    fromDirectory = 'data/notInput/'
    toDirectory = 'data/inputed/'
    files = os.listdir(fromDirectory)
    for g in files:
        shutil.move(fromDirectory + g, toDirectory)

def parse(path: str):
    parser = CSA.Parser.parse_file(path)[0]
    moves = parser.moves
    winner: Winner
    if parser.win == 0:
        winner = Winner.FIRST
    elif parser.win == 1:
        winner = Winner.SECOND
    else:
        winner = Winner.DRAW
    shogi_board = cshogi.Board()
    nodes: List[Node] = []
    for i, v in enumerate(moves):
        shogi_board.push(v)
        parentId = ''
        if i != 0:
            parentId = nodes[i - 1].id
        else:
            parentId = '7fc22d27-9439-4f05-bbd7-444be019c788'
        node = Node(
            parentId, 
            shogi_board.move_number,
            v,
            shogi_board.sfen(),
            1, 
            1 if winner == Winner.DRAW else 0, 
            1 if winner == Winner.FIRST else 0, 
            1 if winner == Winner.SECOND else 0
        )
        nodes.append(node)
    bulkinsert(nodes)

def parseAll(dirPath: str = 'data/notInput/', usedDirPath: str = 'data/inputed/'):
    notInputFileNames = glob.glob(dirPath + '*.csa')
    count = len(notInputFileNames)

    parseStart = datetime.now()
    print(f"insert to db {count} records")
    for i, file in enumerate(notInputFileNames):
        parse(file)
        now = datetime.now()
        lap = (now - parseStart) / (i + 1)
        eta = now + (count - (i + 1)) * lap
        print(f"\r{((i + 1) / count):.2%}, eta: {eta}", end="")
    print("")
    
    moveStart = datetime.now()
    print("move csa file")
    moveFiles = os.listdir(dirPath)
    for i, g in enumerate(moveFiles):
        shutil.move(dirPath + g, usedDirPath)
        now = datetime.now()
        lap = (now - moveStart) / (i + 1)
        eta = now + (count - (i + 1)) * lap
        print(f"\r{((i + 1) / count):.2%}, eta: {eta}", end="")
    print("")