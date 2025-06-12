// CChatDialog.h

#pragma once
#include <afxwin.h>
#include "afxdialogex.h"

class CChatDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CChatDialog)

public:
    CChatDialog(CWnd* pParent = nullptr);   // Constructor mặc định
    virtual ~CChatDialog();

    void SetTargetUser(const CString& username);  // ✅ Thêm dòng này

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_CHAT_DIALOG };
#endif

protected:
    CString m_targetUser; // ✅ Thêm biến để lưu tên người dùng

    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
};

