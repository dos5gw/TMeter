void init_crc32_table();
DWORD GenerateCRC32(BYTE* DataBuf,int len);
void GenerateCRC32(CByteArray& DataBuf);
DWORD GenerateCRC32(CString str);
BOOL CheckCrc32(CByteArray& DataBuf,DWORD& crc);
void LoadCrc32Table(CString file);
