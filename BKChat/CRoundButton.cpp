#include "pch.h"
#include "CRoundButton.h"
void CRoundButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect = lpDrawItemStruct->rcItem;

    // Màu nền
    COLORREF bgColor = m_bHover ? RGB(70, 70, 74) : RGB(58, 58, 62);
    COLORREF textColor = RGB(255, 255, 255);

    // Vẽ background tròn
    CBrush brush(bgColor);
    CPen pen(PS_SOLID, 1, bgColor);
    CBrush* pOldBrush = pDC->SelectObject(&brush);
    CPen* pOldPen = pDC->SelectObject(&pen);

    pDC->RoundRect(&rect, CPoint(8, 8));

    // Vẽ text
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(textColor);

    CString text;
    GetWindowText(text);
    pDC->DrawText(text, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
}

void CRoundButton::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bHover)
    {
        m_bHover = TRUE;
        Invalidate();

        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hWnd;
        TrackMouseEvent(&tme);
    }
    CButton::OnMouseMove(nFlags, point);
}

void CRoundButton::OnMouseLeave()
{
    m_bHover = FALSE;
    Invalidate();
    CButton::OnMouseLeave();
}

BEGIN_MESSAGE_MAP(CRoundButton, CButton)
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()