/** Game of life
 * by Dan
 * 27/09/22
 */

#define TB_IMPL
#include "../termbox.h"

/* cell field index */
#define FILLED 1
#define ALIVE 2
#define KILLED 4
#define BIRTHED 8
/* status field index */
#define PAUSED 1
#define SINGLESTP 2
//#define PRINTSTATUS 4
#define NEXT 8
#define EDIT 16
#define QUIT 32

typedef struct gameBoard {
    int width;
    int height;
    char* board;
    uint32_t ch_alive;
    uint32_t ch_dead;
    uint32_t ch_empty;
    uintattr_t bg;
    uintattr_t fg_alive;
    uintattr_t fg_dead;
} gameBoard;

typedef struct gameStatus {
    char status_byte;
    size_t iteration;
    size_t num_alive;
    uintattr_t fg;
    uintattr_t bg;
    int cursx;
    int cursy;
} gameStatus;

int gameBoard_create(gameBoard* board_in);
void gameBoard_destroy(gameBoard* board_in);
void gameBoard_push(const gameBoard* board_in);
void gameBoard_evolve(gameBoard* board_in);
void gameBoard_murbir(gameBoard* board_in);
void gameBoard_settle(gameBoard* board_in);
void gameBoard_populate(gameBoard* board_in, int x, int y, int width, int height);

void gameStatus_print(const gameStatus* status_in);
void print_help(void);

void state_running(gameStatus* status_in, gameBoard* board_in);
void state_edit(gameBoard* board_in);

void cursor_move(gameStatus* status_in, int key);
void cursor_murbir(gameStatus* status_in, gameBoard* board_in);

int count_neighbours(gameBoard* board_in, int x, int y);
size_t count_allAlive(gameBoard* board_in);

