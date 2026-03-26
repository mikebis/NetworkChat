#pragma once
#include "afxsock.h"
#include "StdAfx.h"
#include "Sock.h"
#include "networkchat.h"
#include "networkchatDlg.h"

typedef struct fragm_message{
	int id;
	int length;
	int total_fragm;
	int received;
//	CString *fragments[];
} FRAGM_MESSAGE;


class Sock :
	public CAsyncSocket
{
public:
//int port2;
//void PrepareListen2();
WORD total_msgs, msg_number;
CString contacts[50][2];
void Sock::OnReceive(int nErrorCode);
CDialog *pointer;
};