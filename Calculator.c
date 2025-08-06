#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <wchar.h>

#pragma comment(lib, "gdi32.lib")

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void AddControls(HWND);

#define IDC_EDIT_DISPLAY 100
#define IDC_BUTTON_0 101
#define IDC_BUTTON_1 102
#define IDC_BUTTON_2 103
#define IDC_BUTTON_3 104
#define IDC_BUTTON_4 105
#define IDC_BUTTON_5 106
#define IDC_BUTTON_6 107
#define IDC_BUTTON_7 108
#define IDC_BUTTON_8 109
#define IDC_BUTTON_9 110
#define IDC_BUTTON_PLUS 111
#define IDC_BUTTON_MINUS 112
#define IDC_BUTTON_MULTIPLY 113
#define IDC_BUTTON_DIVIDE 114
#define IDC_BUTTON_MODULUS 115
#define IDC_BUTTON_EQUALS 116
#define IDC_BUTTON_CLEAR 117

HFONT hFont;
HBRUSH hBrushBkg, hBrushBtn, hBrushDisplay;
HWND hDisplay;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    COLORREF clrBkg = RGB(32, 32, 32);
    hBrushBkg = CreateSolidBrush(clrBkg);

    WNDCLASSW wc = {0};

    wc.hbrBackground = hBrushBkg;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"MyCalculatorClass";
    wc.lpfnWndProc = WindowProcedure;
    wc.style = 0;

    if (!RegisterClassW(&wc))
    {
        return -1;
    }

    CreateWindowW(L"MyCalculatorClass", L"Calculator", 
                  WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, 
                  100, 100, 275, 400, NULL, NULL, hInst, NULL);

    MSG msg = {0};

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    static BOOL bIsNewCalculation = TRUE;

    switch (msg)
    {
    case WM_CREATE:
        {
            hFont = CreateFontW(26, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

            hBrushBtn = CreateSolidBrush(RGB(50, 50, 50));
            hBrushDisplay = CreateSolidBrush(RGB(32, 32, 32));
            
            AddControls(hWnd);
        }
        break;
    
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORSTATIC:
        {
            HDC hdc = (HDC)wp;
            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkColor(hdc, RGB(32, 32, 32));
            
            if ((HWND)lp == hDisplay) {
                return (INT_PTR)hBrushDisplay;
            }
            return (INT_PTR)hBrushBtn;
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wp))
        {
        case IDC_BUTTON_0:
        case IDC_BUTTON_1:
        case IDC_BUTTON_2:
        case IDC_BUTTON_3:
        case IDC_BUTTON_4:
        case IDC_BUTTON_5:
        case IDC_BUTTON_6:
        case IDC_BUTTON_7:
        case IDC_BUTTON_8:
        case IDC_BUTTON_9:
        {
            wchar_t currentText[256];
            wchar_t buttonText[2];
            GetWindowTextW(GetDlgItem(hWnd, LOWORD(wp)), buttonText, 2);
            GetWindowTextW(hDisplay, currentText, 256);

            if (bIsNewCalculation || wcscmp(currentText, L"0") == 0)
            {
                SetWindowTextW(hDisplay, buttonText);
                bIsNewCalculation = FALSE;
            }
            else
            {
                wcscat(currentText, buttonText);
                SetWindowTextW(hDisplay, currentText);
            }
            break;
        }

        case IDC_BUTTON_PLUS:
        case IDC_BUTTON_MINUS:
        case IDC_BUTTON_MULTIPLY:
        case IDC_BUTTON_DIVIDE:
        case IDC_BUTTON_MODULUS:
        {
            wchar_t currentText[256];
            wchar_t operatorText[4]; 
            wchar_t buttonSymbol[2];

            GetWindowTextW(GetDlgItem(hWnd, LOWORD(wp)), buttonSymbol, 2);
            GetWindowTextW(hDisplay, currentText, 256);

            swprintf(operatorText, L" %s ", buttonSymbol);
            wcscat(currentText, operatorText);
            SetWindowTextW(hDisplay, currentText);
            bIsNewCalculation = FALSE;
            break;
        }

        case IDC_BUTTON_EQUALS:
        {
            wchar_t expression[256];
            GetWindowTextW(hDisplay, expression, 256);

            double num1 = 0.0, num2 = 0.0, result = 0.0;
            wchar_t op = 0;
            BOOL error = FALSE;

            if (swscanf(expression, L"%lf %lc %lf", &num1, &op, &num2) != 3)
            {
                SetWindowTextW(hDisplay, expression);
                bIsNewCalculation = TRUE;
                break;
            }
            
            switch (op)
            {
            case L'+': result = num1 + num2; break;
            case L'-': result = num1 - num2; break;
            case L'*': result = num1 * num2; break;
            case L'/':
                if (num2 == 0) {
                    MessageBoxW(hWnd, L"Cannot divide by zero.", L"Error", MB_OK);
                    error = TRUE;
                } else {
                    result = num1 / num2;
                }
                break;
            case L'%':
                if (num2 == 0) {
                    MessageBoxW(hWnd, L"Cannot divide by zero.", L"Error", MB_OK);
                    error = TRUE;
                } else {
                    result = fmod(num1, num2);
                }
                break;
            }

            if (!error)
            {
                wchar_t resultText[256];
                swprintf(resultText, L"%g", result);
                SetWindowTextW(hDisplay, resultText);
            }
            
            bIsNewCalculation = TRUE;
            break;
        }

        case IDC_BUTTON_CLEAR:
        {
            SetWindowTextW(hDisplay, L"0");
            bIsNewCalculation = TRUE;
            break;
        }
        }
        break;

    case WM_DESTROY:
        DeleteObject(hFont);
        DeleteObject(hBrushBkg);
        DeleteObject(hBrushBtn);
        DeleteObject(hBrushDisplay);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }
    return 0;
}

