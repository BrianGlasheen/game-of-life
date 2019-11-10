#include <iostream>
#include <fstream>
#include <iomanip>

#include <windows.h>
#include <gdiplus.h>

using namespace std;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void makeBoard(string board[20][20]);
int getNeighbors(string board[20][20], int x, int y);
void step(string board[20][20]);
void drawBoard(string board[20][20], HDC hdc);
void drive(HDC hdc);


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    WNDCLASSW wc = {0};

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"myWindowClass";
    wc.lpfnWndProc = WindowProcedure;

    if(!RegisterClassW(&wc))
        return -1;

    CreateWindowW(L"myWindowClass", L"b", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 560, 140, 816, 839, NULL, NULL, NULL, NULL);

    MSG msg = {0};

    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch(msg)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        drive(hdc);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }
    return 0;
}

void drive(HDC hdc)
{
    const int iterations = 125;
    const int sleep = 50; // time in ms between each update

    string board[20][20];
    makeBoard(board);

    for (int i=0; i<iterations; i++)
    {
        step(board);
        drawBoard(board, hdc);
        Sleep(sleep);
    }
}

void makeBoard(string board[20][20])
{
    for (int i=0; i<20; i++)
    {
        for (int j=0; j<20; j++)
        {
            board[i][j] = "-";
        }
    }

    const char* path = "life100.txt";
    std::ifstream inFile(path);

    int x = 0;
    int y = 0;
    while(!inFile.eof())
    {
        inFile >> x;
        inFile >> y;
        board[y][x] = "X";
    }
}

int getNeighbors(string board[20][20], int x, int y)
{
    int c = 0;

    for (int i=-1; i<2; i++)
    {
        for (int j=-1; j<2; j++)
        {
            if (!(i == j && i == 0))
            {
                if ((x + i > -1 && y + j > -1) && (x + i < 20 && y + j < 20))
                {
                    if (board[x+i][y+j] == "X")
                    {
                        c++;
                    }
                }
            }
        }
    }
    return c;
}

void step(string board[20][20])
{
    string copyBoard[20][20];
    for (int i=0; i<20; i++)
    {
        for (int j=0; j<20; j++)
        {
            copyBoard[i][j] = board[i][j];
        }
    }

    for (int i=0; i<20; i++)
    {
        for (int j=0; j<20; j++)
        {
            int neighbors = getNeighbors(copyBoard, i, j);

            if (neighbors < 2 || neighbors > 3)
            {
                board[i][j] = "-";
            }
            else if (neighbors == 3 && copyBoard[i][j] == "-")
            {
                board[i][j] = "X";
            }
            else
            {
                board[i][j] = copyBoard[i][j];
            }
        }
    }
}

void drawBoard(string board[20][20], HDC hdc)
{
    Gdiplus::Graphics gf(hdc);
    Gdiplus::SolidBrush blackBrush(Gdiplus::Color(255, 0, 0, 0));
    Gdiplus::SolidBrush whiteBrush(Gdiplus::Color(255, 255, 255, 255));

    for (int i=0; i<20; i++)
    {
        for (int j=0; j<20; j++)
        {
            if (board[i][j] == "X")
                gf.FillRectangle(&whiteBrush, i*40, j*40, 40, 40);
            else
                gf.FillRectangle(&blackBrush, i*40, j*40, 40, 40);
        }
    }
}
