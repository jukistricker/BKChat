#pragma once
#include "afxdialogex.h"
#include "afxcmn.h"
#include "afxbutton.h"
#include "MyCustomButton.h"
#include "MyCustomTransparentButton.h"
#include "CMyEdit.h"

// RegisterDlg dialog

class RegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RegisterDlg)

public:
	RegisterDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~RegisterDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGISTER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEditRegUsername();
protected:
	CEdit edt_reg_username_;
	CEdit edt_reg_pw_;
	CEdit edt_reg_retype_pw_;
	MyCustomButton btn_reg_create_account_;
	CFont _font_global;
public:
	afx_msg void OnEnChangeEditRegRetypePw();
protected:
	CStatic idc_reg_username_;
	CStatic idc_reg_pw_;
	CStatic idc_retype_pw_;
public:
	afx_msg void OnStnClickedStaticRegRetypepw();
protected:
	CStatic idc_er_reg_;
public:
	afx_msg void OnBnClickedButtonCreateAccount();
};
