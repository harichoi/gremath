//kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib 
#include <math.h>
#include <afxext.h>
#include <afxdisp.h>
#include <afxdtctl.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#endif 
#include "resource.h"
#define pi 3.1415926535

#define max2(a,b) ((a>b)?a:b)
#define min2(a,b) ((a>b)?b:a)

LRESULT CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DrawProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ToolProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MathProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MultiMathProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ToolupProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#define X 500
#define Y 500
void crereg(HINSTANCE hInstance );
void fillboard(int x,int y, COLORREF col);
double cal2d(char *sfirst,char *ssecond);
void DrawCal(char *str);
double cal(char *sfirst,char *ssecond,char *sthird);
double drawimg(char *sfirst,char *ssecond,char *sthird);
double percent(double a, double b);
int umm(char c);

HWND MainHwnd, DrawHwnd, ToolHwnd, ToolupHwnd, MathHwnd,MultiMathHwnd;
HDC MemDC;
POINT start, finish, que[1000000], before;
int flag, can;
int ispi;
int dx[4]={1,0,-1,0};
int dy[4]={0,1,0,-1};
//double writex[100000];
//double writey[100000];
double mmmin, mminc, mmin, mmax, mmmax, minc;
HINSTANCE hInst;
COLORREF firstc, secondc;
HBITMAP copyright;
/////////////////////////////////////////////////////
char *charread(char *str,int &n);
int getmz(char c);
int decide(char c);
int gwal(char c);
int isfunc(char *c);
double ntom(char *c);
char *mton(double c);
void del(int n);
void timer(char *c);
void control();

double thes, thef;
int ani;
int n;
char str[2560];
char split[10000][200];
int first[10000];
int us, nn, grid=1;
int chawon=2;
char theta[10000], beta[10000];
double jb[1000][1000];

/////////////////////////////////////////////////////

void rot(double ax, double ay, double az, double &x, double &y, double &z)
{
	double x1, x2, y1, y2, z1, z2, x3, y3, z3;
/*
	x1=z*sin(ay)+x*cos(ay);
	y1=y;
	z1=z*cos(ay)-x*sin(ay);
	x2=x1;
	y2=z1*sin(ax)+y1*cos(ax);
	z2=z1*cos(ax)-y1*sin(ax);
	x3=x2*cos(az)-y2*sin(az);
	y3=x2*sin(az)+y2*cos(az);
	z3=z3;
	x=x3;
	y=y3;
	z=z3;
*/	/*x 축의 중심으로 */
	y1=y*cos(ax)-z*sin(ax);
	z1=y*sin(ax)+z*cos(ax);
/*y 축의 중심으로 */
	z2=z1*cos(ay)-x*sin(ay);
	x1=z1*sin(ay)+x*cos(ay);
/*z 축의 중심으로 */
	x2=x1*cos(az)-y1*sin(az);
	y2=x1*sin(az)+y1*cos(az);
	x=x2;
	y=y2;
	z=z2;
}

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPreInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
//	FILE *out=fopen("can.exe","w");
//	fclose(out);
//	FILE *in=fopen("c:\\who.txt","r");
//	if(!in)
//	{
//		MessageBox(0,"당신은 이 프로그램을 사용할수 있는 권한이 없습니다. 그러므로 자동종료 됩니다.",0,0);
//		exit(1);
//	}
//	fclose(in);
//	MessageBox(0,"이 프로그램은 자기 자신만이 쓸것이며, 남한테 배포하지 않는다고 약속한 경우만 사용하실수 있습니다. 이것은 나중에 더 잘만들어서 유료화하여 팔것이기 때문에, 절대로 비공개입니다.",0,0);
	copyright=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_COPYRIGHT));
	hInst=hInstance;
	MSG msg;
	
	crereg(hInstance);
	MainHwnd = CreateWindow("main","Gremath",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,700,600,0,0,hInstance,0);
	DrawHwnd = CreateWindow("draw","Images1",WS_OVERLAPPEDWINDOW,200,50,X,Y,MainHwnd,0,0,hInstance,0);
	ToolHwnd = CreateWindow("tool","Tool",WS_SYSMENU,10,100,130,500,MainHwnd,0,0,hInstance,0);
	ToolupHwnd = CreateWindow("toolup","Tool 기능 확장",WS_SYSMENU,750,200,130,175,MainHwnd,0,0,hInstance,0);
	MathHwnd = CreateWindow("math","Math Graph",WS_SYSMENU,50,600,800,100,MainHwnd,0,hInst,0);
	MultiMathHwnd = CreateWindow("multimath","Multi Math Graph",WS_SYSMENU,55,500,800,160,MainHwnd,0,hInst,0);
	
	ShowWindow(MainHwnd,SW_MAXIMIZE);
	ShowWindow(DrawHwnd,SW_SHOW);
//	ShowWindow(ToolHwnd,SW_SHOW);
	ShowWindow(ToolupHwnd,SW_SHOW);
	ShowWindow(MultiMathHwnd,SW_SHOW);
	ShowWindow(MathHwnd,SW_SHOW);
	UpdateWindow(MainHwnd);
	UpdateWindow(DrawHwnd);
	UpdateWindow(ToolHwnd);
	UpdateWindow(ToolupHwnd);
	UpdateWindow(MathHwnd);
	UpdateWindow(MultiMathHwnd);

	HDC hDC=GetDC(MainHwnd);
	HBITMAP bg=CreateCompatibleBitmap(hDC,X,Y);
	MemDC=CreateCompatibleDC(hDC);
	SelectObject(MemDC,bg);
	DeleteObject(bg);
	DeleteObject(hDC);
	Rectangle(MemDC,0,0,X,Y);
//	int i;
//	for(i=0 ; i<100 ; i++)
//	{
//		MoveToEx(MemDC,i*10,0,0);
//		LineTo(MemDC,i*10,490);
//		MoveToEx(MemDC,i*10+5,1000,0);
//		LineTo(MemDC,i*10+5,10);
//	}
	while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CString str;
	int i, j, t1, t2, t3;
		HDC hDC;
		HDC MemDC;
		HBITMAP old;
//	CFileDialog f(FALSE);
//	CFileDialog l(TRUE);
	FILE *in, *out;
	switch(msg)
	{
	case WM_PAINT:
		hDC=GetDC(hWnd);
		MemDC=CreateCompatibleDC(hDC);
		old=(HBITMAP)SelectObject(MemDC,copyright);
		for(i=1 ; i<=4 ; i++)
			for(j=1 ; j<15 ; j++)
		BitBlt(hDC,300*i-300,50*j-50,300,50,MemDC,0,0,SRCCOPY);
		SelectObject(MemDC,old);
		DeleteObject(hDC);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_COMMAND:
		switch(wParam)
		{
		case ID_SAVE:
//			if(f.DoModal()==IDOK)
			{
//				str=f.GetPathName();
				out=fopen(str,"wb");
				for(i=0 ; i<Y ; i++)
					for(j=0 ; j<X ; j++)
						fprintf(out,"%c%c%c",GetRValue(GetPixel(MemDC,i,j)),GetGValue(GetPixel(MemDC,i,j)),GetBValue(GetPixel(MemDC,i,j)));
					fclose(out);
			}
			break;
		case ID_LOAD:
//			if(l.DoModal()==IDOK)
			{
				Rectangle(MemDC,0,0,X,Y);
				SendMessage(DrawHwnd,WM_PAINT,0,0);
//				str=l.GetPathName();
				in=fopen(str,"rb");
				for(i=0 ; i<Y ; i++)
					for(j=0 ; j<X ; j++)
					{
						fscanf(in,"%c%c%c",&t1,&t2,&t3);
						SetPixel(MemDC,i,j,RGB(t1,t2,t3));
					}
				fclose(in);
				SendMessage(DrawHwnd,WM_PAINT,0,0);
			}
			break;
		};
		break;
	};
//	DeleteObject(f);
//	DeleteObject(l);
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

int nb1, nb2, nb3, nb4, nb5, nb6, nb7, nb8;
int mb1;

LRESULT CALLBACK MathProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND edit1, but1;
	char str[2560];
	switch(msg)
	{
	case WM_CREATE:
		edit1=CreateWindow("edit","2*x",WS_VISIBLE|WS_BORDER|WS_CHILD,10,10,700,50,hWnd,(HMENU)1,hInst,0);
		but1=CreateWindow("button","그리기",WS_VISIBLE|BS_PUSHBUTTON|WS_CHILD|WS_BORDER,710,10,80,50,hWnd,0,hInst,0);
		mb1=(int)but1;
		break;
	case WM_COMMAND:
		if(lParam==mb1)
		{
			GetDlgItemText(hWnd,1,str,2560);
			DrawCal(str);
			
		}
		break;
	};
	return DefWindowProc(hWnd,msg,wParam,lParam);
}
int mbs,mba,mbd;
LRESULT CALLBACK MultiMathProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND edit1, but1, but2, but3, edit2,edit3,edit4,edit5,edit6,edit7,edit8,edit9;
	
	switch(msg)
	{
	case WM_CREATE:
		edit1=CreateWindow("edit","-20",WS_VISIBLE|WS_BORDER|WS_CHILD,10,10,100,25,hWnd,(HMENU)1,hInst,0);
		edit2=CreateWindow("edit","20",WS_VISIBLE|WS_BORDER|WS_CHILD,110,10,100,25,hWnd,(HMENU)2,hInst,0);
		edit3=CreateWindow("edit","1",WS_VISIBLE|WS_BORDER|WS_CHILD,210,10,100,25,hWnd,(HMENU)3,hInst,0);
		edit7=CreateWindow("edit","-20",WS_VISIBLE|WS_BORDER|WS_CHILD,410,10,100,25,hWnd,(HMENU)7,hInst,0);
		edit8=CreateWindow("edit","20",WS_VISIBLE|WS_BORDER|WS_CHILD,510,10,100,25,hWnd,(HMENU)8,hInst,0);
		edit9=CreateWindow("edit","1",WS_VISIBLE|WS_BORDER|WS_CHILD,610,10,100,25,hWnd,(HMENU)9,hInst,0);
		edit4=CreateWindow("edit","t",WS_VISIBLE|WS_BORDER|WS_CHILD,10,35,700,25,hWnd,(HMENU)4,hInst,0);
		edit5=CreateWindow("edit","b",WS_VISIBLE|WS_BORDER|WS_CHILD,10,60,700,25,hWnd,(HMENU)5,hInst,0);
		edit6=CreateWindow("edit","sin(sqrt(t^2+b^2))",WS_VISIBLE|WS_BORDER|WS_CHILD,10,85,700,25,hWnd,(HMENU)6,hInst,0);
		but1=CreateWindow("button","2d그리기",WS_VISIBLE|BS_PUSHBUTTON|WS_CHILD|WS_BORDER,710,10,80,33,hWnd,0,hInst,0);
		but2=CreateWindow("button","3d그리기",WS_VISIBLE|BS_PUSHBUTTON|WS_CHILD|WS_BORDER,710,43,80,33,hWnd,0,hInst,0);
		but3=CreateWindow("button","Image Draw",WS_VISIBLE|BS_PUSHBUTTON|WS_CHILD|WS_BORDER,710,76,80,33,hWnd,0,hInst,0);
		mbs=(int)but2;
		mba=(int)but1;
		mbd=(int)but3;
		break;
	case WM_COMMAND:
		if(lParam==mba)
		{
			char str[2560];
	char first[2560];
	char second[2560];
	char third[2560];
	double befx, befy;
	double aa=0;
	double kx, ky, kz, inc;//, min, max, i, j;//, mmin, mmax, minc;
			GetDlgItemText(hWnd,1,str,2560);
			mmin=ntom(str);
			GetDlgItemText(hWnd,2,str,2560);
			mmax=ntom(str);
			GetDlgItemText(hWnd,3,str,2560);
			minc=ntom(str);
			GetDlgItemText(hWnd,7,str,2560);
			mmmin=ntom(str);
			GetDlgItemText(hWnd,8,str,2560);
			mmmax=ntom(str);
			GetDlgItemText(hWnd,9,str,2560);
			mminc=ntom(str);
			GetDlgItemText(hWnd,4,first,2560);
			GetDlgItemText(hWnd,5,second,2560);
			GetDlgItemText(hWnd,6,third,2560);
			Rectangle(MemDC,0,0,500,500);
		
				cal2d(first,second);
				
			SendMessage(DrawHwnd,WM_PAINT,0,0);
	}
		if(lParam==mbs)
		{
			char str[2560];
	char first[2560];
	char second[2560];
	char third[2560];
	double befx, befy;
	double aa=0;
	double kx, ky, kz, inc;//, min, max, i, j;//, mmin, mmax, minc;
			GetDlgItemText(hWnd,1,str,2560);
			mmin=ntom(str);
			GetDlgItemText(hWnd,2,str,2560);
			mmax=ntom(str);
			GetDlgItemText(hWnd,3,str,2560);
			minc=ntom(str);
			GetDlgItemText(hWnd,7,str,2560);
			mmmin=ntom(str);
			GetDlgItemText(hWnd,8,str,2560);
			mmmax=ntom(str);
			GetDlgItemText(hWnd,9,str,2560);
			mminc=ntom(str);
			GetDlgItemText(hWnd,4,first,2560);
			GetDlgItemText(hWnd,5,second,2560);
			GetDlgItemText(hWnd,6,third,2560);
			Rectangle(MemDC,0,0,500,500);
		
				cal(first,second,third);
			
			SendMessage(DrawHwnd,WM_PAINT,0,0);
	}
		if(lParam==mbd)
		{
			char str[2560];
	char first[2560];
	char second[2560];
	char third[2560];
	double befx, befy;
	double aa=0;
	double kx, ky, kz, inc;//, min, max, i, j;//, mmin, mmax, minc;
			GetDlgItemText(hWnd,1,str,2560);
			mmin=ntom(str);
			GetDlgItemText(hWnd,2,str,2560);
			mmax=ntom(str);
			GetDlgItemText(hWnd,3,str,2560);
			minc=ntom(str);
			GetDlgItemText(hWnd,7,str,2560);
			mmmin=ntom(str);
			GetDlgItemText(hWnd,8,str,2560);
			mmmax=ntom(str);
			GetDlgItemText(hWnd,9,str,2560);
			mminc=ntom(str);
			GetDlgItemText(hWnd,4,first,2560);
			GetDlgItemText(hWnd,5,second,2560);
			GetDlgItemText(hWnd,6,third,2560);
			Rectangle(MemDC,0,0,500,500);
		
				drawimg(first,second,third);
			
			SendMessage(DrawHwnd,WM_PAINT,0,0);
	}
		break;
	};
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

