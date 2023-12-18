#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

typedef struct TCell {
    struct TCell * m_Right;
    struct TCell * m_Down;
    int m_Row;
    int m_Col;
    int m_Data;
} TCELL;


typedef struct TRowCol {
    struct TRowCol * m_Next;
    TCELL * m_Cells;
    int m_Idx;
} TROWCOL;


typedef struct TSparseMatrix {
    TROWCOL * m_Rows;
    TROWCOL * m_Cols;
} TSPARSEMATRIX;
#endif /* __PROGTEST__ */

#define nullptr NULL

void initMatrix(TSPARSEMATRIX * m) {
    m->m_Cols = nullptr;
    m->m_Rows = nullptr;
}


void addSetCell(TSPARSEMATRIX * m, int rowIdx, int colIdx, int data) {
    if (!m->m_Cols) {
        m->m_Cols = (TROWCOL*)calloc(1, sizeof(TROWCOL));
        m->m_Rows = (TROWCOL*)calloc(1, sizeof(TROWCOL));
        m->m_Cols->m_Idx = colIdx;
        m->m_Rows->m_Idx = rowIdx;
    }

    TROWCOL * row_ptr;
    TROWCOL * col_ptr;

    for (col_ptr = m->m_Cols; col_ptr->m_Next && col_ptr->m_Next->m_Idx <= colIdx; col_ptr = col_ptr->m_Next);
    for (row_ptr = m->m_Rows; row_ptr->m_Next && row_ptr->m_Next->m_Idx <= rowIdx; row_ptr = row_ptr->m_Next);

    if (col_ptr->m_Idx != colIdx) {
        TROWCOL * new__ = (TROWCOL *)calloc(1, sizeof(TROWCOL));
        new__->m_Idx = colIdx;

        if (colIdx < m->m_Cols->m_Idx) {
            new__->m_Next = m->m_Cols;
            m->m_Cols = new__;
            col_ptr = m->m_Cols;
        } else {
            if (col_ptr->m_Next) {
                new__->m_Next = col_ptr->m_Next;
            }

            col_ptr->m_Next = new__;
            col_ptr = col_ptr->m_Next;
        }
    }

    if (row_ptr->m_Idx != rowIdx) {
        TROWCOL * new__ = (TROWCOL*)calloc(1, sizeof(TROWCOL));
        new__->m_Idx = rowIdx;

        if (rowIdx < m->m_Rows->m_Idx) {
            new__->m_Next = m->m_Rows;
            m->m_Rows = new__;
            row_ptr = m->m_Rows;
        } else {
            if (row_ptr->m_Next) {
                new__->m_Next = row_ptr->m_Next;
            }

            row_ptr->m_Next = new__;
            row_ptr = row_ptr->m_Next;
        }
    }

    TCELL * row_cell = row_ptr->m_Cells;
    TCELL * col_cell = col_ptr->m_Cells;

    while (row_cell && row_cell->m_Right && row_cell->m_Right->m_Col <= col_ptr->m_Idx) {
        row_cell = row_cell->m_Right;
    }

    while (col_cell && col_cell->m_Down && col_cell->m_Down->m_Row <= row_ptr->m_Idx) {
        col_cell = col_cell->m_Down;
    }

    {
        TCELL * new__ = (TCELL*)calloc(1, sizeof(TCELL));
        new__->m_Data = data;
        new__->m_Col = colIdx;
        new__->m_Row = rowIdx;

        if (!row_cell) {
            row_ptr->m_Cells = new__;
        } else {
            if (colIdx < row_ptr->m_Cells->m_Col) {
                new__->m_Right = row_ptr->m_Cells;
                row_ptr->m_Cells = new__;
            } else {
                new__->m_Right = row_cell->m_Right;
                row_cell->m_Right = new__;
            }
        }

        if (!col_cell) {
            col_ptr->m_Cells = new__;
        } else {
            if (rowIdx < col_ptr->m_Cells->m_Row) {
                new__->m_Down = col_ptr->m_Cells;
                col_ptr->m_Cells = new__;
            } else {
                new__->m_Down = col_cell->m_Down;
                col_cell->m_Down = new__;
            }
        }
    }
}


