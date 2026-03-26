// networkchatDlg.cpp : implementation file
//

#pragma once
#include "stdafx.h"
#include "networkchat.h"
#include "networkchatDlg.h"
#include "Sock.h"
#include "math.h"

#define MAX_LENGTH 1460


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Sock gListenSock;
int messageID = 0;


CnetworkchatDlg::CnetworkchatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CnetworkchatDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CnetworkchatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, bn_send);
	DDX_Control(pDX, IDC_EDIT1, text_log);
	DDX_Control(pDX, IDC_EDIT2, text_message);
	DDX_Control(pDX, IDC_LIST1, list_contacts);
	DDX_Control(pDX, IDC_EDIT3, text_nickname);
	DDX_Control(pDX, IDC_EDIT4, text_port);
	DDX_Control(pDX, IDC_BUTTON2, bn_save);
	DDX_Control(pDX, IDC_IPADDRESS1, text_ip);
	DDX_Control(pDX, IDC_EDIT5, text_sendport);
	DDX_Control(pDX, IDC_BUTTON3, bn_pridajip);
	DDX_Control(pDX, IDC_CHECK1, check_sum);
	DDX_Control(pDX, IDC_BUTTON4, bn_clear);
}

BEGIN_MESSAGE_MAP(CnetworkchatDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CnetworkchatDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CnetworkchatDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CnetworkchatDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CnetworkchatDlg::OnBnClickedButton4)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CnetworkchatDlg::OnNMClickList1)
	ON_EN_SETFOCUS(IDC_EDIT1, &CnetworkchatDlg::OnEnSetfocusEdit1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CnetworkchatDlg::OnNMRClickList1)
END_MESSAGE_MAP()


// CnetworkchatDlg message handlers

BOOL CnetworkchatDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// inicializacia zoznamu IP
	list_contacts.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	list_contacts.InsertColumn(0,"Prezývka",0,95,0);
	list_contacts.InsertColumn(1,"IP adresa",0,95,0);
	list_contacts.InsertColumn(2,"Port",0,42,0);
	


	// inicializacia nastaveni
	CString temp;
	nickname = "Nick";
	port = 1234;
	temp.Format("%d",port);
	text_nickname.SetWindowTextA(nickname);
	text_port.SetWindowTextA(temp);


	//inicializacia socketov
	WSAData wsadata;
	HOSTENT *HostEnt;

	int state = WSAStartup( MAKEWORD(2, 2), &wsadata);

	HostEnt = gethostbyname("");
	local_addr.S_un.S_un_b.s_b1 = HostEnt->h_addr[0];
	local_addr.S_un.S_un_b.s_b2 = HostEnt->h_addr[1];
	local_addr.S_un.S_un_b.s_b3 = HostEnt->h_addr[2];
	local_addr.S_un.S_un_b.s_b4 = HostEnt->h_addr[3];

	temp.Format("%s:%d",inet_ntoa(local_addr),port);
	SetWindowText("Network Chat - " + nickname + " [running @ " + temp + "]");

	text_ip.SetAddress(HostEnt->h_addr[0],HostEnt->h_addr[1],HostEnt->h_addr[2],HostEnt->h_addr[3]);
	text_sendport.SetWindowTextA("1234");

	PrepareListen();

	text_message.SetFocus();

	return FALSE;  // return TRUE  unless you set the focus to a control
}




// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CnetworkchatDlg::OnPaint()
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
HCURSOR CnetworkchatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CnetworkchatDlg::PrepareListen(){
	gListenSock.Create(port, SOCK_DGRAM, FD_READ, NULL);
	gListenSock.pointer = this;
}

CString CnetworkchatDlg::getTime(){
	CString time;
	CTime t = CTime::GetCurrentTime();
	time.Format("%d:%.2d:%.2d",t.GetHour(),t.GetMinute(),t.GetSecond());
	return time;
}

WORD getChecksum(CString data){
	WORD word;	//16b
	DWORD sum;	//32b
	int length = data.GetLength();

	if ( length %2 == 1 ) {
		data.Append(0);
		length++;
	}
	sum = 0;

	for (int i=0;i<length;i=i+2){
		word = data[i]<<8;
		word = word & 0xFF00;
		word += data[i+1];
		sum = sum + (WORD)word;
	}

	while (sum>>16) sum = (sum & 0xFFFF)+(sum >> 16);
	sum = ~sum;

	return ((WORD) sum);
}