LRESULT CALLBACK ToolProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC=GetDC(hWnd);
	HWND but1, but2, but3, but4, but5, but6, but7, but8;
	switch(msg)
	{
	case WM_PAINT:
		HBRUSH brs, *old;
		brs=CreateSolidBrush(firstc);
		old=(HBRUSH *)SelectObject(hDC,brs);
		Rectangle(hDC,0,300,65,350);
		SelectObject(hDC,old);
		DeleteObject(brs);
		DeleteObject(old);
		brs=CreateSolidBrush(secondc);
		old=(HBRUSH *)SelectObject(hDC,brs);
		Rectangle(hDC,65,300,130,350);
		SelectObject(hDC,old);
		DeleteObject(brs);
		DeleteObject(old);
		break;
	case WM_CREATE:
		but1=CreateWindow("button","Line",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE|WS_BORDER,0,0,130,25,hWnd,0,hInst,0);
		but2=CreateWindow("button","Rectangle",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE|WS_BORDER,0,25,130,25,hWnd,0,hInst,0);
		but3=CreateWindow("button","Circle",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE|WS_BORDER,0,50,130,25,hWnd,0,hInst,0);
		but4=CreateWindow("button","Fill",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE|WS_BORDER,0,75,130,25,hWnd,0,hInst,0);
		but5=CreateWindow("button","FirstColor",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE|WS_BORDER,0,100,130,25,hWnd,0,hInst,0);
		but6=CreateWindow("button","SecondColor",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE|WS_BORDER,0,125,130,25,hWnd,0,hInst,0);
		but7=CreateWindow("button","Brush",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE|WS_BORDER,0,150,130,25,hWnd,0,hInst,0);
		but8=CreateWindow("button","Clear",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE|WS_BORDER,0,175,130,25,hWnd,0,hInst,0);
		nb1=(int)but1;
		nb2=(int)but2;
		nb3=(int)but3;
		nb4=(int)but4;
		nb5=(int)but5;
		nb6=(int)but6;
		nb7=(int)but7;
		nb8=(int)but8;
		break;
	case WM_COMMAND:
		if(lParam==nb1)
		{
			flag=0;
		}
		else if(lParam==nb2)
		{
			flag=1;
		}
		else if(lParam==nb3)
		{
			flag=2;
		}
		else if(lParam==nb4)
		{
			flag=3;
		}
		else if(lParam==nb5)
		{
			CColorDialog col;
			col.DoModal();
			firstc=col.GetColor();
		}
		else if(lParam==nb6)
		{
			CColorDialog col;
			col.DoModal();
			secondc=col.GetColor();
		}
		else if(lParam==nb7)
		{
			flag=4;
		}
		else if(lParam==nb8)
		{
			Rectangle(MemDC,0,0,X,Y);
			SendMessage(DrawHwnd,WM_PAINT,0,0);
		}
		SendMessage(hWnd,WM_PAINT,0,0);
	};
	DeleteObject(hDC);
	return DefWindowProc(hWnd,msg,wParam,lParam);
}
LRESULT CALLBACK ToolupProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND edit1, edit2, edit3, edit4, edit5, edit6;
	switch(msg)
	{
	case WM_CREATE:
		edit1=CreateWindow("edit","10",WS_CHILD|WS_VISIBLE|WS_BORDER,60,0,130,25,hWnd,0,hInst,0);
		edit2=CreateWindow("edit","10",WS_CHILD|WS_VISIBLE|WS_BORDER,60,25,130,25,hWnd,(HMENU)1,hInst,0);
		edit3=CreateWindow("edit","10",WS_CHILD|WS_VISIBLE|WS_BORDER,60,50,130,25,hWnd,(HMENU)2,hInst,0);
		edit4=CreateWindow("edit","49",WS_CHILD|WS_VISIBLE|WS_BORDER,60,75,130,25,hWnd,(HMENU)3,hInst,0);
		edit5=CreateWindow("edit","23",WS_CHILD|WS_VISIBLE|WS_BORDER,60,100,130,25,hWnd,(HMENU)4,hInst,0);
		edit6=CreateWindow("edit","18",WS_CHILD|WS_VISIBLE|WS_BORDER,60,125,130,25,hWnd,(HMENU)5,hInst,0);
		break;
	case WM_PAINT:
		HDC hDC=GetDC(hWnd);
		TextOut(hDC,0,0,"x축간격",7);
		TextOut(hDC,0,25,"y축간격",7);
		TextOut(hDC,0,50,"z축간격",7);
		TextOut(hDC,0,75,"x축회전",7);
		TextOut(hDC,0,100,"y축회전",7);
		TextOut(hDC,0,125,"z축회전",7);
		break;
	};
	return DefWindowProc(hWnd,msg,wParam,lParam);
}
LRESULT CALLBACK DrawProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC=GetDC(hWnd);
	char str[2560];
	switch(msg)
	{
	case WM_CREATE:
		firstc=RGB(255,255,255);
		break;
	case WM_MOUSEMOVE:
		if(can)
		{
			BitBlt(hDC,0,0,X,Y,MemDC,0,0,SRCCOPY);
			HPEN pen, *op;
			HBRUSH brs, *ob;

			finish.x=LOWORD(lParam);
			finish.y=HIWORD(lParam);
			SetROP2(hDC,GetDlgItemInt(ToolupHwnd,2,0,1));
			pen=CreatePen(GetDlgItemInt(ToolupHwnd,0,0,1),GetDlgItemInt(ToolupHwnd,1,0,1),secondc);
			brs=CreateSolidBrush(firstc);
			op=(HPEN *)SelectObject(hDC,pen);
			ob=(HBRUSH *)SelectObject(hDC,brs);
			if(flag==0)
			{
				MoveToEx(hDC,start.x,start.y,0);
				LineTo(hDC,finish.x,finish.y);
			}
			else if(flag==1)
			{
				Rectangle(hDC,start.x,start.y,finish.x,finish.y);
			}
			else if(flag==2)
			{
				Ellipse(hDC,start.x,start.y,finish.x,finish.y);
			}
			else if(flag==3)
			{
				fillboard(finish.x,finish.y,firstc);
			}
			else if(flag==4)
			{
				HPEN *op;
				HBRUSH *ob;
				op=(HPEN *)SelectObject(MemDC,pen);
				ob=(HBRUSH *)SelectObject(MemDC,brs);
				MoveToEx(MemDC,before.x,before.y,0);
				before=finish;
				LineTo(MemDC,before.x,before.y);
				SelectObject(MemDC,op);
				SelectObject(MemDC,ob);
				DeleteObject(op);
				DeleteObject(ob);
			}
			SelectObject(MemDC,op);
			SelectObject(MemDC,ob);
			DeleteObject(pen);
			DeleteObject(op);
			DeleteObject(brs);
			DeleteObject(ob);
		}
		break;
	case WM_PAINT:
		BitBlt(hDC,0,0,X,Y,MemDC,0,0,SRCCOPY);
		break;
	case WM_LBUTTONDOWN:
		start.x=LOWORD(lParam);
		start.y=HIWORD(lParam);
		before=start;
		can=1;
		break;
	case WM_LBUTTONUP:
		can=0;
		HPEN pen, *op;
		HBRUSH brs, *ob;

		finish.x=LOWORD(lParam);
		finish.y=HIWORD(lParam);
		pen=CreatePen(GetDlgItemInt(ToolupHwnd,0,0,1),GetDlgItemInt(ToolupHwnd,1,0,1),secondc);
		brs=CreateSolidBrush(firstc);
		SetROP2(MemDC,GetDlgItemInt(ToolupHwnd,2,0,1));
		op=(HPEN *)SelectObject(MemDC,pen);
		ob=(HBRUSH *)SelectObject(MemDC,brs);
		if(flag==0)
		{
			MoveToEx(MemDC,start.x,start.y,0);
			LineTo(MemDC,finish.x,finish.y);
		}
		else if(flag==1)
		{
			Rectangle(MemDC,start.x,start.y,finish.x,finish.y);
		}
		else if(flag==2)
		{
			Ellipse(MemDC,start.x,start.y,finish.x,finish.y);
		}
		else if(flag==3)
		{
			fillboard(finish.x,finish.y,firstc);
		}
		BitBlt(hDC,0,0,X,Y,MemDC,0,0,SRCCOPY);
		SelectObject(MemDC,op);
		SelectObject(MemDC,ob);
		DeleteObject(pen);
		DeleteObject(op);
		DeleteObject(brs);
		DeleteObject(ob);
		break;
	case WM_TIMER:
		GetDlgItemText(MathHwnd,1,str,2560);
		timer(str);
	};
	DeleteObject(hDC);
	return DefWindowProc(hWnd,msg,wParam,lParam);
}
void crereg(HINSTANCE hInstance)
{
	WNDCLASS wnd;

	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wnd.hCursor = LoadCursor(0, MAKEINTRESOURCE(IDC_ARROW));
	wnd.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wnd.hInstance = hInstance;
	wnd.lpfnWndProc = MainProc;
	wnd.lpszClassName = "main";
	wnd.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN);
	wnd.style = 0;

	RegisterClass(&wnd);

	wnd.lpfnWndProc = DrawProc;
	wnd.lpszClassName = "draw";
	wnd.lpszMenuName = 0;
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	
	RegisterClass(&wnd);
	
	wnd.lpfnWndProc = ToolProc;
	wnd.lpszClassName = "tool";
	wnd.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	
	RegisterClass(&wnd);

	wnd.lpfnWndProc = ToolupProc;
	wnd.lpszClassName = "toolup";
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	
	RegisterClass(&wnd);

	wnd.lpfnWndProc = MathProc;
	wnd.lpszClassName = "math";
	
	RegisterClass(&wnd);

	wnd.lpfnWndProc = MultiMathProc;
	wnd.lpszClassName = "multimath";
	
	RegisterClass(&wnd);
}

