# include <dos.h>
# include <conio.h>
# define RXD 0
# define TXD 0
# define LSB 0
# define MSB 1
# define IER 1
# define IIR 2
# define LCR 3
# define MCR 4
# define LSR 5
# define MSR 6

# define IERV 1
# define IESE 2
# define OUT2 8
# define ERTS 2
# define EDTR 1
# define EMPTY 0x20
# define READY 0x30
# define DSR  0x20
# define RTS  0x10

# define ICREG 0x20
# define IMASKREG 0x21
# define EOI 0x20

# define WAITCOUNT 5000
# define BUFFLEN 6144
# define ALTE 0x12
# define ALTQ 0x10


enum BaudIdx {B600,B1200,B2400,B4800,B9600,B19200,B28800,B38400,B57600,B115200};
enum ParityIdx {NONE,ODD,EVEN,ONE,ZERO};
BYTE ReadComBuffer();
BOOL CheckComBuffer();
void ComSender(BYTE ch);
BOOL StartComService(int port,BaudIdx bIdx,int datab,int stopb,ParityIdx pIdx);
void StopComService();
void UpdateCommport();
void AddToSendBuffer(BYTE ch);
void Add2ToSendBuffer(WORD word,DWORD& crc);
void Add4ToSendBuffer(DWORD dword,DWORD& crc);
void LAdd2ToSendBuffer(WORD word,DWORD& crc);
void LAdd4ToSendBuffer(DWORD dword,DWORD& crc);

void ResetSendBuffer();

void ResetCommandBuffer();
int GetBufferLen();
void ClearReceiveBuff();