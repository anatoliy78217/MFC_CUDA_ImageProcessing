// oglMFCDialogDlg.h : header file
//
#pragma once

#include "OpenGLControl.h"


#include "afxwinappex.h"
#include "afxdialogex.h"

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

class CoglMFCDialogDlg : public CDialog
{
	private:
		COpenGLControl m_oglWindow;

	// Construction
	public:
		CoglMFCDialogDlg(CWnd* pParent = NULL);	// standard constructor

		// Dialog Data
		enum { IDD = IDD_OGLMFCDIALOG_DIALOG };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Implementation
	protected:
		HICON m_hIcon;

		// Generated message map functions
		virtual BOOL OnInitDialog();
		afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
		afx_msg void OnPaint();
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg HCURSOR OnQueryDragIcon();
		DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAboutbox();
	afx_msg void OnFileExit();
	afx_msg void OnFileOpen();
};



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	//DECLARE_MESSAGE_MAP()
public:
	//DECLARE_MESSAGE_MAP()
	//afx_msg void OnFileOpen();
	//afx_msg void OnAboutbox();
};


