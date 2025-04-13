#include <iostream>
#include<string>
#include <iomanip> // For setw


using namespace std;

const int N = 9;

// Function to print centered text
void printCentered(const string& text, int width) {
    int length = text.length();
    int spaces = (width - length) / 2;
    cout << setw(spaces) << "" << text << endl;
}

// Function to print the Sudoku board centered
void printBoard(int grid[N][N]) {
    int consoleWidth = 80; // Assume console width is 80 characters

    printCentered("<================================================================================>", consoleWidth);
    printCentered("|                        WELCOME TO SUDOKU Game!                                  |", consoleWidth);
    printCentered("|       Fill in the missing numbers (represented by 0) to solve the puzzle.       |", consoleWidth);
    printCentered("<================================================================================>", consoleWidth);

    for (int row = 0; row < N; row++) {
        string rowText;
        for (int col = 0; col < N; col++) {
            if (col == 3 || col == 6)
                rowText += " | ";
            rowText += to_string(grid[row][col]) + " ";
        }
        printCentered(rowText, consoleWidth);
        if (row == 2 || row == 5) {
            printCentered("-----------------------------------", consoleWidth);
        }
    }
}

struct Cell {
    int row, col, numPossibilities;

    Cell(int r = -1, int c = -1, int num = -1) {
        row = r;
        col = c;
        numPossibilities = num;
    }
};

struct PriorityQueue {
    Cell* heap;
    int capacity;
    int size;

    PriorityQueue(int capacity) {
        this->capacity = capacity;
        heap = new Cell[capacity];
        size = 0;
    }

    void push(Cell c) {
        if (size == capacity)
            return;

        int i = size - 1;
        while (i >= 0 && heap[i].numPossibilities > c.numPossibilities) {
            heap[i + 1] = heap[i];
            i--;
        }
        heap[i + 1] = c;
        size++;
    }

    Cell pop() {
        if (size <= 0)
            return Cell(-1, -1, -1); // Return default Cell object

        Cell root = heap[0];
        for (int i = 0; i < size - 1; i++) {
            heap[i] = heap[i + 1];
        }
        size--;
        return root;
    }

    bool empty() {
        return size == 0;
    }
};

bool isSafe(int board[N][N], int row, int col, int num) {
    for (int i = 0; i < N; i++)
        if (board[row][i] == num)
            return false;

    for (int i = 0; i < N; i++)
        if (board[i][col] == num)
            return false;

    int boxRowStart = row - row % 3;
    int boxColStart = col - col % 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i + boxRowStart][j + boxColStart] == num)
                return false;

    return true;
}

bool solveSudoku(int board[N][N]) {
    PriorityQueue pq(N * N);

    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (board[row][col] == 0) {
                int numPossibilities = 0;
                for (int num = 1; num <= 9; num++) {
                    if (isSafe(board, row, col, num)) {
                        numPossibilities++;
                    }
                }
                pq.push(Cell(row, col, numPossibilities));
            }
        }
    }

    while (!pq.empty()) {
        Cell cell = pq.pop();
        int row = cell.row;
        int col = cell.col;

        if (board[row][col] != 0) continue;

        for (int num = 1; num <= 9; num++) {
            if (isSafe(board, row, col, num)) {
                board[row][col] = num;

                if (solveSudoku(board))
                    return true;

                board[row][col] = 0;
            }
        }
        return false;
    }
    return true;
}

bool isSolvedCompletely(int grid[N][N]) {
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
            if (grid[row][col] == 0)
                return false;

    return true;
}

void copyBoard(int src[N][N], int dest[N][N]) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            dest[row][col] = src[row][col];
        }
    }
}

void playGame(int originalBoard[N][N], int solvedBoard[N][N]) {
    int board[N][N];
    copyBoard(originalBoard, board);

    int row, col, num;
    while (true) {
        printBoard(board);
        cout << endl << endl;
        cout << "Unable to solve? Enter -1 as row, col, and num to view the solved sudoku." << endl;
        cout << "Enter row: ";
        cin >> row;
        cout << "Enter column: ";
        cin >> col;
        cout << "Enter number: ";
        cin >> num;

        if (row == -1 || col == -1 || num == -1) {
            solveSudoku(board);
            printBoard(board);
            cout << endl;
            cout << "Better luck next time!!!" << endl;
            return;
        }

        row--;
        col--;

        if (board[row][col] != 0) {
            cout << "This slot is already filled." << endl;
            continue;
        }

        if (solvedBoard[row][col] != num) {
            cout << "Wrong move. Try again." << endl;
            continue;
        }

        if (!isSafe(board, row, col, num)) {
            cout << "Invalid move. Try again." << endl;
            continue;
        }

        board[row][col] = num;

        if (isSolvedCompletely(board)) {
            cout << "Congratulations! You solved the Sudoku." << endl;
            printBoard(board);
            return;
        }
        else {
            cout << "Correct move. Keep going!" << endl;
        }
    }
}

