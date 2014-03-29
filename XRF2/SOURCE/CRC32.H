# include "Typedef.h"
void init_crc32_table();
void UpdateCRC32(BYTE byte,DWORD& crc32);
DWORD GenerateCRC32(BYTE* DataBuf,int len);
void LoadCrc32Table(char* file);
DWORD shieldPC(DWORD dw);
