// networkchatDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "StdAfx.h"
#include "Sock.h"
#include "networkchat.h"


// CnetworkchatDlg dialog
class CnetworkchatDlg : public CDialog
{
// Construction
public:
	CnetworkchatDlg(CWnd* pParent = NULL);	// standard constructor
	void PrepareListen();

// Dialog Data
	enum { IDD = IDD_NETWORKCHAT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString getTime();
	int port;
	CString nickname;
	in_addr local_addr;
	CButton bn_send;
	CEdit text_log;
	CEdit text_message;
	afx_msg void OnBnClickedButton1();
	CListCtrl list_contacts;
	CEdit text_nickname;
	CEdit text_port;
	CButton bn_save;
	afx_msg void OnBnClickedButton2();
	CIPAddressCtrl text_ip;
	CEdit text_sendport;
	CButton bn_addip;
	afx_msg void OnBnClickedButton3();
	CButton check_sum;
	afx_msg void OnBnClickedButton4();
	void sendNickname();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	CButton bn_clear;
	afx_msg void OnEnSetfocusEdit1();
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
