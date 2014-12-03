
// FuckQQDlg.cpp : 实现文件
//
#include "stdafx.h"
#include <windows.h>
#include <iostream>
//#include <shellapi.h>
#include <psapi.h>
//#include <HIDLL/HIDll.h>
#include <tlhelp32.h>
#include <string.h>


#include "stdafx.h"
#include "FuckQQ.h"
#include "FuckQQDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define   WM_SHOWTASK   WM_USER+100
#define   WM_ABOUTBOX   WM_USER+101

using namespace std;
BOOL    CALLBACK    EnumProc(HWND,LPARAM);
unsigned    long    GetPidByName(LPCSTR    pname);
DWORD    PID=0;
char*    wname=NULL;
DWORD*    LPID=NULL;
UINT    NUMPID=0;
string    propmt;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFuckQQDlg 对话框



CFuckQQDlg::CFuckQQDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFuckQQDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFuckQQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFuckQQDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CFuckQQDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_MESSAGE(WM_SHOWTASK, OnShowTask)
	ON_COMMAND(WM_ABOUTBOX, ShowAbout)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON2, &CFuckQQDlg::OnBnClickedButton2)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()


// CFuckQQDlg 消息处理程序

BOOL CFuckQQDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_nid.cbSize = sizeof(m_nid);  
	m_nid.hWnd   = this->GetSafeHwnd();              //自定义消息发送的窗口  
	m_nid.hIcon  = AfxGetApp()->LoadIcon(IDR_MAINFRAME);  //导入的ICON,在托盘中显示  
	m_nid.uCallbackMessage = WM_SHOWTASK;            //自定义的消息  
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;  
	m_nid.uID = 100;   /*WM_SHOWTASK*/               //ID,好像随便定个值就OK了  
	wcscpy_s(m_nid.szTip, TEXT("守护QQ"));           //信息提示条  
	Shell_NotifyIcon(NIM_ADD, &m_nid);               //在托盘区添加图标

	m_CurrentIconIndex = 0;

	//在任务栏和atl+tab列表中隐藏：
	SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_HIDEWINDOW);   
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);   

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFuckQQDlg::ShowAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CFuckQQDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFuckQQDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFuckQQDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


#define WINDOW_TEXT_LENGTH 256

BOOL CALLBACK EnumChildWindowCallBack(HWND hWnd, LPARAM lParam)  
{  
	DWORD dwPid = 0;  
	GetWindowThreadProcessId(hWnd, &dwPid); // 获得找到窗口所属的进程  
	if(dwPid == lParam) // 判断是否是目标进程的窗口  
	{  
		TRACE("0x%08X    \n", hWnd); // 输出窗口信息  
		TCHAR buf[WINDOW_TEXT_LENGTH];  
		SendMessage(hWnd, WM_GETTEXT, WINDOW_TEXT_LENGTH, (LPARAM)buf);  
		//TRACE(L"%s\n", buf);  
		EnumChildWindows(hWnd, EnumChildWindowCallBack, lParam);    // 递归查找子窗口  
	}  
	return TRUE;  
} 


