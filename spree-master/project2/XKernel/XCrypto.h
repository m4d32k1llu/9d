#define _XNEWCRYPTO

#ifdef _XNEWCRYPTO

#pragma once

#ifndef IN
#define IN
#define OUT
#endif


#define KEYBOX_FILE			"lump.dat"	// Ű�ڽ� ���� �Դϴ�.
#define KEYVALLENTH			16			// ��������Ű�� �ִ� ũ��
#define KEY_RANGE			0xff		// Ű ����..
#define KEY_X_RANGE			100			// x Ű�� ���� (0~99) : ������ ���� ���ƾ���..
#define KEY_Y_RANGE			100			// y Ű�� ���� (0~99)
#define BUF_SIZE_			2048 * 10000 		// ���� ����..


/*================================================================================
 CLASS NAME : _j_Crypto 
 EXP		: ����Ÿ ��ȣȭ / ��ȣȭ
 AUTHOR		: ozzywow
 DATE		: 2003.11. ,   SE : 2005,4,07
=================================================================================*/

struct _sCryptoHeader
{
	short	sLength : 12;		// -2048 - 2047
	short	sCrypto : 2;		// 0 : false, �������� true
	short	sCompressed : 2;	// 0 : flase, �������� true
};

class _j_Crypto
{

private:	
	
	DWORD	m_dwKeyLength;									// Key �� ����	
	char m_cKeyBox[KEY_RANGE][KEYVALLENTH];					// Key ���� ����ִ� BOX	
	
	int		m_iBufMaxSize ;
	int		m_iBufCurSet ;
	

public:		
	_j_Crypto::_j_Crypto( size_t bufSize ) : m_iBufMaxSize( bufSize ), m_iBufCurSet(0)
	{
		memset(m_cKeyBox,0,sizeof(m_cKeyBox));		
	}
	_j_Crypto::~_j_Crypto()
	{
	}

	
	// �ʱ�ȭ (Ű�� ����)
	// pathName : Ű ������ ����� ���� �̸�..
	// Ŭ������ �����ұ� �ʱ� ��ǥ�� (x,y) �� �־ �ʱ�ȭ�� �ּ���.
	bool _j_Crypto::Init(FILE* pKeyMapFile);
	