bool removeCell(TSPARSEMATRIX * m, int rowIdx, int colIdx) {
    if (!m->m_Cols) {
        return false;
    }

    TROWCOL * col_ptr = m->m_Cols;
    TROWCOL * row_ptr = m->m_Rows;
    TROWCOL * prev_col_ptr = NULL;
    TROWCOL * prev_row_ptr = NULL;

    while (col_ptr) {
        if (col_ptr->m_Idx == colIdx) {
            break;
        }

        prev_col_ptr = col_ptr;
        col_ptr = col_ptr->m_Next;
    }
    if (!col_ptr) {
        return false;
    }


    while (row_ptr) {
        if (row_ptr->m_Idx == rowIdx) {
            break;
        }

        prev_row_ptr = row_ptr;
        row_ptr = row_ptr->m_Next;
    }
    if (!row_ptr) {
        return false;
    }

    TCELL * col_cell = col_ptr->m_Cells;
    TCELL * row_cell = row_ptr->m_Cells;
    TCELL * prev_col_cell = NULL;
    TCELL * prev_row_cell = NULL;

    while (col_cell) {
        if (col_cell->m_Row == rowIdx) {
            break;
        }

        prev_col_cell = col_cell;
        col_cell = col_cell->m_Down;
    }
    if (!col_cell) {
        return false;
    }

    while (row_cell) {
        if (row_cell->m_Col == colIdx) {
            break;
        }

        prev_row_cell = row_cell;
        row_cell = row_cell->m_Right;
    }
    if (!row_cell) {
        return false;
    }

    TCELL * __delete = row_cell;

    if (!prev_row_cell) {
        row_ptr->m_Cells = __delete->m_Right;
    } else {
        prev_row_cell->m_Right = __delete->m_Right;
    }

    if (!prev_col_cell) {
        col_ptr->m_Cells = __delete->m_Down;
    } else {
        prev_col_cell->m_Down = __delete->m_Down;
    }

    free(__delete);

    if (!row_ptr->m_Cells) {
        if (!prev_row_ptr) {
            m->m_Rows = row_ptr->m_Next;
        } else {
            prev_row_ptr->m_Next = row_ptr->m_Next;
        }

        free(row_ptr);
    }

    if (!col_ptr->m_Cells) {
        if (!prev_col_ptr) {
            m->m_Cols = col_ptr->m_Next;
        } else {
            prev_col_ptr->m_Next = col_ptr->m_Next;
        }

        free(col_ptr);
    }

    return true;
}


void freeMatrix(TSPARSEMATRIX * m) {
    TROWCOL * __temp = m->m_Cols;
    while (__temp) {
        TROWCOL * next = __temp->m_Next;
        free(__temp);
        __temp = next;
    }

    TROWCOL * temp = m->m_Rows;

    while (temp) {
        TROWCOL * next = temp->m_Next;

        TCELL * cell_temp = temp->m_Cells;

        while (cell_temp) {
            TCELL * cell_next = cell_temp->m_Right;
            free(cell_temp);
            cell_temp = cell_next;
        }

        free(temp);
        temp = next;
    }
}


