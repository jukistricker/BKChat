#include "pch.h"
#include "CFriendItem.h"
#include "CChatDialog.h"
#include "resource.h"

// CFriendItem.cpp


CFriendItem::CFriendItem() {}
CFriendItem::~CFriendItem() {}

BEGIN_MESSAGE_MAP(CFriendItem, CWnd)
    ON_WM_CTLCOLOR()
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_MESSAGE(WM_MOUSELEAVE, &CFriendItem::OnMouseLeave)
    ON_WM_SETCURSOR()


    ON_WM_LBUTTONDOWN()


END_MESSAGE_MAP()

BOOL CFriendItem::Create(CWnd* pParent, int x, int y, int width, int height, const CString& name, HBITMAP hAvatar)
{
    CRect rect(x, y, x + width, y + height);
    BOOL result = CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParent, 0);

    if (!result)
        return FALSE;

    // Avatar
    m_avatar.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_BITMAP, CRect(10, 5, 60, 55), this);
    m_avatar.SetBitmap(hAvatar);

    // Username
    m_username.Create(name, WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(70, 20, width - 10, 50), this);
    m_username.SetFont(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));

    return TRUE;
}

HBRUSH CFriendItem::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    static CBrush normalBrush(RGB(255, 255, 255));
    static CBrush hoverBrush(RGB(220, 240, 255));

    if (pWnd == &m_username)
    {
        pDC->SetBkMode(OPAQUE);
        pDC->SetBkColor(m_hovered ? RGB(220, 240, 255) : RGB(255, 255, 255));
        pDC->SetTextColor(RGB(0, 0, 0));
        return m_hovered ? (HBRUSH)hoverBrush : (HBRUSH)normalBrush;
    }

    return CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
}


void CFriendItem::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_tracking)
    {
        TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, m_hWnd };
        _TrackMouseEvent(&tme);
        m_tracking = true;
    }

    if (!m_hovered)
    {
        m_hovered = true;
        Invalidate();
        m_username.Invalidate(); // cập nhật nền username
    }

    CWnd::OnMouseMove(nFlags, point);
}

//bỏ màu sau khi rời hover
LRESULT CFriendItem::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
    m_hovered = false;
    m_tracking = false;
    Invalidate();
    m_username.Invalidate(); // cập nhật nền username
    return 0;
}

//custom màu hover
void CFriendItem::OnPaint()
{
    CPaintDC dc(this);
    CRect rect;
    GetClientRect(&rect);

    COLORREF bgColor = m_hovered ? RGB(220, 240, 255) : RGB(255, 255, 255);
    dc.FillSolidRect(&rect, bgColor);
}

//mở chatdialog
void CFriendItem::OnLButtonDown(UINT nFlags, CPoint point)  
{  
    CWnd::OnLButtonDown(nFlags, point);  
    CChatDialog* pChatDlg = new CChatDialog();
    CString username;
    m_username.GetWindowText(username);
    pChatDlg->Create(IDD_CHAT_DIALOG);   
    pChatDlg->SetTargetUser(username);   
    pChatDlg->ShowWindow(SW_SHOW);
}

//hiển thị cusor
BOOL CFriendItem::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_hovered)
    {
        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
        return TRUE;
    }

    return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