BOOL CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam)  
{  
	DWORD dwPid = 0;  
	GetWindowThreadProcessId(hWnd, &dwPid); // 获得找到窗口所属的进程  
	if(dwPid == lParam) // 判断是否是目标进程的窗口  
	{  
		TRACE("--0x%08X--", hWnd); // 输出窗口信息  
		TCHAR buf[WINDOW_TEXT_LENGTH];  
		SendMessage(hWnd, WM_GETTEXT, WINDOW_TEXT_LENGTH, (LPARAM)buf);  
		//TRACE(L"%s--\n", buf); 

		CString str = CString(buf);
		if(str.Find(_T("系统设置"))!=-1){
			TRACE("系统设置");
			ShowWindow(hWnd,FALSE);
			SendMessage(hWnd,WM_CLOSE,   0,   0);
			MessageBox(NULL,TEXT("禁止此项操作，有问题请联系管理员。"),TEXT("警告"),MB_ICONINFORMATION|MB_OK);
		}else if(str.Find(_T("举报"))!=-1){
			//ShowWindow(hWnd,TRUE);
			TRACE("举报");
			SendMessage(hWnd,WM_CLOSE,   0,   0);
			MessageBox(NULL,TEXT("禁止此项操作，有问题请联系管理员。"),TEXT("警告"),MB_ICONINFORMATION|MB_OK);
		}else if(str.Find(_T("文件助手"))!=-1){
			TRACE("文件助手");
			ShowWindow(hWnd,FALSE);
			SendMessage(hWnd,WM_CLOSE,   0,   0);
			MessageBox(NULL,TEXT("禁止此项操作，有问题请联系管理员。"),TEXT("警告"),MB_ICONINFORMATION|MB_OK);
		}else if(str.Find(_T("打开"))!=-1){
			TRACE("打开");
			SendMessage(hWnd,WM_CLOSE,   0,   0);
			MessageBox(NULL,TEXT("禁止此项操作，有问题请联系管理员。"),TEXT("警告"),MB_ICONINFORMATION|MB_OK);
		}else if(str.Find(_T("选择文件"))!=-1){
			TRACE("选择文件");
			SendMessage(hWnd,WM_CLOSE,   0,   0);
			MessageBox(NULL,TEXT("禁止此项操作，有问题请联系管理员。"),TEXT("警告"),MB_ICONINFORMATION|MB_OK);
		}else if(str.Find(_T("删除好友"))!=-1){
			TRACE("删除好友");
			SendMessage(hWnd,WM_CLOSE,   0,   0);
			MessageBox(NULL,TEXT("禁止此项操作，有问题请联系管理员。"),TEXT("警告"),MB_ICONINFORMATION|MB_OK);
		}else if(str.Find(_T("移至黑名单"))!=-1){
			TRACE("移至黑名单");
			ShowWindow(hWnd,FALSE);
			SendMessage(hWnd,WM_CLOSE,   0,   0);
			MessageBox(NULL,TEXT("禁止此项操作，有问题请联系管理员。"),TEXT("警告"),MB_ICONINFORMATION|MB_OK);
		}else if(str.Find(_T("腾讯网迷你版"))!=-1){
			TRACE("腾讯网迷你版");
			SendMessage(hWnd,WM_CLOSE,   0,   0);	
			ShowWindow(hWnd,FALSE);
			//RECT rect;
			//::GetWindowRect(hWnd,&rect);			
			//mouseLeftClick(rect.right-25,rect.top+15);
			//TRACE("%d,%d,%d,%d -- ",rect.top,rect.left,rect.right,rect.bottom);
			///::GetClientRect(hWnd,&rect);
			//TRACE("%d,%d,%d,%d",rect.top,rect.left,rect.right,rect.bottom);
		}else if(str.Find(_T("视频通话"))!=-1){
			TRACE("视频通话");
			ShowWindow(hWnd,TRUE);
			RECT rect;
			::GetWindowRect(hWnd,&rect);	
			::SetWindowPos(hWnd,CWnd::wndTopMost,100,200,rect.right,rect.bottom,SWP_NOMOVE|SWP_NOSIZE);
			//::SetWindowPos(hWnd,CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);//置顶
		}else if(str.Find(_T("腾讯网新闻"))!=-1){
			TRACE("腾讯网新闻");
			ShowWindow(hWnd,FALSE);
			SendMessage(hWnd,WM_CLOSE,   0,   0);
		}else if(str.Find(_T("腾讯视频推荐"))!=-1){
			TRACE("腾讯视频推荐");
			ShowWindow(hWnd,FALSE);
			SendMessage(hWnd,WM_CLOSE,   0,   0);
		}else if(str.Compare(_T("QQ"))==0){
			ShowWindow(hWnd,FALSE);
		}

		OutputDebugString(str);
		TRACE("\n");
		//EnumChildWindows(hWnd, EnumChildWindowCallBack, lParam);    // 继续查找子窗口  
	}  
	return TRUE;  
}  



void CFuckQQDlg::FuckQQ()
{

	HANDLE hSnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	PROCESSENTRY32* processInfo=new PROCESSENTRY32;
	processInfo->dwSize=sizeof(PROCESSENTRY32);
	int index=0;
	TRACE("****************开始列举进程****************\n");
	while(Process32Next(hSnapShot,processInfo)!=FALSE)
	{
		///index++;
		//TRACE("****************** %d ******************\n",index);
		//TRACE("PID Name Current Threads\n");
		//TRACE("%-15d%-25s%-4d\n",processInfo->th32ProcessID,processInfo->szExeFile,processInfo->cntThreads);
		EnumWindows(EnumWindowCallBack, processInfo->th32ProcessID);
	}

	CloseHandle(hSnapShot);
	TRACE("****************进程列举结束****************\n");
}

void CFuckQQDlg::OnBnClickedButton1()
{

	

	SetTimer(1,100,NULL);
	MessageBox(L"设置成功",L"QQ守护");

	CWnd *pCwnd;
	pCwnd = (CWnd *)GetDlgItem(IDC_BUTTON1);
	pCwnd->EnableWindow(FALSE);

	pCwnd = (CWnd *)GetDlgItem(IDC_BUTTON2);
	pCwnd->EnableWindow(TRUE);

}

