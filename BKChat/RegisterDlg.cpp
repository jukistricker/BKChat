// RegisterDlg.cpp : implementation file
//

#include "pch.h"
#include "BKChat.h"
#include "afxdialogex.h"
#include "RegisterDlg.h"


// RegisterDlg dialog

IMPLEMENT_DYNAMIC(RegisterDlg, CDialogEx)

RegisterDlg::RegisterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REGISTER_DIALOG, pParent)
{

}

RegisterDlg::~RegisterDlg()
{
}

void RegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_REG_USERNAME, edt_reg_username_);
	DDX_Control(pDX, IDC_EDIT_REG_PW, edt_reg_pw_);
	DDX_Control(pDX, IDC_EDIT_REG_RETYPE_PW, edt_reg_retype_pw_);
	DDX_Control(pDX, IDC_BUTTON_CREATE_ACCOUNT, btn_reg_create_account_);
	DDX_Control(pDX, IDC_STATIC_REG_USERNAME, idc_reg_username_);
	DDX_Control(pDX, IDC_STATIC_REG_PW, idc_reg_pw_);
	DDX_Control(pDX, IDC_STATIC_REG_RETYPEPW, idc_retype_pw_);
	DDX_Control(pDX, IDC_ERR_REG, idc_er_reg_);
}


BEGIN_MESSAGE_MAP(RegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &RegisterDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_REG_USERNAME, &RegisterDlg::OnEnChangeEditRegUsername)
	ON_EN_CHANGE(IDC_EDIT_REG_RETYPE_PW, &RegisterDlg::OnEnChangeEditRegRetypePw)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_REG_RETYPEPW, &RegisterDlg::OnStnClickedStaticRegRetypepw)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_ACCOUNT, &RegisterDlg::OnBnClickedButtonCreateAccount)
END_MESSAGE_MAP()

BOOL RegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Tạo font Roboto 20pt
	_font_global.CreateFont(
		-20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("Roboto")
	);

	// Danh sách ID control có chữ
	int controlIDs[] = {
		IDC_STATIC_REG_USERNAME,
		IDC_STATIC_REG_PW,
		IDC_STATIC_REG_RETYPEPW,
		IDC_BUTTON_CREATE_ACCOUNT,

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

HBRUSH RegisterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	const int id = pWnd->GetDlgCtrlID();

	if (id == IDC_ERR_REG) {
		pDC->SetTextColor(RGB(236, 70, 34)); // Màu chữ
		pDC->SetBkMode(TRANSPARENT);
	}

	// Các control khác thì trả về nền trắng (hoặc mặc định)
	return (HBRUSH)GetStockObject(WHITE_BRUSH);
}

void RegisterDlg::OnPaint()
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

	}
	else
	{
		CDialogEx::OnPaint();
	}
}



// RegisterDlg message handlers

void RegisterDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

void RegisterDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void RegisterDlg::OnEnChangeEditRegUsername()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void RegisterDlg::OnEnChangeEditRegRetypePw()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void RegisterDlg::OnStnClickedStaticRegRetypepw()
{
	// TODO: Add your control notification handler code here
}

void RegisterDlg::OnBnClickedButtonCreateAccount()
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	CString username, password, cfpassword;
	edt_reg_username_.GetWindowText(username);
	edt_reg_pw_.GetWindowText(password);
	edt_reg_retype_pw_.GetWindowText(cfpassword);
	if (username.IsEmpty()) {
		idc_er_reg_.SetWindowText(_T("Tên đăng nhập không được để trống"));
		return;
	}
	else if (password.IsEmpty()) {
		idc_er_reg_.SetWindowText(_T("Mật khẩu không được để trống"));
		return;
	}
	else if (cfpassword.IsEmpty()) {
		idc_er_reg_.SetWindowText(_T("Vui lòng xác nhận mật khẩu"));
		return;
	}
	else if (password != cfpassword) {
		idc_er_reg_.SetWindowText(_T("Mật khẩu không khớp !"));
		return;
	}
	else if (username == "admin" && password == "12345678"&&password==cfpassword) {
		idc_er_reg_.SetWindowText(_T("Tài khoản đã tồn tại !"));
		return;
	}
	else{
		idc_er_reg_.SetWindowText(_T(""));
		return;
	}
	UpdateData(FALSE);
}
