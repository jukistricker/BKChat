// CHomeDialog.cpp : implementation file
//

#include "pch.h"
#include "BKChat.h"
#include "afxdialogex.h"
#include "CHomeDialog.h"


// CHomeDialog dialog

IMPLEMENT_DYNAMIC(CHomeDialog, CDialogEx)

CHomeDialog::CHomeDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HOME_DIALOG, pParent)
{

}

BOOL CHomeDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Tạo container
    m_friendContainer.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL, CRect(10, 50, 350, 500), this, 999);

    LoadFriendList();

    return TRUE;
}


CHomeDialog::~CHomeDialog()
{
}

void CHomeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	

}


BEGIN_MESSAGE_MAP(CHomeDialog, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_REG_PW, &CHomeDialog::OnStnClickedStaticRegPw)
    ON_WM_ERASEBKGND()
	
END_MESSAGE_MAP()


// CHomeDialog message handlers

void CHomeDialog::OnStnClickedStaticRegPw()
{
	// TODO: Add your control notification handler code here
}

void CHomeDialog::OnBnClickedFriendInf()
{
	// TODO: Add your control notification handler code here
}

void CHomeDialog::OnStnClickedInfUsername()
{
	// TODO: Add your control notification handler code here
}

void CHomeDialog::OnStnClickedAvatar()
{
	// TODO: Add your control notification handler code here
}

void CHomeDialog::LoadFriendList()
{
    // Giả lập dữ liệu
    struct FriendData { CString name; CString avatarPath; };
    CArray<FriendData, FriendData> friendList;
    friendList.Add({ _T("Alice"), _T("res\\avt1.bmp") });
    friendList.Add({ _T("Bob"), _T("res\\avt2.bmp") });
    friendList.Add({ _T("Carol"), _T("res\\bitmap1.bmp") });

    const int itemHeight = 60;
    const int width = 410;
    int y = 10;

    for (int i = 0; i < friendList.GetSize(); ++i)
    {
        CFriendItem* pItem = new CFriendItem();
        HBITMAP bmp = LoadAvatarOrDefault(friendList[i].avatarPath);

        if (pItem->Create(&m_friendContainer, 10, y, width, itemHeight, friendList[i].name, bmp))
        {
            m_friendItems.Add(pItem);
            y += itemHeight + 5;
        }
        else
        {
            delete pItem;
        }
    }

    m_friendContainer.MoveWindow(0, 0, width + 20, y);
}

HBITMAP CHomeDialog::LoadAvatarOrDefault(const CString& path)
{
    HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!hBitmap)
    {
        // Dùng ảnh mặc định đã import vào Resource
        hBitmap = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_DEFAULT_AVT));
    }
    return hBitmap;
}

void CHomeDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    int nDelta = 0;
    SCROLLINFO si = { sizeof(si) };
    si.fMask = SIF_ALL;
    GetScrollInfo(SB_VERT, &si);

    int oldPos = si.nPos;

    switch (nSBCode)
    {
    case SB_LINEUP:
        nDelta = -20;
        break;
    case SB_LINEDOWN:
        nDelta = 20;
        break;
    case SB_PAGEUP:
        nDelta = -(int)si.nPage;
        break;
    case SB_PAGEDOWN:
        nDelta = (int)si.nPage;
        break;
    case SB_THUMBTRACK:
        nDelta = (int)nPos - (int)si.nPos;
        break;
    default:
        return;
    }


    int newPos = max(0, min(si.nMax, si.nPos + nDelta));
    SetScrollPos(SB_VERT, newPos);

    int deltaY = oldPos - newPos;

    m_friendContainer.SetWindowPos(NULL, 10, 50 + deltaY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

BOOL CHomeDialog::OnEraseBkgnd(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);
    CBrush myBrush(RGB(255, 255, 255));    // dialog background color
    CBrush* pOld = pDC->SelectObject(&myBrush);
    BOOL bRes = pDC->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOld);    // restore old brush
    return bRes;                       // CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CHomeDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

    // Các control khác thì trả về nền trắng (hoặc mặc định)
    return (HBRUSH)GetStockObject(WHITE_BRUSH);
}