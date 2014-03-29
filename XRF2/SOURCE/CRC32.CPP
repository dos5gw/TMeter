# include "CRC32.h"
# include <stdio.h>
DWORD crc32_table[256];
DWORD ulPolynomial = 0x04c11db7;
void WriteCrcTable(char* file);
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

void WriteCrcTable(char* file) {
	FILE* fp=fopen(file,"wt");
	for(int i=0;i<256;i++) {
		fprintf(fp,"%lu\n",crc32_table[i]);
		}
	fclose(fp);
	}

DWORD GenerateCRC32(BYTE* DataBuf,int len) {
	DWORD ulCRC(0xffffffff);
	for(int i=0;i<len;i++)
		ulCRC = (ulCRC >> 8) ^ crc32_table[(ulCRC & 0xFF)^DataBuf[i]];
	return ulCRC ^ 0xffffffff;
	}

void UpdateCRC32(BYTE byte,DWORD& crc32) {
	crc32 = (crc32 >> 8) ^ crc32_table[(crc32 & 0xFF)^byte];
	}

void LoadCrc32Table(char* file) {
	FILE *fp=fopen(file,"rt");
	if(fp!=NULL) {
		for(int i=0;i<256;i++) {
			fscanf(fp,"%lu",&crc32_table[i]);
			}
		fclose(fp);
		}
	else init_crc32_table();
	}