void fillboard(int x,int y, COLORREF col)
{
	int rear=0, front=-1, sx, sy, i, befx, befy;
	COLORREF c;

	que[0].x=x;
	que[0].y=y;
	c=GetPixel(MemDC,x,y);

	if(c==col)
		return;
	do{
		front++;
		for(i=0 ; i<4 ; i++)
		{
			sx=que[front].x+dx[i];
			sy=que[front].y+dy[i];
			if(sx<0 || sy<0 || sx>X || sy>Y)
				continue;
			if(GetPixel(MemDC,sx,sy)==c)	
			{
				SetPixel(MemDC,sx,sy,RGB(GetRValue(col)+front/GetDlgItemInt(ToolupHwnd,0,0,1) ,GetGValue(col)+front/GetDlgItemInt(ToolupHwnd,1,0,1),GetBValue(col)+front/GetDlgItemInt(ToolupHwnd,2,0,1)));
				que[++rear].x=sx;
				que[rear].y=sy;
			}
		}
		if(front%100==1)
			SendMessage(DrawHwnd,WM_PAINT,0,0);
		
	}while(rear!=front);
}
int con=0;
//#define hng 15
//#define basic 0.1
void DrawCal(char *str)
{
	/*
	int i, j, k, l, flag, pr, swp, tre, cha, fc, befx, befy;
	int length=strlen(str);
	int num[256], tree[256];
	int bh[256], sw[256]; /* sw: 순위 */
/*	int nn=-1, nb=-1;
*/
	int i, uk;
	double basic, hngx, hngy, hngz;
	
	double rx, ry, rz;
	rx=double(GetDlgItemInt(ToolupHwnd,3,0,1))*pi/180;
	ry=double(GetDlgItemInt(ToolupHwnd,4,0,1))*pi/180;
	rz=double(GetDlgItemInt(ToolupHwnd,5,0,1))*pi/180;

	hngx=double(GetDlgItemInt(ToolupHwnd,0,0,1));
	hngy=double(GetDlgItemInt(ToolupHwnd,1,0,1));
	hngz=double(GetDlgItemInt(ToolupHwnd,2,0,1));
	//double bxs=(-250)/hngx/hngx, bxf=(250)/hngx/hngx;
	double bxs=-250/hngx, bxf=250/hngx;
	i=0;
	if(!strcmp(charread(str,i),"#pigrid"))
		ispi=1;
	i=0;
	if(!strcmp(charread(str,i),"#numgrid"))
		ispi=0;
	i=0;
	if(!strcmp(charread(str,i),"#gridhide"))
		grid=0;
	i=0;
	if(!strcmp(charread(str,i),"#gridshow"))
		grid=1;
	i=0;
	if(!strcmp(charread(str,i),"#continue"))
		con=1;
	i=0;
	if(!strcmp(charread(str,i),"#stop"))
		con=0;
	i=0;
	if(!strcmp(charread(str,i),"#3d"))
		chawon=3;
	i=0;
	if(!strcmp(charread(str,i),"#2d"))
		chawon=2;
	i=0;
	
	if(!strcmp(charread(str,i),"#animation"))
	{
		ani=1;
		SetTimer(DrawHwnd,1,1,0);
		char temp[2560];
		sprintf(temp,charread(str,i));
		if(temp[0]=='-')
			thes=-ntom(charread(str,i));
		else
			thes=ntom(temp);
		sprintf(temp,charread(str,i));
		if(temp[0]=='-')
			thef=-ntom(charread(str,i));
		else
			thef=ntom(temp);
	}
	i=0;
	
	if(!strcmp(charread(str,i),"#closeanimation"))
	{
		ani=0;
	}
	i=0;
	if(!strcmp(charread(str,i),"#x"))
	{
		char temp[2560];
		sprintf(temp,charread(str,i));
		if(temp[0]=='-')
			bxs=-ntom(charread(str,i));
		else
			bxs=ntom(temp);
		sprintf(temp,charread(str,i));
		if(temp[0]=='-')
			bxf=-ntom(charread(str,i));
		else
			bxf=ntom(temp);
	}
	
	
	if(con==0)
		Rectangle(MemDC,0,0,500,500);
	
if(grid==1 && con==0 && chawon==2)
{
	{
		HPEN pen, *op;
		HBRUSH brs, *ob;

		pen=CreatePen(0,1/*GetDlgItemInt(ToolupHwnd,0,0,1),GetDlgItemInt(ToolupHwnd,1,0,1)*/,RGB(200,200,200));
		brs=CreateSolidBrush(firstc);
		op=(HPEN *)SelectObject(MemDC,pen);
		ob=(HBRUSH *)SelectObject(MemDC,brs);
		

	for(i=-250/min2(hngx,hngy) ; i<250/min2(hngx,hngy) ; i++)
	{
		MoveToEx(MemDC,250+hngx*i,0,0);
		LineTo(MemDC,250+hngx*i,500);
		MoveToEx(MemDC,0,250+hngy*i,0);
		LineTo(MemDC,500,250+hngy*i);
	}
		SelectObject(MemDC,op);
		SelectObject(MemDC,ob);
		DeleteObject(pen);
		DeleteObject(op);
		DeleteObject(brs);
		DeleteObject(ob);
	}
}
if(chawon==2)
	{
		HPEN pen, *op;
		HBRUSH brs, *ob;

		pen=CreatePen(0,1/*GetDlgItemInt(ToolupHwnd,0,0,1),GetDlgItemInt(ToolupHwnd,1,0,1)*/,RGB(0,0,0));
		brs=CreateSolidBrush(firstc);
		op=(HPEN *)SelectObject(MemDC,pen);
		ob=(HBRUSH *)SelectObject(MemDC,brs);
		
		if(ispi==0)
		{
	for(i=-250/min2(hngx,hngy) ; i<250/min2(hngx,hngy) ; i++)
	{
		MoveToEx(MemDC,250+hngx*i,245,0);
		LineTo(MemDC,250+hngx*i,255);
		char abc[256];
		if(i%(40/(int(hngx))+1)==0)
		{
		sprintf(abc,"%d",i);
		if(i!=0)
		TextOut(MemDC,250+hngx*i-strlen(abc)*4,255,abc,strlen(abc));
		}
		MoveToEx(MemDC,245,250+hngy*i,0);
		LineTo(MemDC,255,250+hngy*i);
		if(i%(40/(int(hngy))+1)==0)
		{
		sprintf(abc,"%d",i);
		if(i!=0)
		TextOut(MemDC,235-strlen(abc)*4,250-hngy*i-7,abc,strlen(abc));
		}
		TextOut(MemDC,235,255,"0",1);
		
	}
		}
		else
		{
	double i;		
	for(i=0 ; i<250/min2(hngx,hngy) ; i+=3.141592/2)
	{
		if(i==0)
			continue;
		MoveToEx(MemDC,250+hngx*i,245,0);
		LineTo(MemDC,250+hngx*i,255);
		char abc[256];
		if(i<=3.15)
		{
			if(percent(i/3.141592,1)>=0.3 && percent(i/3.141592,1)<=0.7)
			sprintf(abc,"1/2π");
			else
			sprintf(abc,"π");
		}
		else
		{
			if(percent(i/3.141592,1)>=0.3 && percent(i/3.141592,1)<=0.7)
			sprintf(abc,"%d/2π",int(i/3.141592)*2+1);
			else
			sprintf(abc,"%dπ",int(i/3.141592));
		}
		TextOut(MemDC,250+hngx*i-strlen(abc)*4,255,abc,strlen(abc));
		MoveToEx(MemDC,250-hngx*i,245,0);
		LineTo(MemDC,250-hngx*i,255);
		if(i<=3.15)
		{
			if(percent(i/3.141592,1)>=0.3 && percent(i/3.141592,1)<=0.7)
			sprintf(abc,"-1/2π");
			else
			sprintf(abc,"-π");
		}
		else
		{
			if(percent(i/3.141592,1)>=0.3 && percent(i/3.141592,1)<=0.7)
			sprintf(abc,"-%d/2π",int(i/3.141592)*2+1);
			else
			sprintf(abc,"-%dπ",int(i/3.141592));
		}
		TextOut(MemDC,250+hngx*(-i)-strlen(abc)*4,255,abc,strlen(abc));
	}
	{
		int i;
	for(i=-250/min2(hngx,hngy) ; i<250/min2(hngx,hngy) ; i++)
	{
		char abc[256];
		MoveToEx(MemDC,245,250+hngy*i,0);
		LineTo(MemDC,255,250+hngy*i);
		
		if(int(i)%(40/(int(hngy))+1)==0)
		{
		sprintf(abc,"%d",i);
		if(i!=0)
		TextOut(MemDC,235-strlen(abc)*4,250-hngy*i-7,abc,strlen(abc));
		}
		TextOut(MemDC,235,255,"0",1);
		
	}
	}
		}
		SelectObject(MemDC,op);
		SelectObject(MemDC,ob);
		DeleteObject(pen);
		DeleteObject(op);
		DeleteObject(brs);
		DeleteObject(ob);
	}
if(chawon==2){
	MoveToEx(MemDC,0,250,0);
	LineTo(MemDC,500,250);
	MoveToEx(MemDC,250,0,0);
	LineTo(MemDC,250,500);
}
	char aaa[2560];
	sprintf(aaa,"%s",str);
	int k;
	double bef=0, befx=0, befy=0;
	double l, m;
	basic=0.1;
	bef=0;
	if(chawon==3 && ani==0)
	{//#3d 120*cos(sqrt(x^2+y^2))+7*cos(11*sqrt(x^2+y^2))

		//////////////////////////////////////////////////////////////
		for(m=bxs ; m<=bxf ; m+=hngx/30 )
	{
			for(l=bxs ; l<=bxf ; l+=hngx/9)
			{
		nn=us=n=0;
		sprintf(str,"%s",aaa);
		uk=strlen(str);
		str[uk]='"';
		str[uk+1]=0;
		for(i=0 ; ; i++)
		{
			sprintf(split[i],charread(str,n));
			if(split[i][0]=='#')
			{
				i--;
			}
			if(!strcmp(split[i+1],"#x") || !strcmp(split[i+1],"#animation"))
			{
				if(charread(str,n)[0]=='-')
					charread(str,n);
				if(charread(str,n)[0]=='-')
					charread(str,n);

				//charread(str,n);
			}
			nn=i;
			if(n==strlen(str)-1)
				break;
		}
		for(i=0 ; i<=nn ; i++)
			first[i]=0;
		for(i=0 ; i<=nn ; i++)
		{
			//first[i]=0;
			if(gwal(split[i][0])==1)
			{
				us+=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(gwal(split[i][0])==2)
			{
				us-=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(decide(split[i][0])>0 || isfunc(split[i]))
			{
				first[i]=decide(split[i][0])+us+isfunc(split[i])*4;
			}
		}
		//////
		int j, max;
		double prev, next, result;

		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"x"))
				sprintf(split[i],"%lf",m);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"y"))
				sprintf(split[i],"%lf",l);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"e"))
				sprintf(split[i],"%.9lf",exp(1));
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"p"))
				sprintf(split[i],"3.141592654");
		max=0;
		for(i=0 ; i<=nn ; i++)
		{
			if(max<first[i])
				max=first[i];
		}
	//	char arm[256];
	//	sprintf(arm,"%d",max);
	//	MessageBox(0,arm,0,0);
		for(i=max ; i>=1 ; i--)
		{
			for(j=0 ; j<=nn ; j++)
			{
				if(first[j]==i)
				{
					if(isfunc(split[j]))
					{
						next=ntom(split[j+1]);

						if(!strcmp(split[j],"sqrt"))
							result=sqrt(next);
						if(!strcmp(split[j],"abs"))
						{
							result=next;
							if(result<0)
								result=-result;
						}
						if(!strcmp(split[j],"xcon"))
						{
							result=next;
							if(result>=0) 
								result=1;
							else result=0;
						}
						if(!strcmp(split[j],"cos"))
							result=cos(next);
						else if(!strcmp(split[j],"sin"))
							result=sin(next);
						else if(!strcmp(split[j],"tan"))
							result=tan(next);
						else if(!strcmp(split[j],"ln"))
							result=log(next);
						else if(!strcmp(split[j],"log"))
							result=log10(next);
						else if(!strcmp(split[j],"rtn"))
							result=next/3.1415926535897*180;
						else if(!strcmp(split[j],"ntr"))
							result=next*3.1415926535897/180;
						else if(!strcmp(split[j],"exp"))
							result=exp(next);
						else if(!strcmp(split[j],"sec"))
							result=(next);
						else if(!strcmp(split[j],"cot"))
							result=(next);
						else if(!strcmp(split[j],"cosec"))
							result=(next);
						
						del(j);
						first[j]=0;
						sprintf(split[j],mton(result));
						j-=1;
					}
					else
					{
						if(umm(split[j-1][0])!=1 && umm(split[j-1][1])!=1 || j==0)
						{
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=next;
								break;
							case '-':
								result=-next;
								break;
							};
							del(j);
							first[j]=0;
							sprintf(split[j],mton(result));
							j-=1;
						}
						else
						{
							prev=ntom(split[j-1]);
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=prev+next;
								break;
							case '-':
								result=prev-next;
								break;
							case '*':
								result=next*prev;
								break;
							case '/':
								result=prev/next;
								break;
							case '%':
								result=percent(prev,next);
								break;
							case '^':
								result=pow(prev,next);
								break;
							case '!':
								result=1;
								for(k=1 ; k<=prev ; k++)
									result*=k;
								del(j);
								j--;
								sprintf(split[j-1],mton(result));
								first[j-1]=0;
								continue;
								break;
							};
							del(j);
							del(j);
							first[j-1]=0;
							sprintf(split[j-1],mton(result));
							j-=2;
						}
					}
				}
			}
			
		}
		
		HPEN pen, *op;
		HBRUSH brs, *ob;

		pen=CreatePen(0,1/*GetDlgItemInt(ToolupHwnd,0,0,1),GetDlgItemInt(ToolupHwnd,1,0,1)*/,RGB(l*3.6+128,m*3.6+128,l*1.7+m*1.7+128));//secondc);
		brs=CreateSolidBrush(firstc);
		op=(HPEN *)SelectObject(MemDC,pen);
		ob=(HBRUSH *)SelectObject(MemDC,brs);
		
		MoveToEx(MemDC,befx,befy,0);
		double xx, yy, zz;
		zz=ntom(split[0])*hngz;
		xx=l*hngx;
		yy=-m*hngy;
		rot(rx,ry,rz,xx,yy,zz);
		befx=xx+250;
		befy=yy+250;
		if(l!=bxs)
			LineTo(MemDC,befx,befy);
	//	TextOut(MemDC,0,0,"만든이 : 최재석(유료화 예정)",28);
		
		SelectObject(MemDC,op);
		SelectObject(MemDC,ob);
		DeleteObject(pen);
		DeleteObject(op);
		DeleteObject(brs);
		DeleteObject(ob);
		}
		}
		//////////////////////////////////////////////////////
	}
	if(chawon==2 && ani==0)
	{
		double plus=(bxf-bxs)/500;
	for(l=bxs ; l<=bxf ; l+=plus)
	{
		nn=us=n=0;
		sprintf(str,"%s",aaa);
		uk=strlen(str);
		str[uk]='"';
		str[uk+1]=0;
		for(i=0 ; ; i++)
		{
			sprintf(split[i],charread(str,n));
			if(split[i][0]=='#')
			{
				i--;
			}
			if(!strcmp(split[i+1],"#x"))
			{
				if(charread(str,n)[0]=='-')
					charread(str,n);
				if(charread(str,n)[0]=='-')
					charread(str,n);

				//charread(str,n);
			}
			nn=i;
			if(n==strlen(str)-1)
				break;
		}
		for(i=0 ; i<=nn ; i++)
		{
			if(gwal(split[i][0])==1)
			{
				us+=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(gwal(split[i][0])==2)
			{
				us-=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(decide(split[i][0])>0 || isfunc(split[i]))
			{
				first[i]+=decide(split[i][0])+us+isfunc(split[i])*4;
				
			}
		}
		//////
		int j, max;
		double prev, next, result;

		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"x"))
				sprintf(split[i],"%lf",l);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"e"))
				sprintf(split[i],"%.9lf",exp(1));
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"p"))
				sprintf(split[i],"3.141592654");
		max=0;
		for(i=0 ; i<=nn ; i++)
		{
			if(max<first[i])
				max=first[i];
		}
		int pb=1;
		for(i=max ; i>=1 ; i--)
		{
		for(j=0 ; j<=nn ; j++)
		{
			char str[256];
				sprintf(str,"%lf, \"%s\" %d",l,split[j],first[j]);

	//			MessageBox(0,str,str,0);
		}
			for(j=0 ; j<=nn ; j++)
			{
				if(first[j]==i)
				{
					if(isfunc(split[j]))
					{
						next=ntom(split[j+1]);

						if(!strcmp(split[j],"sqrt"))
							result=sqrt(next);
						if(!strcmp(split[j],"abs"))
						{
							result=next;
							if(result<0)
								result=-result;
						}
						if(!strcmp(split[j],"xcon"))
						{
							result=next;
							if(result>=0) 
								result=1;
							else result=0;
						}
						if(!strcmp(split[j],"cos"))
							result=cos(next);
						else if(!strcmp(split[j],"sin"))
							result=sin(next);
						else if(!strcmp(split[j],"tan"))
							result=tan(next);
						else if(!strcmp(split[j],"ln"))
							result=log(next);
						else if(!strcmp(split[j],"log"))
							result=log10(next);
						else if(!strcmp(split[j],"rtn"))
							result=next/3.1415926535897*180;
						else if(!strcmp(split[j],"ntr"))
							result=next*3.1415926535897/180;
						else if(!strcmp(split[j],"exp"))
							result=exp(next);
						else if(!strcmp(split[j],"sec"))
							result=(next);
						else if(!strcmp(split[j],"cot"))
							result=(next);
						else if(!strcmp(split[j],"cosec"))
							result=(next);
						
						del(j);
						first[j]=0;
						sprintf(split[j],mton(result));
						j-=1;
					}
					else
					{
						if(umm(split[j-1][0])!=1 && umm(split[j-1][1])!=1 || j==0)
						{
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=next;
								break;
							case '-':
								result=-next;
								break;
							};
							del(j);
							first[j]=0;
							sprintf(split[j],mton(result));
							j-=1;
						}
						else
						{
							prev=ntom(split[j-1]);
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=prev+next;
								break;
							case '-':
								result=prev-next;
								break;
							case '*':
								result=next*prev;
								break;
							case '/':
								result=prev/next;
								break;
							case '%':
								result=percent(prev,next);
								break;
							case '^':
								result=pow(prev,next);
								break;
							case '!':
								result=1;
								for(k=1 ; k<=prev ; k++)
									result*=k;
								del(j);
								j--;
								sprintf(split[j-1],mton(result));
								first[j-1]=0;
								continue;
								break;
							};
							del(j);
							del(j);
							first[j-1]=0;
							sprintf(split[j-1],mton(result));
							j-=2;
						}
					}
				}
			}
			
			
		}
		
		HPEN pen, *op;
		HBRUSH brs, *ob;

		pen=CreatePen(0,1/*GetDlgItemInt(ToolupHwnd,0,0,1),GetDlgItemInt(ToolupHwnd,1,0,1)*/,secondc);
		brs=CreateSolidBrush(firstc);
		op=(HPEN *)SelectObject(MemDC,pen);
		ob=(HBRUSH *)SelectObject(MemDC,brs);
		
		int mea, meb;
		int rea, reb;
		MoveToEx(MemDC,befx,250-bef*hngy,0);
		mea=befx;
		meb=250-bef*hngy;
		bef=ntom(split[0]);
		befx=l*hngx+250;
		rea=befx;
		reb=250-bef*hngy;
		if(l!=bxs)
			if(mea<0 && rea<0 || mea>500 && rea>500)
			{}else if(meb<0 && reb<0 || meb>500 && reb>500){}
			else
			{
		/*		if(hypot(mea-rea,meb-reb)>100)
				{
					if(plus > (bxf-bxs)/19000)
					{
					l-=plus;
					plus/=2;
					}
				}
				else */if(mea<-99 || mea>599){}
				else if(meb<-99 || meb>599){}
				else
				//else if(pb==1){
					LineTo(MemDC,befx,250-bef*hngy);
				//}
			//	if(hypot(mea-rea,meb-reb)<=100)
			//		plus=(bxf-bxs)/500;
			}
		//TextOut(MemDC,0,0,"만든이 : 최재석(유료화 예정)",28);
		
		SelectObject(MemDC,op);
		SelectObject(MemDC,ob);
		DeleteObject(pen);
		DeleteObject(op);
		DeleteObject(brs);
		DeleteObject(ob);
		}
	}
	SendMessage(DrawHwnd,WM_PAINT,0,0);/////////////////


}
////////////////////////////////////////////////////////////////

int gwal(char c)
{
	if(c=='(' || c=='[')
		return 1;
	else if(c==')' || c==']')
		return 2;
	return 0;
}

void del(int n)
{
	int i;

	nn--;
	for(i=n ; i<80 ; i++)
	{
		sprintf(split[i],split[i+1]);
		first[i]=first[i+1];
	}
	
}

int decide(char c)
{
	switch(c)
	{
	case '!':
		return 3;
	case '[':
	case ']':
		return 5;
	case '(':
	case ')':
		return 5;
	case '%':
	case '*':
	case '/':
		return 2;
	case '-':
	case '+':
		return 1;
	case '^':
		return 3;
	case '=':
		return 0;
	default:
		return 0;
	};
}

int getmz(char c)
{
	switch(c)
	{
	case '!':
		return 2;
	case '-':
	case '+':
	case '%':
	case '*':
	case '/':
	case '^':
	case '(':
	case ')':
	case '[':
	case ']':
		return 0;
	case '=':
		return 5;
	case '1':
	case '2':
	case '0':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return 6;
	case '"':
		return 7;
	default:
		return 6;
	};
}

int umm(char c)
{
	switch(c)
	{
	case '1':
	case '2':
	case '0':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return 1;
	};
	return 0;
}

int isfunc(char *c)
{
	if(!strcmp(c,"sqrt"))
		return 1;
	if(!strcmp(c,"abs"))
		return 1;
	if(!strcmp(c,"xcon"))
		return 1;
	if(!strcmp(c,"cos"))
		return 1;
	if(!strcmp(c,"sin"))
		return 1;
	if(!strcmp(c,"tan"))
		return 1;
	if(!strcmp(c,"log"))
		return 1;
	if(!strcmp(c,"ln"))
		return 1;
	if(!strcmp(c,"rtn"))
		return 1;
	if(!strcmp(c,"ntr"))
		return 1;
	if(!strcmp(c,"exp"))
		return 1;
	if(!strcmp(c,"sec"))
		return 1;
	if(!strcmp(c,"cot"))
		return 1;
	if(!strcmp(c,"cosec"))
		return 1;
	return 0;
}

double ntom(char *c)
{
	int i, re=0;
	double flag;
	double num=0;

	flag=10;
	for(i=0 ; i<strlen(c) ; i++)
	{
		if(c[i]=='-')
		{
			re=1;
		}
		else if(c[i]=='.')
		{
			flag=0.1;
		}
		else if(flag>=1)
		{
			num*=flag;
			num+=c[i]-'0';
		}
		else
		{
			num+=flag*(c[i]-'0');
			flag*=0.1;
		}
	}
	if(re==0)
		return num;
	else
		return -num;
}

char *mton(double c)
{
	char str[2560];
	double i;
	int n=0;
	if(c<0)
		str[n++]='-', c=-c;
	int zari=log10(c);
	if(zari<1)
		zari=0;
	for(i=pow(10,zari) ; i>=0.000000001 ; i/=10)
	{

		if(i==0.1)
			str[n++]='.';
		str[n++]=int(c/i)+'0';
		c-=int(c/i)*i;
	}
	str[n++]=0;
	return str;
}

char *charread(char *str,int &n)
{
	int i, mz, u;
	char ret[2560];

	sprintf(ret,"");

	while(1)
	{
		if(str[n]!=' ')
		{
			mz=getmz(str[n]);
			break;
		}
		n++;
	}
	for(i=n ; i<strlen(str) ; i++)
	{
		if(str[i]==' ')
		{
			n=i;
			return ret;
		}
		if(getmz(str[i])==mz)
		{
			u=strlen(ret);
			ret[u]=str[i];
			ret[u+1]=0;
			if(mz==0)
				mz=10;
		}
		else
		{
			n=i;
		
			return ret;
		}
	}
	n=strlen(str)-1;
	return ret;
}

