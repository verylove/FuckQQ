
// FuckQQDlg.h : 头文件
//

#pragma once


// CFuckQQDlg 对话框
class CFuckQQDlg : public CDialogEx
{
// 构造
public:
	CFuckQQDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FUCKQQ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	bool isfuck;
	int  m_CurrentIconIndex;
	NOTIFYICONDATA    m_nid;
	HICON _pIcons[8];
	bool visible ;

	afx_msg void FuckQQ();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnDestroy();
	afx_msg void ShowAbout();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};