	//��ȣȭ//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// �� �Լ��� ����Ÿ�� ��ȣȭ �Ѵ�.   									//
	// �Լ� : Encryption(char * pData, USHORT usDataLen)					//
	// ���� : Ű�� ��Ʈ ������ �ϰ� �밢���� ��ġ �������� ����Ÿ�� ȥ��������//
	//////////////////////////////////////////////////////////////////////////
	inline bool _j_Crypto::Encryption( IN OUT char * pData, const USHORT usDataLen, const unsigned char key )
	{
	
		// Get key string
		char * cValKey ;
		UINT * uiValKey ;
		cValKey = GetKey( key ) ;		
		uiValKey = (UINT*)cValKey ;	
		

		try {

			int x, i,j,k,l ;
			
			UINT * puiData = (UINT *) pData ;						// 'int *' type data �� ��ȯ		

			j = 0 ;							// line step
			k = usDataLen / KEYVALLENTH ;	// line count
			l = usDataLen % KEYVALLENTH ;	// char count (������ ����Ÿ)			
		
			
			// line data block Encrypto
			for( i = 0, x = 0, j = 0 ; i < k ; i++, x += 4, j += 16 ) {	// Key �� pData �� ��ġ�Ѵ�.				
				puiData[x+0] = uiValKey[0] ^ puiData[x+0] ;
				puiData[x+1] = ~(uiValKey[2] ^ puiData[x+1]) ;
				puiData[x+2] = uiValKey[1] ^ puiData[x+2] ;
				puiData[x+3] = ~(uiValKey[3] ^ puiData[x+3]) ;
			}
			
			// char data Encrypto (������ ����Ÿ)
			//j += KEYVALLENTH ;
			for(int h = 0 ; h < l ; h++ ) {
				pData[j+h] = ~(cValKey[h] ^ pData[j+h]) ;
			}
			
		} // end try
		catch (...) {
			return false ;
		}	
		return true ;
		
	}

#ifdef _SERVER_VERSION_
	//��ȣȭ//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// �� �Լ��� ����Ÿ�� ��ȣȭ �Ѵ�.   									//
	// �Լ� : Decryption(char * pData, USHORT usDataLen)						//
	// ���� : Ű�� ��Ʈ ������ �ϰ� �밢���� ��ġ �������� ����Ÿ�� ȥ��������//
	//////////////////////////////////////////////////////////////////////////
	inline bool _j_Crypto::Decryption(char * pData, const USHORT usDataLen, const unsigned char key )
	{	

		// Get key string
		char * cValKey ;
		UINT * uiValKey ;
		cValKey = GetKey( key ) ;		
		uiValKey = (UINT*)cValKey ;	

		try {	
			
			int x, i,j,k,l ;
			
			UINT * puiData = (UINT *) pData ;						// 'int *' type data �� ��ȯ		
			
			j = 0 ;							// line step
			k = usDataLen / KEYVALLENTH ;	// line count
			l = usDataLen % KEYVALLENTH ;	// char count (������ ����Ÿ)			
			
			
			// line data block Encrypto
			for( i = 0, x = 0, j = 0 ; i < k ; i++, x += 4, j += 16 ) {	// Key �� pData �� ��ġ�Ѵ�.				
				puiData[x+0] = uiValKey[0] ^ puiData[x+0] ;
				puiData[x+1] = ~(uiValKey[2] ^ puiData[x+1]) ;
				puiData[x+2] = uiValKey[1] ^ puiData[x+2] ;
				puiData[x+3] = ~(uiValKey[3] ^ puiData[x+3]) ;
			}
			
			// char data Encrypto (������ ����Ÿ)
			//j += KEYVALLENTH ;
			for(int h = 0 ; h < l ; h++ ) {
				pData[j+h] = ~(cValKey[h] ^ pData[j+h]) ;
			}

		}// try
		catch (...) {
			return false ;
		}
		return true ;		
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// �� �Լ��� ����Ÿ�� ��/��ȣȭ �Ѵ�.									//
	// �Լ� : Xor(char * pData, USHORT usDataLen)	�μ� : ���ڿ� ������(plantext)	//
	// ���� : ���ڿ��� �Էµ� Ű�� �ܼ��� XOR ������ �����Ѵ�.              //
	//////////////////////////////////////////////////////////////////////////
	inline char * _j_Crypto::Xor(char *pData, USHORT usDataLen, const unsigned char key )
	{	
		char * cValKey ;
		cValKey = GetKey( key ) ;		
		
		for(USHORT i = 0; i < usDataLen; i++){
			pData[i]=~ pData[i]^cValKey[i % KEYVALLENTH];
		}

		return pData;		
	}	

	
	inline unsigned char _j_Crypto::checksum(unsigned char *buffer, int size) {
		
		unsigned long cksum=0;
		
		while(size >1) {
			cksum+=*buffer++;
			size -=sizeof(unsigned char);
		}
		
		if(size ) {
			cksum += *(unsigned char*)buffer;
		}
		
		cksum = (cksum >> 16) + (cksum & 0xffff);
		cksum += (cksum >>16);		
		
		return (unsigned char)(~cksum);
	}


private:
	inline char * _j_Crypto::GetKey(unsigned char key , const char * pathName = NULL )	
	{				
#ifdef _XDEF_NEW_PACKET_CRYPT_20071115_KERYGMA
		key += 97; 
#endif
		return m_cKeyBox[key % KEY_RANGE];
	}	
};



struct _Tail {
	unsigned char		crc ;
	unsigned char		seq ;
};

struct _Encapsule_info {	
	char *				buf ;
	unsigned short 		length ;
};

struct _Decapsule_info {	
	char *				buf ;
	unsigned short		length ;
	unsigned char		seq ;
};


#ifdef _SERVER_VERSION_
#define _CAPSULE_BUF_SIZE_		1048576 * 5
#else
#define _CAPSULE_BUF_SIZE_		4096
#endif

#define _TAIL_SIZE_				2
class CCapsulateCrypto : public _j_Crypto
{

private:
	char *	m_pCapsule_buf ;
	int		m_iBufMaxSize ;
	int		m_iBufCurSet ;
	unsigned char	m_ucSeq ;

#ifdef _SERVER_VERSION_
	CRITICAL_SECTION	m_cs ;
	inline void lock() { EnterCriticalSection( &m_cs ) ; }
	inline void unlock() { LeaveCriticalSection( &m_cs ) ; }	
#endif

public:

