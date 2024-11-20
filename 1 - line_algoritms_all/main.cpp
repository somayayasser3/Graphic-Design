#include<iostream>
using namespace std;

#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) WHITE_BRUSH;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               1000,                 /* The programs width */
               750,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
int Round(double x)
{
    return int(0.5+x);
}
void parametric(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
{
    for(double t=0; t<=1; t+=0.001)
    {
        int x=Round(x1+(x2-x1)*t);
        int y=Round(y1+(y2-y1)*t);
        SetPixel(hdc,x,y,color);
    }
}
void modifiedparametric(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
{
    double dx=x2-x1;
    double dy=y2-y1;
    double dt=1.0/max(abs(dx),abs(dy));
    for(double t=0; t<=1; t+=dt)
    {
        int x=Round(x1+(x2-x1)*t);
        int y=Round(y1+(y2-y1)*t);
        SetPixel(hdc,x,y,color);
    }
}

void DDA(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
{
    double dx=x2-x1;
    double dy=y2-y1;
    if(abs(dy)<=abs(dx))
    {
        double slope=dy/dx;
        if(x1>x2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }
        int x=x1;
        double y=y1;
        SetPixel(hdc,x,y,color);
        while(x<=x2)
        {
            x++;
            y+=slope;
            SetPixel(hdc,x,y,color);
        }
    }
    else
    {
        double slope=dx/dy;
        if(y1>y2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }
        double x=x1;
        int y=y1;
        SetPixel(hdc,x,y,color);
        while(y<=y2)
        {
            y++;
            x+=slope;
            SetPixel(hdc,x,y,color);
        }
    }
}

void modifiedDDA(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
{
    double dx=x2-x1;
    double dy=y2-y1;
    if(abs(dy)<=abs(dx))
    {
        double slope=dy/dx;
        if(x1>x2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }
        double c=y1-slope*x1;
        int x=x1;
        int y=y1;
        SetPixel(hdc,x,y,color);
        while(x<=x2)
        {
            x++;
            int y=Round(slope*x+c);
            SetPixel(hdc,x,y,color);
        }
    }
    else
    {
        double slope=dx/dy;
        if(y1>y2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }
        double c=x1-slope*y1;
        int y=y1;
        int x=x1;
        SetPixel(hdc,x,y,color);
        while(y<=y2)
        {
            y++;
            int x=Round(slope*y+c);
            SetPixel(hdc,x,y,color);

        }

    }
}

void midpoint(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
{
    double dx=x2-x1;
    double dy=y2-y1;
    double slope =dy/dx;
    if((abs(dy)<=abs(dx))&&(slope>=0))
    {
        if(x1>x2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }
        double d=(x2-x1)-2*(y2-y1);
        double change1=2*(x2-x1)-2*(y2-y1);
        double change2=-2*(y2-y1);
        int x=x1;
        int y=y1;
        SetPixel(hdc,x,y,color);
        while(x<=x2)
        {
            if(d<=0)
            {
                y++;
                d+=change1;
            }
            else
            {
                d+=change2;
            }
            x++;
            SetPixel(hdc,x,y,color);
        }
    }
    else if((abs(dy)>abs(dx))&&(slope>0))
    {
        if(y1>y2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }
        double d=2*(x2-x1)-(y2-y1);
        double change1=2*(x2-x1);
        double change2= 2*((x2-x1)-(y2-y1));
        int x=x1;
        int y=y1;
        SetPixel(hdc,x,y,color);
        while(y<=y2)
        {
            if(d<0)
            {
                d+=change1;
            }
            else
            {
                x++;
                d+=change2;
            }
            y++;
            SetPixel(hdc,x,y,color);
        }

    }
    else if((slope<0)&&(abs(dy)<=abs(dx)))
    {
        if(x1>x2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }
        int x=x1;
        int y=y1;
        double d=-(x2-x1)-2*(y2-y1);
        double change1=-2*(y2-y1);
        double change2=-2*((x2-x1)+(y2-y1));
        SetPixel(hdc,x,y,color);
        while(x<=x2)
        {
            if(d<0)
            {
                d+=change1;
            }
            else
            {
                y--;
                d+=change2;
            }
            x++;
            SetPixel(hdc,x,y,color);
        }

    }
    else
    {
        if(y1>y2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }
        int x=x1;
        int y=y1;
        double d=2*(x2-x1)+(y2-y1);
        double change1=2*(x2-x1);
        double change2=2*((x2-x1)+(y2-y1));
        SetPixel(hdc,x,y,color);
        while(y<=y2)
        {
            if(d>0)
            {
                d+=change1;
            }
            else
            {
                d+=change2;
                x--;
            }
            y++;
            SetPixel(hdc,x,y,color);
        }
    }

}
/*  This function is called by the Windows function DispatchMessage()  */
int x1,y1,x2,y2;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc=GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;

    case WM_LBUTTONDBLCLK:
        x1=LOWORD(lParam);
        y1=HIWORD(lParam);
        break;

    case WM_RBUTTONDBLCLK:
        x2=LOWORD(lParam);
        y2=HIWORD(lParam);
        midpoint(hdc,x1,y1,x2,y2,RGB(0,0,255));
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);

    }

    return 0;
}