unsigned    long    GetPidByName(LPCSTR    pname){
	unsigned long    pid=0;
	string    name    =    pname;
	//char*    dname    =    NULL;
	name=::CharLowerA(    (char*)name.c_str() );

	PROCESSENTRY32    lp;
	HANDLE            lh=NULL;
	memset(&lp,0,sizeof(lp));
	lp.dwSize    =    sizeof(lp);

	lh  = ::CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	BOOL ok = ::Process32First( lh,&lp);

	while(ok){
		char* dname    =    new char[(int)((wcslen(lp.szExeFile)+1)*sizeof(wchar_t))];
		//wchar_t    dname[]    =lp.szExeFile;
		::WideCharToMultiByte(CP_ACP,0,lp.szExeFile,-1,dname,static_cast<int>(wcslen(lp.szExeFile)+1),NULL,NULL);
		dname    =    ::CharLowerA(dname);
		if    ( 0==name.compare(dname) ){
			pid    =    lp.th32ProcessID;        
			delete [] dname;dname=NULL;
			break;
		}
		delete [] dname;dname=NULL;
		ok        =    ::Process32NextW(lh,&lp);
	}
	return pid;
}


void CFuckQQDlg::OnTimer(UINT_PTR nIDEvent)
{

	//FuckQQ();
	this->isfuck = TRUE;

	CWnd *pCwnd;
	pCwnd = (CWnd *)GetDlgItem(IDC_BUTTON1);
	pCwnd->EnableWindow(FALSE);

	pCwnd = (CWnd *)GetDlgItem(IDC_BUTTON2);
	pCwnd->EnableWindow(TRUE);


	++m_CurrentIconIndex;
	if(m_CurrentIconIndex==8)m_CurrentIconIndex = 0;

	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; 
	m_nid.hIcon = _pIcons[m_CurrentIconIndex];
	this->SetIcon(m_nid.hIcon,FALSE);
	Shell_NotifyIcon(NIM_MODIFY,&m_nid);//发送更新图标的消息;
	Shell_NotifyIcon(NIM_ADD,&m_nid);//发送增加系统托盘的消息;


	CDialogEx::OnTimer(nIDEvent);
}


LRESULT CFuckQQDlg::OnShowTask(WPARAM wParam, LPARAM lParam)  
{  
	switch(lParam)  
	{  
	case WM_RBUTTONUP:        //右键弹时弹出菜单  
		{     
			CPoint point;  
			::GetCursorPos(&point);                    //得到鼠标位置 			
			
			CMenu menu;  
			menu.CreatePopupMenu();   //声明一个弹出式菜单  
			menu.AppendMenu(MF_STRING, IDC_BUTTON1, L"启动守护");  
			menu.AppendMenu(MF_STRING, IDC_BUTTON2, L"关闭守护");  
			menu.AppendMenu(MF_STRING, WM_DESTROY,  L"退出系统");  
			menu.AppendMenu(MF_SEPARATOR);  
			menu.AppendMenu(MF_STRING, WM_ABOUTBOX, L"关于系统");  
			menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);  
			HMENU hmenu = menu.Detach();  
			menu.DestroyMenu(); 
			
			
		}  
		break;  
	case WM_LBUTTONDBLCLK:    //双击左键的处理  
		{  
			
			
			//menu.EnableMenuItem(IDC_BUTTON1,MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			//menu.ModifyMenuW(menu.GetMenuItemID(0),MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			//SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_SHOWWINDOW);  
			//MessageBox(L"不要乱点的啦~~",L"QQ守护");
			FuckQQ();
		}  
		break;  
	}  
	return 0;  
}

int CFuckQQDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	visible = false; 
	
	memset(_pIcons,0,sizeof(HICON));
	_pIcons[0] =  AfxGetApp()->LoadIcon(IDI_ICON1);
	_pIcons[1] =  AfxGetApp()->LoadIcon(IDI_ICON2);
	_pIcons[2] =  AfxGetApp()->LoadIcon(IDI_ICON3);
	_pIcons[3] =  AfxGetApp()->LoadIcon(IDI_ICON4);
	_pIcons[4] =  AfxGetApp()->LoadIcon(IDI_ICON5);
	_pIcons[5] =  AfxGetApp()->LoadIcon(IDI_ICON6);
	_pIcons[6] =  AfxGetApp()->LoadIcon(IDI_ICON7);
	_pIcons[7] =  AfxGetApp()->LoadIcon(IDI_ICON8);
	
	this->isfuck = FALSE;

	SetTimer(1,100,NULL);

	return 0;
}


void CFuckQQDlg::OnDestroy()
{
	CDialogEx::OnDestroy();


	Shell_NotifyIcon(NIM_DELETE, &m_nid);   // 在托盘区删除图标 
}


void CFuckQQDlg::OnBnClickedButton2()
{
	KillTimer(1);
	MessageBox(L"取消成功",L"QQ守护");

	CWnd *pCwnd;
	pCwnd = (CWnd *)GetDlgItem(IDC_BUTTON1);
	pCwnd->EnableWindow(TRUE);

	pCwnd = (CWnd *)GetDlgItem(IDC_BUTTON2);
	pCwnd->EnableWindow(FALSE);
}


void CFuckQQDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanging(lpwndpos);

	if(!visible) 
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
}
