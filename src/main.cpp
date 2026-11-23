#include <Windows.h>
#include <stdio.h>

#define IDC_INCREMENT 1001
#define IDC_DECREMENT 1002
#define IDC_RESET 1003

int counter = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CTLCOLORSTATIC: // override background to always be transparent, not the standard light grey background.
	{
		HDC hdc = (HDC)wParam;

		SetBkMode(hdc, TRANSPARENT);

		return (INT_PTR)GetStockObject(NULL_BRUSH);
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		wchar_t buffer[32]; // create the integer buffer
		
		// This forces the static text to become an integer.
		wsprintf(buffer, L"%d", counter);

		// This code block updates the counter UI upon receiving btn click messages.
		TextOut(
			hdc,
			50,
			100,
			buffer,
			lstrlen(buffer)
		);

		EndPaint(hWnd, &ps);
		return 0;
	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_INCREMENT:
		{
			counter++; // increase counter by 1
			InvalidateRect(hWnd, NULL, TRUE); // update UI with new value
			break; // exit the INNER switch
		}

		case IDC_DECREMENT:
		{
			counter--; // decrease counter by 1
			InvalidateRect(hWnd, NULL, TRUE); // update UI with new value
			break; // exit the INNER switch
		}
		
		case IDC_RESET:
		{
			counter = 0; // reset counter to 0
			InvalidateRect(hWnd, NULL, TRUE); // update UI with new value
			break; // exit the INNER switch
		}
		}

		return 0;
	}

	case WM_DESTROY:
		PostQuitMessage(0); // destroy the window.
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam); // only runs when all other cases return 0 or FALSE.
		break;
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASS wcex = { 0 };
	wcex.lpfnWndProc = WndProc;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hInstance = hInst;
	wcex.lpszClassName = L"AppWindowClass";

	RegisterClass(&wcex);

	// this creates the window.
	HWND hWnd = CreateWindow(
		L"AppWindowClass",
		L"Basic BOBO Counter",
		WS_OVERLAPPEDWINDOW,
		100, 100, 400, 400,
		NULL, NULL, hInst, NULL
	);

	// the following code creates all the UI elements.
	CreateWindow(
		L"STATIC", L"Basic BOBO Counter",
		WS_CHILD | WS_VISIBLE,
		50, 50, 150, 20,
		hWnd, NULL, hInst, NULL
	);

	CreateWindow(
		L"STATIC", L"Increase:",
		WS_CHILD | WS_VISIBLE,
		50, 150, 150, 20,
		hWnd, NULL, hInst, NULL
	);

	CreateWindow(
		L"STATIC", L"Decrease:",
		WS_CHILD | WS_VISIBLE,
		50, 250, 150, 20,
		hWnd, NULL, hInst, NULL
	);

	CreateWindow(
		L"BUTTON", L"+",
		WS_CHILD | WS_VISIBLE,
		120, 150, 40, 20,
		hWnd, (HMENU)IDC_INCREMENT, hInst, NULL
	);

	CreateWindow(
		L"BUTTON", L"-",
		WS_CHILD | WS_VISIBLE,
		120, 250, 40, 20,
		hWnd, (HMENU)IDC_DECREMENT, hInst, NULL
	);

	CreateWindow(
		L"BUTTON", L"Reset",
		WS_CHILD | WS_VISIBLE,
		50, 300, 100, 20,
		hWnd, (HMENU)IDC_RESET, hInst, NULL
	);

	// shows and updates the window.
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// the msg logic to send the UI events to the WndProc to be processed.
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0; // return desired value.
}
