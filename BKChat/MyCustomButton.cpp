#include "pch.h"
#include "MyCustomButton.h"

IMPLEMENT_DYNAMIC(MyCustomButton, CButton)

BEGIN_MESSAGE_MAP(MyCustomButton, CButton)
    // Thêm message nếu cần, ví dụ: ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void MyCustomButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC dc;
    dc.Attach(lpDrawItemStruct->hDC);

    UINT state = lpDrawItemStruct->itemState;
    CRect rect = lpDrawItemStruct->rcItem;

    COLORREF clrFace = RGB(20, 106, 224);
    if (state & ODS_SELECTED) {
        clrFace = RGB(10, 60, 140);
    }
    else if (state & ODS_FOCUS) {
        clrFace = RGB(25, 120, 250);
    }

    dc.FillSolidRect(rect, RGB(255, 255, 255));  // Clear background

    CRgn rgn;
    rgn.CreateRoundRectRgn(rect.left, rect.top, rect.right, rect.bottom, 2, 2);
    CBrush brush(clrFace);
    dc.FillRgn(&rgn, &brush);

    CString strText;
    GetWindowText(strText);

    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(RGB(255, 255, 255));
    dc.DrawText(strText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    dc.Detach();
}