int main() {
    struct tb_event ev;
    tb_init();

    gameBoard gol_board = {
            .width = 80, //tb_width(),
            .height = 24, //tb_height(),
            .ch_alive = 0x2588,
            .ch_dead = 0x2591,
            .ch_empty = 0x0020,
            .bg = TB_BLACK,
            .fg_alive = TB_CYAN,
            .fg_dead = TB_BLUE
    };

    gameStatus gol_status = {
            .status_byte = 0, //PAUSED | PRINTSTATUS,
            .iteration = 0,
            .fg = TB_BLACK,
            .bg = TB_BLUE,
            .cursx = 0,
            .cursy = 0
    };


    if (gameBoard_create(&gol_board) < 0) {
        fprintf(stderr, "malloc failed for gameBoard_create\n");
        return EXIT_FAILURE;
    }

    gameBoard_populate(&gol_board, 10, 1, 48, 20);
    gameBoard_push(&gol_board);
    tb_present();

    while ( !(gol_status.status_byte & QUIT) ) {

        if (gol_status.status_byte & PAUSED) {
            tb_print(0, 0, gol_status.fg | TB_BLINK, gol_status.bg, "PAUSED");
        } else if (gol_status.status_byte & SINGLESTP) {
            state_running(&gol_status, &gol_board);
            tb_print(0, 0, gol_status.fg | TB_BLINK, gol_status.bg, "SINGLE STEP");
        } else if (gol_status.status_byte & EDIT) {
            state_edit(&gol_board);
            tb_print(0, 0, gol_status.fg | TB_BLINK, gol_status.bg, "EDIT");
            tb_set_cursor(gol_status.cursx, gol_status.cursy);
        } else {
            state_running(&gol_status, &gol_board);
        }

        gameStatus_print(&gol_status);
        tb_present();

        do {
            int ev_buf;
            gol_status.status_byte &= ~NEXT;

            if (gol_status.status_byte & SINGLESTP) {
                ev_buf = tb_poll_event(&ev);
            } else {
                ev_buf = tb_peek_event(&ev, 100);
                gol_status.status_byte |= NEXT;
            }

            if (ev_buf != TB_ERR_NO_EVENT) {
                if (gol_status.status_byte & EDIT) {
                    cursor_move(&gol_status, ev.key);
                }
                switch (ev.key) {
                    case TB_KEY_CTRL_P:
                        gol_status.status_byte ^= PAUSED;
                        break;
                    case TB_KEY_CTRL_H:
                        print_help();
                        gol_status.status_byte ^= PAUSED;
                        break;
                    case TB_KEY_CTRL_O:
                        gol_status.status_byte ^= SINGLESTP;
                        gol_status.status_byte &= ~PAUSED;
                        break;
                    case TB_KEY_ARROW_UP:
                        gol_status.status_byte |= NEXT;
                        break;
                    case TB_KEY_ARROW_RIGHT:
                        gol_status.status_byte |= NEXT;
                        break;
                    case TB_KEY_ARROW_LEFT:
                        gol_status.status_byte |= NEXT;
                        break;
                    case TB_KEY_ARROW_DOWN:
                        gol_status.status_byte |= NEXT;
                        break;
                    case TB_KEY_CTRL_I:
                        gol_status.status_byte ^=EDIT;
                        gol_status.status_byte &= ~PAUSED;
                        tb_hide_cursor();
                        break;
                    case TB_KEY_CTRL_T:
                        cursor_murbir(&gol_status, &gol_board);
                        gol_status.status_byte |= NEXT;
                        break;
                    default:
                        gol_status.status_byte |= QUIT;
                        gol_status.status_byte |= NEXT;
                }
            }

        } while (!(gol_status.status_byte & NEXT));
    }

    gameBoard_destroy(&gol_board);
    tb_shutdown();

    return EXIT_SUCCESS;
}
/* Allocate memory for the game board */
int gameBoard_create(gameBoard* board_in) {
    board_in->board = calloc(board_in->width * board_in->height, sizeof(char));

    if (!board_in->board) {
        return -1;
    }
    return 0;
}
/* Free memory for the game board */
void gameBoard_destroy(gameBoard* board_in) {
    free(board_in->board);
}
/* Push Alive, Dead, or filled status to display buffer */
void gameBoard_push(const gameBoard* board_in) {
    for (int j = 0; j < board_in->height; j++) {
        char *row_index = &board_in->board[j * board_in->width];
        for (int i = 0; i < board_in->width; i++) {
            if ( !(row_index[i] & FILLED) ) {
                tb_set_cell(i, j, board_in->ch_empty, board_in->fg_alive, board_in->bg);
            } else if (row_index[i] & ALIVE) {
                tb_set_cell(i, j, board_in->ch_alive, board_in->fg_alive, board_in->bg);
            } else {
                tb_set_cell(i, j, board_in->ch_dead, board_in->fg_dead, board_in->bg);
            }
        }
    }
}
/* Evolve cells */
void gameBoard_evolve(gameBoard* board_in) {
    gameBoard_murbir(board_in);
    gameBoard_settle(board_in);
}
/* Murder/Birth cells on the gameboard */
void gameBoard_murbir(gameBoard* board_in) {
    for (int j = 0; j < board_in->height; j++) {
        for (int i = 0; i < board_in->width; i++) {
            int n = count_neighbours(board_in, i, j);
            if (n < 2 || n > 3) {
                board_in->board[i + (j * board_in->width)] |= KILLED;
            } else if (n == 3) {
                board_in->board[i + (j * board_in->width)] |= BIRTHED;
            }
        }
    }
}
/* Clear KILLED/BIRTHED bits after setting ALIVE accordingly */
void gameBoard_settle(gameBoard* board_in) {
    for (int j = 0; j < board_in->height; j++) {
        char *row_index = &board_in->board[j * board_in->width];
        for (int i = 0; i < board_in->width; i++) {
            if (row_index[i] & KILLED) {
                row_index[i] &= ~( ALIVE | KILLED);
            } else if (row_index[i] & BIRTHED) {
                row_index[i] |= ALIVE | FILLED;
                row_index[i] &= ~BIRTHED;
            }
        }
    }
}
/* Fills a box with random alive cells */
void gameBoard_populate(gameBoard* board_in, int x, int y, int width, int height) {
    for (int j = y; j < (y + height); j++) {
        for (int i = x; i < (x + width); i++) {
            if ((rand() % 100) <= 25) {
                board_in->board[i + (j * board_in->width)] = ( FILLED | ALIVE);
            }
        }
    }
}