void control()
{
	int i, uk;
	uk=strlen(str);
	str[uk]='"';
	str[uk+1]=0;
	for(i=0 ; ; i++)
	{
		sprintf(split[i],charread(str,n));
		nn=i;
		if(n==strlen(str)-1)
			break;
	}
	for(i=0 ; i<=nn ; i++)
	{
		if(gwal(split[i][0])==1)
		{
			us+=decide(split[i][0]);
			del(i);
			i--;
		}
		else if(gwal(split[i][0])==2)
		{
			us-=decide(split[i][0]);
			del(i);
			i--;
		}
		else if(decide(split[i][0])>0 || isfunc(split[i]))
		{
			first[i]+=decide(split[i][0])+us+isfunc(split[i])*4;
		}
	}
	int j, k, max;
	double prev, next, result;

	max=0;
	for(i=0 ; i<=nn ; i++)
	{
		if(max<first[i])
			max=first[i];
	}
	for(i=max ; i>=1 ; i--)
	{
		for(j=0 ; j<=nn ; j++)
		{
			if(first[j]==i)
			{
				if(isfunc(split[j]))
				{
					next=ntom(split[j+1]);

					if(!strcmp(split[j],"sqrt"))
						result=sqrt(next);
						if(!strcmp(split[j],"abs"))
						{
							result=next;
							if(result<0)
								result=-result;
						}
						if(!strcmp(split[j],"xcon"))
						{
							result=next;
							if(result>=0) 
								result=1;
							else result=0;
						}
					if(!strcmp(split[j],"cos"))
						result=cos(next);
					if(!strcmp(split[j],"sin"))
						result=sin(next);
					if(!strcmp(split[j],"tan"))
						result=tan(next);
					if(!strcmp(split[j],"log"))
						result=log(next);
					if(!strcmp(split[j],"exp"))
						result=exp(next);
					if(!strcmp(split[j],"sec"))
						result=(next);
					if(!strcmp(split[j],"cot"))
						result=(next);
					if(!strcmp(split[j],"cosec"))
						result=(next);
					
					del(j);
					first[j]=0;
					sprintf(split[j],mton(result));
					j-=1;
				}
				else
				{
					
						if(umm(split[j-1][0])!=1 && umm(split[j-1][1])!=1 || j==0){
						next=ntom(split[j+1]);
						switch(split[j][0])
						{
						case '+':
							result=next;
							break;
						case '-':
							result=-next;
							break;
						};
						del(j);
						first[j]=0;
						sprintf(split[j],mton(result));
						j-=1;
					}
					else
					{
						prev=ntom(split[j-1]);
						next=ntom(split[j+1]);
						switch(split[j][0])
						{
						case '+':
							result=prev+next;
							break;
						case '-':
							result=prev-next;
							break;
						case '*':
							result=next*prev;
							break;
						case '/':
							result=prev/next;
							break;
						case '%':
							result=percent(prev,next);
							break;
						case '^':
							result=pow(prev,next);
							break;
						case '!':
							result=1;
							for(k=1 ; k<=prev ; k++)
								result*=k;
							del(j);
							j--;
							sprintf(split[j-1],mton(result));
							first[j-1]=0;
							continue;
							break;
						};
						del(j);
						del(j);
						first[j-1]=0;
						sprintf(split[j-1],mton(result));
						j-=2;
					}
				}
			}
		}
		
	}
}

