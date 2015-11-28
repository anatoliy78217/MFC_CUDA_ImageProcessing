// oglMFCDialogDlg.cpp : implementation file
//
#include "stdafx.h"
#include "oglMFCDialog.h"
#include "oglMFCDialogDlg.h"
#include ".\oglmfcdialogdlg.h"

#include "afxwinappex.h"
#include "afxdialogex.h"

#include <stdio.h>
#include <string.h>

#include "Utility.h"

//#include "opencv2/opencv.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;

// CoglMFCDialogDlg dialog
CoglMFCDialogDlg::CoglMFCDialogDlg(CWnd* pParent /*=NULL*/) : CDialog(CoglMFCDialogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CoglMFCDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CoglMFCDialogDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_COMMAND(IDD_ABOUTBOX, &CoglMFCDialogDlg::OnAboutbox)
	ON_COMMAND(ID_FILE_EXIT, &CoglMFCDialogDlg::OnFileExit)
	ON_COMMAND(ID_FILE_OPEN, &CoglMFCDialogDlg::OnFileOpen)
END_MESSAGE_MAP()

//BEGIN_MESSAGE_MAP(CoglMFCDialogDlg, CWinApp)
//	ON_COMMAND(ID_APP_ABOUT, &CoglMFCDialogDlg::OnAboutbox)
//END_MESSAGE_MAP()

// CoglMFCDialogDlg message handlers
BOOL CoglMFCDialogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	/***************/
	/* ADDED CODE: */
	/***************/
	CRect rect;

	// Get size and position of the template textfield we created before in the dialog editor
	GetDlgItem(IDC_OPENGL)->GetWindowRect(rect);

	// Convert screen coordinates to client coordinates
	ScreenToClient(rect);

	// Create OpenGL Control window
	m_oglWindow.oglCreate(rect, this);

	// Setup the OpenGL Window's timer to render
	m_oglWindow.m_unpTimer = m_oglWindow.SetTimer(1, 1, 0);


	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CoglMFCDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CoglMFCDialogDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CoglMFCDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CoglMFCDialogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
    
	switch (nType)
	{
		case SIZE_RESTORED:
		{
			if (m_oglWindow.m_bIsMaximized)
			{
				m_oglWindow.OnSize(nType, cx, cy);
				m_oglWindow.m_bIsMaximized = false;
			}

			break;
		}

		case SIZE_MAXIMIZED:
		{
			m_oglWindow.OnSize(nType, cx, cy);
			m_oglWindow.m_bIsMaximized = true;

			break;
		}
	}
}



CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{

	/*CWnd *opencvVersion = GetDlgItem(IDC_OPENCV_VERSION);
	opencvVersion->SetWindowText(_T("%s", CV_VERSION));*/

	/*SetDlgItemText(IDC_OPENCV_VERSION, _T("OpenCV Version %f", CV_VERSION));*/
	//SetDlgItemText(IDC_OPENCV_VERSION, _T("OpenCV Version "));
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CoglMFCDialogDlg::OnAboutbox()
{
	// TODO: Add your command handler code here

	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


void CoglMFCDialogDlg::OnFileExit()
{
	// TODO: Add your command handler code here
	ASSERT(AfxGetMainWnd() != NULL);
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}


void CoglMFCDialogDlg::OnFileOpen()
{
	// TODO: Add your command handler code here

	CFileDialog dlg(TRUE);

	CString filename;
	dlg.DoModal();
	filename = dlg.GetPathName(); // return full path and filename

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(filename);
	// construct a std::string using the LPCSTR input
	std::string filePath(pszConvertedAnsiString);

	//Utility util;
	Utility::SetImagePath(filePath);
	
	
	//string tmp = Utility::GetImagePath();


}
