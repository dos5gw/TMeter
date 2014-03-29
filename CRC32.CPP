#include "stdafx.h"
# include "CRC32.h"
DWORD crc32_table[256];

void WriteCrcTable(CString file);
DWORD Reflect(DWORD ref, char ch) {
	DWORD value(0);
	for(int i = 1; i < (ch + 1); i++) {		
		if(ref & 1)	value |= 1 << (ch - i);
	    ref >>= 1;	
		}
	return value;
	}

void init_crc32_table() {
	int i,j;DWORD crc;
	for (i = 0; i < 256; i++) {
		crc = i;
		for (j = 0; j < 8; j++)	{
			if (crc & 1) crc = (crc >> 1) ^ 0xEDB88320;
			else crc >>= 1;
			}
		crc32_table[i] = crc;
		}
	WriteCrcTable("Crc32.tab");
	}

void WriteCrcTable(CString file) {
	CString text;
	CStdioFile tableFile(file,CFile::modeCreate|CFile::modeWrite);
	for(int i=0;i<256;i++) {
		text.Format("%lu\n",crc32_table[i]);
		tableFile.WriteString(text);
		}
	tableFile.Close();
	}

DWORD GenerateCRC32(BYTE* DataBuf,int len) {
	DWORD ulCRC(0xffffffff);
	for(int i=0;i<len;i++) 
		ulCRC = (ulCRC >> 8) ^ crc32_table[(ulCRC & 0xFF)^DataBuf[i]];
	return ulCRC ^ 0xffffffff;
	}

void GenerateCRC32(CByteArray& DataBuf) {
	int len=DataBuf.GetSize();
	DWORD ulCRC(0xffffffff);
	for (int i=0;i<len;i++) {
		ulCRC = (ulCRC >> 8) ^ crc32_table[(ulCRC & 0xFF)^DataBuf.GetAt(i)];
		}
	ulCRC^=0xffffffff;
	BYTE c=(BYTE)((ulCRC >>24)&0xff);DataBuf.Add(c);
	c=(BYTE)((ulCRC >>16)&0xff);DataBuf.Add(c);
	c=(BYTE)((ulCRC >>8)&0xff);DataBuf.Add(c);
	c=(BYTE)(ulCRC &0xff);DataBuf.Add(c);
	}

BOOL CheckCrc32(CByteArray& DataBuf,DWORD& crc) {
	int len=DataBuf.GetSize()-4;
	DWORD ulCRC(0xffffffff);
	for (int i=0;i<len;i++) {
		ulCRC = (ulCRC >> 8) ^ crc32_table[(ulCRC & 0xFF)^DataBuf.GetAt(i)];
		}
	ulCRC^=0xffffffff;
	crc=ulCRC; 
	DWORD crc32inbuffer=0x00,dword;
	dword=DataBuf.GetAt(len);crc32inbuffer|=(dword<<24)&0xff000000;
	dword=DataBuf.GetAt(len+1);crc32inbuffer|=(dword<<16)&0x00ff0000;
	dword=DataBuf.GetAt(len+2);crc32inbuffer|=(dword<<8)&0x0000ff00;
	dword=DataBuf.GetAt(len+3);crc32inbuffer|=dword&0x000000ff;
	return crc32inbuffer==ulCRC;
	}


void LoadCrc32Table(CString file) {
	CString text;
	CStdioFile tableFile;
	if(tableFile.Open(file,CFile::modeRead)) {
		for(int i=0;i<256;i++) {
			crc32_table[i]=0;
			tableFile.ReadString(text);
			crc32_table[i]=atoi(text);
			}
		tableFile.Close();	
		}
	else init_crc32_table();
	}


DWORD GenerateCRC32(CString str) {
	DWORD ulCRC(0xffffffff);unsigned char ch;
	int len=str.GetLength();
	for(int i=0;i<len;i++) {
		ch=str.GetAt(i);
		ulCRC = (ulCRC >> 8) ^ crc32_table[(ulCRC & 0xFF)^ch];
		}
	return ulCRC ^ 0xffffffff;
	}
