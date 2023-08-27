#include "db_midiate.h"

void setNode(Node *node, MYSQL_ROW mysql_row, unsigned int fields)
{
    initNode(node);
    for (unsigned int col = 0; col < fields; col++)
    {
        switch (col)
        {
        case ID:
            // ポインタを渡すやり方だと、狂うので、値渡しにする　parentIdも同じ
            for (int i = 0; i < 43; i++)
            {
                (*node).id[i] = mysql_row[col][i];
            }
            break;
        case PARENTID:
            if (mysql_row[col] != 0)
            {
                for (int i = 0; i < 43; i++)
                {
                    (*node).parentId[i] = mysql_row[col][i];
                }
            }
            break;
        case TURNNUMBER:
            (*node).turnNumber = atoi(mysql_row[col]);
            break;
        case ROW:
            (*node).move.address.row = atoi(mysql_row[col]);
            break;
        case COLUMN:
            (*node).move.address.column = atoi(mysql_row[col]);
            break;
        case PIECENAME:
            (*node).move.piece.piece.name = atoi(mysql_row[col]);
            break;
        case PIECEID:
            (*node).move.piece.index = atoi(mysql_row[col]);
            break;
        case THROUGHCOUNT:
            (*node).throughCount = atoi(mysql_row[col]);
            break;
        case DRAWCOUNT:
            (*node).drawCount = atoi(mysql_row[col]);
            break;
        case FIRSTWINCOUNT:
            (*node).fiWinCount = atoi(mysql_row[col]);
            break;
        case SECONDWINCOUNT:
            (*node).seWinCount = atoi(mysql_row[col]);
            break;
        case CONDITION:
            if (mysql_row[col] != 0)
            {
                for (int i = 0; i < 168; i++)
                {
                    char c = mysql_row[col][i];
                    if (i == 0)
                    {
                        printb(&((*node).condition[i * 8]), 5, c);
                    }
                    else
                    {
                        printb(&((*node).condition[i * 8 - 3]), 8, c);
                    }
                }
            }
            break;
        }
    }
    (*node).move.piece = generatePiece((*node).move.piece.piece.name, (*node).move.piece.index, (*node).turnNumber % 2 == 1);
}

void setGroupedNode(GroupedNode *gnode, MYSQL_ROW mysql_row, unsigned int fields)
{
    for (unsigned int col = 0; col < fields; col++)
    {
        switch (col)
        {
        case G_PARENTID:
            if (mysql_row[col] != 0)
            {
                (*gnode).parentId = (char*)calloc(43, sizeof(char));
                for (int i = 0; i < 43; i++)
                {
                    (*gnode).parentId[i] = mysql_row[col][i];
                }
            }
            break;
        case G_TURNNUMBER:
            (*gnode).turnNumber = atoi(mysql_row[col]);
            break;
        case G_ROW:
            (*gnode).move.address.row = atoi(mysql_row[col]);
            break;
        case G_COLUMN:
            (*gnode).move.address.column = atoi(mysql_row[col]);
            break;
        case G_PIECENAME:
            (*gnode).move.piece.piece.name = atoi(mysql_row[col]);
            break;
        case G_PIECEID:
            (*gnode).move.piece.index = atoi(mysql_row[col]);
            break;
        case G_COUNT:
            (*gnode).count = atoi(mysql_row[col]);
            break;
        case G_THROUGHCOUNT:
            (*gnode).throughCount = atoi(mysql_row[col]);
            break;
        case G_DRAWCOUNT:
            (*gnode).drawCount = atoi(mysql_row[col]);
            break;
        case G_FIRSTWINCOUNT:
            (*gnode).fiWinCount = atoi(mysql_row[col]);
            break;
        case G_SECONDWINCOUNT:
            (*gnode).seWinCount = atoi(mysql_row[col]);
            break;
        }
    }
}

