// Screen Saver Created by Ed J, for Gomi
// 9/25/2002
//

#include <windows.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN

static char pClassName[] = "GomiSSaver";
static HINSTANCE ghInst = 0;
WNDCLASSEX wce;
int ScreenW,ScreenH;
COLORREF color;
HDC dc;
HWND whwnd;
int IDT_TIMER = 7;
int timerflg = 0;
int MouseX = -7, MouseY = -7, TopY = 0, TopX = 0;
int savermode = 0;


void SystemParamSS(int);
void Lightining(void);
char * cright(char [], unsigned int);
void strcpymf(char *, char *);
void timeout(long);


LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_MOUSEMOVE://picks up any mouse movement and exits the program
		{
			if (savermode == 0)
			{
				POINT p;
				p.x = LOWORD(lParam);
				p.y = HIWORD(lParam);
				if (MouseX == -7 && MouseY == -7)
				{//first time in, so just set the mouse position
					MouseX = p.x;
					MouseY = p.y;
				}
				else if(p.x != MouseX || p.y != MouseY) //if the mouse has moved, exit
					SendMessage(whwnd,WM_CLOSE,0,0);
			}
		}
		break;
		case WM_KEYDOWN://picks up all keyboard activity and exits the program
			if (savermode == 0)
			{
				SendMessage(whwnd,WM_CLOSE,0,0);
			}
		break;
		case WM_TIMER: //runs the lightining function ever few seconds
			if ((int)wParam == IDT_TIMER && timerflg == 0)
			{
				timerflg = 1;
				Lightining();
				KillTimer(whwnd,IDT_TIMER);
				SetTimer(whwnd, IDT_TIMER, (rand()%4000) + 50,(TIMERPROC) NULL); //randomize the timer!
				timerflg = 0;
			}
		break;
		case WM_CLOSE://Shutdown application when WM_CLOSE is recieved
			savermode = 1;//prevents any reentry problems
			KillTimer(whwnd,IDT_TIMER);
			ReleaseDC(NULL,dc);
			if (savermode == 0)
				SystemParamSS(0);
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char *coml = GetCommandLine(); //grab the Command line string
	MSG msg;

	//MessageBox(NULL,coml,"CommandLine",0); //displays the command line for you

	if (strstr(coml," /p") != NULL)
	{	
		//ScreenSaver miniture view mode
		// RECT rc;
		// char tmp[1000];
		// strcpy(tmp,strstr(coml," /p"));
		// strcpymf(tmp,cright(tmp,4));
		// whwnd=(HWND)atoi(tmp);
		// GetWindowRect(whwnd,&rc);
		// ScreenW=rc.right-rc.left;
		// ScreenH=rc.bottom-rc.top;
		// TopX = 0;
		// TopY = 0;
		// savermode = 1;
		//return(0);//comment this line to allow the miniture view mode to work
	}
	else if (strstr(coml," /c") != NULL)
	{
		//ScreenSaver Property mode
		savermode = 2;
		MessageBox(NULL,"Screen Saver Created by Ed J, for Gomi!", "Properties",0);
		return(0);
	}
	else
	{
		savermode = 0;
		ScreenW = GetSystemMetrics(SM_CXSCREEN);//get the screen width
		ScreenH = GetSystemMetrics(SM_CYSCREEN);//height
		SetCursorPos(ScreenW,0);
		//ScreenSaver Normal mode
	}
	
	color = RGB(255,255,0);//pick the color for the lightining
	ghInst = hInstance;//globalize dah programs instance
		
	wce.cbSize = sizeof(WNDCLASSEX);//setup the window's class
	wce.style = 0;
	wce.lpfnWndProc = WndProc;
	wce.cbClsExtra = 0;
	wce.cbWndExtra = 0;
	wce.hInstance = ghInst;
	wce.hIcon = LoadIcon(0, IDI_WINLOGO);
	wce.hCursor = LoadCursor(0, IDC_ARROW);
	wce.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wce.lpszMenuName = NULL;
	wce.lpszClassName = pClassName;
	wce.hIconSm = LoadIcon(0, IDI_WINLOGO);

	if(!RegisterClassEx(&wce))// Create the windows class, if it can't exit the program
		return(0);

	//create the window, which is use to capture mouse and keyboard data mostly
	srand((unsigned int)clock());// seed the rand function
	if (savermode == 0)
	{
		whwnd = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, pClassName, "", WS_POPUP ,
				0, 0 , ScreenW, ScreenH, 0, 0 , ghInst, 0);	
		SystemParamSS(1);//disable the CTRL+ALT+DEL and ALT+TAB
	}
	else
	{
		whwnd = CreateWindowEx(0, pClassName, "", WS_CHILD ,
				TopX, TopY , ScreenW, ScreenH, whwnd, 0 , ghInst, 0);
		dc = GetDC(whwnd);
	}
	ShowWindow(whwnd, nCmdShow);//Show the created window
	UpdateWindow(whwnd);//update it
	dc = GetDC(whwnd);//capture the Window device context

	SetTimer(whwnd, IDT_TIMER, (rand()%4000) + 50,(TIMERPROC) NULL); // turn the lightining timer on
	
	while(GetMessage(&msg, 0, 0, 0)) // Grab all the window messages
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return(0);
}

//this function disables the CTRL+ALT+DEL and ALT+TAB key combinations while the screen saver is running
void SystemParamSS(int i)
{
    SystemParametersInfo(SPI_SCREENSAVERRUNNING, i, 0, 0);
}

char * cright(char s[], unsigned int n)
{
	char *c = (char *) malloc(strlen(s)+1);
	//static char c[charlimit];
	if (c != NULL)
	{
		strcpy(c, s);
		strcpy(c, strrev(c));
		if ((strlen(c) - n) >= 0 && (strlen(c) - n) < strlen(s))
 			c[strlen(c) - n] = 0;
		strcpy(c,strrev(c));
	}
	else
		c[0]=0;
	return c;
}

void strcpymf(char *a, char *b)
{
	strcpy(a,b);
	free(b);
}

void timeout(long delay)
{
	clock_t start_time,delta_time;
	delta_time = delay;//((long)delay * CLOCKS_PER_SEC);
	start_time = clock();
	while((clock()-start_time) < delta_time);
	return;
}

//the lightining function, which draws the lightining bolt
void Lightining(void)
{
	int x;
	int y;
	int r,i=0,z=0;
	int lx = 0;
	y = 0;
	x = (rand()%ScreenW)+TopX;
	for (i = 0; i < ScreenH && savermode == 0; i++)
	{
		r = (rand()%2)+1;
		if (r == 2)
		{
			x-=2;
		}
		else if(r == 1);
		{
			x++;
		}
		SetPixel(dc,x,y,color);
		y++;
	}
	timeout(200);
	//erase the lightining bolt
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0)); // RGB(0,0,0) is black
	RECT clientRect;
    GetClientRect(whwnd, &clientRect);
	FillRect(dc, &clientRect, hBrush);
    DeleteObject(hBrush);
}