double cal2d(char *sfirst,char *ssecond)
{
	/*
	int i, j, k, l, flag, pr, swp, tre, cha, fc, befx, befy;
	int length=strlen(str);
	int num[256], tree[256];
	int bh[256], sw[256]; /* sw: 순위 */
/*	int nn=-1, nb=-1;
*/
	int i, uk;
//	double basic;
	double basic, hngx, hngy, hngz, bxs, bxf;
	hngx=double(GetDlgItemInt(ToolupHwnd,0,0,1));
	hngy=double(GetDlgItemInt(ToolupHwnd,1,0,1));
	hngz=double(GetDlgItemInt(ToolupHwnd,2,0,1));
	//double bxs=(-250)/hngx/hngx, bxf=(250)/hngx/hngx;
	HPEN pen, *op;
		HBRUSH brs, *ob;

		pen=CreatePen(0,1/*GetDlgItemInt(ToolupHwnd,0,0,1),GetDlgItemInt(ToolupHwnd,1,0,1)*/,RGB(200,200,200));
		brs=CreateSolidBrush(firstc);
		op=(HPEN *)SelectObject(MemDC,pen);
		ob=(HBRUSH *)SelectObject(MemDC,brs);
		

	for(i=-250/min2(hngx,hngy) ; i<250/min2(hngx,hngy) ; i++)
	{
		MoveToEx(MemDC,250+hngx*i,0,0);
		LineTo(MemDC,250+hngx*i,500);
		MoveToEx(MemDC,0,250+hngy*i,0);
		LineTo(MemDC,500,250+hngy*i);
	}
		SelectObject(MemDC,op);
		SelectObject(MemDC,ob);
		DeleteObject(pen);
		DeleteObject(op);
		DeleteObject(brs);
		DeleteObject(ob);

		pen=CreatePen(0,1/*GetDlgItemInt(ToolupHwnd,0,0,1),GetDlgItemInt(ToolupHwnd,1,0,1)*/,RGB(0,0,0));
		brs=CreateSolidBrush(firstc);
		op=(HPEN *)SelectObject(MemDC,pen);
		ob=(HBRUSH *)SelectObject(MemDC,brs);
		
		
	for(i=-250/min2(hngx,hngy) ; i<250/min2(hngx,hngy) ; i++)
	{
		MoveToEx(MemDC,250+hngx*i,245,0);
		LineTo(MemDC,250+hngx*i,255);
		MoveToEx(MemDC,245,250+hngy*i,0);
		LineTo(MemDC,255,250+hngy*i);
	}
		SelectObject(MemDC,op);
		SelectObject(MemDC,ob);
		DeleteObject(pen);
		DeleteObject(op);
		DeleteObject(brs);
		DeleteObject(ob);
	MoveToEx(MemDC,0,250,0);
	LineTo(MemDC,500,250);
	MoveToEx(MemDC,250,0,0);
	LineTo(MemDC,250,500);
	char aaa[2560];
	sprintf(aaa,"%s",sfirst);
	char bbb[2560];
	sprintf(bbb,"%s",ssecond);
	int k;
	double bef=0, befx=0, befy=0, xx, yy, zz;
	double l, m;
	basic=0.1;
	bef=0;
	
	for(m=mmmin ; m<=mmmax ; m+=mminc )
//	for(m=-90 ; m<=90 ; m+=2)
	{
//		for(l=-90 ; l<=90 ; l+=2) 
			for(l=mmin ; l<=mmax ; l+=minc)
			{
		nn=us=n=0;
		sprintf(str,"%s",aaa);
		uk=strlen(str);
		str[uk]='"';
		str[uk+1]=0;
		for(i=0 ; ; i++)
		{
			sprintf(split[i],charread(str,n));
			if(split[i][0]=='#')
			{
				i--;
			}
			if(!strcmp(split[i+1],"#x") || !strcmp(split[i+1],"#animation"))
			{
				if(charread(str,n)[0]=='-')
					charread(str,n);
				if(charread(str,n)[0]=='-')
					charread(str,n);

				//charread(str,n);
			}
			nn=i;
			if(n==strlen(str)-1)
				break;
		}
		for(i=0 ; i<=nn ; i++)
			first[i]=0;
		for(i=0 ; i<=nn ; i++)
		{
			//first[i]=0;
			if(gwal(split[i][0])==1)
			{
				us+=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(gwal(split[i][0])==2)
			{
				us-=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(decide(split[i][0])>0 || isfunc(split[i]))
			{
				first[i]=decide(split[i][0])+us+isfunc(split[i])*4;
			}
		}
		//////
		int j, max;
		double prev, next, result;

		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"b"))
				sprintf(split[i],"%lf",m);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"t"))
				sprintf(split[i],"%lf",l);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"e"))
				sprintf(split[i],"%.9lf",exp(1));
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"p"))
				sprintf(split[i],"3.141592654");
		max=0;
		for(i=0 ; i<=nn ; i++)
		{
			if(max<first[i])
				max=first[i];
		}
	//	char arm[256];
	//	sprintf(arm,"%d",max);
	//	MessageBox(0,arm,0,0);
		for(i=max ; i>=1 ; i--)
		{
			for(j=0 ; j<=nn ; j++)
			{
				if(first[j]==i)
				{
					if(isfunc(split[j]))
					{
						next=ntom(split[j+1]);

						if(!strcmp(split[j],"sqrt"))
							result=sqrt(next);
						if(!strcmp(split[j],"abs"))
						{
							result=next;
							if(result<0)
								result=-result;
						}
						if(!strcmp(split[j],"xcon"))
						{
							result=next;
							if(result>=0) 
								result=1;
							else result=0;
						}
						if(!strcmp(split[j],"cos"))
							result=cos(next);
						else if(!strcmp(split[j],"sin"))
							result=sin(next);
						else if(!strcmp(split[j],"tan"))
							result=tan(next);
						else if(!strcmp(split[j],"ln"))
							result=log(next);
						else if(!strcmp(split[j],"log"))
							result=log10(next);
						else if(!strcmp(split[j],"rtn"))
							result=next/3.1415926535897*180;
						else if(!strcmp(split[j],"ntr"))
							result=next*3.1415926535897/180;
						else if(!strcmp(split[j],"exp"))
							result=exp(next);
						else if(!strcmp(split[j],"sec"))
							result=(next);
						else if(!strcmp(split[j],"cot"))
							result=(next);
						else if(!strcmp(split[j],"cosec"))
							result=(next);
						
						del(j);
						first[j]=0;
						sprintf(split[j],mton(result));
						j-=1;
					}
					else
					{
						if(umm(split[j-1][0])!=1 && umm(split[j-1][1])!=1 || j==0)
						{
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=next;
								break;
							case '-':
								result=-next;
								break;
							};
							del(j);
							first[j]=0;
							sprintf(split[j],mton(result));
							j-=1;
						}
						else
						{
							prev=ntom(split[j-1]);
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=prev+next;
								break;
							case '-':
								result=prev-next;
								break;
							case '*':
								result=next*prev;
								break;
							case '/':
								result=prev/next;
								break;
							case '%':
								result=percent(prev,next);
								break;
							case '^':
								result=pow(prev,next);
								break;
							case '!':
								result=1;
								for(k=1 ; k<=prev ; k++)
									result*=k;
								del(j);
								j--;
								sprintf(split[j-1],mton(result));
								first[j-1]=0;
								continue;
								break;
							};
							del(j);
							del(j);
							first[j-1]=0;
							sprintf(split[j-1],mton(result));
							j-=2;
						}
					}
				}
			}
			
		}
		xx=ntom(split[0])*hngx;
		
		nn=us=n=0;
		sprintf(str,"%s",bbb);
		uk=strlen(str);
		str[uk]='"';
		str[uk+1]=0;
		for(i=0 ; ; i++)
		{
			sprintf(split[i],charread(str,n));
			if(split[i][0]=='#')
			{
				i--;
			}
			if(!strcmp(split[i+1],"#x") || !strcmp(split[i+1],"#animation"))
			{
				if(charread(str,n)[0]=='-')
					charread(str,n);
				if(charread(str,n)[0]=='-')
					charread(str,n);

				//charread(str,n);
			}
			nn=i;
			if(n==strlen(str)-1)
				break;
		}
		for(i=0 ; i<=nn ; i++)
			first[i]=0;
		for(i=0 ; i<=nn ; i++)
		{
			//first[i]=0;
			if(gwal(split[i][0])==1)
			{
				us+=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(gwal(split[i][0])==2)
			{
				us-=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(decide(split[i][0])>0 || isfunc(split[i]))
			{
				first[i]=decide(split[i][0])+us+isfunc(split[i])*4;
			}
		}
		//////
//		int j, max;
//		double prev, next, result;

		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"b"))
				sprintf(split[i],"%lf",m);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"t"))
				sprintf(split[i],"%lf",l);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"e"))
				sprintf(split[i],"%.9lf",exp(1));
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"p"))
				sprintf(split[i],"3.141592654");
		max=0;
		for(i=0 ; i<=nn ; i++)
		{
			if(max<first[i])
				max=first[i];
		}
	//	char arm[256];
	//	sprintf(arm,"%d",max);
	//	MessageBox(0,arm,0,0);
		for(i=max ; i>=1 ; i--)
		{
			for(j=0 ; j<=nn ; j++)
			{
				if(first[j]==i)
				{
					if(isfunc(split[j]))
					{
						next=ntom(split[j+1]);

						if(!strcmp(split[j],"sqrt"))
							result=sqrt(next);
						if(!strcmp(split[j],"abs"))
						{
							result=next;
							if(result<0)
								result=-result;
						}
						if(!strcmp(split[j],"xcon"))
						{
							result=next;
							if(result>=0) 
								result=1;
							else result=0;
						}
						if(!strcmp(split[j],"cos"))
							result=cos(next);
						else if(!strcmp(split[j],"sin"))
							result=sin(next);
						else if(!strcmp(split[j],"tan"))
							result=tan(next);
						else if(!strcmp(split[j],"ln"))
							result=log(next);
						else if(!strcmp(split[j],"log"))
							result=log10(next);
						else if(!strcmp(split[j],"rtn"))
							result=next/3.1415926535897*180;
						else if(!strcmp(split[j],"ntr"))
							result=next*3.1415926535897/180;
						else if(!strcmp(split[j],"exp"))
							result=exp(next);
						else if(!strcmp(split[j],"sec"))
							result=(next);
						else if(!strcmp(split[j],"cot"))
							result=(next);
						else if(!strcmp(split[j],"cosec"))
							result=(next);
						
						del(j);
						first[j]=0;
						sprintf(split[j],mton(result));
						j-=1;
					}
					else
					{
						if(umm(split[j-1][0])!=1 && umm(split[j-1][1])!=1 || j==0)
						{
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=next;
								break;
							case '-':
								result=-next;
								break;
							};
							del(j);
							first[j]=0;
							sprintf(split[j],mton(result));
							j-=1;
						}
						else
						{
							prev=ntom(split[j-1]);
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=prev+next;
								break;
							case '-':
								result=prev-next;
								break;
							case '*':
								result=next*prev;
								break;
							case '/':
								result=prev/next;
								break;
							case '%':
								result=percent(prev,next);
								break;
							case '^':
								result=pow(prev,next);
								break;
							case '!':
								result=1;
								for(k=1 ; k<=prev ; k++)
									result*=k;
								del(j);
								j--;
								sprintf(split[j-1],mton(result));
								first[j-1]=0;
								continue;
								break;
							};
							del(j);
							del(j);
							first[j-1]=0;
							sprintf(split[j-1],mton(result));
							j-=2;
						}
					}
				}
			}
			
		}

		yy=ntom(split[0])*hngy;

		HPEN pen, *op;
		HBRUSH brs, *ob;

		pen=CreatePen(0,1/*GetDlgItemInt(ToolupHwnd,0,0,1),GetDlgItemInt(ToolupHwnd,1,0,1)*/,RGB(0,200,0));//(l-mmin)/(mmax-mmin)*255,(m-mmmin)/(mmmax-mmmin)*255,(l-mmin)/(mmax-mmin)*127+(m-mmmin)/(mmmax-mmmin)*127));//secondc);
		brs=CreateSolidBrush(firstc);
		op=(HPEN *)SelectObject(MemDC,pen);
		ob=(HBRUSH *)SelectObject(MemDC,brs);
		
		MoveToEx(MemDC,befx,befy,0);
	//	double xx, yy, zz;
	//	zz=ntom(split[0])/hngz*20;
	//	xx=l*hngx;
	//	yy=-m*hngy;
		befx=xx+250;
		befy=-yy+250;
		if(l!=mmin)
			LineTo(MemDC,befx,befy);
	//	TextOut(MemDC,0,0,"만든이 : 최재석(유료화 예정)",28);
		
		SelectObject(MemDC,op);
		SelectObject(MemDC,ob);
		DeleteObject(pen);
		DeleteObject(op);
		DeleteObject(brs);
		DeleteObject(ob);
		}
		}
		//////////////////////////////////////////////////////////////
	
		/*	nn=us=n=0;
		sprintf(str,"%s",aaa);
		uk=strlen(str);
		str[uk]='"';
		str[uk+1]=0;
		for(i=0 ; ; i++)
		{
			sprintf(split[i],charread(str,n));
			
			nn=i;
			if(n==strlen(str)-1)
				break;
		}
		for(i=0 ; i<=nn ; i++)
			first[i]=0;
		for(i=0 ; i<=nn ; i++)
		{
			//first[i]=0;
			if(gwal(split[i][0])==1)
			{
				us+=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(gwal(split[i][0])==2)
			{
				us-=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(decide(split[i][0])>0 || isfunc(split[i]))
			{
				first[i]=decide(split[i][0])+us+isfunc(split[i])*4;
			}
		}
		//////
		int j, max;
		double prev, next, result;

		for(i=0 ; i<=nn ; i++){
			if(!strcmp(split[i],"t"))
				sprintf(split[i],"%s",theta);
	//	for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"b"))
				sprintf(split[i],"%s",beta);
	//	for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"e"))
				sprintf(split[i],"%.9lf",exp(1));
	//	for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"p"))
				sprintf(split[i],"3.141592654");
		}
		max=0;
		for(i=0 ; i<=nn ; i++)
		{
			if(max<first[i])
				max=first[i];
		}
	//	char arm[256];
	//	sprintf(arm,"%d",max);
	//	MessageBox(0,arm,0,0);
		for(i=max ; i>=1 ; i--)
		{
			for(j=0 ; j<=nn ; j++)
			{
				if(first[j]==i)
				{
					if(isfunc(split[j]))
					{
						next=ntom(split[j+1]);

						if(!strcmp(split[j],"sqrt"))
							result=sqrt(next);
						if(!strcmp(split[j],"abs"))
						{
							result=next;
							if(result<0)
								result=-result;
						}
						if(!strcmp(split[j],"xcon"))
						{
							result=next;
							if(result>=0) 
								result=1;
							else result=0;
						}
						if(!strcmp(split[j],"cos"))
							result=cos(next);
						else if(!strcmp(split[j],"sin"))
							result=sin(next);
						else if(!strcmp(split[j],"tan"))
							result=tan(next);
						else if(!strcmp(split[j],"ln"))
							result=log(next);
						else if(!strcmp(split[j],"log"))
							result=log10(next);
						else if(!strcmp(split[j],"rtn"))
							result=next/3.1415926535897*180;
						else if(!strcmp(split[j],"ntr"))
							result=next*3.1415926535897/180;
						else if(!strcmp(split[j],"exp"))
							result=exp(next);
						else if(!strcmp(split[j],"sec"))
							result=(next);
						else if(!strcmp(split[j],"cot"))
							result=(next);
						else if(!strcmp(split[j],"cosec"))
							result=(next);
						
						del(j);
						first[j]=0;
						sprintf(split[j],mton(result));
						j-=1;
					}
					else
					{
						if(umm(split[j-1][0])!=1 && umm(split[j-1][1])!=1 || j==0)
						{
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=next;
								break;
							case '-':
								result=-next;
								break;
							};
							del(j);
							first[j]=0;
							sprintf(split[j],mton(result));
							j-=1;
						}
						else
						{
							prev=ntom(split[j-1]);
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=prev+next;
								break;
							case '-':
								result=prev-next;
								break;
							case '*':
								result=next*prev;
								break;
							case '/':
								result=prev/next;
								break;
							case '%':
								result=int(prev)%int(next);
								break;
							case '^':
								result=pow(prev,next);
								break;
							case '!':
								result=1;
								for(k=1 ; k<=prev ; k++)
									result*=k;
								del(j);
								j--;
								sprintf(split[j-1],mton(result));
								first[j-1]=0;
								continue;
								break;
							};
							del(j);
							del(j);
							first[j-1]=0;
							sprintf(split[j-1],mton(result));
							j-=2;
						}
					}
				}
			}
			
		}
		*/
		return ntom(split[0]);
		
}
double cal(char *sfirst,char *ssecond,char *sthird)
{
	/*
	int i, j, k, l, flag, pr, swp, tre, cha, fc, befx, befy;
	int length=strlen(str);
	int num[256], tree[256];
	int bh[256], sw[256]; /* sw: 순위 */
/*	int nn=-1, nb=-1;
*/
	int i, j, uk;
//	double basic;
	
	double rx, ry, rz, af, bf;
	rx=double(GetDlgItemInt(ToolupHwnd,3,0,1))*pi/180;
	ry=double(GetDlgItemInt(ToolupHwnd,4,0,1))*pi/180;
	rz=double(GetDlgItemInt(ToolupHwnd,5,0,1))*pi/180;
	double basic, hngx, hngy, hngz, bxs, bxf;
	hngx=double(GetDlgItemInt(ToolupHwnd,0,0,1));
	hngy=double(GetDlgItemInt(ToolupHwnd,1,0,1));
	hngz=double(GetDlgItemInt(ToolupHwnd,2,0,1));
	//double bxs=(-250)/hngx/hngx, bxf=(250)/hngx/hngx;
	
	for(i=1 ; i<=500 ; i++)
		for(j=1 ; j<=500 ; j++)
			jb[i][j]= -9999999999999999999;
	char aaa[2560];
	sprintf(aaa,"%s",sfirst);
	char bbb[2560];
	sprintf(bbb,"%s",ssecond);
	char ccc[2560];
	sprintf(ccc,"%s",sthird);
	int k;
	double bef=0, befx=0, befy=0, xx, yy, zz, xq, yq, zq, afx, afy, bfx, bfy;
	double l, m;
	basic=0.1;
	bef=0;
	
	xq=100;
	yq=0;
	zq=0;
	rot(rx,ry,rz,xq,yq,zq);
	xx=-100;
	yy=0;
	zz=0;
	rot(rx,ry,rz,xx,yy,zz);
	for(l=0 ; l<=1 ; l+=0.001)
	{
		SetPixel(MemDC,(xx*l+xq*(1-l))+250,-(yy*l+yq*(1-l))+250,RGB(-zz*10+127,-zz*10+127,-zz*10+127));
	}
	yq=100;
	xq=0;
	zq=0;
	rot(rx,ry,rz,xq,yq,zq);
	yy=-100;
	xx=0;
	zz=0;
	rot(rx,ry,rz,xx,yy,zz);
	for(l=0 ; l<=1 ; l+=0.001)
	{
		SetPixel(MemDC,(xx*l+xq*(1-l))+250,-(yy*l+yq*(1-l))+250,RGB(-zz*10+127,-zz*10+127,-zz*10+127));
	}
	zq=100;
	yq=0;
	xq=0;
	rot(rx,ry,rz,xq,yq,zq);
	zz=-100;
	yy=0;
	xx=0;
	rot(rx,ry,rz,xx,yy,zz);
	for(l=0 ; l<=1 ; l+=0.001)
	{
		SetPixel(MemDC,(xx*l+xq*(1-l))+250,-(yy*l+yq*(1-l))+250,RGB(l*40+127,l*40+127,l*40+127));
	}
	for(l=mmin ; l<=mmax ; l+=minc)
//	for(m=-90 ; m<=90 ; m+=2)
	{
//		for(l=-90 ; l<=90 ; l+=2) 
		FILE *file=fopen("tempr.txt","r");
		FILE *written=fopen("tempw.txt","w");
			for(m=mmmin ; m<=mmmax ; m+=mminc )
			{
		nn=us=n=0;
		sprintf(str,"%s",aaa);
		uk=strlen(str);
		str[uk]='"';
		str[uk+1]=0;
		for(i=0 ; ; i++)
		{
			sprintf(split[i],charread(str,n));
			if(split[i][0]=='#')
			{
				i--;
			}
			if(!strcmp(split[i+1],"#x") || !strcmp(split[i+1],"#animation"))
			{
				if(charread(str,n)[0]=='-')
					charread(str,n);
				if(charread(str,n)[0]=='-')
					charread(str,n);

				//charread(str,n);
			}
			nn=i;
			if(n==strlen(str)-1)
				break;
		}
		for(i=0 ; i<=nn ; i++)
			first[i]=0;
		for(i=0 ; i<=nn ; i++)
		{
			//first[i]=0;
			if(gwal(split[i][0])==1)
			{
				us+=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(gwal(split[i][0])==2)
			{
				us-=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(decide(split[i][0])>0 || isfunc(split[i]))
			{
				first[i]=decide(split[i][0])+us+isfunc(split[i])*4;
			}
		}
		//////
		int j, max;
		double prev, next, result;

		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"b"))
				sprintf(split[i],"%lf",m);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"t"))
				sprintf(split[i],"%lf",l);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"e"))
				sprintf(split[i],"%.9lf",exp(1));
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"p"))
				sprintf(split[i],"3.141592654");
		max=0;
		for(i=0 ; i<=nn ; i++)
		{
			if(max<first[i])
				max=first[i];
		}
	//	char arm[256];
	//	sprintf(arm,"%d",max);
	//	MessageBox(0,arm,0,0);
		for(i=max ; i>=1 ; i--)
		{
			for(j=0 ; j<=nn ; j++)
			{
				if(first[j]==i)
				{
					if(isfunc(split[j]))
					{
						next=ntom(split[j+1]);

						if(!strcmp(split[j],"sqrt"))
							result=sqrt(next);
						if(!strcmp(split[j],"abs"))
						{
							result=next;
							if(result<0)
								result=-result;
						}
						if(!strcmp(split[j],"xcon"))
						{
							result=next;
							if(result>=0) 
								result=1;
							else result=0;
						}
						if(!strcmp(split[j],"cos"))
							result=cos(next);
						else if(!strcmp(split[j],"sin"))
							result=sin(next);
						else if(!strcmp(split[j],"tan"))
							result=tan(next);
						else if(!strcmp(split[j],"ln"))
							result=log(next);
						else if(!strcmp(split[j],"log"))
							result=log10(next);
						else if(!strcmp(split[j],"rtn"))
							result=next/3.1415926535897*180;
						else if(!strcmp(split[j],"ntr"))
							result=next*3.1415926535897/180;
						else if(!strcmp(split[j],"exp"))
							result=exp(next);
						else if(!strcmp(split[j],"sec"))
							result=(next);
						else if(!strcmp(split[j],"cot"))
							result=(next);
						else if(!strcmp(split[j],"cosec"))
							result=(next);
						
						del(j);
						first[j]=0;
						sprintf(split[j],mton(result));
						j-=1;
					}
					else
					{
						if(umm(split[j-1][0])!=1 && umm(split[j-1][1])!=1 || j==0)
						{
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=next;
								break;
							case '-':
								result=-next;
								break;
							};
							del(j);
							first[j]=0;
							sprintf(split[j],mton(result));
							j-=1;
						}
						else
						{
							prev=ntom(split[j-1]);
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=prev+next;
								break;
							case '-':
								result=prev-next;
								break;
							case '*':
								result=next*prev;
								break;
							case '/':
								result=prev/next;
								break;
							case '%':
								result=percent(prev,next);
								break;
							case '^':
								result=pow(prev,next);
								break;
							case '!':
								result=1;
								for(k=1 ; k<=prev ; k++)
									result*=k;
								del(j);
								j--;
								sprintf(split[j-1],mton(result));
								first[j-1]=0;
								continue;
								break;
							};
							del(j);
							del(j);
							first[j-1]=0;
							sprintf(split[j-1],mton(result));
							j-=2;
						}
					}
				}
			}
			
		}
		xx=ntom(split[0])*hngx;
		
		nn=us=n=0;
		sprintf(str,"%s",bbb);
		uk=strlen(str);
		str[uk]='"';
		str[uk+1]=0;
		for(i=0 ; ; i++)
		{
			sprintf(split[i],charread(str,n));
			if(split[i][0]=='#')
			{
				i--;
			}
			if(!strcmp(split[i+1],"#x") || !strcmp(split[i+1],"#animation"))
			{
				if(charread(str,n)[0]=='-')
					charread(str,n);
				if(charread(str,n)[0]=='-')
					charread(str,n);

				//charread(str,n);
			}
			nn=i;
			if(n==strlen(str)-1)
				break;
		}
		for(i=0 ; i<=nn ; i++)
			first[i]=0;
		for(i=0 ; i<=nn ; i++)
		{
			//first[i]=0;
			if(gwal(split[i][0])==1)
			{
				us+=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(gwal(split[i][0])==2)
			{
				us-=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(decide(split[i][0])>0 || isfunc(split[i]))
			{
				first[i]=decide(split[i][0])+us+isfunc(split[i])*4;
			}
		}
		//////
//		int j, max;
//		double prev, next, result;

		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"b"))
				sprintf(split[i],"%lf",m);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"t"))
				sprintf(split[i],"%lf",l);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"e"))
				sprintf(split[i],"%.9lf",exp(1));
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"p"))
				sprintf(split[i],"3.141592654");
		max=0;
		for(i=0 ; i<=nn ; i++)
		{
			if(max<first[i])
				max=first[i];
		}
	//	char arm[256];
	//	sprintf(arm,"%d",max);
	//	MessageBox(0,arm,0,0);
		for(i=max ; i>=1 ; i--)
		{
			for(j=0 ; j<=nn ; j++)
			{
				if(first[j]==i)
				{
					if(isfunc(split[j]))
					{
						next=ntom(split[j+1]);

						if(!strcmp(split[j],"sqrt"))
							result=sqrt(next);
						if(!strcmp(split[j],"abs"))
						{
							result=next;
							if(result<0)
								result=-result;
						}
						if(!strcmp(split[j],"xcon"))
						{
							result=next;
							if(result>=0) 
								result=1;
							else result=0;
						}
						if(!strcmp(split[j],"cos"))
							result=cos(next);
						else if(!strcmp(split[j],"sin"))
							result=sin(next);
						else if(!strcmp(split[j],"tan"))
							result=tan(next);
						else if(!strcmp(split[j],"ln"))
							result=log(next);
						else if(!strcmp(split[j],"log"))
							result=log10(next);
						else if(!strcmp(split[j],"rtn"))
							result=next/3.1415926535897*180;
						else if(!strcmp(split[j],"ntr"))
							result=next*3.1415926535897/180;
						else if(!strcmp(split[j],"exp"))
							result=exp(next);
						else if(!strcmp(split[j],"sec"))
							result=(next);
						else if(!strcmp(split[j],"cot"))
							result=(next);
						else if(!strcmp(split[j],"cosec"))
							result=(next);
						
						del(j);
						first[j]=0;
						sprintf(split[j],mton(result));
						j-=1;
					}
					else
					{
						if(umm(split[j-1][0])!=1 && umm(split[j-1][1])!=1 || j==0)
						{
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=next;
								break;
							case '-':
								result=-next;
								break;
							};
							del(j);
							first[j]=0;
							sprintf(split[j],mton(result));
							j-=1;
						}
						else
						{
							prev=ntom(split[j-1]);
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=prev+next;
								break;
							case '-':
								result=prev-next;
								break;
							case '*':
								result=next*prev;
								break;
							case '/':
								result=prev/next;
								break;
							case '%':
								result=percent(prev,next);
								break;
							case '^':
								result=pow(prev,next);
								break;
							case '!':
								result=1;
								for(k=1 ; k<=prev ; k++)
									result*=k;
								del(j);
								j--;
								sprintf(split[j-1],mton(result));
								first[j-1]=0;
								continue;
								break;
							};
							del(j);
							del(j);
							first[j-1]=0;
							sprintf(split[j-1],mton(result));
							j-=2;
						}
					}
				}
			}
			
		}

		yy=ntom(split[0])*hngy;

		nn=us=n=0;
		sprintf(str,"%s",ccc);
		uk=strlen(str);
		str[uk]='"';
		str[uk+1]=0;
		for(i=0 ; ; i++)
		{
			sprintf(split[i],charread(str,n));
			if(split[i][0]=='#')
			{
				i--;
			}
			if(!strcmp(split[i+1],"#x") || !strcmp(split[i+1],"#animation"))
			{
				if(charread(str,n)[0]=='-')
					charread(str,n);
				if(charread(str,n)[0]=='-')
					charread(str,n);

				//charread(str,n);
			}
			nn=i;
			if(n==strlen(str)-1)
				break;
		}
		for(i=0 ; i<=nn ; i++)
			first[i]=0;
		for(i=0 ; i<=nn ; i++)
		{
			//first[i]=0;
			if(gwal(split[i][0])==1)
			{
				us+=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(gwal(split[i][0])==2)
			{
				us-=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(decide(split[i][0])>0 || isfunc(split[i]))
			{
				first[i]=decide(split[i][0])+us+isfunc(split[i])*4;
			}
		}
		//////
//		int j, max;

		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"b"))
				sprintf(split[i],"%lf",m);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"t"))
				sprintf(split[i],"%lf",l);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"e"))
				sprintf(split[i],"%.9lf",exp(1));
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"p"))
				sprintf(split[i],"3.141592654");
		max=0;
		for(i=0 ; i<=nn ; i++)
		{
			if(max<first[i])
				max=first[i];
		}
	//	char arm[256];
	//	sprintf(arm,"%d",max);
	//	MessageBox(0,arm,0,0);
		for(i=max ; i>=1 ; i--)
		{
			for(j=0 ; j<=nn ; j++)
			{
				if(first[j]==i)
				{
					if(isfunc(split[j]))
					{
						next=ntom(split[j+1]);

						if(!strcmp(split[j],"sqrt"))
							result=sqrt(next);
						if(!strcmp(split[j],"abs"))
						{
							result=next;
							if(result<0)
								result=-result;
						}
						if(!strcmp(split[j],"xcon"))
						{
							result=next;
							if(result>=0) 
								result=1;
							else result=0;
						}
						if(!strcmp(split[j],"cos"))
							result=cos(next);
						else if(!strcmp(split[j],"sin"))
							result=sin(next);
						else if(!strcmp(split[j],"tan"))
							result=tan(next);
						else if(!strcmp(split[j],"ln"))
							result=log(next);
						else if(!strcmp(split[j],"log"))
							result=log10(next);
						else if(!strcmp(split[j],"rtn"))
							result=next/3.1415926535897*180;
						else if(!strcmp(split[j],"ntr"))
							result=next*3.1415926535897/180;
						else if(!strcmp(split[j],"exp"))
							result=exp(next);
						else if(!strcmp(split[j],"sec"))
							result=(next);
						else if(!strcmp(split[j],"cot"))
							result=(next);
						else if(!strcmp(split[j],"cosec"))
							result=(next);
						
						del(j);
						first[j]=0;
						sprintf(split[j],mton(result));
						j-=1;
					}
					else
					{
						if(umm(split[j-1][0])!=1 && umm(split[j-1][1])!=1 || j==0)
						{
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=next;
								break;
							case '-':
								result=-next;
								break;
							};
							del(j);
							first[j]=0;
							sprintf(split[j],mton(result));
							j-=1;
						}
						else
						{
							prev=ntom(split[j-1]);
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=prev+next;
								break;
							case '-':
								result=prev-next;
								break;
							case '*':
								result=next*prev;
								break;
							case '/':
								result=prev/next;
								break;
							case '%':
								result=percent(prev,next);
								break;
							case '^':
								result=pow(prev,next);
								break;
							case '!':
								result=1;
								for(k=1 ; k<=prev ; k++)
									result*=k;
								del(j);
								j--;
								sprintf(split[j-1],mton(result));
								first[j-1]=0;
								continue;
								break;
							};
							del(j);
							del(j);
							first[j-1]=0;
							sprintf(split[j-1],mton(result));
							j-=2;
						}
					}
				}
			}
			
		}

		zz=ntom(split[0])*hngz;
		double oz=zz;
		
	//	double xx, yy, zz;
	//	zz=ntom(split[0])/hngz*20;
	//	xx=l*hngx;
	//	yy=-m*hngy;
		rot(rx,ry,rz,xx,yy,zz);
		if(l!=mmin)
		{
			fscanf(file,"%lf %lf ",&bfx, &bfy);
			if(m!=mmmin)
			{
				HBRUSH brs, *ob;
HPEN pen, *op;
		
		pen=CreatePen(0,1,RGB(l/(mmax-mmin)*60+127,187,m/(mmmax-mmmin)*60+127));//(l-mmin)/(mmax-mmin)*255,(m-mmmin)/(mmmax-mmmin)*255,(l-mmin)/(mmax-mmin)*127+(m-mmmin)/(mmmax-mmmin)*127));//secondc);
		
		op=(HPEN *)SelectObject(MemDC,pen);
		
		brs=CreateSolidBrush(RGB(l/(mmax-mmin)*127+127,255,m/(mmmax-mmmin)*127+127));
		ob=(HBRUSH *)SelectObject(MemDC,brs);
				POINT pt[5];
				pt[0].x=afx;
				pt[0].y=afy;
				pt[1].x=bfx;
				pt[1].y=bfy;
				pt[2].x=xx+250;
				pt[2].y=-yy+250;
				pt[3].x=befx;
				pt[3].y=befy;
				Polygon(MemDC,pt,4);
		SelectObject(MemDC,ob);
		DeleteObject(brs);
		DeleteObject(ob);
		SelectObject(MemDC,op);
		DeleteObject(pen);
		DeleteObject(op);
			}
			afx=bfx;
			afy=bfy;
		}
		
		befx=xx+250;
		befy=-yy+250;
		fprintf(written,"%lf %lf ",befx, befy);
		}
		
		if(l!=mmin)
			fclose(file);
		fclose(written);
		file=fopen("tempw.txt","r");
		written=fopen("tempr.txt","w");
		while(!feof(file))
		{
			fputc(fgetc(file),written);
		}
		fclose(file);
		fclose(written);
		}
		//////////////////////////////////////////////////////////////
	
		/*	nn=us=n=0;
		sprintf(str,"%s",aaa);
		uk=strlen(str);
		str[uk]='"';
		str[uk+1]=0;
		for(i=0 ; ; i++)
		{
			sprintf(split[i],charread(str,n));
			
			nn=i;
			if(n==strlen(str)-1)
				break;
		}
		for(i=0 ; i<=nn ; i++)
			first[i]=0;
		for(i=0 ; i<=nn ; i++)
		{
			//first[i]=0;
			if(gwal(split[i][0])==1)
			{
				us+=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(gwal(split[i][0])==2)
			{
				us-=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(decide(split[i][0])>0 || isfunc(split[i]))
			{
				first[i]=decide(split[i][0])+us+isfunc(split[i])*4;
			}
		}
		//////
		int j, max;
		double prev, next, result;

		for(i=0 ; i<=nn ; i++){
			if(!strcmp(split[i],"t"))
				sprintf(split[i],"%s",theta);
	//	for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"b"))
				sprintf(split[i],"%s",beta);
	//	for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"e"))
				sprintf(split[i],"%.9lf",exp(1));
	//	for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"p"))
				sprintf(split[i],"3.141592654");
		}
		max=0;
		for(i=0 ; i<=nn ; i++)
		{
			if(max<first[i])
				max=first[i];
		}
	//	char arm[256];
	//	sprintf(arm,"%d",max);
	//	MessageBox(0,arm,0,0);
		for(i=max ; i>=1 ; i--)
		{
			for(j=0 ; j<=nn ; j++)
			{
				if(first[j]==i)
				{
					if(isfunc(split[j]))
					{
						next=ntom(split[j+1]);

						if(!strcmp(split[j],"sqrt"))
							result=sqrt(next);
						if(!strcmp(split[j],"abs"))
						{
							result=next;
							if(result<0)
								result=-result;
						}
						if(!strcmp(split[j],"xcon"))
						{
							result=next;
							if(result>=0) 
								result=1;
							else result=0;
						}
						if(!strcmp(split[j],"cos"))
							result=cos(next);
						else if(!strcmp(split[j],"sin"))
							result=sin(next);
						else if(!strcmp(split[j],"tan"))
							result=tan(next);
						else if(!strcmp(split[j],"ln"))
							result=log(next);
						else if(!strcmp(split[j],"log"))
							result=log10(next);
						else if(!strcmp(split[j],"rtn"))
							result=next/3.1415926535897*180;
						else if(!strcmp(split[j],"ntr"))
							result=next*3.1415926535897/180;
						else if(!strcmp(split[j],"exp"))
							result=exp(next);
						else if(!strcmp(split[j],"sec"))
							result=(next);
						else if(!strcmp(split[j],"cot"))
							result=(next);
						else if(!strcmp(split[j],"cosec"))
							result=(next);
						
						del(j);
						first[j]=0;
						sprintf(split[j],mton(result));
						j-=1;
					}
					else
					{
						if(umm(split[j-1][0])!=1 && umm(split[j-1][1])!=1 || j==0)
						{
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=next;
								break;
							case '-':
								result=-next;
								break;
							};
							del(j);
							first[j]=0;
							sprintf(split[j],mton(result));
							j-=1;
						}
						else
						{
							prev=ntom(split[j-1]);
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=prev+next;
								break;
							case '-':
								result=prev-next;
								break;
							case '*':
								result=next*prev;
								break;
							case '/':
								result=prev/next;
								break;
							case '%':
								result=int(prev)%int(next);
								break;
							case '^':
								result=pow(prev,next);
								break;
							case '!':
								result=1;
								for(k=1 ; k<=prev ; k++)
									result*=k;
								del(j);
								j--;
								sprintf(split[j-1],mton(result));
								first[j-1]=0;
								continue;
								break;
							};
							del(j);
							del(j);
							first[j-1]=0;
							sprintf(split[j-1],mton(result));
							j-=2;
						}
					}
				}
			}
			
		}
		*/
		return ntom(split[0]);
		
}

