#include "pch.h"
#include "CRoundEdit.h"
void CRoundEdit::OnNcPaint()
{
    CWindowDC dc(this);
    CRect rect;
    GetWindowRect(&rect);
    rect.OffsetRect(-rect.left, -rect.top);

    // Vẽ border tròn
    CBrush brush(RGB(68, 68, 72));
    CPen pen(PS_SOLID, 2, RGB(68, 68, 72));
    CBrush* pOldBrush = dc.SelectObject(&brush);
    CPen* pOldPen = dc.SelectObject(&pen);

    dc.RoundRect(&rect, CPoint(10, 10));

    dc.SelectObject(pOldBrush);
    dc.SelectObject(pOldPen);
}

void CRoundEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
    // Gọi hàm gốc nếu cần
    CEdit::OnNcCalcSize(bCalcValidRects, lpncsp);

    if (bCalcValidRects && lpncsp) {
        lpncsp->rgrc[0].left += 10;
        lpncsp->rgrc[0].top += 5;
        lpncsp->rgrc[0].right -= 10;
        lpncsp->rgrc[0].bottom -= 5;
    }
}




BEGIN_MESSAGE_MAP(CRoundEdit, CEdit)
    ON_WM_NCPAINT()
    ON_WM_NCCALCSIZE()
END_MESSAGE_MAP()