#ifndef __PROGTEST__
int main(int argc, char * argv []) {
    TSPARSEMATRIX  m;
    initMatrix(&m);
    addSetCell(&m, 0, 1, 10);
    addSetCell(&m, 1, 0, 20);
    addSetCell(&m, 1, 5, 30);
    addSetCell(&m, 2, 1, 40);

    /*addSetCell(&m, 3, 3, 40);
    addSetCell(&m, 3, 4, 40);
    addSetCell(&m, 2, 6, 40);
    addSetCell(&m, 7, 1, 40);
    addSetCell(&m, 5, 10, 40);
    addSetCell(&m, 8, 7, 40);
    addSetCell(&m, 2, 10, 40);
    addSetCell(&m, 7, 3, 40);
    addSetCell(&m, 7, 4, 40);
    addSetCell(&m, 10, 3, 40);
    addSetCell(&m, 5, 3, 33);
    addSetCell(&m, 2, 3, 66);
    addSetCell(&m, 0, 0, 1);
    addSetCell(&m, 0, 3, 8);*/
    assert ( m . m_Rows
             && m . m_Rows -> m_Idx == 0
             && m . m_Rows -> m_Cells
             && m . m_Rows -> m_Cells -> m_Row == 0
             && m . m_Rows -> m_Cells -> m_Col == 1
             && m . m_Rows -> m_Cells -> m_Data == 10
             && m . m_Rows -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next
             && m . m_Rows -> m_Next -> m_Idx == 1
             && m . m_Rows -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Cells -> m_Row == 1
             && m . m_Rows -> m_Next -> m_Cells -> m_Col == 0
             && m . m_Rows -> m_Next -> m_Cells -> m_Data == 20
             && m . m_Rows -> m_Next -> m_Cells -> m_Right
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Row == 1
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Col == 5
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Data == 30
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next
             && m . m_Rows -> m_Next -> m_Next -> m_Idx == 2
             && m . m_Rows -> m_Next -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Row == 2
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Data == 40
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Cols
             && m . m_Cols -> m_Idx == 0
             && m . m_Cols -> m_Cells
             && m . m_Cols -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Cells -> m_Col == 0
             && m . m_Cols -> m_Cells -> m_Data == 20
             && m . m_Cols -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next
             && m . m_Cols -> m_Next -> m_Idx == 1
             && m . m_Cols -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Cells -> m_Row == 0
             && m . m_Cols -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Cols -> m_Next -> m_Cells -> m_Data == 10
             && m . m_Cols -> m_Next -> m_Cells -> m_Down
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Row == 2
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Col == 1
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Data == 40
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next
             && m . m_Cols -> m_Next -> m_Next -> m_Idx == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Col == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Data == 30
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Rows -> m_Cells == m . m_Cols -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Cells == m . m_Cols -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Cells -> m_Right == m . m_Cols -> m_Next -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Cells -> m_Down );
    addSetCell ( &m, 230, 190, 50 );
    assert ( m . m_Rows
             && m . m_Rows -> m_Idx == 0
             && m . m_Rows -> m_Cells
             && m . m_Rows -> m_Cells -> m_Row == 0
             && m . m_Rows -> m_Cells -> m_Col == 1
             && m . m_Rows -> m_Cells -> m_Data == 10
             && m . m_Rows -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next
             && m . m_Rows -> m_Next -> m_Idx == 1
             && m . m_Rows -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Cells -> m_Row == 1
             && m . m_Rows -> m_Next -> m_Cells -> m_Col == 0
             && m . m_Rows -> m_Next -> m_Cells -> m_Data == 20
             && m . m_Rows -> m_Next -> m_Cells -> m_Right
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Row == 1
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Col == 5
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Data == 30
             && m . m_Rows -> m_Next -> m_Cells -> m_Right -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next
             && m . m_Rows -> m_Next -> m_Next -> m_Idx == 2
             && m . m_Rows -> m_Next -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Row == 2
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Data == 40
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Next
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Idx == 230
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Row == 230
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Col == 190
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Data == 50
             && m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Cols
             && m . m_Cols -> m_Idx == 0
             && m . m_Cols -> m_Cells
             && m . m_Cols -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Cells -> m_Col == 0
             && m . m_Cols -> m_Cells -> m_Data == 20
             && m . m_Cols -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next
             && m . m_Cols -> m_Next -> m_Idx == 1
             && m . m_Cols -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Cells -> m_Row == 0
             && m . m_Cols -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Cols -> m_Next -> m_Cells -> m_Data == 10
             && m . m_Cols -> m_Next -> m_Cells -> m_Down
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Row == 2
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Col == 1
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Data == 40
             && m . m_Cols -> m_Next -> m_Cells -> m_Down -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next
             && m . m_Cols -> m_Next -> m_Next -> m_Idx == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Col == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Data == 30
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next -> m_Next
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Idx == 190
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Row == 230
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Col == 190
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Data == 50
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Rows -> m_Cells == m . m_Cols -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Cells == m . m_Cols -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Cells -> m_Right == m . m_Cols -> m_Next -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Cells -> m_Down );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells );
    assert ( removeCell ( &m, 0, 1 ) );
    assert ( !removeCell ( &m, 0, 1 ) );
    assert ( !removeCell ( &m, 1, 2 ) );
    assert ( m . m_Rows
             && m . m_Rows -> m_Idx == 1
             && m . m_Rows -> m_Cells
             && m . m_Rows -> m_Cells -> m_Row == 1
             && m . m_Rows -> m_Cells -> m_Col == 0
             && m . m_Rows -> m_Cells -> m_Data == 20
             && m . m_Rows -> m_Cells -> m_Right
             && m . m_Rows -> m_Cells -> m_Right -> m_Row == 1
             && m . m_Rows -> m_Cells -> m_Right -> m_Col == 5
             && m . m_Rows -> m_Cells -> m_Right -> m_Data == 30
             && m . m_Rows -> m_Cells -> m_Right -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next
             && m . m_Rows -> m_Next -> m_Idx == 2
             && m . m_Rows -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Cells -> m_Row == 2
             && m . m_Rows -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Rows -> m_Next -> m_Cells -> m_Data == 40
             && m . m_Rows -> m_Next -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next
             && m . m_Rows -> m_Next -> m_Next -> m_Idx == 230
             && m . m_Rows -> m_Next -> m_Next -> m_Cells
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Row == 230
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Col == 190
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Data == 50
             && m . m_Rows -> m_Next -> m_Next -> m_Cells -> m_Right == nullptr );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Cols
             && m . m_Cols -> m_Idx == 0
             && m . m_Cols -> m_Cells
             && m . m_Cols -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Cells -> m_Col == 0
             && m . m_Cols -> m_Cells -> m_Data == 20
             && m . m_Cols -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next
             && m . m_Cols -> m_Next -> m_Idx == 1
             && m . m_Cols -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Cells -> m_Row == 2
             && m . m_Cols -> m_Next -> m_Cells -> m_Col == 1
             && m . m_Cols -> m_Next -> m_Cells -> m_Data == 40
             && m . m_Cols -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next
             && m . m_Cols -> m_Next -> m_Next -> m_Idx == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Row == 1
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Col == 5
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Data == 30
             && m . m_Cols -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next -> m_Next
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Idx == 190
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Row == 230
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Col == 190
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Data == 50
             && m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells -> m_Down == nullptr );
    assert ( m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
    assert ( m . m_Rows -> m_Cells == m . m_Cols -> m_Cells );
    assert ( m . m_Rows -> m_Cells -> m_Right == m . m_Cols -> m_Next -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Cells );
    assert ( m . m_Rows -> m_Next -> m_Next -> m_Cells == m . m_Cols -> m_Next -> m_Next -> m_Next -> m_Cells );
    freeMatrix ( &m );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
