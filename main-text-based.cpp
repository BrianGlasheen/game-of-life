#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

void makeBoard(string board[20][20]) {
    for (int i=0; i<20; i++) {
        for (int j=0; j<20; j++) {
            board[i][j] = "-";
        }
    }

    const char* path = "life100.txt";
    std::ifstream inFile(path);

    int x = 0;
    int y = 0;
    while(!inFile.eof()) {
        inFile >> x;
        inFile >> y;
        board[x][y] = "X";
    }
}

int getNeighbors(string board[20][20], int x, int y) {
    int c = 0;

    for (int i=-1; i<2; i++) {
        for (int j=-1; j<2; j++) {
            if (!(i == j && i == 0)) {
                if ((x + i > -1 && y + j > -1) && (x + i < 20 && y + j < 20)) {
                    if (board[x+i][y+j] == "X") {
                        c++;
                    }
                }
            }
        }
    }
    return c;
}

void step(string board[20][20]) {
    string copyBoard[20][20];
    for (int i=0; i<20; i++) {
        for (int j=0; j<20; j++) {
            copyBoard[i][j] = board[i][j];
        }
    }

    for (int i=0; i<20; i++) {
        for (int j=0; j<20; j++) {
            int neighbors = getNeighbors(copyBoard, i, j);

            if (neighbors < 2 || neighbors > 3) {
                board[i][j] = "-";
            } else if (neighbors == 3 && copyBoard[i][j] == "-") {
                board[i][j] = "X";
            } else {
                board[i][j] = copyBoard[i][j];
            }
        }
    }
}

void printBoard(string board[20][20]) {
    for (int i=0; i<20; i++) {
        for (int j=0; j<20; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\n" << endl;
}

int main() {
    string board[20][20];
    makeBoard(board);
    cout << "Iteration: 0" << endl;
    printBoard(board);
    for (int i=0; i<5; i++) {
        cout << "Iteration: " << i+1 << endl;
        step(board);
        printBoard(board);
    }
    return 0;
}
