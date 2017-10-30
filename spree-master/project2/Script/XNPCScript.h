// XNPCScript.h: interface for the _XNPCScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XNPCSCRIPT_H__F6F5B7CC_7E9F_4726_A470_254789548AE8__INCLUDED_)
#define AFX_XNPCSCRIPT_H__F6F5B7CC_7E9F_4726_A470_254789548AE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XItem.h"
#include "XList.h"
#include "XNPC.h"
#include "XProc_ProcessDefine.h"


class _XNPCScriptItem : public _XItem  
{
public:
	BOOL  enable;

	BYTE  scriptclass;
	int	  tempvalue1;
	BYTE  compareclass;						// ���ǽĿ��� ��� ����, �ϳ� �̻� ���� 
	int	  tempvalue2;
	
	int	  condition1;
	int	  condition2;
	int	  condition3;	//2004.03.10->oneway48 insert : ������ ����Ʈ�� 3���� �þ 
	int	  condition4;
	int	  condition5;
	int	  condition6;		 
	int	  condition7;
	int	  condition8;
	int	  condition9;		 
	int	  condition10;		 
	int	  condition11;
	int	  condition12;	//2004.11.17->oneway48 insert : ��ų ����Ʈ�� 12���� �þ
	int	  condition13;	//2005.01.07->oneway48 insert : ��ų ����Ʈ�� 13���� �þ

	int   quantitynumber1;
	int   quantitynumber2;
	int   quantitynumber3;
	int   quantitynumber4;
	int   quantitynumber5;
	LPSTR quantitystring;

	LPSTR messagestring;
	
	LPSTR		strMessage1[5];				// ��ȭ�� �Ϲ��� �� ��ȭ ���� , ����&�亯 �� ���� ���� 
	LPSTR		strMessage2[5];				// ��ȭ�� �ܰ��� �� ��ȭ ���� , ����&�亯 �� �亯 ���� 
	LPSTR		strMessage3[5];				// ��ȭ�� ������ ���� �� ��ȭ ����
	
	_XList		m_SubScriptConditionList;	// ���� ������ ������ ���� ����Ʈ (_XNPCScriptConditionItem�� ������ �� ���δ�.)

public:
	_XNPCScriptItem();
	virtual ~_XNPCScriptItem();

	void SetMessageString( LPSTR _message );
	void SetQuantityString( LPSTR _message );

	
	_XNPCCONDITION_RESULT ProcessCondition();
	
	void SetstrMessage3(int index, LPSTR _message);
	void SetstrMessage2(int index, LPSTR _message);
	void SetstrMessage1(int index, LPSTR _message);	
};


class _XNPCMainScript : public _XItem
{

public:
	int			m_UniqueID;
	TCHAR		scriptname[32];
	
	BYTE		compareclass;						// and, or
	int			tempvalue3;
		
	LPSTR		messagestring1;
	LPSTR		messagestring2;
	LPSTR		messagestring3;
	LPSTR		messagestring4;
	LPSTR		messagestring5;

	int			soundindex1;
	int			soundindex2;
	int			soundindex3;
	int			soundindex4;
	int			soundindex5;
	
	LPSTR		strPrevGreetingMessage[5];			// �λ縻 �� ��縦 ����
	LPSTR		strGreetingGeneralMessage[5];		// �Ϲ��� �� �λ縻 ���� 
	LPSTR		strGreetingHostilityMessage[5];		// ������ �� �λ縻 ����
	LPSTR		strGreetingCustomerMessage[5];		// �ܰ��� �� �λ縻 ���� 
	LPSTR		strGreetingHeroStayMessage[5];		// ������ ���� �� �λ縻 ����
	
	//2004.07.05->oneway48 insert
	LPSTR		strGoodbyeGeneralMessage;	// �Ϲ��� �� ����� �� ��� ����
	LPSTR		strGoodbyeHostilityMessage;	// ������ �� ����� �� ��� ����
	LPSTR		strGoodbyeCustomerMessage;	// �ܰ��� �� ����� �� ��� ����
	LPSTR		strGoodbyeHeroStayMessage;	// ������ ���� �� ����� �� ��� ����
	int			indexGoodbyeSound[4];	// 0:�Ϲ� 1:���� 2:�ܰ� 3:����
	//insert end

	int			indexPrevGreetingSound[5];		
	int			indexGreetingGeneralSound[5];
	int			indexGreetingHostilitySound[5];
	int			indexGreetingCustomerSound[5];
	int			indexGreetingHeroStaySound[5];
	
	_XList		m_SubScriptList;	
	int			m_EventType;	//�̺�Ʈ ���� 0:������-�йи���Ʈ 1:�������� 2:������߱�
	
	// ó�� �λ縻�� �� �� �Ϲ�,����,�ܰ�,���� �Ǻ��ϱ� ���� ���� ����Ʈ 
	_XList		m_NPCScriptConditionList;
	
public:
	_XNPCCONDITION_RESULT ProcessNPCCondition();
	_XNPCMainScript();
	virtual ~_XNPCMainScript();

	void SetIndexGreetingHeroStaySound(int index, int soundfileindex);
	void SetIndexGreetingCustomerSound(int index, int soundfileindex);
	void SetIndexGreetingHostilitySound(int index, int soundfileindex);
	void SetIndexGreetingGeneralSound(int index, int soundfileindex);
	void SetIndexPrevGreetingSound(int index, int soundfileindex);	
	void SetIndexGoodByeSound(int index, int soundfileindex);//2004.07.05->oneway48 insert	
	void SetGreetingHeroStayMessage(int index, LPSTR _message);
	void SetGreetingCustomerMessage(int index, LPSTR _message);
	void SetGreetingHostilityMessage(int index, LPSTR _message);
	void SetGreetingGeneralMessage(int index, LPSTR _message);
	void SetPrevGreetingMessage( int index, LPSTR _message );
	void SetGoodByeMessage(int index, LPSTR _message);//2004.07.05->oneway48 insert	

	void SetScriptName( LPSTR _scriptname );

	void SetMessageString( LPSTR _message, int index );
	void SetSoundIndex(int index, int soundfileindex);

	void insertSubScriptItem( _XNPCScriptItem* scriptitem );
	_XNPCScriptItem* getSubScriptItem( int index );

	BOOL LoadScript( FILE* FilePtr );	
};



#endif // !defined(AFX_XNPCSCRIPT_H__F6F5B7CC_7E9F_4726_A470_254789548AE8__INCLUDED_)
