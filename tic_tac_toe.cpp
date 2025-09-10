#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Function to display the game board
void displayBoard(const vector<vector<string>>& board) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << board[i][j];
            if (j < 2) {
                cout << " | ";
            }
        }
        cout << endl;
        if (i < 2) {
            cout << "---------" << endl;
        }
    }
}

// Function to check for a win
bool checkWin(const vector<vector<string>>& board, string player) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
            return true; // Horizontal win
        }
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) {
            return true; // Vertical win
        }
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
        return true; // Diagonal win (top-left to bottom-right)
    }
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
        return true; // Diagonal win (top-right to bottom-left)
    }
    return false;
}

// Function to check for a draw
bool checkDraw(const vector<vector<string>>& board) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != "X" && board[i][j] != "O") {
                return false; // Empty cell found, the game is not a draw
            }
        }
    }
    return true; // All cells are filled, and there's no winner, so it's a draw
}

int main() {
    vector<vector<string>> board(3, vector<string>(3, " ")); // Initialize the 3x3 game board with empty cells
    string currentPlayer = "X";
    bool gameOver = false;

    cout << "Welcome to Tic-Tac-Toe!" << endl;

    while (!gameOver) {
        cout << "Current board:" << endl;
        displayBoard(board);

        int row, col;
        cout << "Player " << currentPlayer << ", enter your move (row and column, e.g., 1 2): ";
        cin >> row >> col;

        if (row < 1 || row > 3 || col < 1 || col > 3 || board[row - 1][col - 1] != " ") {
            cout << "Invalid move. Try again." << endl;
        } else {
            board[row - 1][col - 1] = currentPlayer;

            if (checkWin(board, currentPlayer)) {
                cout << "Player " << currentPlayer << " wins!" << endl;
                gameOver = true;
            } else if (checkDraw(board)) {
                cout << "It's a draw!" << endl;
                gameOver = true;
            }

            // Switch players
            currentPlayer = (currentPlayer == "X") ? "O" : "X";
        }
    }

    cout << "Game over. Do you want to play again? (yes or no): ";
    string playAgain;
    cin >> playAgain;

    if (playAgain == "yes") {
        // Reset the game board and continue playing
        board = vector<vector<string>>(3, vector<string>(3, " "));
        currentPlayer = "X";
        gameOver = false;
    }

    return 0;
}
