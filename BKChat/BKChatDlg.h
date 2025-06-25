
// BKChatDlg.h : header file
//
#include "afxcmn.h"
#include "afxbutton.h"
#include "MyCustomButton.h"
#include "MyCustomTransparentButton.h"
#include "CMyEdit.h"



#pragma once


// CBKChatDlg dialog
class CBKChatDlg : public CDialogEx
{
// Construction
public:
	CBKChatDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BKCHAT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg) override;



// Implementation
protected:
	HICON m_hIcon;
	virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNcPaint();               // Vẽ lại thanh tiêu đề (title bar)
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg LRESULT OnNcHitTest(CPoint pt); // Cho phép kéo cửa sổ bằng thanh tiêu đề tùy chỉnh
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
protected:
	CEdit edt_username_;
	CEdit edt_password_;
	CButton chk_remember_;
	MyCustomButton btn_login_;
	MyCustomTransparentButton btn_to_register_;
	CFont _font_bkav_chat;
	CFont _font_global;
	CStatic _idc_bkav_chat_;
	CStatic _idc_username_;
	CStatic _idc_password_;
	CStatic idc_err_login_;
public:
	afx_msg void OnEnChangeEditUsername();
	afx_msg void OnEnChangeEditPassword();
	afx_msg void OnBnClickedRememberAcc();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonToRegister();
};
