#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>
using namespace std;

/**defining utilities for the game **/

// for maximizer
#define u1 10
// for minimizer
#define u2 -10
// for draw
#define u3 0

struct Move {
    int row;
    int col;
};

char player, opponent;
int max_depth;

//Evaluation Function for Early Termination of Minimax Algorithm
int eval_func(vector<vector<char> >& b)
{
    int M=0,O=0;
    int horizontal_max = 0, vertical_max = 0, diagonal_max = 0;
    int horizontal_min = 0, vertical_min = 0, diagonal_min = 0;

    /////////Max///////////////
    for (int i = 0; i < 3; i++) 
    {
        int c = 0;
        for (int j = 0; j < 3; j++) 
        {
            if (b[i][j] == player || b[i][j] == '.')
                c++;
        }
        if (c == 3)
        horizontal_max++;
    }
    for (int i = 0; i < 3; i++) 
    {
        int c = 0;
        for (int j = 0; j < 3; j++) 
        {
            if (b[j][i] == player || b[j][i] == '.')
                c++;
        }
        if (c == 3)
        vertical_max++;
    }

    int c = 0;
    for (int i = 0; i < 3; i++) 
    {
        if (b[i][i] == player || b[i][i] == '.')
            c++;
    }
    diagonal_max += c / 3;
    c = 0;
    for (int i = 0; i < 3; i++) 
    {
        if (b[2 - i][i] == player || b[2 - i][i] == '.')
            c++;
    }
    diagonal_max += c / 3;

    M = horizontal_max + vertical_max + diagonal_max;

    /////////////MIN////////////////

    for (int i = 0; i < 3; i++) 
    {
        int c = 0;
        for (int j = 0; j < 3; j++) 
        {
            if (b[i][j] == opponent || b[i][j] == '.')
                c++;
        }
        if (c == 3)
            horizontal_min++;
    }
    for (int i = 0; i < 3; i++) 
    {
        int c = 0;
        for (int j = 0; j < 3; j++) 
        {
            if (b[j][i] == opponent || b[j][i] == '.')
                c++;
        }
        if (c == 3)
            vertical_min++;
    }

    int c1 = 0;
    for (int i = 0; i < 3; i++) 
    {
        if (b[i][i] == opponent || b[i][i] == '.')
            c1++;
    }
    diagonal_min += c1 / 3;
    c1 = 0;
    for (int i = 0; i < 3; i++) 
    {
        if (b[2 - i][i] == opponent || b[2 - i][i] == '.')
            c1++;
    }
    diagonal_min += c1 / 3;

    O = horizontal_min + vertical_min + diagonal_min;

    int E=M-O;
    return E;
}

// This Function checks number if any moves are left in game
bool checkMoves(vector<vector<char> >& board)
{
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++) 
        {
            if (board[i][j] == '-')
                return true;
        }
    }
    return false;
}

//Function to check the winner
int checkwinner(vector<vector<char> >& board)
{
    int winner = 0;

    // Check victory for x or o along the row
    for (int r = 0; r < 3; r++) {
        if (board[r][0] == board[r][1] && board[r][1] == board[r][2]) {
            if (board[r][0] == player)
                winner = u1;
            else if (board[r][0] == opponent)
                winner = u2;
        }
    }

    // Check victory for x or o along the column
    for (int c = 0; c < 3; c++) {
        if (board[0][c] == board[1][c] && board[1][c] == board[2][c]) {
            if (board[0][c] == player)
                winner = u1;

            else if (board[0][c] == opponent)
                winner = u2;
        }
    }

    // Check victory for x or o along the left diagonal
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == player)
            winner = u1;
        else if (board[0][0] == opponent)
            winner = u2;
    }
    // Check victory for x or o along the right diagonal
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == player)
            winner = u1;
        else if (board[0][2] == opponent)
            winner = u2;
    }

    // Else if none of them have won then return 0
    int openSpots = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == '-') {
                openSpots++;
            }
        }
    }

    if (winner == 0 && openSpots == 0) {
        return u3;
    }
    else {
        return winner;
    }
}

