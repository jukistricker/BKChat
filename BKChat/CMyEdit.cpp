#include "pch.h"
#include "CMyEdit.h"
// MyEdit.cpp

CMyEdit::CMyEdit() {}
CMyEdit::~CMyEdit() {}

BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
    ON_WM_PAINT()
    ON_WM_NCPAINT()

END_MESSAGE_MAP()

void CMyEdit::OnPaint()
{
    CPaintDC dc(this);

    CRect rect;
    GetClientRect(&rect);

    dc.FillSolidRect(rect, RGB(255, 255, 255));

    DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0);

    CRgn rgn;
    rgn.CreateRoundRectRgn(rect.left, rect.top, rect.right + 1, rect.bottom + 1, 3, 3);

    CPen pen(PS_SOLID, 1, RGB(205, 205, 205));
    CPen* pOldPen = dc.SelectObject(&pen);
    dc.SelectClipRgn(&rgn);

    dc.RoundRect(rect, CPoint(3, 3));

    dc.SelectClipRgn(NULL);
    dc.SelectObject(pOldPen);


}

void CMyEdit::OnNcPaint()
{
    // Không gọi DefWindowProc(WM_NCPAINT) để tránh vẽ viền mặc định
}
