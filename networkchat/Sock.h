#pragma once
#include "afxsock.h"
#include "StdAfx.h"
#include "Sock.h"
#include "networkchat.h"
#include "networkchatDlg.h"


class Sock :
	public CAsyncSocket
{
public:
WORD total_msgs, msg_number;
CString contacts[50][3];
void OnReceive(int nErrorCode);
CDialog *pointer;
};