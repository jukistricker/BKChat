// CChatDialog.cpp : implementation file
//

#include "pch.h"
#include "BKChat.h"
#include "afxdialogex.h"
#include "CChatDialog.h"


// CChatDialog dialog

IMPLEMENT_DYNAMIC(CChatDialog, CDialogEx)

CChatDialog::CChatDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHAT_DIALOG, pParent)
{

}

CChatDialog::~CChatDialog()
{
}

void CChatDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChatDialog, CDialogEx)
END_MESSAGE_MAP()


// CChatDialog message handlers
void CChatDialog::SetTargetUser(const CString& username)
{
	m_targetUser = username;
	// Có thể gọi SetDlgItemText để hiển thị tên trong dialog nếu cần
	SetWindowText(username);

}