	CCapsulateCrypto(size_t bufSize = _CAPSULE_BUF_SIZE_ ) : _j_Crypto(bufSize), m_iBufMaxSize(bufSize), m_iBufCurSet(0), m_ucSeq(0)
	{
#ifdef _SERVER_VERSION_
#if (_WIN32_WINNT > 0x0403)
		if( FALSE == InitializeCriticalSectionAndSpinCount( &m_cs, (0xf0000000)|2000 ) )
		{
			throw ;
		}
#else
		InitializeCriticalSection( &m_cs ) ;		
#endif
#endif
		m_pCapsule_buf = new char[bufSize] ;
	}
	~CCapsulateCrypto()
	{
		delete [] m_pCapsule_buf ;
#ifdef _SERVER_VERSION_
		DeleteCriticalSection( &m_cs ) ;
#endif
	}

	inline void InitSeqNum( unsigned char seq ) { m_ucSeq = seq ; }
	inline unsigned char GetSeqNum( void ){ return m_ucSeq; }
	
	inline _Encapsule_info Encapsulate( char * packet, unsigned char key = 0  ) 
	{
		if( key == 0 ) 	{ key = m_ucSeq ; }
		_sCryptoHeader * pHeader = (_sCryptoHeader*)packet ;
		_Encapsule_info capsuleInfo ;

#ifdef _SERVER_VERSION_
		lock() ;
		if( pHeader->sLength + _TAIL_SIZE_ >= m_iBufMaxSize - m_iBufCurSet )
		{
			m_iBufCurSet = 0 ;
		}
		memcpy( &m_pCapsule_buf[m_iBufCurSet], packet, pHeader->sLength ) ;
		packet = &m_pCapsule_buf[m_iBufCurSet] ;
		m_iBufCurSet += pHeader->sLength + _TAIL_SIZE_ ;
		unlock() ;
#else		
		if( pHeader->sLength + _TAIL_SIZE_ > m_iBufMaxSize )
		{
			capsuleInfo.buf = NULL ;
			capsuleInfo.length = 0 ;
			return capsuleInfo ;
		}

		memcpy( m_pCapsule_buf, packet, pHeader->sLength ) ;
		packet = m_pCapsule_buf ;		
#endif
		
		//  Add tail
		pHeader = (_sCryptoHeader*)packet ;
		_Tail * pTail = (_Tail*)&packet[pHeader->sLength] ;
		pTail->crc = checksum( (unsigned char*)packet, pHeader->sLength ) ;
		pTail->seq = m_ucSeq;
		pHeader->sLength +=  _TAIL_SIZE_ ;

		if( m_ucSeq >= 255 )
		{
			m_ucSeq = 1;
		}
		else
		{
			m_ucSeq++;
		}
		

		// encryption
		if( pHeader->sCrypto == 1 )
		{
#ifdef _XUSEENCRYPTPATTERN
			if( Encryption( (char*)&packet[2], pHeader->sLength-_TAIL_SIZE_, key ) == false )		
#else
			if( Encryption( (char*)&packet[2], pHeader->sLength, key ) == false )
#endif
			{
				capsuleInfo.buf = NULL ;
				capsuleInfo.length = 0 ;
				return capsuleInfo ;
			}
		}

		capsuleInfo.buf = packet ;
		capsuleInfo.length = pHeader->sLength ;

		return capsuleInfo ;
	}

#ifdef _SERVER_VERSION_
	inline _Decapsule_info Decapsulate( char * packet, unsigned char key = 0  ) 
	{
		if( key == 0 ) 	{ key = m_ucSeq ; }
		_sCryptoHeader * pHeader = (_sCryptoHeader*)packet ;
		_Decapsule_info	decapsule_info ;
		if( pHeader->sCrypto == 1 )
		{
			if( Decryption( (char*)&packet[2], pHeader->sLength, key ) == false )
			{
				decapsule_info.buf = NULL ;
				decapsule_info.length = 0 ;
				return decapsule_info ;
			}
		}

		pHeader->sLength -= _TAIL_SIZE_ ;
		_Tail * pTail = (_Tail*)&packet[pHeader->sLength] ;
		if( pTail->crc == checksum( (unsigned char*)packet, pHeader->sLength ) )
		{
			decapsule_info.buf = packet ;
			decapsule_info.length = pHeader->sLength ;			
		}
		else
		{
			decapsule_info.buf = NULL ;
			decapsule_info.length = 0 ;
		}
		decapsule_info.seq = pTail->seq ;

		return decapsule_info ;
	}
#endif
};




#else // old crypto

#ifndef _XCRIPTO_
#define _XCRIPTO_

/*
��ȣȭ ����� �ΰ��� �ֽ��ϴ�.
API �� ����� ��ȣȭ ����.. OS �������� ����� �ֽ��ϴ�.
�Ʒ� '//#define API_CRYPTO_FUNC' �ּ��� �����ϸ�..
API�� �̿��� ��ȣȭ ���� ������ �˴ϴ�.
*/

//#define API_CRYPTO_FUNC

#ifndef API_CRYPTO_FUNC 
// API ��ȣȭ �Լ� ������� �������


#define KEYBOX_FILE			"lump.dat"	// Ű�ڽ� ���� �Դϴ�.
#define PACKET_HEADER_LEN	1			// ��Ŷ�� ��� ����(���� : ��Ŷ���� ��� �κ�(��������)�� ��ȣȭ ���� �ʽ��ϴ�.)

#define KEYVALLENTH			16			// ��������Ű�� �ִ� ũ��
#define KEY_X_RANGE			100			// x Ű�� ���� (0~99) : ������ ���� ���ƾ���..
#define KEY_Y_RANGE			100			// y Ű�� ���� (0~99)


/*================================================================================
CLASS NAME : _j_Crypto 
EXP		: ����Ÿ ��ȣȭ / ��ȣȭ
AUTHOR		: ozzywow
DATE		: 2003.11.
=================================================================================*/


class _j_Crypto
{

protected:
		
