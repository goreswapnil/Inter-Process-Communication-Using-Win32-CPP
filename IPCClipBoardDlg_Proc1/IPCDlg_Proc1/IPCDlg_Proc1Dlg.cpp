
// IPCDlg_Proc1Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "IPCDlg_Proc1.h"
#include "IPCDlg_Proc1Dlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void WINAPI EventCreate(LPVOID lpParam);
LPCSTR strEventName_1 = TEXT("Global\\MyEventProc_1");
LPCSTR strEventName_2 = TEXT("Global\\MyEventProc_2");

// CAboutDlg dialog used for App About
CIPCDlgProc1Dlg* g_ptrCIPCDlgProc1Dlg;
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
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CIPCDlgProc1Dlg dialog



CIPCDlgProc1Dlg::CIPCDlgProc1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IPCDLG_PROC1_DIALOG, pParent)
	
	, Edit_Receive(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIPCDlgProc1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT2, m_edtToClipboard);
	DDX_Text(pDX, IDC_EDIT1, Edit_Receive);
	DDX_Control(pDX, IDC_EDIT1, m_edt1);
}


BEGIN_MESSAGE_MAP(CIPCDlgProc1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CIPCDlgProc1Dlg::OnEnChangeClipReci)
	ON_EN_CHANGE(IDC_EDIT2, &CIPCDlgProc1Dlg::OnEnChangeClipSend)
	ON_BN_CLICKED(IDOK, &CIPCDlgProc1Dlg::OnBnClickedClipButton)
END_MESSAGE_MAP()


// CIPCDlgProc1Dlg message handlers

BOOL CIPCDlgProc1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	g_ptrCIPCDlgProc1Dlg = this;

	m_hThread = CreateThread(
		NULL,              // default security
		0,                 // default stack size
		(LPTHREAD_START_ROUTINE)EventCreate,        // name of the thread function
		NULL,              // no thread parameters
		0,                 // default startup flags
		NULL);

	if (m_hThread == NULL)
	{
		//printf("CreateThread failed (%d)\n", GetLastError());
		return FALSE;
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIPCDlgProc1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIPCDlgProc1Dlg::OnPaint()
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
HCURSOR CIPCDlgProc1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CIPCDlgProc1Dlg::OnBnClickedClipButton()
{
	// TODO: Add your control notification handler code here
	

	if (UpdateData())
	{
		CString strData;
		m_edtToClipboard.GetWindowText(strData);


		OpenClipboard();
		
		HGLOBAL hClipboardData;
		hClipboardData = GlobalAlloc(GMEM_DDESHARE, strData.GetLength() + 1);

		char* pchData;
		pchData = (char*)GlobalLock(hClipboardData);

		StringCchCopyA(pchData, strData.GetLength() + 1, strData);

		GlobalUnlock(hClipboardData);
		m_hEvent_1 = OpenEvent(EVENT_ALL_ACCESS, FALSE, strEventName_2);
		if (m_hEvent_1 != NULL)
		{
			SetEvent(m_hEvent_1);

		}
		//EmptyClipboard();
		SetClipboardData(CF_TEXT, hClipboardData);
		CloseClipboard();
	}
}

void CIPCDlgProc1Dlg::OnEnChangeClipReci()
{
	// TODO:  Add your control notification handler code here
				
}


void CIPCDlgProc1Dlg::OnEnChangeClipSend()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
void CIPCDlgProc1Dlg::ChkEvent()
{
	if (OpenClipboard())
	{
		HANDLE hclipboard = GetClipboardData(CF_TEXT);
		if (hclipboard)
		{
			LPSTR dclipboardbuffer = (LPSTR)GlobalLock(hclipboard);
			//Edit_Receive = m_dclipboardbuffer;
			//UpdateData(false);
			m_edt1.SetWindowText(dclipboardbuffer);
			GlobalUnlock(hclipboard);
		}

		CloseClipboard();
		
	}
	//CloseHandle(hclipboard);
}


void WINAPI EventCreate(LPVOID lpParam)
{
	HANDLE hEvent_2 = CreateEvent(NULL, FALSE, FALSE, strEventName_1);//NAME of event
	if (hEvent_2 == NULL)
	{
		OutputDebugString("Event creation failed");
	}
	while (1)
	{
		DWORD RetVal = WaitForSingleObject(hEvent_2, INFINITE);

		if (WAIT_OBJECT_0 == RetVal)
		{
			g_ptrCIPCDlgProc1Dlg->ChkEvent();
			
		}
	}

}