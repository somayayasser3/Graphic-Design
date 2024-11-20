#include<iostream>
#include<cmath>
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
/*void draw4pixels(HDC hdc,int xc,int yc,int x,int y,COLORREF color)
{
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc-x,yc-y,color);
    SetPixel(hdc,xc+x,yc-y,color);
}
void ellipse(HDC hdc,int xc,int yc,int a,int b,COLORREF color)
{
    double dtheta=1.0/a;
    double stheta=sin(dtheta);
    double ctheta=cos(dtheta);
    double x=a;
    double y=0;
    double u= double( b/  a);
    double v= double(a/ b);
    SetPixel(hdc,round(x+xc),round(y+yc),color);
    for(double theta=0; theta<=6.28; theta+=dtheta)
    {
        double x1=x*ctheta+y*(y/x)*v*v*stheta;
        y=y*ctheta-x1*(x/y)*u*u*stheta;
        x=x1;
        SetPixel(hdc,round(x+xc),round(y+yc),color);
    }
}
*/
/*void modifiedparametric2(HDC hdc,int xc,int yc,int r,COLORREF color)
{
    double dtheta=1.0/r;
    double ctheta=cos(dtheta);
    double stheta=sin(dtheta);
    double x=r;
    double y=0.0;
    Draw8Point(hdc,xc,yc,x,y,color);
    while(y<x)
    {
        double x1=x*ctheta-y*stheta;
        y=x*stheta+*ctheta;
        x=x1;
        Draw8Point(hdc,xc,yc,Round(x),Round(y),color);
    }

}
*/

struct point
{
    int x,y;
};
typedef struct
{
    double xleft,xright;
} edgerec;

typedef edgerec table[750];

void initial(table tbl)
{
    for(int i=0; i<750; i++)
    {
        tbl[i].xleft=1000;
        tbl[i].xright=-1000;
    }
}

void edge2table(point p1,point p2,table tbl)
{
    if(p1.y==p2.y)
        return;
    if(p1.y>p2.y)
        swap(p1,p2);
    double x=p1.x;
    int y=p1.y;
    double minv=double(p2.x-p1.x)/(p2.y-p1.y);
    while(y<p2.y)
    {
        if(x<tbl[y].xleft)
            tbl[y].xleft=x;
        if(x>tbl[y].xright)
            tbl[y].xright=x;
        y++;
        x+=minv;
    }
}

void polygon2table(point p[],int n, table tbl)
{
    point v1=p[n-1];
    for(int i=0; i<n; i++)
    {
        point v2=p[i];
        edge2table(v1,v2,tbl);
        v1=v2;
    }
}
void line(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
{
    double dx=double(x2-x1);
    double dy=double(y2-y1);
    double dt=1.0/max(abs(dx),abs(dy));
    for(int t=0; t<=1; t+=dt)
    {
        int x=x1+(x2-x1)*t;
        int y=y1+(y2-y1)*t;
        SetPixel(hdc, x, y,color);
    }
}
void table2screen(HDC hdc,table tbl,COLORREF color)
{
    for(int i=0; i<750; i++)
    {
        if(tbl[i].xleft<tbl[i].xright)
        {
            line(hdc,ceil(tbl[i].xleft),i,floor(tbl[i].xright),i,color);
        }
    }
}

void convexfill(HDC hdc,point p[],int n,COLORREF color)
{
    table tbl;
    initial(tbl);
    polygon2table(p,n,tbl);
    table2screen(hdc,tbl,color);
}
/*  This function is called by the Windows function DispatchMessage()  */
int xa,xb,xc,xd,xe,ya,yb,yc,yd,ye;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc=GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    case WM_RBUTTONDBLCLK:
        xa=LOWORD(lParam);
        ya=HIWORD(lParam);
        break;
    case WM_RBUTTONUP:
        xb=LOWORD(lParam);
        yb=HIWORD(lParam);
        break;
    case WM_LBUTTONDOWN:
        xc=LOWORD(lParam);
        yc=HIWORD(lParam);
        break;
    case WM_RBUTTONDOWN:
        xd=LOWORD(lParam);
        yd=HIWORD(lParam);
        break;
    case WM_LBUTTONUP:
        xe=LOWORD(lParam);
        ye=HIWORD(lParam);
        point p[5];
        p[0].x=xa;
        p[0].y=ya;
        p[1].x=xb;
        p[1].y=yb;
        p[2].x=xc;
        p[2].y=yc;
        p[3].x=xd;
        p[3].y=yd;
        p[4].x=xe;
        p[4].y=ye;
        convexfill(hdc,p,8,RGB(0,0,255));
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