double drawimg(char *sfirst,char *ssecond,char *sthird)
{
	/*
	int i, j, k, l, flag, pr, swp, tre, cha, fc, befx, befy;
	int length=strlen(str);
	int num[256], tree[256];
	int bh[256], sw[256]; /* sw: 순위 */
/*	int nn=-1, nb=-1;
*/
	int i, j, uk;
//	double basic;
	
	double rx, ry, rz, af, bf;
	rx=double(GetDlgItemInt(ToolupHwnd,3,0,1))*pi/180;
	ry=double(GetDlgItemInt(ToolupHwnd,4,0,1))*pi/180;
	rz=double(GetDlgItemInt(ToolupHwnd,5,0,1))*pi/180;
	double basic, hngx, hngy, hngz, bxs, bxf;
	hngx=double(GetDlgItemInt(ToolupHwnd,0,0,1));
	hngy=double(GetDlgItemInt(ToolupHwnd,1,0,1));
	hngz=double(GetDlgItemInt(ToolupHwnd,2,0,1));
	//double bxs=(-250)/hngx/hngx, bxf=(250)/hngx/hngx;
	
	for(i=1 ; i<=500 ; i++)
		for(j=1 ; j<=500 ; j++)
			jb[i][j]= -9999999999999999999;
	char aaa[2560];
	sprintf(aaa,"%s",sfirst);
	char bbb[2560];
	sprintf(bbb,"%s",ssecond);
	char ccc[2560];
	sprintf(ccc,"%s",sthird);
	int k;
	double bef=0, befx=0, befy=0, xx, yy, zz, xq, yq, zq, afx, afy, bfx, bfy;
	double l, m;
	basic=0.1;
	bef=0;
	
	xq=100;
	yq=0;
	zq=0;
	rot(rx,ry,rz,xq,yq,zq);
	xx=-100;
	yy=0;
	zz=0;
	rot(rx,ry,rz,xx,yy,zz);
	for(l=0 ; l<=1 ; l+=0.001)
	{
		SetPixel(MemDC,(xx*l+xq*(1-l))+250,-(yy*l+yq*(1-l))+250,RGB(-zz*10+127,-zz*10+127,-zz*10+127));
	}
	yq=100;
	xq=0;
	zq=0;
	rot(rx,ry,rz,xq,yq,zq);
	yy=-100;
	xx=0;
	zz=0;
	rot(rx,ry,rz,xx,yy,zz);
	for(l=0 ; l<=1 ; l+=0.001)
	{
		SetPixel(MemDC,(xx*l+xq*(1-l))+250,-(yy*l+yq*(1-l))+250,RGB(-zz*10+127,-zz*10+127,-zz*10+127));
	}
	zq=100;
	yq=0;
	xq=0;
	rot(rx,ry,rz,xq,yq,zq);
	zz=-100;
	yy=0;
	xx=0;
	rot(rx,ry,rz,xx,yy,zz);
	for(l=0 ; l<=1 ; l+=0.001)
	{
		SetPixel(MemDC,(xx*l+xq*(1-l))+250,-(yy*l+yq*(1-l))+250,RGB(l*40+127,l*40+127,l*40+127));
	}
	for(l=mmin ; l<=mmax ; l+=minc)
//	for(m=-90 ; m<=90 ; m+=2)
	{
//		for(l=-90 ; l<=90 ; l+=2) 
		FILE *file=fopen("tempr.txt","r");
		FILE *written=fopen("tempw.txt","w");
			for(m=mmmin ; m<=mmmax ; m+=mminc )
			{
		nn=us=n=0;
		sprintf(str,"%s",aaa);
		uk=strlen(str);
		str[uk]='"';
		str[uk+1]=0;
		for(i=0 ; ; i++)
		{
			sprintf(split[i],charread(str,n));
			if(split[i][0]=='#')
			{
				i--;
			}
			if(!strcmp(split[i+1],"#x") || !strcmp(split[i+1],"#animation"))
			{
				if(charread(str,n)[0]=='-')
					charread(str,n);
				if(charread(str,n)[0]=='-')
					charread(str,n);

				//charread(str,n);
			}
			nn=i;
			if(n==strlen(str)-1)
				break;
		}
		for(i=0 ; i<=nn ; i++)
			first[i]=0;
		for(i=0 ; i<=nn ; i++)
		{
			//first[i]=0;
			if(gwal(split[i][0])==1)
			{
				us+=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(gwal(split[i][0])==2)
			{
				us-=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(decide(split[i][0])>0 || isfunc(split[i]))
			{
				first[i]=decide(split[i][0])+us+isfunc(split[i])*4;
			}
		}
		//////
		int j, max;
		double prev, next, result;

		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"b"))
				sprintf(split[i],"%lf",m);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"t"))
				sprintf(split[i],"%lf",l);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"e"))
				sprintf(split[i],"%.9lf",exp(1));
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"p"))
				sprintf(split[i],"3.141592654");
		max=0;
		for(i=0 ; i<=nn ; i++)
		{
			if(max<first[i])
				max=first[i];
		}
	//	char arm[256];
	//	sprintf(arm,"%d",max);
	//	MessageBox(0,arm,0,0);
		for(i=max ; i>=1 ; i--)
		{
			for(j=0 ; j<=nn ; j++)
			{
				if(first[j]==i)
				{
					if(isfunc(split[j]))
					{
						next=ntom(split[j+1]);

						if(!strcmp(split[j],"sqrt"))
							result=sqrt(next);
						if(!strcmp(split[j],"abs"))
						{
							result=next;
							if(result<0)
								result=-result;
						}
						if(!strcmp(split[j],"xcon"))
						{
							result=next;
							if(result>=0) 
								result=1;
							else result=0;
						}
						if(!strcmp(split[j],"cos"))
							result=cos(next);
						else if(!strcmp(split[j],"sin"))
							result=sin(next);
						else if(!strcmp(split[j],"tan"))
							result=tan(next);
						else if(!strcmp(split[j],"ln"))
							result=log(next);
						else if(!strcmp(split[j],"log"))
							result=log10(next);
						else if(!strcmp(split[j],"rtn"))
							result=next/3.1415926535897*180;
						else if(!strcmp(split[j],"ntr"))
							result=next*3.1415926535897/180;
						else if(!strcmp(split[j],"exp"))
							result=exp(next);
						else if(!strcmp(split[j],"sec"))
							result=(next);
						else if(!strcmp(split[j],"cot"))
							result=(next);
						else if(!strcmp(split[j],"cosec"))
							result=(next);
						
						del(j);
						first[j]=0;
						sprintf(split[j],mton(result));
						j-=1;
					}
					else
					{
						if(umm(split[j-1][0])!=1 && umm(split[j-1][1])!=1 || j==0)
						{
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=next;
								break;
							case '-':
								result=-next;
								break;
							};
							del(j);
							first[j]=0;
							sprintf(split[j],mton(result));
							j-=1;
						}
						else
						{
							prev=ntom(split[j-1]);
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=prev+next;
								break;
							case '-':
								result=prev-next;
								break;
							case '*':
								result=next*prev;
								break;
							case '/':
								result=prev/next;
								break;
							case '%':
								result=percent(prev,next);
								break;
							case '^':
								result=pow(prev,next);
								break;
							case '!':
								result=1;
								for(k=1 ; k<=prev ; k++)
									result*=k;
								del(j);
								j--;
								sprintf(split[j-1],mton(result));
								first[j-1]=0;
								continue;
								break;
							};
							del(j);
							del(j);
							first[j-1]=0;
							sprintf(split[j-1],mton(result));
							j-=2;
						}
					}
				}
			}
			
		}
		xx=ntom(split[0]);
		
		nn=us=n=0;
		sprintf(str,"%s",bbb);
		uk=strlen(str);
		str[uk]='"';
		str[uk+1]=0;
		for(i=0 ; ; i++)
		{
			sprintf(split[i],charread(str,n));
			if(split[i][0]=='#')
			{
				i--;
			}
			if(!strcmp(split[i+1],"#x") || !strcmp(split[i+1],"#animation"))
			{
				if(charread(str,n)[0]=='-')
					charread(str,n);
				if(charread(str,n)[0]=='-')
					charread(str,n);

				//charread(str,n);
			}
			nn=i;
			if(n==strlen(str)-1)
				break;
		}
		for(i=0 ; i<=nn ; i++)
			first[i]=0;
		for(i=0 ; i<=nn ; i++)
		{
			//first[i]=0;
			if(gwal(split[i][0])==1)
			{
				us+=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(gwal(split[i][0])==2)
			{
				us-=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(decide(split[i][0])>0 || isfunc(split[i]))
			{
				first[i]=decide(split[i][0])+us+isfunc(split[i])*4;
			}
		}
		//////
//		int j, max;
//		double prev, next, result;

		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"b"))
				sprintf(split[i],"%lf",m);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"t"))
				sprintf(split[i],"%lf",l);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"e"))
				sprintf(split[i],"%.9lf",exp(1));
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"p"))
				sprintf(split[i],"3.141592654");
		max=0;
		for(i=0 ; i<=nn ; i++)
		{
			if(max<first[i])
				max=first[i];
		}
	//	char arm[256];
	//	sprintf(arm,"%d",max);
	//	MessageBox(0,arm,0,0);
		for(i=max ; i>=1 ; i--)
		{
			for(j=0 ; j<=nn ; j++)
			{
				if(first[j]==i)
				{
					if(isfunc(split[j]))
					{
						next=ntom(split[j+1]);

						if(!strcmp(split[j],"sqrt"))
							result=sqrt(next);
						if(!strcmp(split[j],"abs"))
						{
							result=next;
							if(result<0)
								result=-result;
						}
						if(!strcmp(split[j],"xcon"))
						{
							result=next;
							if(result>=0) 
								result=1;
							else result=0;
						}
						if(!strcmp(split[j],"cos"))
							result=cos(next);
						else if(!strcmp(split[j],"sin"))
							result=sin(next);
						else if(!strcmp(split[j],"tan"))
							result=tan(next);
						else if(!strcmp(split[j],"ln"))
							result=log(next);
						else if(!strcmp(split[j],"log"))
							result=log10(next);
						else if(!strcmp(split[j],"rtn"))
							result=next/3.1415926535897*180;
						else if(!strcmp(split[j],"ntr"))
							result=next*3.1415926535897/180;
						else if(!strcmp(split[j],"exp"))
							result=exp(next);
						else if(!strcmp(split[j],"sec"))
							result=(next);
						else if(!strcmp(split[j],"cot"))
							result=(next);
						else if(!strcmp(split[j],"cosec"))
							result=(next);
						
						del(j);
						first[j]=0;
						sprintf(split[j],mton(result));
						j-=1;
					}
					else
					{
						if(umm(split[j-1][0])!=1 && umm(split[j-1][1])!=1 || j==0)
						{
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=next;
								break;
							case '-':
								result=-next;
								break;
							};
							del(j);
							first[j]=0;
							sprintf(split[j],mton(result));
							j-=1;
						}
						else
						{
							prev=ntom(split[j-1]);
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=prev+next;
								break;
							case '-':
								result=prev-next;
								break;
							case '*':
								result=next*prev;
								break;
							case '/':
								result=prev/next;
								break;
							case '%':
								result=percent(prev,next);
								break;
							case '^':
								result=pow(prev,next);
								break;
							case '!':
								result=1;
								for(k=1 ; k<=prev ; k++)
									result*=k;
								del(j);
								j--;
								sprintf(split[j-1],mton(result));
								first[j-1]=0;
								continue;
								break;
							};
							del(j);
							del(j);
							first[j-1]=0;
							sprintf(split[j-1],mton(result));
							j-=2;
						}
					}
				}
			}
			
		}

		yy=ntom(split[0]);

		nn=us=n=0;
		sprintf(str,"%s",ccc);
		uk=strlen(str);
		str[uk]='"';
		str[uk+1]=0;
		for(i=0 ; ; i++)
		{
			sprintf(split[i],charread(str,n));
			if(split[i][0]=='#')
			{
				i--;
			}
			if(!strcmp(split[i+1],"#x") || !strcmp(split[i+1],"#animation"))
			{
				if(charread(str,n)[0]=='-')
					charread(str,n);
				if(charread(str,n)[0]=='-')
					charread(str,n);

				//charread(str,n);
			}
			nn=i;
			if(n==strlen(str)-1)
				break;
		}
		for(i=0 ; i<=nn ; i++)
			first[i]=0;
		for(i=0 ; i<=nn ; i++)
		{
			//first[i]=0;
			if(gwal(split[i][0])==1)
			{
				us+=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(gwal(split[i][0])==2)
			{
				us-=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(decide(split[i][0])>0 || isfunc(split[i]))
			{
				first[i]=decide(split[i][0])+us+isfunc(split[i])*4;
			}
		}
		//////
//		int j, max;

		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"b"))
				sprintf(split[i],"%lf",m);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"t"))
				sprintf(split[i],"%lf",l);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"e"))
				sprintf(split[i],"%.9lf",exp(1));
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"p"))
				sprintf(split[i],"3.141592654");
		max=0;
		for(i=0 ; i<=nn ; i++)
		{
			if(max<first[i])
				max=first[i];
		}
	//	char arm[256];
	//	sprintf(arm,"%d",max);
	//	MessageBox(0,arm,0,0);
		for(i=max ; i>=1 ; i--)
		{
			for(j=0 ; j<=nn ; j++)
			{
				if(first[j]==i)
				{
					if(isfunc(split[j]))
					{
						next=ntom(split[j+1]);

						if(!strcmp(split[j],"sqrt"))
							result=sqrt(next);
						if(!strcmp(split[j],"abs"))
						{
							result=next;
							if(result<0)
								result=-result;
						}
						if(!strcmp(split[j],"xcon"))
						{
							result=next;
							if(result>=0) 
								result=1;
							else result=0;
						}
						if(!strcmp(split[j],"cos"))
							result=cos(next);
						else if(!strcmp(split[j],"sin"))
							result=sin(next);
						else if(!strcmp(split[j],"tan"))
							result=tan(next);
						else if(!strcmp(split[j],"ln"))
							result=log(next);
						else if(!strcmp(split[j],"log"))
							result=log10(next);
						else if(!strcmp(split[j],"rtn"))
							result=next/3.1415926535897*180;
						else if(!strcmp(split[j],"ntr"))
							result=next*3.1415926535897/180;
						else if(!strcmp(split[j],"exp"))
							result=exp(next);
						else if(!strcmp(split[j],"sec"))
							result=(next);
						else if(!strcmp(split[j],"cot"))
							result=(next);
						else if(!strcmp(split[j],"cosec"))
							result=(next);
						
						del(j);
						first[j]=0;
						sprintf(split[j],mton(result));
						j-=1;
					}
					else
					{
						if(umm(split[j-1][0])!=1 && umm(split[j-1][1])!=1 || j==0)
						{
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=next;
								break;
							case '-':
								result=-next;
								break;
							};
							del(j);
							first[j]=0;
							sprintf(split[j],mton(result));
							j-=1;
						}
						else
						{
							prev=ntom(split[j-1]);
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=prev+next;
								break;
							case '-':
								result=prev-next;
								break;
							case '*':
								result=next*prev;
								break;
							case '/':
								result=prev/next;
								break;
							case '%':
								result=percent(prev,next);
								break;
							case '^':
								result=pow(prev,next);
								break;
							case '!':
								result=1;
								for(k=1 ; k<=prev ; k++)
									result*=k;
								del(j);
								j--;
								sprintf(split[j-1],mton(result));
								first[j-1]=0;
								continue;
								break;
							};
							del(j);
							del(j);
							first[j-1]=0;
							sprintf(split[j-1],mton(result));
							j-=2;
						}
					}
				}
			}
			
		}

		zz=ntom(split[0]);
		double oz=zz;
		
	//	double xx, yy, zz;
	//	zz=ntom(split[0])/hngz*20;
	//	xx=l*hngx;
	//	yy=-m*hngy;e
		if(l!=mmin)
		{
			fscanf(file,"%lf %lf ",&bfx, &bfy);
			if(m!=mmmin)
			{
				HBRUSH brs, *ob;
HPEN pen, *op;
		
		pen=CreatePen(0,1,RGB(xx,yy,zz));//(l-mmin)/(mmax-mmin)*255,(m-mmmin)/(mmmax-mmmin)*255,(l-mmin)/(mmax-mmin)*127+(m-mmmin)/(mmmax-mmmin)*127));//secondc);
		
		op=(HPEN *)SelectObject(MemDC,pen);
		
		brs=CreateSolidBrush(RGB(xx,yy,zz));
		ob=(HBRUSH *)SelectObject(MemDC,brs);
				POINT pt[5];
				pt[0].x=afx;
				pt[0].y=afy;
				pt[1].x=bfx;
				pt[1].y=bfy;
				pt[2].x=l*hngx+250;
				pt[2].y=-m*hngy+250;
				pt[3].x=befx;
				pt[3].y=befy;
				Polygon(MemDC,pt,4);
		SelectObject(MemDC,ob);
		DeleteObject(brs);
		DeleteObject(ob);
		SelectObject(MemDC,op);
		DeleteObject(pen);
		DeleteObject(op);
			}
			afx=bfx;
			afy=bfy;
		}
		
		befx=l*hngx+250;
		befy=-m*hngy+250;
		fprintf(written,"%lf %lf ",befx, befy);
		}
		
		if(l!=mmin)
			fclose(file);
		fclose(written);
		file=fopen("tempw.txt","r");
		written=fopen("tempr.txt","w");
		while(!feof(file))
		{
			fputc(fgetc(file),written);
		}
		fclose(file);
		fclose(written);
		}
		//////////////////////////////////////////////////////////////
	
		/*	nn=us=n=0;
		sprintf(str,"%s",aaa);
		uk=strlen(str);
		str[uk]='"';
		str[uk+1]=0;
		for(i=0 ; ; i++)
		{
			sprintf(split[i],charread(str,n));
			
			nn=i;
			if(n==strlen(str)-1)
				break;
		}
		for(i=0 ; i<=nn ; i++)
			first[i]=0;
		for(i=0 ; i<=nn ; i++)
		{
			//first[i]=0;
			if(gwal(split[i][0])==1)
			{
				us+=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(gwal(split[i][0])==2)
			{
				us-=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(decide(split[i][0])>0 || isfunc(split[i]))
			{
				first[i]=decide(split[i][0])+us+isfunc(split[i])*4;
			}
		}
		//////
		int j, max;
		double prev, next, result;

		for(i=0 ; i<=nn ; i++){
			if(!strcmp(split[i],"t"))
				sprintf(split[i],"%s",theta);
	//	for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"b"))
				sprintf(split[i],"%s",beta);
	//	for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"e"))
				sprintf(split[i],"%.9lf",exp(1));
	//	for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"p"))
				sprintf(split[i],"3.141592654");
		}
		max=0;
		for(i=0 ; i<=nn ; i++)
		{
			if(max<first[i])
				max=first[i];
		}
	//	char arm[256];
	//	sprintf(arm,"%d",max);
	//	MessageBox(0,arm,0,0);
		for(i=max ; i>=1 ; i--)
		{
			for(j=0 ; j<=nn ; j++)
			{
				if(first[j]==i)
				{
					if(isfunc(split[j]))
					{
						next=ntom(split[j+1]);

						if(!strcmp(split[j],"sqrt"))
							result=sqrt(next);
						if(!strcmp(split[j],"abs"))
						{
							result=next;
							if(result<0)
								result=-result;
						}
						if(!strcmp(split[j],"xcon"))
						{
							result=next;
							if(result>=0) 
								result=1;
							else result=0;
						}
						if(!strcmp(split[j],"cos"))
							result=cos(next);
						else if(!strcmp(split[j],"sin"))
							result=sin(next);
						else if(!strcmp(split[j],"tan"))
							result=tan(next);
						else if(!strcmp(split[j],"ln"))
							result=log(next);
						else if(!strcmp(split[j],"log"))
							result=log10(next);
						else if(!strcmp(split[j],"rtn"))
							result=next/3.1415926535897*180;
						else if(!strcmp(split[j],"ntr"))
							result=next*3.1415926535897/180;
						else if(!strcmp(split[j],"exp"))
							result=exp(next);
						else if(!strcmp(split[j],"sec"))
							result=(next);
						else if(!strcmp(split[j],"cot"))
							result=(next);
						else if(!strcmp(split[j],"cosec"))
							result=(next);
						
						del(j);
						first[j]=0;
						sprintf(split[j],mton(result));
						j-=1;
					}
					else
					{
						if(umm(split[j-1][0])!=1 && umm(split[j-1][1])!=1 || j==0)
						{
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=next;
								break;
							case '-':
								result=-next;
								break;
							};
							del(j);
							first[j]=0;
							sprintf(split[j],mton(result));
							j-=1;
						}
						else
						{
							prev=ntom(split[j-1]);
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=prev+next;
								break;
							case '-':
								result=prev-next;
								break;
							case '*':
								result=next*prev;
								break;
							case '/':
								result=prev/next;
								break;
							case '%':
								result=int(prev)%int(next);
								break;
							case '^':
								result=pow(prev,next);
								break;
							case '!':
								result=1;
								for(k=1 ; k<=prev ; k++)
									result*=k;
								del(j);
								j--;
								sprintf(split[j-1],mton(result));
								first[j-1]=0;
								continue;
								break;
							};
							del(j);
							del(j);
							first[j-1]=0;
							sprintf(split[j-1],mton(result));
							j-=2;
						}
					}
				}
			}
			
		}
		*/
		return ntom(split[0]);
		
}