void AddControls(HWND hWnd)
{
    hDisplay = CreateWindowW(L"Edit", L"0", WS_VISIBLE | WS_CHILD | ES_RIGHT,
                              10, 10, 240, 40, hWnd, (HMENU)IDC_EDIT_DISPLAY, NULL, NULL);
    SendMessage(hDisplay, WM_SETFONT, (WPARAM)hFont, TRUE);

    HWND hBtn; 
    int btnW = 55, btnH = 55, gap = 5;
    int startX = 10, startY = 60;

    hBtn = CreateWindowW(L"Button", L"7", WS_VISIBLE | WS_CHILD, startX, startY, btnW, btnH, hWnd, (HMENU)IDC_BUTTON_7, NULL, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
    hBtn = CreateWindowW(L"Button", L"8", WS_VISIBLE | WS_CHILD, startX + btnW + gap, startY, btnW, btnH, hWnd, (HMENU)IDC_BUTTON_8, NULL, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
    hBtn = CreateWindowW(L"Button", L"9", WS_VISIBLE | WS_CHILD, startX + 2*(btnW + gap), startY, btnW, btnH, hWnd, (HMENU)IDC_BUTTON_9, NULL, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    startY += btnH + gap;
    hBtn = CreateWindowW(L"Button", L"4", WS_VISIBLE | WS_CHILD, startX, startY, btnW, btnH, hWnd, (HMENU)IDC_BUTTON_4, NULL, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
    hBtn = CreateWindowW(L"Button", L"5", WS_VISIBLE | WS_CHILD, startX + btnW + gap, startY, btnW, btnH, hWnd, (HMENU)IDC_BUTTON_5, NULL, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
    hBtn = CreateWindowW(L"Button", L"6", WS_VISIBLE | WS_CHILD, startX + 2*(btnW + gap), startY, btnW, btnH, hWnd, (HMENU)IDC_BUTTON_6, NULL, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);

    startY += btnH + gap;
    hBtn = CreateWindowW(L"Button", L"1", WS_VISIBLE | WS_CHILD, startX, startY, btnW, btnH, hWnd, (HMENU)IDC_BUTTON_1, NULL, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
    hBtn = CreateWindowW(L"Button", L"2", WS_VISIBLE | WS_CHILD, startX + btnW + gap, startY, btnW, btnH, hWnd, (HMENU)IDC_BUTTON_2, NULL, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
    hBtn = CreateWindowW(L"Button", L"3", WS_VISIBLE | WS_CHILD, startX + 2*(btnW + gap), startY, btnW, btnH, hWnd, (HMENU)IDC_BUTTON_3, NULL, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);

    startY += btnH + gap;
    hBtn = CreateWindowW(L"Button", L"0", WS_VISIBLE | WS_CHILD, startX, startY, btnW * 2 + gap, btnH, hWnd, (HMENU)IDC_BUTTON_0, NULL, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    int opX = startX + 3*(btnW + gap);
    startY = 60;
    hBtn = CreateWindowW(L"Button", L"+", WS_VISIBLE | WS_CHILD, opX, startY, btnW, btnH, hWnd, (HMENU)IDC_BUTTON_PLUS, NULL, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    startY += btnH + gap;
    hBtn = CreateWindowW(L"Button", L"-", WS_VISIBLE | WS_CHILD, opX, startY, btnW, btnH, hWnd, (HMENU)IDC_BUTTON_MINUS, NULL, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);

    startY += btnH + gap;
    hBtn = CreateWindowW(L"Button", L"*", WS_VISIBLE | WS_CHILD, opX, startY, btnW, btnH, hWnd, (HMENU)IDC_BUTTON_MULTIPLY, NULL, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    startY += btnH + gap;
    hBtn = CreateWindowW(L"Button", L"/", WS_VISIBLE | WS_CHILD, opX, startY, btnW, btnH, hWnd, (HMENU)IDC_BUTTON_DIVIDE, NULL, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    startY = 240;
    hBtn = CreateWindowW(L"Button", L"=", WS_VISIBLE | WS_CHILD, startX + 2*(btnW + gap), startY, btnW, btnH, hWnd, (HMENU)IDC_BUTTON_EQUALS, NULL, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    startY += btnH + gap;
    
    int halfWidth = btnW * 2 + gap;
    hBtn = CreateWindowW(L"Button", L"%", WS_VISIBLE | WS_CHILD, startX, startY, halfWidth, btnH, hWnd, (HMENU)IDC_BUTTON_MODULUS, NULL, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
    hBtn = CreateWindowW(L"Button", L"C", WS_VISIBLE | WS_CHILD, startX + halfWidth + gap, startY, halfWidth, btnH, hWnd, (HMENU)IDC_BUTTON_CLEAR, NULL, NULL);
    SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
}
