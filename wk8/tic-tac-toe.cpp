#include <bits/stdc++.h>

#define X 1
#define O -1
#define EMPTY 0


#define INVALID_MOVE -1
#define INF 100000000
#define Three 1000000
#define Two 1000
#define One 100

struct board {
    int cell[3][3];

    void show() {
        for (int i = 0 ; i < 3; ++i) {
            for (int j = 0 ; j < 3; ++j) {
                if (cell[i][j] == X) {
                    printf("X ");
                } else if (cell[i][j] == O) {
                    printf("O ");
                } else {
                    printf(". ");
                }
            }
            printf("\n");
        }
    }

    int eval() {
        return linescore(cell[0][0], cell[0][1], cell[0][2]) +
               linescore(cell[1][0], cell[1][1], cell[1][2]) +
               linescore(cell[2][0], cell[2][1], cell[2][2]) + 
               linescore(cell[0][0], cell[1][0], cell[2][0]) +
               linescore(cell[0][1], cell[1][1], cell[2][1]) +
               linescore(cell[0][2], cell[1][2], cell[2][2]) +
               linescore(cell[0][0], cell[1][1], cell[2][2]) +
               linescore(cell[0][2], cell[1][1], cell[2][0]); 
    }

    int linescore(int pos1, int pos2, int pos3) {
        if (pos1 == X && pos2 == X && pos3 == X) return Three;
        
        if (pos1 == O && pos2 == O && pos3 == O) return -Three;

        if (pos1 == X && pos2 == X && pos3 == EMPTY) return Two;
        if (pos1 == X && pos2 == EMPTY && pos3 == X) return Two; 
        if (pos1 == EMPTY && pos2 == X && pos3 == X) return Two;
        
        if (pos1 == O && pos2 == O && pos3 == EMPTY) return -Two;
        if (pos1 == O && pos2 == EMPTY && pos3 == O) return -Two; 
        if (pos1 == EMPTY && pos2 == O && pos3 == O) return -Two;

        if (pos1 == EMPTY && pos2 == X && pos3 == EMPTY) return One;
        if (pos1 == X && pos2 == EMPTY && pos3 == EMPTY) return One; 
        if (pos1 == EMPTY && pos2 == EMPTY && pos3 == X) return One;
        
        if (pos1 == EMPTY && pos2 == O && pos3 == EMPTY) return -One;
        if (pos1 == O && pos2 == EMPTY && pos3 == EMPTY) return -One; 
        if (pos1 == EMPTY && pos2 == EMPTY && pos3 == O) return -One;
        
        return 0;
    }

    bool xwin() {
        for (int i = 0 ; i < 3; ++i) {
            if (cell[i][0] == X && cell[i][1] == X && cell[i][2] == X) return true;
            if (cell[0][i] == X && cell[1][i] == X && cell[2][i] == X) return true;
        }
        
        if (cell[0][2] == X && cell[1][1] == X && cell[2][0] == X) return true;
        if (cell[0][0] == X && cell[1][1] == X && cell[2][2] == X) return true;
        return false;
    }

    bool owin() {
        for (int i = 0 ; i < 3; ++i) {
            if (cell[i][0] == O && cell[i][1] == O && cell[i][2] == O) return true;
            if (cell[0][i] == O && cell[1][i] == O && cell[2][i] == O) return true;
        }
        
        if (cell[0][2] == O && cell[1][1] == O && cell[2][0] == O) return true;
        if (cell[0][0] == O && cell[1][1] == O && cell[2][2] == O) return true;
        return false;
    }

    bool full() {
        for (int i = 0 ; i < 3; ++i) {
            for (int j = 0 ; j < 3; ++j) {
                if (cell[i][j] == EMPTY) return false;
            }
        }

        return true;
    }

    bool terminal() {
        return xwin() || owin() || full();
    }
};

// return the heuristic score and the move
std::pair<int, int> alphabeta(board &state, int alpha, int beta, int turn, int depth) {
    if (state.terminal() || depth == 0) return std::make_pair(state.eval(), INVALID_MOVE);
    int move = INVALID_MOVE;
    if (turn == X) {
        for (int i = 0 ; i < 9; ++i) {
            if (state.cell[i/3][i%3] == EMPTY) {
                state.cell[i/3][i%3] = X;
                std::pair<int, int> nxt = alphabeta(state, alpha, beta, -turn, depth - 1);
                state.cell[i/3][i%3] = EMPTY;
                if (alpha < nxt.first) {
                    move = i;
                    alpha = nxt.first;
                }

                if (alpha >= beta) {
                    return std::make_pair(alpha, move);
                }
            }
        }

        return std::make_pair(alpha, move);
    } else {
        for (int i = 0 ; i < 9; ++i) {
            if (state.cell[i/3][i%3] == EMPTY) {
                state.cell[i/3][i%3] = O;
                std::pair<int, int> nxt = alphabeta(state, alpha, beta, -turn, depth - 1);
                state.cell[i/3][i%3] = EMPTY;
                if (beta > nxt.first) {
                    move = i;
                    beta = nxt.first;
                }

                if (alpha >= beta) return std::make_pair(beta, move);
            }
        }

        return std::make_pair(beta, move);
    }
}

board state;

int main(int argc, char *argv[]) {
    int move, turn = X;
    if (argc != 3 || atoi(argv[2]) <= 2) {
        printf("Usage ./tic-tac-toe [x|o] [depth >= 3]\n");
        return 0;
    }       

    if (argv[1][0] == 'x') {
        state.show();
        printf("Please input a move [1-9]: ");
        scanf("%d", &move);
        if (move < 1 || move > 9) {
            printf("Invalid move!\n");
            return 0;
        }
        move--;
        state.cell[move/3][move%3] = X;
        turn = O;
    }

    while (!state.terminal()) {
        auto nxt = alphabeta(state, -INF, INF, turn, atoi(argv[2]));
        state.cell[nxt.second / 3][nxt.second % 3] = turn;
        turn *= -1;
        if (state.terminal()) break;
        state.show();
        printf("Please input a move [1-9]: ");
        scanf("%d", &move);
        if (move < 1 || move > 9) {
            printf("Invalid move!\n");
            return 0;
        }
        move--;
        if (state.cell[move/3][move%3] != EMPTY) {
            printf("Invalid move, cell occupied!\n");
            return 0;
        }
        state.cell[move/3][move%3] = turn;
        turn *= -1;
    }

    state.show();
    printf("\n");
    if (state.xwin()) {
        printf("X win!\n");
    } else if (state.owin()) {
        printf("O win!\n");
    } else {
        printf("Draw!\n");
    }
    return 0;
}