void timer(char *str)
{
	
	double rx, ry, rz;
	rx=double(GetDlgItemInt(ToolupHwnd,3,0,1))*pi/180;
	ry=double(GetDlgItemInt(ToolupHwnd,4,0,1))*pi/180;
	rz=double(GetDlgItemInt(ToolupHwnd,5,0,1))*pi/180;
	thes++;
if(thes>thef)
{
	KillTimer(DrawHwnd,1);
	return;
}
	int i, uk;
	double basic, hngx, hngy, hngz;
	hngx=double(GetDlgItemInt(ToolupHwnd,0,0,1))/14;
	hngy=double(GetDlgItemInt(ToolupHwnd,1,0,1))/14;
	hngz=double(GetDlgItemInt(ToolupHwnd,2,0,1))/14;
	//double bxs=(-250)/hngx/hngx, bxf=(250)/hngx/hngx;
	double bxs=-250/hngx, bxf=250/hngx;
		if(con==0)
		Rectangle(MemDC,0,0,500,500);
	
if(grid==1 && con==0 && chawon==2)
{
	{
		HPEN pen, *op;
		HBRUSH brs, *ob;

		pen=CreatePen(0,1/*GetDlgItemInt(ToolupHwnd,0,0,1),GetDlgItemInt(ToolupHwnd,1,0,1)*/,RGB(200,200,200));
		brs=CreateSolidBrush(firstc);
		op=(HPEN *)SelectObject(MemDC,pen);
		ob=(HBRUSH *)SelectObject(MemDC,brs);
		

	for(i=-250/min2(hngx,hngy) ; i<250/min2(hngx,hngy) ; i++)
	{
		MoveToEx(MemDC,250+hngx*i,0,0);
		LineTo(MemDC,250+hngx*i,500);
		MoveToEx(MemDC,0,250+hngy*i,0);
		LineTo(MemDC,500,250+hngy*i);
	}
		SelectObject(MemDC,op);
		SelectObject(MemDC,ob);
		DeleteObject(pen);
		DeleteObject(op);
		DeleteObject(brs);
		DeleteObject(ob);
	}
}
if(chawon==2)
	{
		HPEN pen, *op;
		HBRUSH brs, *ob;

		pen=CreatePen(0,1/*GetDlgItemInt(ToolupHwnd,0,0,1),GetDlgItemInt(ToolupHwnd,1,0,1)*/,RGB(0,0,0));
		brs=CreateSolidBrush(firstc);
		op=(HPEN *)SelectObject(MemDC,pen);
		ob=(HBRUSH *)SelectObject(MemDC,brs);
		
		
	for(i=-250/min2(hngx,hngy) ; i<250/min2(hngx,hngy) ; i++)
	{
		MoveToEx(MemDC,250+hngx*i,245,0);
		LineTo(MemDC,250+hngx*i,255);
		MoveToEx(MemDC,245,250+hngy*i,0);
		LineTo(MemDC,255,250+hngy*i);
	}
		SelectObject(MemDC,op);
		SelectObject(MemDC,ob);
		DeleteObject(pen);
		DeleteObject(op);
		DeleteObject(brs);
		DeleteObject(ob);
	}
if(chawon==2){
	MoveToEx(MemDC,0,250,0);
	LineTo(MemDC,500,250);
	MoveToEx(MemDC,250,0,0);
	LineTo(MemDC,250,500);
}
	char aaa[2560];
	sprintf(aaa,"%s",str);
	int k;
	double bef=0, befx=0, befy=0;
	double l, m;
	basic=0.1;
	bef=0;
	if(chawon==3)
	{//#3d 120*cos(sqrt(x^2+y^2))+7*cos(11*sqrt(x^2+y^2))

		//////////////////////////////////////////////////////////////

		for(m=bxs ; m<=bxf ; m+=hngx/3)
	{
			for(l=bxs ; l<=bxf ; l+=hngx/3)
			{
		nn=us=n=0;
		sprintf(str,"%s",aaa);
		uk=strlen(str);
		str[uk]='"';
		str[uk+1]=0;
		for(i=0 ; ; i++)
		{
			sprintf(split[i],charread(str,n));
			if(split[i][0]=='#')
			{
				i--;
			}
			if(!strcmp(split[i+1],"#x") || !strcmp(split[i+1],"#animation"))
			{
				if(charread(str,n)[0]=='-')
					charread(str,n);
				if(charread(str,n)[0]=='-')
					charread(str,n);

				//charread(str,n);
			}
			nn=i;
			if(n==strlen(str)-1)
				break;
		}
		for(i=0 ; i<=nn ; i++)
			first[i]=0;
		for(i=0 ; i<=nn ; i++)
		{
			//first[i]=0;
			if(gwal(split[i][0])==1)
			{
				us+=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(gwal(split[i][0])==2)
			{
				us-=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(decide(split[i][0])>0 || isfunc(split[i]))
			{
				first[i]=decide(split[i][0])+us+isfunc(split[i])*4;
			}
		}
		//////
		int j, max;
		double prev, next, result;

		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"x"))
				sprintf(split[i],"%lf",m);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"t"))
				sprintf(split[i],"%lf",thes);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"y"))
				sprintf(split[i],"%lf",l);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"e"))
				sprintf(split[i],"%.9lf",exp(1));
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"p"))
				sprintf(split[i],"3.141592654");
		max=0;
		for(i=0 ; i<=nn ; i++)
		{
			if(max<first[i])
				max=first[i];
		}
	//	char arm[256];
	//	sprintf(arm,"%d",max);
	//	MessageBox(0,arm,0,0);
		for(i=max ; i>=1 ; i--)
		{
			for(j=0 ; j<=nn ; j++)
			{
				if(first[j]==i)
				{
					if(isfunc(split[j]))
					{
						next=ntom(split[j+1]);

						if(!strcmp(split[j],"sqrt"))
							result=sqrt(next);
						if(!strcmp(split[j],"abs"))
						{
							result=next;
							if(result<0)
								result=-result;
						}
						if(!strcmp(split[j],"xcon"))
						{
							result=next;
							if(result>=0) 
								result=1;
							else result=0;
						}
						if(!strcmp(split[j],"cos"))
							result=cos(next);
						if(!strcmp(split[j],"sin"))
							result=sin(next);
						if(!strcmp(split[j],"tan"))
							result=tan(next);
						if(!strcmp(split[j],"ln"))
							result=log(next);
						if(!strcmp(split[j],"log"))
							result=log10(next);
						if(!strcmp(split[j],"rtn"))
							result=next/3.1415926535897*180;
						if(!strcmp(split[j],"ntr"))
							result=next*3.1415926535897/180;
						if(!strcmp(split[j],"exp"))
							result=exp(next);
						if(!strcmp(split[j],"sec"))
							result=(next);
						if(!strcmp(split[j],"cot"))
							result=(next);
						if(!strcmp(split[j],"cosec"))
							result=(next);
						
						del(j);
						first[j]=0;
						sprintf(split[j],mton(result));
						j-=1;
					}
					else
					{
						if(umm(split[j-1][0])!=1 && umm(split[j-1][1])!=1 || j==0)
						{
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=next;
								break;
							case '-':
								result=-next;
								break;
							};
							del(j);
							first[j]=0;
							sprintf(split[j],mton(result));
							j-=1;
						}
						else
						{
							prev=ntom(split[j-1]);
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=prev+next;
								break;
							case '-':
								result=prev-next;
								break;
							case '*':
								result=next*prev;
								break;
							case '/':
								result=prev/next;
								break;
							case '%':
								result=percent(prev,next);
								break;
							case '^':
								result=pow(prev,next);
								break;
							case '!':
								result=1;
								for(k=1 ; k<=prev ; k++)
									result*=k;
								del(j);
								j--;
								sprintf(split[j-1],mton(result));
								first[j-1]=0;
								continue;
								break;
							};
							del(j);
							del(j);
							first[j-1]=0;
							sprintf(split[j-1],mton(result));
							j-=2;
						}
					}
				}
			}
			
		}
		
		HPEN pen, *op;
		HBRUSH brs, *ob;

		pen=CreatePen(0,1/*GetDlgItemInt(ToolupHwnd,0,0,1),GetDlgItemInt(ToolupHwnd,1,0,1)*/,secondc);
		brs=CreateSolidBrush(firstc);
		op=(HPEN *)SelectObject(MemDC,pen);
		ob=(HBRUSH *)SelectObject(MemDC,brs);
		
		MoveToEx(MemDC,befx,befy,0);
		double xx, yy, zz;
		zz=ntom(split[0])/hngz;
		xx=l*hngx;
		yy=-m*hngy;
		rot(rx,ry,rz,xx,yy,zz);
		befx=xx+250;
		befy=yy+250;
		if(l!=bxs)
			LineTo(MemDC,befx,befy);
		//TextOut(MemDC,0,0,"만든이 : 최재석(유료화 예정)",28);
		
		SelectObject(MemDC,op);
		SelectObject(MemDC,ob);
		DeleteObject(pen);
		DeleteObject(op);
		DeleteObject(brs);
		DeleteObject(ob);
		}
		}
		//////////////////////////////////////////////////////
	}
	if(chawon==2)
	{
	for(l=bxs ; l<=bxf ; l+=hngx/100)
	{
		nn=us=n=0;
		sprintf(str,"%s",aaa);
		uk=strlen(str);
		str[uk]='"';
		str[uk+1]=0;
		for(i=0 ; ; i++)
		{
			sprintf(split[i],charread(str,n));
			if(split[i][0]=='#')
			{
				i--;
			}
			if(!strcmp(split[i+1],"#x"))
			{
				if(charread(str,n)[0]=='-')
					charread(str,n);
				if(charread(str,n)[0]=='-')
					charread(str,n);

				//charread(str,n);
			}
			if(!strcmp(split[i+1],"#animation"))
			{
				if(charread(str,n)[0]=='-')
					charread(str,n);
				if(charread(str,n)[0]=='-')
					charread(str,n);

				//charread(str,n);
			}
			nn=i;
			if(n==strlen(str)-1)
				break;
		}
		for(i=0 ; i<=nn ; i++)
		{
			if(gwal(split[i][0])==1)
			{
				us+=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(gwal(split[i][0])==2)
			{
				us-=decide(split[i][0]);
				del(i);
				i--;
			}
			else if(decide(split[i][0])>0 || isfunc(split[i]))
			{
				first[i]+=decide(split[i][0])+us+isfunc(split[i])*4;
			}
		}
		//////
		int j, max;
		double prev, next, result;

		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"x"))
				sprintf(split[i],"%lf",l);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"t"))
				sprintf(split[i],"%lf",thes);
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"e"))
				sprintf(split[i],"%.9lf",exp(1));
		for(i=0 ; i<=nn ; i++)
			if(!strcmp(split[i],"p"))
				sprintf(split[i],"3.141592654");
		max=0;
		for(i=0 ; i<=nn ; i++)
		{
			if(max<first[i])
				max=first[i];
		}
		for(i=max ; i>=1 ; i--)
		{
			for(j=0 ; j<=nn ; j++)
			{
				if(first[j]==i)
				{
					if(isfunc(split[j]))
					{
						next=ntom(split[j+1]);

						if(!strcmp(split[j],"sqrt"))
							result=sqrt(next);
						if(!strcmp(split[j],"abs"))
						{
							result=next;
							if(result<0)
								result=-result;
						}
						if(!strcmp(split[j],"xcon"))
						{
							result=next;
							if(result>=0) 
								result=1;
							else result=0;
						}
						if(!strcmp(split[j],"cos"))
							result=cos(next);
						if(!strcmp(split[j],"sin"))
							result=sin(next);
						if(!strcmp(split[j],"tan"))
							result=tan(next);
						if(!strcmp(split[j],"ln"))
							result=log(next);
						if(!strcmp(split[j],"log"))
							result=log10(next);
						if(!strcmp(split[j],"rtn"))
							result=next/3.1415926535897*180;
						if(!strcmp(split[j],"ntr"))
							result=next*3.1415926535897/180;
						if(!strcmp(split[j],"exp"))
							result=exp(next);
						if(!strcmp(split[j],"sec"))
							result=(next);
						if(!strcmp(split[j],"cot"))
							result=(next);
						if(!strcmp(split[j],"cosec"))
							result=(next);
						
						del(j);
						first[j]=0;
						sprintf(split[j],mton(result));
						j-=1;
					}
					else
					{
						if(umm(split[j-1][0])!=1 && umm(split[j-1][1])!=1 || j==0)
						{
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=next;
								break;
							case '-':
								result=-next;
								break;
							};
							del(j);
							first[j]=0;
							sprintf(split[j],mton(result));
							j-=1;
						}
						else
						{
							prev=ntom(split[j-1]);
							next=ntom(split[j+1]);
							switch(split[j][0])
							{
							case '+':
								result=prev+next;
								break;
							case '-':
								result=prev-next;
								break;
							case '*':
								result=next*prev;
								break;
							case '/':
								result=prev/next;
								break;
							case '%':
								result=percent(prev,next);
								break;
							case '^':
								result=pow(prev,next);
								break;
							case '!':
								result=1;
								for(k=1 ; k<=prev ; k++)
									result*=k;
								del(j);
								j--;
								sprintf(split[j-1],mton(result));
								first[j-1]=0;
								continue;
								break;
							};
							del(j);
							del(j);
							first[j-1]=0;
							sprintf(split[j-1],mton(result));
							j-=2;
						}
					}
				}
			}
			
		}
		
		HPEN pen, *op;
		HBRUSH brs, *ob;

		pen=CreatePen(0,1/*GetDlgItemInt(ToolupHwnd,0,0,1),GetDlgItemInt(ToolupHwnd,1,0,1)*/,secondc);
		brs=CreateSolidBrush(firstc);
		op=(HPEN *)SelectObject(MemDC,pen);
		ob=(HBRUSH *)SelectObject(MemDC,brs);
		
		int mea, meb;
		int rea, reb;
		MoveToEx(MemDC,befx,250-bef*hngy,0);
		mea=befx;
		meb=250-bef*hngy;
		bef=ntom(split[0]);
		befx=l*hngx+250;
		rea=befx;
		reb=250-bef*hngy;
		if(l!=bxs)
			if(mea<0 && rea<0 || mea>500 && rea>500)
			{}else if(meb<0 && reb<0 || meb>500 && reb>500){}
			else
				LineTo(MemDC,befx,250-bef*hngy);
		TextOut(MemDC,0,0,"만든이 : 최재석(유료화 예정)",28);
		
		SelectObject(MemDC,op);
		SelectObject(MemDC,ob);
		DeleteObject(pen);
		DeleteObject(op);
		DeleteObject(brs);
		DeleteObject(ob);
		}
	}
	SendMessage(DrawHwnd,WM_PAINT,0,0);/////////////////