int base(Node *node, GroupedNode *gnode, char* query, int queryLength, SQL_TYPE type, NODE_TYPE nodeType)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    const char *SERV = E_HOST;
    const char *USER = E_USER;
    const char *PASSWORD = E_PIN;
    const char *DB_NAME = DBNAME;
    const unsigned int PORT = E_PORT;

    conn = mysql_init(NULL); 
    mysql_options(conn, MYSQL_SET_CHARSET_NAME, "cp932"); 
    int ssl_mode = SSL_MODE_DISABLED;
    mysql_options(conn, MYSQL_OPT_SSL_MODE, &ssl_mode); //ssl-mode=DISABLED
 
    if( !mysql_real_connect(conn,SERV,USER,PASSWORD,DB_NAME,PORT,NULL,0)){
        fprintf(stderr, "c%s\r\n", mysql_error(conn)); 
        exit(-1);
    }
 
    if( mysql_query( conn, query) ){
        fprintf(stderr, "e%s\nsql:%s\r\n", mysql_error(conn), query);
        exit(-1);
    }
    res = mysql_store_result(conn);
 
    int resultCount;
    if (type == SELECT)
    {
        unsigned long long rows = mysql_num_rows(res);
        for (int i = 0; i < rows; i++)
        {
            row = mysql_fetch_row(res);
            unsigned int fields = mysql_num_fields(res);
            switch (nodeType)
            {
            case NODE:
                setNode(&(node[i]), row, fields);
                break;
            case GROUPED_NODE:
                setGroupedNode(&(gnode[i]), row, fields);
                break;
            }
        }
        resultCount = rows;
    }

    if(NULL != res){
        mysql_free_result(res);
    }
 
    if(NULL != conn){
        mysql_close(conn);
    }

    memset(query, 0, queryLength * sizeof(char));

    return resultCount;
}

// SELECT
int selectq(Node *node, char query[256])
{
    GroupedNode *g;
    return base(node, g, query, 256, SELECT, NODE);
}

// INSERT
void insert(char query[1600])
{
    Node *node;
    GroupedNode *gnode;
    base(node, gnode, query, 1600, INSERT, NODE);
}

// UPDATE
void update(char query[1600])
{
    Node *node;
    GroupedNode *gnode;
    base(node, gnode, query, 1600, UPDATE, NODE);
}

// DELETE
void delete(char query[1000])
{
    Node *node;
    GroupedNode *gnode;
    base(node, gnode, query, 1000, SQL_DELETE, NODE);
}

// INSERT from Node
char* insertFromNode(Node node)
{
    char values[1600];
    sprintf(values, "('%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, 0b%s)", \
        node.id, node.parentId, node.turnNumber, node.move.address.row, node.move.address.column, node.move.piece.piece.name, node.move.piece.index, \
        node.throughCount, node.drawCount, node.fiWinCount, node.seWinCount, node.condition);
    char query[1600];
    sprintf(query, "insert into %s.%s(ID, parentID, turnNumber, `row`, `column`, pieceName, pieceID, throughCount, drawCount, firstWinCount, secondWinCount, `condition`)values %s;\0",\
        DBNAME, TABLENAME, values);
    insert(query);
    memset(query, 0, 1600 * sizeof(char));
    memset(values, 0, 1600 * sizeof(char));
    return node.id;
}

// MULTI INSERT
void bulkinsert(Node *node, int count)
{
    char* values = (char*)calloc(1600 * count, sizeof(char));
    for (int i = 0; i < count; i++)
    {
        char* value = (char*)calloc(1600, sizeof(char));
        sprintf(value, "('%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, 0b%s)", \
            node[i].id, node[i].parentId, node[i].turnNumber, node[i].move.address.row, node[i].move.address.column, node[i].move.piece.piece.name, node[i].move.piece.index, \
            node[i].throughCount, node[i].drawCount, node[i].fiWinCount, node[i].seWinCount, node[i].condition);
        if (i > 0)strcat(values, ",");
        strcat(values, value);
        memset(value, 0, 1600 * sizeof(char));
        free(value);
    }
    char* query = (char*)calloc(1600 * (count + 1), sizeof(char));
    sprintf(query, "insert into %s.%s(ID, parentID, turnNumber, `row`, `column`, pieceName, pieceID, throughCount, drawCount, firstWinCount, secondWinCount, `condition`)values %s;\0", \
        DBNAME, TABLENAME, values);
    Node *n;
    GroupedNode *g;
    base(n, g, query, 1600 * (count + 1), INSERT, NODE);
    memset(query, 0, 1600 * (count + 1) * sizeof(char));
    memset(values, 0, 1600 * count * sizeof(char));
    free(values);
    free(query);
}

// UPDATE from Node
void updateFromNode(Node node)
{
    char query[1600];
    memset(query, 0, 1600 * sizeof(char));
    sprintf(query, "update %s.%s set \
        turnNumber = %d, `row` = %d, `column` = %d, pieceName = %d, pieceID = %d, \
        throughCount = %d, drawCount = %d, firstWinCount = %d, secondWinCount = %d, `condition` = 0b%s where ID = '%s';\0",\
        DBNAME, TABLENAME, node.turnNumber, node.move.address.row, node.move.address.column, node.move.piece.piece.name, node.move.piece.index, \
        node.throughCount, node.drawCount, node.fiWinCount, node.seWinCount, node.condition, node.id);
    Node *n;
    GroupedNode *g;
    base(n, g, query, 1600, UPDATE, NODE);
    memset(query, 0, 1600 * sizeof(char));
}

