#ifndef DB_MIDIATE_H
#define DB_MIDIATE_H

#include "mysql-8.1.0-winx64/include/mysql.h"
#include "node.h"

#define DBNAME "shogi"
#define TABLENAME "node"
#define E_HOST "127.0.0.1"
#define E_PORT 3306
#define E_USER "root"
#define E_PIN "root"

typedef enum SQL_TYPE
{
    SELECT = 1,
    INSERT = 2,
    UPDATE = 3,
    SQL_DELETE = 4
} SQL_TYPE;

typedef enum NODE_COLUMN_NO
{
    ID = 0,
    PARENTID = 1,
    TURNNUMBER = 2,
    ROW = 3,
    COLUMN = 4,
    PIECENAME = 5,
    PIECEID = 6,
    THROUGHCOUNT = 7,
    DRAWCOUNT = 8,
    FIRSTWINCOUNT = 9,
    SECONDWINCOUNT = 10,
    CONDITION = 11,
} NODE_COLUMN_NO;

typedef enum GROUOED_NODE_COLUMN_NO
{
    G_PARENTID = 0,
    G_TURNNUMBER = 1,
    G_ROW = 2,
    G_COLUMN = 3,
    G_PIECENAME = 4,
    G_PIECEID = 5,
    G_COUNT = 6,
    G_THROUGHCOUNT = 7,
    G_DRAWCOUNT = 8,
    G_FIRSTWINCOUNT = 9,
    G_SECONDWINCOUNT = 10
} GROUOED_NODE_COLUMN_NO;

void setNode(Node *node, MYSQL_ROW mysql_row, unsigned int fields);
void setGroupedNode(GroupedNode *gnode, MYSQL_ROW mysql_row, unsigned int fields);
int base(Node *node, GroupedNode *gnode, char* query, int queryLength, SQL_TYPE type, NODE_TYPE nodeType);

// SELECT
int selectq(Node *node, char query[256]);

// INSERT
void insert(char query[256]);

// UPDATE
void update(char query[256]);

// DELETE
void delete(char query[1000]);

// INSERT from Node
char* insertFromNode(Node node);

// MULTI INSERT
void bulkinsert(Node *node, int count);

// UPDATE from Node
void updateFromNode(Node node);

// SELECT where ID
int selectWhereID(Node *node, char ID[43]);

// SELECT where parentID
int selectWhereParentID(char parentID[43], Node *node);

// SELECT duplicated
int selectDuplicated(GroupedNode *gnode, Node *node, int *count);

// DELETE duplicated
void deleteDuplicated(Node *node, int count);

// UPDATE parentId
void updateParentId(Node set, Node *where, int count);

#endif