/*	flag=0;
	swp=0;

	for(k=-X/2 ; k<X/2 ; k++)
	{
		flag=swp=0;
		nn=-1, nb=-1;
		for(i=0 ; i<length ; i++)
			tree[i]=i;
		for(i=0 ; i<length ; i++)
		{
			if(str[i]>='0' && str[i]<='9' || str[i]=='x')
			{
				if(str[i]=='x')
					pr=k;
				else
					pr=str[i]-'0';
				if(flag==0)
				{
					flag=1;
					num[++nn]=pr;
				}
				else
				{
					num[nn]=num[nn]*10+pr;
				}
			}
			else
			{
				if(str[i]=='(')
				{
					swp++;
				}
				else if(str[i]==')')
				{
					swp--;
				}
				else
				{
					flag=0;
					bh[++nb]=str[i];
					switch(str[i])
					{
					case '^':
						sw[nb]=3;
						break;
					case '*':
					case '/':
						sw[nb]=2;
						break;
					case '+':
					case '-':
						sw[nb]=1;
						break;
					};
				}
			}
		}
		for(i=3 ; i>=1 ; i--)
		{
			for(j=0 ; j<=nb ; j++)
			{
				if(sw[j]==i)//우선순위가 맞을시에
				{
					if(bh[j]=='+')
					{
						num[j]=num[j+1]+num[j];
					}
					else if(bh[j]=='-')
					{
						num[j]-=num[j+1];
					}
					else if(bh[j]=='*')
					{
						num[j]*=num[j+1];
					}
					else if(bh[j]=='/')
					{
						num[j]/=num[j+1];
					}
					num[j+1]=num[j];
					cha=min2(tree[j],tree[j+1]);
					tre=max2(tree[j],tree[j+1]);
					fc=num[j];

					for(l=0 ; l<nn ; l++)
						if(tree[l]==tre || tree[l]==cha)
						{
							tree[l]=cha;
							num[l]=fc;
						}
				}
			}
		}
		HPEN pen, *op;
		HBRUSH brs, *ob;

		pen=CreatePen(GetDlgItemInt(ToolupHwnd,0,0,1),GetDlgItemInt(ToolupHwnd,1,0,1),secondc);
		brs=CreateSolidBrush(firstc);
		op=(HPEN *)SelectObject(MemDC,pen);
		ob=(HBRUSH *)SelectObject(MemDC,brs);
		
		if(k!=-X/2)
		{
			MoveToEx(MemDC,befx,befy,0);
			befx=k+X/2;
			befy=Y/2-num[0];
			LineTo(MemDC,befx,befy);
		}
		SelectObject(MemDC,op);
		SelectObject(MemDC,ob);
		DeleteObject(pen);
		DeleteObject(op);
		DeleteObject(brs);
		DeleteObject(ob);
	}	
	SendMessage(DrawHwnd,WM_PAINT,0,0);*/

}


double percent(double a, double b)
{
	if(a<0)
		a= -a;
	while(a>=0)
		a-=b;
	return a+b;
}