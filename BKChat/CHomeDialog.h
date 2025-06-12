#pragma once

#include "afxdialogex.h"
#include "CFriendItem.h"

// CHomeDialog dialog

class CHomeDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CHomeDialog)

public:
	CHomeDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CHomeDialog();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOME_DIALOG };
#endif

protected:
	virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog(); // Khởi tạo giao diện

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

public:
	// Các control tĩnh có sẵn trong dialog
	CStatic inf_username;
	CStatic inf_avatar;
	CStatic group_friend_inf;

	// Các thành phần để hiển thị danh sách bạn bè
	CWnd m_friendContainer; // vùng chứa các CFriendItem
	CArray<CFriendItem*, CFriendItem*> m_friendItems; // danh sách CFriendItem

	// Hàm tiện ích
	void LoadFriendList(); // load danh sách bạn bè (giả lập hoặc từ DB)
	HBITMAP LoadAvatarOrDefault(const CString& path); // load avatar hoặc ảnh mặc định

	// Event handlers
	afx_msg void OnStnClickedStaticRegPw();
	afx_msg void OnBnClickedFriendInf();
	afx_msg void OnStnClickedInfUsername();
	afx_msg void OnStnClickedAvatar();

	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	int m_scrollPos = 0;

};

