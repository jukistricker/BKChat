// CChatDialog.h

#pragma once
#include <afxwin.h>
#include "afxdialogex.h"
#include "Chat.h"
#include "CMessageItem.h"

class CChatDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CChatDialog)

public:
    CChatDialog(CWnd* pParent = nullptr);   // Constructor mặc định
    virtual ~CChatDialog();

    void SetTargetUser(const CString& username);  // ✅ Thêm dòng này
    void SetTargetAvatar(HBITMAP hAvatar);

    // Method để tạo message item với avatar
    void AddMessageItem(const Message& message, bool isSenderMe);

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_CHAT_DIALOG };
#endif

protected:
    Chat* m_chat;
    std::vector<CMessageItem*> m_messageControls;

    void RefreshMessages(); // Render lại toàn bộ tin nhắn

    CString m_targetUser; // ✅ Thêm biến để lưu tên người dùng
    HBITMAP m_targetAvatar;

    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    afx_msg LRESULT OnRefreshMessages(WPARAM wParam, LPARAM lParam);
    virtual BOOL OnInitDialog();

    // ✅ Thêm các message handlers
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnSize(UINT nType, int cx, int cy);

    // ✅ Thêm helper methods
    void UpdateScrollInfo();
    void ScrollToPosition(int newPos);
    void ScrollToBottom();
    int GetMaxScrollPos();

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
    afx_msg void OnEnChangeEdit1();
    afx_msg void OnBnClickedButton3();
    afx_msg void OnEnChangeEditInputMsg();
    afx_msg void OnBnClickedButtonIcon();
    afx_msg void OnBnClickedButtonImg();
    afx_msg void OnBnClickedButtonFile();

    

    

private:
    // ✅ Thêm các biến cho scroll
    int m_totalContentHeight;    // Tổng chiều cao nội dung
    int m_scrollPos;            // Vị trí scroll hiện tại
    int m_clientHeight;         // Chiều cao client area
    bool m_isScrollEnabled;     // Có cần scroll không

    // ✅ Thêm các constants
    static const int SCROLL_LINE_SIZE = 20;  // Số pixel scroll mỗi lần
    static const int SCROLL_PAGE_SIZE = 100; // Số pixel scroll khi nhấn page up/down
public:
    CEdit edt_inp_msg;
    CButton btn_icon;
    CButton btn_img;
    CButton btn_file;
    afx_msg void OnBnClickedButton6();
};