/* Running State */
void state_running(gameStatus* status_in, gameBoard* board_in) {
    gameBoard_evolve(board_in);
    gameBoard_push(board_in);
    status_in->iteration++;
    status_in->num_alive = count_allAlive(board_in);
}

/* Counts alive neighbours, out of bounds cells are counted as dead */
int count_neighbours(gameBoard* board_in, int x, int y) {
    int n = 0;
    for (int j = (y - 1); j <= (y + 1); j++) {
        if (j >= 0 && j < board_in->height) {

            for (int i = (x - 1); i <= (x + 1); i++) {
                if (i >= 0 && i < board_in->width) {

                    if (board_in->board[i + (j * board_in->width)] & ALIVE) {
                        n++;
                    }
                }
            }
        }
    }
    if (board_in->board[x + (y * board_in->width)] & ALIVE) {
        n--;
    }
    return n;
}
size_t count_allAlive(gameBoard* board_in) {
    size_t n = 0;
    for (int j = 0; j < board_in->height; j++) {
        char *row_index = &board_in->board[j * board_in->width];
        for (int i = 0; i < board_in->width; i++) {
            if (row_index[i] & ALIVE) {
                n++;
            }
        }
    }
    return n;
}


/* Prints the status of the game at the bottom of terminal */
void gameStatus_print(const gameStatus* status_in) {
    size_t prWidth = 0;
    for (int i = 0; i < tb_width(); i++) {
        tb_set_cell(i, tb_height() - 1, ' ', status_in->fg, status_in->bg);
    }
    tb_printf_ex(0, tb_height() - 1, status_in->fg, status_in->bg, &prWidth,
                 "Iteration:%u | Alive:%u | Ctrl-H for help |" , status_in->iteration, status_in->num_alive);
}

void print_help(void) {
    int y = 1;
    tb_print(0, y++, TB_GREEN, TB_BLACK, "Ctrl-P to pause");
    tb_print(0, y++, TB_GREEN, TB_BLACK, "Ctrl-O to single step");
    tb_print(0, y++, TB_GREEN, TB_BLACK, "    Use right arrow key to step");
    tb_print(0, y++, TB_GREEN, TB_BLACK, "Ctrl-I to edit");
    tb_print(0, y++, TB_GREEN, TB_BLACK, "    move cursor with arrow keys");
    tb_print(0, y, TB_GREEN, TB_BLACK, "    Kill or birth a cell using Ctrl-T");
}

/* Moves the cursor around the gameboard */
void cursor_move(gameStatus* status_in, int key) {
    switch (key) {
        case TB_KEY_ARROW_UP:
            if (status_in->cursy != 0) {
                status_in->cursy--;
            }
            break;
        case TB_KEY_ARROW_DOWN:
            if ( status_in->cursy != (tb_height() - 1) ) {
                status_in->cursy++;
            }
            break;
        case TB_KEY_ARROW_LEFT:
            if (status_in->cursx != 0) {
                status_in->cursx--;
            }
            break;
        case TB_KEY_ARROW_RIGHT:
            if ( status_in->cursx != (tb_width() - 1) ) {
                status_in->cursx++;
            }
            break;
        default:
            return;
    }
}
/* murders or births the cell under the cursor */
void cursor_murbir(gameStatus* status_in, gameBoard* board_in) {
    char* cursor = &board_in->board[ status_in->cursx + (status_in->cursy * board_in->width) ];
    if (*cursor & ALIVE) {
        *cursor |= KILLED;
    } else if ( !(*cursor & ALIVE) ) {
        *cursor |= BIRTHED;
    }
}
/* Shows the edited cells */
void state_edit(gameBoard* board_in) {
    gameBoard_settle(board_in);
    gameBoard_push(board_in);
}

