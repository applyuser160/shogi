import os
from dotenv import load_dotenv
import numpy as np
import pandas as pd
import uuid
import sqlalchemy as sa
from sqlalchemy.engine.url import URL
from typing import List, Self
from sqlalchemy import ForeignKey, insert
from sqlalchemy.schema import Column
from sqlalchemy.orm import *
from sqlalchemy.types import Integer, String
from sqlalchemy.ext.declarative import declarative_base
from sql_setting import session

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