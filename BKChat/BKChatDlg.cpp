
// BKChatDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "BKChat.h"
#include "BKChatDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "RegisterDlg.h"
#include "CHomeDialog.h"


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBKChatDlg dialog



CBKChatDlg::CBKChatDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BKCHAT_DIALOG, pParent)
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBKChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USERNAME, edt_username_);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, edt_password_);
	DDX_Control(pDX, IDC_REMEMBER_ACC, chk_remember_);
	DDX_Control(pDX, IDC_BUTTON_LOGIN, btn_login_);
	DDX_Control(pDX, IDC_BUTTON_TO_REGISTER, btn_to_register_);
	DDX_Control(pDX, IDC_BKAV_CHAT, _idc_bkav_chat_);
	DDX_Control(pDX, IDC_USERNAME, _idc_username_);
	DDX_Control(pDX, IDC_PASSWORD, _idc_password_);
	DDX_Control(pDX, IDC_ERR_LOGIN, idc_err_login_);
}

BEGIN_MESSAGE_MAP(CBKChatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()

	ON_WM_NCPAINT()


	ON_EN_CHANGE(IDC_EDIT_USERNAME, &CBKChatDlg::OnEnChangeEditUsername)
	ON_EN_CHANGE(IDC_EDIT_PASSWORD, &CBKChatDlg::OnEnChangeEditPassword)
	ON_BN_CLICKED(IDC_REMEMBER_ACC, &CBKChatDlg::OnBnClickedRememberAcc)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CBKChatDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_TO_REGISTER, &CBKChatDlg::OnBnClickedButtonToRegister)
	ON_WM_NCPAINT()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CBKChatDlg message handlers

BOOL CBKChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Tạo font Roboto 24px, Bold cho text IDC_BKAV_CHAT
	_font_bkav_chat.CreateFont(
		24,                        // Chiều cao font (point size)
		0,                         // Chiều rộng (0 = tự động)
		0, 0,                      // Góc nghiêng (escapement, orientation)
		FW_BOLD,                  // Độ đậm (FW_BOLD = in đậm)
		FALSE, FALSE, FALSE,      // Italic, underline, strikeout
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		_T("Roboto")              // Tên font
	);

	// Gán font cho control
	CStatic* pBkavChat = (CStatic*)GetDlgItem(IDC_BKAV_CHAT);
	if (pBkavChat) {
		pBkavChat->SetFont(&_font_bkav_chat);
	}

	

	// Tạo font Roboto 20pt
	_font_global.CreateFont(
		-20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("Roboto")
	);

	// Danh sách ID control có chữ
	int controlIDs[] = {
		IDC_USERNAME,
		IDC_PASSWORD,
		IDC_REMEMBER_ACC,
		IDC_BUTTON_LOGIN,
		IDC_BUTTON_TO_REGISTER
	};

	// Duyệt từng ID và gán font
	for (int i = 0; i < sizeof(controlIDs) / sizeof(int); ++i) {
		CWnd* pCtrl = GetDlgItem(controlIDs[i]);
		if (pCtrl) {
			pCtrl->SetFont(&_font_global);
		}
	}


	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	//SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon
	SetIcon(NULL, TRUE);  // Xóa icon lớn
	SetIcon(NULL, FALSE); // Xóa icon nhỏ


	SendMessage(WM_SETICON, ICON_SMALL, 0);
	SendMessage(WM_SETICON, ICON_BIG, 0);
	//// TODO: Add extra initialization here
	//LONG style = GetWindowLong(m_hWnd, GWL_STYLE);
	//style &= ~(WS_MINIMIZEBOX | WS_MAXIMIZEBOX); // Bỏ nút thu nhỏ / phóng to
	//SetWindowLong(m_hWnd, GWL_STYLE, style);


	LONG exStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	exStyle |= WS_EX_DLGMODALFRAME;
	SetWindowLong(m_hWnd, GWL_EXSTYLE, exStyle);

	SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}
HBRUSH CBKChatDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	const int id = pWnd->GetDlgCtrlID();

	if (id == IDC_BKAV_CHAT) // static header
	{
		pDC->SetTextColor(RGB(20, 106, 224)); // Màu chữ
		pDC->SetBkMode(TRANSPARENT);          // Nền trong suốt
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}
	if (id == IDC_ERR_LOGIN) {
		pDC->SetTextColor(RGB(236, 70, 34)); // Màu chữ
		pDC->SetBkMode(TRANSPARENT);
	}

	// Các control khác thì trả về nền trắng (hoặc mặc định)
	return (HBRUSH)GetStockObject(WHITE_BRUSH);
}



void CBKChatDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBKChatDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBKChatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBKChatDlg::OnNcPaint()
{
	CDialogEx::OnNcPaint();

	/*CWindowDC dc(this);
	CRect windowRect;
	GetWindowRect(&windowRect);
	int width = windowRect.Width();

	int titleHeight = ::GetSystemMetrics(SM_CYCAPTION);
	CRect titleRect(0, 0, width, titleHeight);*/

}

LRESULT CBKChatDlg::OnNcHitTest(CPoint point)
{
	// Cho phép kéo cửa sổ bằng thanh tiêu đề tùy chỉnh
	LRESULT hit = CDialogEx::OnNcHitTest(point);
	if (hit == HTCAPTION)
		return HTCAPTION;
	return hit;
}


void CBKChatDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CBKChatDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}

void CBKChatDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
}

void CBKChatDlg::OnEnChangeEditUsername()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CBKChatDlg::OnEnChangeEditPassword()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CBKChatDlg::OnBnClickedRememberAcc()
{
	// TODO: Add your control notification handler code here
}
BOOL CBKChatDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnBnClickedButtonLogin();
		return TRUE; 
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CBKChatDlg::OnBnClickedButtonLogin()
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	CString username,password;
	edt_username_.GetWindowText(username);
	edt_password_.GetWindowText(password);
	if (username.IsEmpty()) {

		idc_err_login_.SetWindowText(_T("Tên đăng nhập không được để trống"));
		return;
	}
	else if (password.IsEmpty()) {
		idc_err_login_.SetWindowText(_T("Mật khẩu không được để trống"));
		return;
	}
	else if (username != "admin" && password != "12345678") {
		idc_err_login_.SetWindowText(_T("Bạn nhập sai tên tài khoản hoặc mật khẩu!"));
		return;
	}
	else {
		idc_err_login_.SetWindowText(_T(""));

		// Ẩn dialog hiện tại
		ShowWindow(SW_HIDE);

		// Tạo và hiển thị Home Dialog
		CHomeDialog homeDlg;
		homeDlg.DoModal();

		// Sau khi đóng Home Dialog, thoát khỏi login dialog
		EndDialog(IDOK);
		
	}
	UpdateData(FALSE);

}

void CBKChatDlg::OnBnClickedButtonToRegister()
{
	// TODO: Add your control notification handler code here
	RegisterDlg register_dlg;
	register_dlg.DoModal();
}