// Function implementing Minimax Algorithm along with Alpha beta pruning
int func_minimax_abp(vector<vector<char> >& board, int depth, bool isMax, int alpha, int beta)
{
    // To ensure early termination of the minimax function
    if (depth == 0) {
        int SCORE = eval_func(board);
        return SCORE;
    }

    // Check the winning score for current state of board
    int score = checkwinner(board);

    // If the game is already won by maximizer
    // return its score
    if (score == u1 || depth == max_depth) {
        //heuristic value of node
        return score - depth;
    }

    // If the game is already won by minimizer
    // return its score
    if (score == u2 || depth == max_depth) {
        //heuristic value of node
        return score + depth;
    }

    // If no moves are left and ther is no winner then
    // it is a draw
    if (checkMoves(board) == false)
        return 0;

    int bestvalue;
    char p;
    Move bestmove;
    if (isMax) {
        bestvalue = max_depth*(-1);
        p = player;
    }
    else {
        bestvalue = max_depth;
        p = opponent;
    }

    int flag = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == '-') {
                board[i][j] = p;
                if (isMax) {
                    // recursive function for next depth and next player
                    alpha = func_minimax_abp(board, depth + 1, !isMax, alpha, beta);

                    // remove test move from actual board.
                    board[i][j] = '-';

                    // update best value and move
                    if (bestvalue < alpha) {
                        bestvalue = alpha;
                        bestmove.row = i;
                        bestmove.col = j;
                    }

                    // Ignore the game subtrees which don't need to be explored
                    if (alpha >= beta) {
                        flag = 1;
                        break;
                    }
                }
                else {
                    // recursive function for next depth and next player
                    beta = func_minimax_abp(board, depth + 1, !isMax, alpha, beta);

                    // remove test move from actual board.
                    board[i][j] = '-';

                    // update best value and move
                    if (bestvalue > beta) {
                        bestvalue = beta;
                        bestmove.row = i;
                        bestmove.col = j;
                    }

                    // Ignore the game subtrees which don't need to be explored
                    if (alpha >= beta) {
                        flag = 1;
                        break;
                    }
                }
            }
        }
        if (flag == 1)
            break;
    }

    if ((isMax && bestvalue == max_depth*(-1)) || (!isMax && bestvalue == max_depth)) {
        if (depth != 0)
            return 0;
        else
            return bestvalue;
    }

    return bestvalue;
}

// helper game function
Move playgame(vector<vector<char> >& board)
{
    int bestVal = max_depth*(-1);
    Move bestMove;
    bestMove.row = -1;
    bestMove.col = -1;

    // Check the winning score for current state of board
    int score = checkwinner(board);

    // If the game is already finished return the best move
    if (score == u2 || score == u1 || checkMoves(board) == false) {
        return bestMove;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // Check if cell is empty
            if (board[i][j] == '-') {
                // put the player in the given position
                board[i][j] = player;

                // evaluate the move using minimax algorithm and alpha beta pruning
                int moveVal = func_minimax_abp(board, 0, false, -1000, 1000);

                // remove the player from the given position
                board[i][j] = '-';

                // The bestVal is updated if value of current move is greater
                if (moveVal > bestVal) {
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    //cout<<"The value of the best Move is :"<<bestVal<<endl;
    return bestMove;
}

// Function to play tic tac toe game
void TicTacToe(vector<vector<char> >& board, int ply)
{

    // set value of ply to max_depth
    max_depth = ply;

    // To check which players turn it is
    int d = 0, xn = 0, on = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == '-')
                d++;
            else if (board[i][j] == 'x')
                xn++;
            else if (board[i][j] == 'o')
                on++;
        }
    }
    if (d == 0)
        return;

    if (xn > on) {
        player = 'o';
        opponent = 'x';
    }
    else {
        player = 'x';
        opponent = 'o';
    }

    Move bestMove = playgame(board);

    // No Moves Possible for any Player
    if (bestMove.row == -1 && bestMove.col == -1)
        return;

    board[bestMove.row][bestMove.col] = player;

    // print the final state of tic tac toe board
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << board[i][j];
        }
    }
}

int main()
{
    // Input the current state of tic tac toe board
    string state;
    cin >> state;

    // convert the state into a 2d matrix board
    vector<vector<char> > board(3, vector<char>(3, '-'));
    int k = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = state[k++];
        }
    }

    //Input an positive integer which contains total maximum number of moves of both players to look ahead
    int ply;
    cin >> ply;

    TicTacToe(board, ply);

    return 0;
}
