#include "pch.h"
#include "CRoundedAvatar.h"
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;


BEGIN_MESSAGE_MAP(CRoundedAvatar, CStatic)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CRoundedAvatar::SetAvatar(HBITMAP hBitmap)
{
    m_hBitmap = hBitmap;
    Invalidate(); // yêu cầu vẽ lại
}

void CRoundedAvatar::OnPaint()
{
    CPaintDC dc(this);
    CRect rect;
    GetClientRect(&rect);

    CDC memDC;
    memDC.CreateCompatibleDC(&dc);

    if (m_hBitmap)
    {
        // Chuyển HBITMAP sang GDI+ Bitmap
        Bitmap* bmp = Gdiplus::Bitmap::FromHBITMAP(m_hBitmap, NULL);
        if (bmp)
        {
            Graphics g(dc);
            GraphicsPath path;
            path.AddEllipse(0, 0, rect.Width(), rect.Height());
            g.SetClip(&path);

            g.DrawImage(bmp, 0, 0, rect.Width(), rect.Height());

            delete bmp;
        }
    }
}
