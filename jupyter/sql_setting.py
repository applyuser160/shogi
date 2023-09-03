import os
from dotenv import load_dotenv
import numpy as np
import pandas as pd
import sqlalchemy as sa
from sqlalchemy.engine.url import URL
from typing import List, Self
from sqlalchemy.schema import Column
from sqlalchemy.orm import *
from sqlalchemy.types import Integer, String
from sqlalchemy.ext.declarative import declarative_base

load_dotenv('../.env')
url = URL.create(
    drivername="mysql",
    username=os.getenv('USER'),
    password=os.getenv('PIN'),
    host=os.getenv('HOST'),
    port=int(os.getenv('PORT')),
    database=os.getenv('DB'),
    query={"charset": "utf8"}
)
ENGINE = sa.create_engine(url, echo=False)
session = scoped_session(
    sessionmaker(
        autocommit=False, 
        autoflush=False, 
        bind=ENGINE
    )
)