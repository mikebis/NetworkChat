#pragma once
#include "StdAfx.h"
#include "Sock.h"
#include "networkchat.h"
#include "networkchatDlg.h"

WORD getChecksum(BYTE* data, int length){
	WORD word;	//16b
	DWORD sum;	//32b

	if ( length %2 == 1 ) {
		data[length] = 0;
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


void Sock::OnReceive(int nErrorCode) {
	CString IP, str,nickname;
	byte* buffer = new byte[1501];
	byte* buffer_orig = buffer;
	int i,size;
	UINT port;
	CnetworkchatDlg *dialog = (CnetworkchatDlg*)pointer;
	WORD checksum,sent_checksum;
	bool error = FALSE, fragment = FALSE, found = FALSE;

	// prijem
	size = ReceiveFrom(buffer,1500,IP,port,0);
	if(size == SOCKET_ERROR) {
		str.Format("Nastala chyba v ReceiveFrom(...)\nKod chyby: %d",GetLastError());
		MessageBoxA(NULL,str,"Chyba",0);
	} else {

		if(size < 13) {
			delete[] buffer_orig;
			return;
		}

		if(buffer[0] == 1){

			msg_number = ((buffer[6]<<8) & 0xFF00) + buffer[7];
			total_msgs = ((buffer[8]<<8) & 0xFF00) + buffer[9];
			fragment = buffer[3]&1;

			buffer[size] = '\0';

			// zlozenie prijateho checksumu
			sent_checksum = buffer[10]<<8;
			sent_checksum &= 0xFF00;
			sent_checksum += buffer[11];
			buffer[10] = 0;
			buffer[11] = 0;

			// vypocitanie checksumu
			checksum = getChecksum(buffer,size);

			error = (checksum != sent_checksum);

			for(i=0; i<50; i++)
				if (IP.Compare(contacts[i][0]) == 0){
					nickname = contacts[i][1];
					found = TRUE;
					break;
				}

			// odstranenie hlavicky
			buffer = buffer + 13*sizeof(byte);
			dialog->text_log.GetWindowTextA(str);
			if(error) str.Append(":: CHYBA PRI PRENOSE ::\r\n\r\n");
			str.AppendFormat("(" + dialog->getTime() + ") Prijat�ch %d znakov",size-13);
			if(fragment) str.AppendFormat("\t\t\t\t\t\t [%d z %d]",msg_number+1,total_msgs);
			str.Append("\r\n");
			if(found) str.Append(nickname + " ");
			str.AppendFormat("(%s:%d)\t[Checksum: prijat� 0x%.4Xh && vypo��tan� 0x%.4Xh]\r\n",IP,port,sent_checksum,checksum);
			str.AppendFormat("%s\r\n\r\n",buffer);
			dialog->text_log.SetWindowTextA(str);
			dialog->text_log.LineScroll(dialog->text_log.GetLineCount(),0);
		} else if (buffer[0] == 0) {
			buffer[size] = '\0';
			buffer = buffer + 13*sizeof(byte);
			nickname.Format("%s",buffer);
			str.Format("%d",port);

			for(i=0; i<50; i++)
				if (IP.Compare(contacts[i][0]) == 0) {  // ak sa nasiel v zozname
					found = TRUE;
					if (nickname.Compare(contacts[i][1]) != 0) {
						contacts[i][1] = nickname;  // ak je iny nick pri ip tak sa opravi
						dialog->list_contacts.SetItemText(i,0,nickname);
						dialog->list_contacts.SetItemText(i,2,str);
					}
				}
				
			if (!found){  // pridanie do zoznamu
				for (i = 0; i<50; i++)
					if (contacts[i][0] == "") {
						contacts[i][0] = IP;
						contacts[i][1] = nickname;
						contacts[i][2] = str;
						dialog->list_contacts.InsertItem(i,nickname);
						dialog->list_contacts.SetItemText(i,1,IP);
						dialog->list_contacts.SetItemText(i,2,str);
						break;
					}
			}



		} 
		CAsyncSocket::OnReceive(nErrorCode);	
	}
	delete[] buffer_orig;
}