// SELECT where ID
int selectWhereID(Node *node, char ID[43])
{
    char query[256];
    memset(query, 0, 256 * sizeof(char));
    sprintf(query, "select * from %s.%s where ID = '%s';\0", DBNAME, TABLENAME, ID);
    int result = selectq(node, query);
    memset(query, 0, 256 * sizeof(char));
    return result;
}

// SELECT where parentID
int selectWhereParentID(char parentID[43], Node *node)
{
    char query[256];
    memset(query, 0, 256 * sizeof(char));
    sprintf(query, "select * from %s.%s where parentID = '%s';\0", DBNAME, TABLENAME, parentID);
    int result = selectq(node, query);
    memset(query, 0, 256 * sizeof(char));
    return result;
}

// SELECT duplicated
int selectDuplicated(GroupedNode *gnode, Node *node, int *count)
{
    char query[800];
    memset(query, 0, 800 * sizeof(char));
    sprintf(query, "select a.* from (select parentID, turnNumber, `row`, \
        `column`, pieceName, pieceID, count(*) as count, sum(throughCount) as sumThroughCount, \
        sum(drawCount) as sumDdawCount, sum(firstWinCount) as sumFirstWinCount, \
        sum(secondWinCount) as sumSecondWinCount from %s.%s group by parentID, \
        turnNumber, `row`, `column`, pieceName, pieceID order by turnNumber asc) as a \
        where a.count > 1 order by a.turnNumber limit 100;", DBNAME, TABLENAME);
    // printf("query: %s\n", query);
    Node *n;
    int result = base(n, gnode, query, 800, SELECT, GROUPED_NODE);
    memset(query, 0, 800 * sizeof(char));
    sprintf(query, "select b.* from %s.%s as b join (\
        select a.* from (select parentID, turnNumber, `row`, `column`, \
        pieceName, pieceID, count(*) as count, sum(throughCount) as sumThroughCount, \
        sum(drawCount) as sumDdawCount, sum(firstWinCount) as sumFirstWinCount, \
        sum(secondWinCount) as sumSecondWinCount from %s.%s group by parentID, \
        turnNumber, `row`, `column`, pieceName, pieceID order by turnNumber asc\
        ) as a where a.count > 1 order by a.turnNumber limit 100) as a \
        on a.parentID = b.parentID and a.turnNumber = b.turnNumber \
        and a.`row` = b.`row` and a.`column` = b.`column` and a.pieceName = b.pieceName \
        and a.pieceID = b.pieceID order by b.turnNumber;", DBNAME, TABLENAME, DBNAME, TABLENAME);
    // printf("query: %s\n", query);
    GroupedNode *g;
    *count = base(node, g, query, 800, SELECT, NODE);
    memset(query, 0, 800 * sizeof(char));
    return result;
}

// DELETE duplicated
void deleteDuplicated(Node *node, int count)
{
    char* values = (char*)calloc(256 * count, sizeof(char));
    for (int i = 0; i < count; i++)
    {
        char* value = (char*)calloc(256, sizeof(char));
        sprintf(value, "ID = '%s'", node[i].id);
        if (i > 0)strcat(values, " or ");
        strcat(values, value);
        memset(value, 0, 256 * sizeof(char));
        free(value);
    }
    char* query = (char*)calloc(256 * (count + 1), sizeof(char));
    sprintf(query, "delete from %s.%s where %s;\0", \
        DBNAME, TABLENAME, values);
    Node *n;
    GroupedNode *g;
    // printf("delete duplicated: %s\n", query);
    base(n, g, query, 256 * (count + 1), SQL_DELETE, NODE);
    memset(query, 0, 256 * (count + 1) * sizeof(char));
    memset(values, 0, 256 * count * sizeof(char));
    free(values);
    free(query);
}

// UPDATE parentId
void updateParentId(Node set, Node *where, int count)
{
    char* values = (char*)calloc(256 * count, sizeof(char));
    for (int i = 0; i < count; i++)
    {
        char* value = (char*)calloc(256, sizeof(char));
        sprintf(value, "parentID = '%s'", where[i].id);
        if (i > 0)strcat(values, " or ");
        strcat(values, value);
        memset(value, 0, 256 * sizeof(char));
        free(value);
    }
    char* query = (char*)calloc(256 * (count + 1), sizeof(char));
    sprintf(query, "update %s.%s set parentID = '%s' where %s;\0", \
        DBNAME, TABLENAME, set.id, values);
    Node *n;
    GroupedNode *g;
    // printf("update pid: %s\n", query);
    base(n, g, query, 256 * (count + 1), UPDATE, NODE);
    memset(query, 0, 256 * (count + 1) * sizeof(char));
    memset(values, 0, 256 * count * sizeof(char));
    free(values);
    free(query);
}