void CnetworkchatDlg::OnBnClickedButton1() // Send message
{
	if(text_message.GetWindowTextLengthA() > 0){
		CString message,text,str,dest_ip;
		int i,total_msgs, msg_num;
		BYTE header[12];
		WORD checksum,sent_checksum;
		CString sent_message;

		text_ip.GetWindowTextA(dest_ip);
		text_sendport.GetWindowTextA(str);

		// Type
		header[0] = 1;
		// Length
		header[1] = text_message.GetWindowTextLengthA()/255;
		header[2] = text_message.GetWindowTextLengthA()%255;
		// Flagy
		header[3] = 0;
		text_message.GetWindowTextLengthA()>1460?header[3]|=1:FALSE;
		// ID
		header[4] = messageID/255;
		header[5] = messageID%255;
		messageID++;

		text_message.GetWindowTextA(message);

		// Total number of packets
		total_msgs = ceil((double)message.GetLength()/MAX_LENGTH);
		header[8] = (int)(total_msgs/255);
		header[9] = (int)(total_msgs%255);

		// Rozdelenie správy na segmenty
		for(msg_num = 0; msg_num<total_msgs; msg_num++){ //message obsahuje spravu na odoslanie

			// Packet Number
			header[6] = msg_num/255;
			header[7] = msg_num%255;

			// Checksum
			header[10] = 0;
			header[11] = 0;
			sent_message = "";
			for(i=0; i<13; i++) sent_message.AppendFormat("%c",header[i]);
			sent_message.Append(message.Mid(msg_num*MAX_LENGTH,MAX_LENGTH));
			checksum = getChecksum(sent_message);
			if(check_sum.GetCheck() == 0){
				sent_checksum = checksum;
				header[10] = HIBYTE(checksum);
				header[11] = LOBYTE(checksum);
			} else {
				sent_checksum = checksum+16;
				header[10] = HIBYTE(sent_checksum);
				header[11] = LOBYTE(sent_checksum);
			}

			// Nove skomponovanie spravy so spravnym checksumom
			sent_message = "";
			for(i=0; i<13; i++) sent_message.AppendFormat("%c",header[i]);
			sent_message.Append(message.Mid(msg_num*MAX_LENGTH,MAX_LENGTH));

			// Odoslanie
			sendNickname();
			i = gListenSock.SendTo(sent_message,sent_message.GetLength(),atoi(str),dest_ip);
			if(i == SOCKET_ERROR) MessageBox("Chyba pri posielani","Chyba",0);

		}
		text_log.GetWindowTextA(text);
		text_message.GetWindowTextA(message);
		text.AppendFormat("(" + getTime() + ") Odoslaných %d znakov\r\n" + nickname + " (%s:%d)\t[Checksum: odoslaný 0x%.4Xh && vypoèítaný 0x%.4Xh]\r\n" + message + "\r\n\r\n",message.GetLength(),inet_ntoa(local_addr),port,sent_checksum,checksum);
		text_log.SetWindowTextA((LPCTSTR)text);
		text_log.LineScroll(text_log.GetLineCount(),0);
		text_message.SetWindowTextA("");
		text_message.SetFocus();
	} else MessageBox("Nejde posla prázdnu správu","Oznam",0);
}

void CnetworkchatDlg::sendNickname(){
	CString message, IP, port;
	BYTE header[12];
	WORD checksum;
	int i;

	text_ip.GetWindowTextA(IP);
	text_sendport.GetWindowTextA(port);
	header[0] = 0;
	header[1] = nickname.GetLength()/255;
	header[2] = nickname.GetLength()%255;
	header[3] = 0;
	header[4] = 0;
	header[5] = 0;
	header[6] = 0;
	header[7] = 0;
	header[8] = 0;
	header[9] = 0;
	header[10] = 0;
	header[11] = 0;
	
	message = "";
	for(i=0; i<13; i++) message.AppendFormat("%c",header[i]);
	message.Append(nickname);
	checksum = getChecksum(message);
	header[10] = HIBYTE(checksum);
	header[11] = LOBYTE(checksum);
	message = "";
	for(i=0; i<13; i++) message.AppendFormat("%c",header[i]);
	message.Append(nickname);

	i = gListenSock.SendTo(message,message.GetLength(),atoi(port),IP);
	if(i == SOCKET_ERROR) MessageBox("Chyba pri posielani","Chyba",0);
}

void CnetworkchatDlg::OnBnClickedButton2() // Settings
{
	CString temp;

	text_nickname.GetWindowTextA(nickname);
	text_port.GetWindowTextA(temp);
	port = atoi(temp);
	temp.Format("%s:%d",inet_ntoa(local_addr),port);
	SetWindowText("Network Chat - " + nickname + " [running @ " + temp + "]");
	gListenSock.Close();
	PrepareListen();
	sendNickname();
	

}

void CnetworkchatDlg::OnBnClickedButton3() // Contact list
{
	CString addIP,temp;

	text_ip.GetWindowTextA(addIP);
	if(text_sendport.GetWindowTextLengthA() != 0)	{
		text_sendport.GetWindowTextA(temp);
		addIP.Append(":"+temp);
	} else addIP.Append(":1234");
	list_contacts.InsertItem(0,"???");
	list_contacts.SetItemText(0,1,addIP);
	text_ip.ClearAddress();
	text_sendport.SetWindowTextA("");
}

void CnetworkchatDlg::OnBnClickedButton4()
{
	text_log.SetWindowTextA("");
	text_message.SetWindowTextA("");
}

void CnetworkchatDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	POSITION select = list_contacts.GetFirstSelectedItemPosition();
	int number = list_contacts.GetNextSelectedItem(select);
	CString str = list_contacts.GetItemText(number, 1);
	text_ip.SetWindowTextA(str);
	str = list_contacts.GetItemText(number,2);
	text_sendport.SetWindowTextA(str);
	*pResult = 0;
}

void CnetworkchatDlg::OnEnSetfocusEdit1()
{
	// TODO: Add your control notification handler code here
//	text_message.SetFocus();
}

void CnetworkchatDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	POSITION select = list_contacts.GetFirstSelectedItemPosition();
	int number = list_contacts.GetNextSelectedItem(select);
	CString str = list_contacts.GetItemText(number, 1);
	list_contacts.DeleteItem(number);	
	gListenSock.contacts[number][0] = "";
	gListenSock.contacts[number][1] = "";
	gListenSock.contacts[number][2] = "";

	*pResult = 0;
}