void Level(int originalBoard[N][N]) {
    int board[N][N];
    copyBoard(originalBoard, board);

    int solvedBoard[N][N];
    copyBoard(originalBoard, solvedBoard);
    if (!solveSudoku(solvedBoard)) {
        cout << "No solution found for the provided Sudoku." << endl;
        return;
    }

    int choice;
    do {
        cout << endl << endl;
        cout << "\t\t[1] Solve the Sudoku" << endl;
        cout << "\t\t[2] Unable to solve? View the solved Sudoku" << endl;
        cout << "\t\t[3] Exit" << endl;
        cout << "\t\tEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            playGame(originalBoard, solvedBoard);
            break;
        case 2:
            cout << "Completely Solved Sudoku is: " << endl;
            cout << endl << endl;
            printBoard(solvedBoard);
            cout << endl;
            cout << "Better luck next time!!!" << endl;
            break;
        case 3:
            cout << "Exiting to main menu..." << endl;
            return;
        default:
            cout << "Invalid choice" << endl;
        }
    } while (choice != 3);
}

int main() {
    system("color 3f");
    int board_easy[N][N] = {
        {3, 0, 6, 5, 0, 8, 4, 0, 0},
        {5, 2, 0, 0, 0, 0, 0, 0, 0},
        {0, 8, 7, 0, 0, 0, 0, 3, 1},
        {0, 0, 3, 0, 1, 0, 0, 8, 0},
        {9, 0, 0, 8, 6, 3, 0, 0, 5},
        {0, 5, 0, 0, 9, 0, 6, 0, 0},
        {1, 3, 0, 0, 0, 0, 2, 5, 0},
        {0, 0, 0, 0, 0, 0, 0, 7, 4},
        {0, 0, 5, 2, 0, 6, 3, 0, 0}
    };

    int board_med[N][N] = {
        {8, 0, 0, 0, 0, 7, 2, 0, 5},
        {0, 6, 0, 0, 5, 3, 0, 9, 0},
        {7, 3, 0, 0, 0, 9, 8, 1, 0},
        {9, 0, 0, 1, 8, 0, 0, 2, 0},
        {5, 1, 0, 0, 0, 0, 0, 7, 8},
        {0, 2, 0, 0, 7, 6, 0, 0, 1},
        {0, 7, 9, 3, 0, 0, 0, 5, 4},
        {0, 8, 0, 5, 9, 0, 0, 6, 0},
        {4, 0, 2, 7, 0, 0, 0, 0, 9}
    };

    int board_hard[N][N] = {
        {0, 0, 0, 0, 0, 0, 5, 4, 8},
        {6, 5, 0, 8, 0, 9, 0, 0, 1},
        {0, 8, 3, 2, 4, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 4, 6, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 7, 0, 9, 6, 0, 0, 0, 5},
        {0, 6, 0, 0, 0, 1, 0, 0, 2},
        {9, 2, 0, 0, 0, 0, 7, 8, 0},
        {8, 0, 7, 6, 0, 0, 0, 0, 0}
    };

    printCentered("<================================================================================>", 80);
    printCentered("|                        WELCOME TO SUDOKU Game!                                  |", 80);
    printCentered("|       Fill in the missing numbers (represented by 0) to solve the puzzle.       |", 80);
    printCentered("<================================================================================>", 80);

    char option;
    while (true) {
        cout << endl << endl;
        printCentered("CHOOSE LEVEL", 80);
        printCentered("[a] Easy", 80);
        printCentered("[b] Medium", 80);
        printCentered("[c] Hard", 80);
        printCentered("[d] Exit", 80);
        cout << "\t\tEnter your choice: ";
        cin >> option;

        switch (option) {
        case 'a':
            Level(board_easy);
            break;
        case 'b':
            Level(board_med);
            break;
        case 'c':
            Level(board_hard);
            break;
        case 'd':
            cout << "Exiting the game..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please select a valid option." << endl;
        }
    }

    return 0;
}