	DWORD	m_dwKeyLength;									// Key �� ����	
	char m_cKeyBox[KEY_X_RANGE][KEY_Y_RANGE][KEYVALLENTH];	// Key ���� ����ִ� BOX
	char * m_cValKey;										// ���� Ű
	UINT * m_uiValKey ;
	
	UINT m_buf[2048] ;
	
	int x,y,z, h,i,j,k,l,m ;

public:	
	 _j_Crypto();	
	~_j_Crypto();	
		
	bool _j_Crypto::Init(const char x, const char y, FILE* pKeyMapFile);
	bool _j_Crypto::Encryption(char * pData, USHORT usDataLen);	
	bool _j_Crypto::Decryption(char * pData, USHORT usDataLen);
	char * _j_Crypto::Xor(char *pData, USHORT usDataLen);


protected:
	bool _j_Crypto::GetKey(const char x, const char y, FILE* pKeyMapFile);

private:
};


#else

#include <Wincrypt.h>


/* If a 128-bit RC4 session key is to be generated, 
the value 0x00800000 is combined with any other dwFlags 
predefined value with a bitwise OR operation. */
//#define _WIN32_WINNT 0x0400 
#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
#define KEYLENGTH			0x00800000	
#define ENCRYPT_ALGORITHM	CALG_RC4	//��ȣȭ �˰��� ����
#define KEYBOX_FILE			"lump.dat"	// Ű�ڽ� ���� �Դϴ�.
#define PACKET_HEADER_LEN	1			// ��Ŷ�� ��� ����(���� : ��Ŷ���� ��� �κ�(��������)�� ��ȣȭ ���� �ʽ��ϴ�.)

#define KEYVALLENTH			10			// ��������Ű�� �ִ� ũ��
#define KEY_X_RANGE			100			// x Ű�� ���� (0~99) : ������ ���� ���ƾ���..
#define KEY_Y_RANGE			100			// y Ű�� ���� (0~99)


/*================================================================================
CLASS NAME : _j_Crypto 
EXP		: ����Ÿ ��ȣȭ / ��ȣȭ
AUTHOR		: ozzywow
DATE		: 2003.11.
=================================================================================*/

class _j_Crypto
{

protected:
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;
	HCRYPTKEY hKey;

	DWORD	m_dwKeyLength;						//Key �� ����	
	char m_cKeyBox[KEY_X_RANGE][KEY_Y_RANGE][KEYVALLENTH];
	char * m_cValKey;

public:		
	_j_Crypto();	
	~_j_Crypto();
	
		   bool Init(const char x, const char y, FILE* pKeyMapFile);	
	inline char * Xor(char *pData, USHORT usDataLen);
	inline bool Encryption(char * pData, USHORT usDataLen);	
	inline bool Decryption(char * pData, USHORT usDataLen);

protected:
	bool Cps();
	bool GetKey(const char x, const char y, FILE* pKeyMapFile);
	void Free();	

private:
};

#endif  // defined API_CRYPTO_FUNC 
	
#endif  // _XCRIPTO_

#endif  // old crypto