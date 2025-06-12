// FriendItem.h
#pragma once
#include "afxwin.h"

class CFriendItem : public CWnd
{
public:
    CFriendItem();
    virtual ~CFriendItem();

    BOOL Create(CWnd* pParent, int x, int y, int width, int height, const CString& name, HBITMAP hAvatar);
    void SetHovered(bool hovered);

    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);


    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    void SetUser(const CString& username) { m_username.SetWindowText(username); }
  

protected:
    CStatic m_groupBox;
    CStatic m_avatar;
    CStatic m_username;

    bool m_hovered = false;
    bool m_tracking = false;

};
