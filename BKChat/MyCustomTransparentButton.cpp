#include "pch.h"
#include "MyCustomTransparentButton.h"


IMPLEMENT_DYNAMIC(MyCustomTransparentButton, CButton)

BEGIN_MESSAGE_MAP(MyCustomTransparentButton, CButton)
    // Thêm message nếu cần, ví dụ: ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void MyCustomTransparentButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC dc;
    dc.Attach(lpDrawItemStruct->hDC);

    UINT state = lpDrawItemStruct->itemState;
    CRect rect = lpDrawItemStruct->rcItem;

    // Nền trắng
    COLORREF clrFace = RGB(255, 255, 255);

    // Clear nền trắng
    dc.FillSolidRect(rect, clrFace);

    // Không vẽ viền (bỏ đoạn tạo vùng và fill rgn)

    // Text màu xanh #047DE7
    CString strText;
    GetWindowText(strText);

    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(RGB(4, 125, 231));
    dc.DrawText(strText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    dc.Detach();
}

