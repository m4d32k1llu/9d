#ifndef __XKERNEL_H__
#define __XKERNEL_H__
#ifdef __cplusplus

/** @file XKernel.h */

#include "xgamebase.h"	

#define XKERNEL_TITLE				"XKernel"
#define XKERNEL_AUTHOR				"9Dragons team"
#define XKERNEL_WEBSITE				"www.indy21.co.kom"
#define XKERNEL_EMAIL				"kerygma@indy21.com"
#define XKERNEL_VERSION				0
#define XKERNEL_SUB_VERSION			3
#define XKERNEL_PATCH_VERSION		0
#define XKERNEL_DATE				20030130

#define _XDEF_LAUNCHER_SIGNAL3   	_T("KC2MB")
#define _XDEF_LAUNCHER_SIGNAL1   	_T("9E2GH")
#define _XDEF_LAUNCHER_SIGNAL5   	_T("KUV63T")
#define _XDEF_LAUNCHER_SIGNAL4   	_T("L4EBN")
#define _XDEF_LAUNCHER_SIGNAL2   	_T("4FWDW")

#define _XMEMORYCHECKDATA_ENCCHARCODE	0x1F
#define _XMEMORYCHECKDATA_ENCBYTECODE	0xC5
#define _XMEMORYCHECKDATA_ENCSHORTCODE	0xCE43
#define _XMEMORYCHECKDATA_ENCWORDCODE	0xBEF1
#define _XMEMORYCHECKDATA_ENCINTCODE	0xAE1CDB23
#define _XMEMORYCHECKDATA_ENCDWORDCODE	0xE137FDB1

#define _XMEMORYCHECKDATA_ENCCHAR(x)	((char)x^(char)_XMEMORYCHECKDATA_ENCCHARCODE)
#define _XMEMORYCHECKDATA_ENCchar(x)	((char)x^(char)_XMEMORYCHECKDATA_ENCCHARCODE)
#define _XMEMORYCHECKDATA_ENCBYTE(x)	((BYTE)x^(BYTE)_XMEMORYCHECKDATA_ENCBYTECODE)
#define _XMEMORYCHECKDATA_ENCSHORT(x)	((short)x^(short)_XMEMORYCHECKDATA_ENCSHORTCODE)
#define _XMEMORYCHECKDATA_ENCshort(x)	((short)x^(short)_XMEMORYCHECKDATA_ENCSHORTCODE)
#define _XMEMORYCHECKDATA_ENCWORD(x)	((WORD)x^(WORD)_XMEMORYCHECKDATA_ENCWORDCODE)
#define _XMEMORYCHECKDATA_ENCINT(x)		((int)x^(int)_XMEMORYCHECKDATA_ENCINTCODE)
#define _XMEMORYCHECKDATA_ENCint(x)		((int)x^(int)_XMEMORYCHECKDATA_ENCINTCODE)
#define _XMEMORYCHECKDATA_ENCDWORD(x)	((DWORD)x^(DWORD)_XMEMORYCHECKDATA_ENCDWORDCODE)
#define _XMEMORYCHECKDATA_ENCBOOL(x)	((BOOL)x^(BOOL)_XMEMORYCHECKDATA_ENCDWORDCODE)

#define _XDEF_ENCVALUE(_type, _val) private: \
									_type _val; \
									public: \
									void Set_## _val( _type _data ){ _val = _XMEMORYCHECKDATA_ENC##_type (_data); } \
									_type Get_##_val( void ){ return _XMEMORYCHECKDATA_ENC##_type (_val); }

// The main function that you must use
void XKernelMain(void);


// Base define
#include "XBaseDefs.h"             
#include "XExternDefs.h"

// Utilities
#include "XUtils.h"

// Application frameWork classes
#include "XDeviceEnum.h"
#include "X3DSettings.h"
#include "XDialogs.h"
#include "XProcess.h"
#include "X3DApp.h"

// Camera control
#include "XCamera.h"

// Math function
#include "XModelContainerDef.h"
#include "XMeshMath.h"
#include "XFastCode.h"

// Collide detection
#include "XCollideMath.h"
#include "XCollide.h"


// D3D Utilities
#include "d3dfont.h"
#include "d3dutil.h"
#include "dsutil.h"
#include "dxutil.h"


// Data manager
#include "Xitem.h"
#include "Xlist.h"                 


// Error detection
#include "XError.h"                
#include "XException.h"            
#include "XExceptionHandler.h"

// Texture manager
#include "XRenderStyleManager.h"
#include "XTexture.h"
#include "XTextureManager.h"

// Font system
#include "X3DUFont.h"              
#include "XFont.h"
#include "XUnicodeFont.h"

// Timer function
#include "XTimeCounter.h"          
#include "XTimer.h"

// XGUI Control
#include "XGUIObject.h"
#include "XGUIControlCenter.h"
#include "XBallGauge.h"
#include "XButton.h"
#include "XCheckButton.h"
#include "XImageStatic.h"
#include "XStatic.h"
#include "XStickGauge.h"
#include "XScrollBar.h"
#include "XToolTip.h"
#include "XHSlider.h"
#include "XVSlider.h"
#include "XWindow.h"
#include "XListBox.h"
#include "XWindowManager.h"
#include "XTreeBox.h"

// Input system
#include "XInput.h"
#include "XIME.h"

// Mesh control
#include "XBillboard.h"
#include "XObb.h"                  

// Security
#include "XEncryptor.h"
//#include "XCrypto.h"
#include "XCompress.h"

// Application data control
#include "XRegistry.h"
#include "XPackage.h"
#include "XMString.h"
#include "XStringItem.h"


// System window
#include "XWindow_IME.h"

#include "XStringDataBase.h"

// Shader

// 2006. 01. 07 ---------------------------------------��
#define _XDEF_NEWANIMATIONLIST
// ----------------------------------------------------��

// 2006. 01. 16 ---------------------------------------��
#define _XTS_NEWSTRING				// eod string ����
// ----------------------------------------------------��

#define _XTS_MONSTERREF_12			// monster reference ver1.2
#define _XTS_PARTYMOREINFO			// ���� �߰����� ���� ON/OFF
#define _XTS_NEWPARTYCOUNTER		// ��ȣ ǳ��Ͽ��� ��Ƽ ī���� ��Ŷ ����

#define _CHARAC_BLOCKING		// character blocking

// 2006. 03. 20 ---------------------------------------��
#define _XTS_PK						// PK system
#define _XTS_MASKSYSTEM				// ���� 
// ----------------------------------------------------��

// 2006. 03. 21 ---------------------------------------��
#define _XTS_MASTERY			// ����
// ----------------------------------------------------��

// 2006. 04. 19 ---------------------------------------��
#define _XTS_MONSTERREF_13			// monster reference ver 1.3
#define _XTS_FAME					// ��/�Ǹ�
#define _XTS_BLOODPOINT				// ����
#define _XTS_PKRECORD				// PK ���� ���
// ----------------------------------------------------��

// 2006. 05. 03 ---------------------------------------��
#define _XDEF_NEWSTRINGMANAGER
// ----------------------------------------------------��

// 2006. 05. 29 ---------------------------------------��
#define _XTS_VIEWPKTARGETLEVEL		// pk target�� ���� ����
// ----------------------------------------------------��

// 2006. 07. 24 ---------------------------------------��	// �븸/��Ʈ���ϰ� ����
#define _XTS_MONSTERHEAL			// mob heal
#define _XTS_ITEM_OPTIMIZATION		// ������ ���� �ڵ� ����ȭ
#define _XTS_QUESTAUTONODE			// ����Ʈ �ڵ���� : ����Ʈ ������ �ٲ�
//#define _XTS_MEDI_TUTORIAL			// ����� �����˾�
#define _XTS_WEAPONSTRADD			// �ٷ¿� ���� ���ⵥ���� ������ ����
#define _XTS_HAVEQUESTLISTNPC		// ����Ʈ�� ������ ������ �ִ� NPC
#define _XTS_VIEWPKTARGETLEVEL		// pk target�� ���� ����
#define _XTS_CHANGEWEAPONDAMAGE		// ���� ������ ��� ���� ����
#define _XTS_AUTOMEDITATION			// �ڵ� �����
#define _XTS_LEVELTOOLTIP			// ���� tooltip
#define _XTS_NEWINSENSIBLE			// ���� animation ����
// ----------------------------------------------------��

#define _XTS_NEWCHARACLIST			// character list ����

// 2006. 08. 05 ---------------------------------------��
#define _XTS_BASEMINMAXWEAPONDAMAGE	// ���� ���� �⺻ ���ݷ�
// ----------------------------------------------------��

// 2006. 08. 24 ---------------------------------------��
#define _XTS_SKILLCOMATYPE			// ��ų Ÿ�� Ÿ�� �߰� - �ڸ� ����
// ----------------------------------------------------��

#define _XTS_ITEM_V20				// ������ ���� 2.0
#define _XTS_ITEM_V21				// item table v2.1

#define _XTS_SHOWTUTORIAL

#define _XDEF_USENEWNPCTEXTSYSTEM		// TextDB���� NPC ���� ��Ʈ���� ������.
#define _XDEF_USENEWQUESTTEXTSYSTEM		// TextDB���� QUEST ���� ��Ʈ���� ������.
#define _XDEF_USENEWAREATEXTSYSTEM		// TextDB���� AREA ���� ��Ʈ���� ������.

// 2006. 11. 14 ---------------------------------------��
/*       ���������� �ؿܹ��� �����ϰ� �����Ŵ.       */ 
#define _XDEF_EXPAND_ORGANIZATIONNAME	/* ������ �̸� ���� ���� */ 
#define _XDEF_EVENTWINDOW				/* �̺�Ʈ â�� �߰��մϴ�.	2006�� 9�� 28�� ������Ʈ ���� �Դϴ�. 09�� 27��: ��Ÿ�� �������� ����� ��Ī	*/										
// ----------------------------------------------------��


#ifdef _XKOREAN
	// ----------------------------------------------------------------------------------------------------------��
	#define _XTS_SERVERUNIFICATION					// ��������	�ѱ��� ����

	#define _XTS_USECASHITEM						// cash item ��� - ĳ�� ������ border�� message â border ó��
	#define _XTS_ITEMMALLBROWSER					// �����۸� - ������ �� browser�� cash item package

	#define _XDEF_NEXONLOGIN						// �ؽ� �α��� - �α��� ��Ŷ ����
	#define _XDEF_NEXONLOGINFINAL					// �ؽ� �޽����� connect �ϴ� �κ��� NEXONLOGINFINAL�� �������� 
	#define _XDEF_NEXONUNIFICATIONGUIDE				// ���ļ��� �ؽ� ���� �ȳ��� ǥ��

	#define _XDEF_NEWDELETECHARACTER				// ĳ���� ���� - �ֹι�ȣ ��4�ڸ��� ����
	#define _XDEF_NEWVERSONFILETERING				// ���͸� ����Ʈ ���׷��̵�

	#define _XDEF_MONSTERREF_14						// ���� ���̺� 1.4

	#define _XDEF_QUICKSLOTSERVERSAVE				// Quick slot server side save.
	#define _XDEF_HELPSYSTEMSERVERSAVE				// Help system server side save.

	// TEST�� (XTrap)
	//#define _XTS_XTRAP_TEST
	
	#define _XTS_CHARACTERFILTERING					// ĳ���� �̸� ���͸� - �ӽ÷� ���� ������ ����

	#define _XTS_NEWSKILLWINDOW						// ��ų â ����
	#define _XTS_ITEM_V22							// item table v2.2

	// ĳ�� ������ ��� ����  - �ؿܹ������� ĳ�� �������� ��븸 �� ��� �Ʒ��� define�鸸 �������ָ� ��. --��
	#define _XDEF_CASHITEM_SHOUT					// ��ġ�� (������)
	#define _XTS_MOVEVILLAGE						// ǥ�� �̵�
	#define _XDEF_CLOTHMASK_DIVIDE					// ġ���ǻ��� ĳ�� �����۰� �Ϲ� ���������� ������.- icon ó��
	#define _XDEF_PERIODITEM_20061027				// �Ⱓ�� ������ : �Ⱓ�� ���� TimeValue �߰�
	// --------------------------------��

	//#define _XTS_NEWCREATECHARACTER				// ĳ���� ����â ������ 
	//#define _XTS_DELETESKILL						// ��ų ����

	// 2006.12.21 --��
	#define _XTS_ALIANCE_20061018					// ������ ���� ���� �������̽�	12�� 21�� ������Ʈ?
	#define _XDEF_ITEMMALL_AUTOLOGIN_20061120		// �����۸� �ڵ� �α��� ��� �߰� 12�� 21�� ������Ʈ?
	#define _XDEF_WEBBOARD_20061211					// �� �Խ��� ��f�� �߰� 12�� 21�� ������Ʈ?

	#define _XDEF_SEARCHPARTYMEMBER					// ���� ã�� ���

	#define	_XDEF_CHRISTMASEVENT					// ũ�������� �̺�Ʈ
	#define _XTS_REFUSE_ORGANIZATION				// ������ �ʴ� �ź�
	// --------------��
	
	#define _XTS_SAFETYZONE							// �������� ����
	#define _XTS_PKZONE_SERVERSETTING				// pk server setting
	#define _XDEF_MOVE_PKSERVER_SELECT				// pk zone���� �̵��� �� ���� �ϵ��� ����

	// 2007.01.04 ---��
	#define _XDEF_MONEYDROP_NO_20070102				// ���� ������ �Ұ���
	#define _XDEF_MONSTERWARNING_MESSAGE_20070102	// ���� ��� ���� -> �ý��� �޽����� �̵�

	#define _XDEF_NOTICE_STRING_PROCESS				// ���� �� �������� Ŭ���� �߰� Author : ����� Last updated: 07/01/04
    // --------------��


	// 2007.02.08 ---��
	#define _XDEF_ALIANCE_WEBBOARD_20070130	// ����� - ������,����ȸ �Խ��� �ٷΰ��� 
    // --------------��

	// 2007.02.15 ---��
	#define _XDEF_ESCAPE_20070205					// Ż�� ��� 
	// --------------��

	// 2007.02.22 ---��
	#define _XDEF_SHOWEVENTNOTICE					// �̺�Ʈ â �˾� ON/OFF
	#define _XDEF_EVENTWINDOW_WEBPAGE_20070214		// �̺�Ʈ â �� �������� �����
	#define _XDEF_NEWSTRINGDBFORMAT					// ND Text DB format ����
	// --------------��

	// 2007.03.15 ---��
	#define _XDEF_CASTLEBATTLE						// �����
	#define _XDEF_CASTLEBATTLE_MOD_CHARACINFO		// Charac info packet ����
	// --------------��

	// 2007.03.22 ---��
	#define _XTS_MASTERY_04							// masterytable.bms ver 0.4
	#define _XTS_MASTERYOPTIONAL					// ���� ���� - Ȯ��
	#define	_XDEF_RESIST_DISPLAY					// ����ȥ�� ���׷� ǥ��
	// --------------��

	// 2007.04.05 ---��
	#define	_XDEF_SKILLDEALY_MODIFY	/*!< @author ����� @breif ��ų ������ ����Ǵ� �κ� ����*/
	#define _XDEF_WASTEBASKET_20070226				// ������ ��� �߰�
	#define _XDEF_PARTY_NOTAVAILABLE				// �ڸ���� & ���� ��ȭ
	
	#define _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK����ġ����, ������ PK����
	#define	_XDEF_INPUTPRESS	/*!< @author ����� @breif �ʽı� ���� */
	// --------------��

	// 2007.04.19 ---��
	#define _XDEF_CASTLEBATTLE_SECONDEDITION		// ����� - 2�� ������Ʈ
	#define _XDEF_CASTLEBATTLE_MAPTAXRATE			// ���� ���� ����
	#define _XDEF_BLOCK_MOVINGATTACK	/*!< @author ����� @breif �ʽı� ���� �ɾ� ��ũ�� : _XDEF_INPUTPRESS�� ���� ����Ǿ�� ��*/
	// --------------��

	// 2007.04.26 ---��
	#define _XDEF_WAREHOSE_RESTRAINT_OF_TRADE	/*!< @author ����� @breif Ư�� ������ â�� �ŷ� ����*/
	// --------------��

	// 2007.05.03 ---��
	#define _XDEF_NPC_ATTACK_CHECK	/*!< @author ����� @breif ������� ���ݽ� ����Ʈ Ŭ��*/
	// --------------��

	// 2007.05.17 ---��
	#define _XDEF_SMELT_ITEM /*!< @author ����� @breif ���/�븰 ���� ������ */
	// --------------��

	// 2007.05.31 ---��
	#define _XDEF_CASTLEBATTLE_CASTLECAPTAIN_20070130	// ����� - ���� �̸�ǥ
	#define _XDEF_CASTLEBATTLE_RECORD					// ����� - ���� 
	#define _XDEF_CASTLEBATTLE_MINIMAP					// ����� - �̴ϸ�
	#define _XDEF_PK_TRACINGSERVICES_20070402 /*!< @author ����� @breif PK ��ġ���� ����Ʈ */
	#define _XDEF_RENEWAL_CHATWINDOW_20070503			// ä��â ��� Ȯ��.
	#define _XDEF_ZONECHATTING_20070108					// �� ä�� (���� ��ȭ)
	#define KR_RB_DELAYSHOWNEXONNOTICEWINDOW_070530_KERYGMA	// 15�� �̿밡 ���� Delay.
	// --------------��

	// 2007.06.07 ---��
	#define _XDEF_AGEGRADE_070607_MAGVIPER	/*!< @author ����� @breif �̿���*/
	// --------------��

	// 2007.06.14 ---��
	#define _XDEF_NEW_WORLDMAP_070528_MAGVIPER	/*!< @author ����� @breif �ű�������*/
	#define ALL_RB_RENEWALCHAT_BUGFIXED_070611_KERYGMA	// ä�� ������ �� (����)���� �κ� ���� ����.
	// --------------��

	// 2007.06.28 ---��
	#define _XDEF_MODELTHREADLOADING				// Character model multithread load ó��:���� ����ȭ
	#define VN_RB_GAMBLEBOX_070607_KUKURI			// �пձ� ���� ������ ���� ���� ���� (MSG_SM_EQUIP_RESULT.. ��Ŷ ����)
	#define _XDEF_RECALLSYSTEM_070604_MAGVIPER /*!< @author ����� @breif ģ����ȯ*/

	// ���߿� ������ define ������ ��.
	#define _XDEF_DUNGEONTICKET_070615_KUKURI			// �δ� �����
//	#define _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI	// �δ� ����� UI �� ��(���� ����x) : �δ������ ���� item ���� ��Ŷ(MSG_EX_BATTLE_ROOM_REQ..)�� ����
	#define VN_RB_BOSSROOM_BUGFIX_070615_KUKURI			// ����� ���� ������ ���� �����ϴ� ���� ����
	// --------------��

	// 2007.07.26 ---��
	#define _XDEF_4EFF_SYSTEM_070625_KUKURI				// ����ȥ�� �ý��� : _XDEF_NEWSTRINGMANAGER/_XTS_PK �ʿ�
	#define _XDEF_EXPANSION_POTION_070709_KUKURI		// ���� ��� Ȯ��
	#define _XDEF_MODIFY_UI_070710_KUKURI				// �������̽� ���� ���� : ��å �̸�, ����ܰ�
	#define _XDEF_MODIFY_UI_MONSTER_070710_KUKURI		// ���� ���� ǥ�� 
	#define _XDEF_ITEM_V23								// item table v2.3
	#define ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA		// ä�� �Է� ���� ����.
	#define _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : ����� //breif : ���� �и�
	// --------------��

	// 2007.08.30 ---��
	#define _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI	// �Ⱓ�� ������ â���� ���� ó�� : ������ǰ���� ��Ŷ �����(MSG_CHARAC_MORE_INFO)
	#define ALL_RB_DRAW_CASHCHECK_070820_KUKURI			// ĳ�� ������ �׵θ� �׸��� ������ : cashcheck==1, 2�϶�
	#define _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI	// ���� �κ� ���� ��Ŷ ���� 
	#define _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI		// ǥ���̵��� ������
	#define _XDEF_LOSTWEAPON_070821_MAGVIPER //Author : ����� //breif ���� ������� ����
	// --------------��

	// 2007.10.04 ---��
	#define _XDEF_NOTICEPOPUPCONTROL__070910_POPUP		// Control popup window
	// --------------��

	// 2007.11.29 ---��
	#define _XDEF_INTERFACE_RENEWAL_071002_KUKURI		// �������̽� ������ : Jinstatusâ ����,����ġâ �߰�
	#define _XDEF_MONSTERREF_15							// ���� ���̺� v 1.5
	#define _XDEF_MIXWEAPON_071009_KUKURI				// ���� ���� �ý���	:_XDEF_SMELT_ITEM�� ������ ��ȿ
	#define _XTS_NEWBATTLEPARAMETER						// ND_Battle_Parameter ���� Ȯ��. 2007.10.10
	#define _XDEF_RESOURCE_RENEWAL_071018_KUKURI		// �Ϲ� ���� ������ ��ġ ����	// _XDEF_EVENTWINDOW �ʿ�
	#define _XTS_COLLECTION_PACKAGE_071023_KUKURI		// ��ǰ ���� - package ���� �߰�
	#define _XDEF_ITEM_ADDVALUE_071024_KUKURI			// ������ - ����, ����, ȸ�Ǽ��� ���� ����ʿ� �ݿ�
	#define _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER  //Author : ����� //breif : �̴ϸ� ����
	#define _XDEF_ITEM_V24								// item table v2.4
	#define _XDEF_NEW_SOCKET_071031_KUKURI				// ���� ���� ���� �߰�	: item table v2.4 �ʿ�
	#define _XDEF_4EFF_MOBNAME_DISPLAY_071106_KUKURI	// ����ȥ�� ���ݷ� ���� �̸� ǥ��
	#define ALL_RB_REBIRTH_USERITEM_071108_KUKURI		// ȭŸ�Ŵ� ���� ����

	#define ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI	// ��Ȱâ ������
	#define _XDEF_MATCH_EVENT_071025_KUKURI				// ���� ��ȸ
	#define _XDEF_LOCK_F12BUTTON_071203_MAGVIPER //Author : ����� //breif : �׾����� F12 �� ������ ���ϰ��� Date : 07/12/03
	#define _XDEF_DDAY_CONTRACTITEM_071122_MAGVIPER //Author : ����� //breif : �Ⱓ�� ������ ����Ⱓ �˷���

	#define _XDEF_CB_SCHEDULE_RENEWAL_071211_KUKURI		// ����� ������ ������
	// --------------��

	// 2008.01.17 ---��
	#define _XDEF_NEWENCRYPTION_PACKDATA	// ndcore.dsc ��� ����.
	// --------------��

	// 2008.02.14 ---��
	#define _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : ����� //breif : �̺�Ʈ ������ ������	
	// --------------��

	// 2008.02.26 ---��
	#define _XDEF_SETITEM_080118_MAGVIPER //Author : ����� //breif : ��Ʈ ������
	// --------------��

	#define _XDEF_TOOLTIP_MIXWEAPON_MESSAGE_MAGVIPER //Author : ����� //breif : ���� ���� ���� �޽��� //date : 08/01/17

	// 2008.03.27 ---��
	#define _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI	// ���� ����
	#define _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI		// 2�� ���� : ���� ������ ����
	#define _XDEF_ITEM_V25								// item table v2.5
	#define _XDEF_MASTERY_TOTALPOINT_080313_KUKURI		// ���� ��Ż ����Ʈ ��� ���� : _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI�� ��ĥ ��
	#define _XDEF_CHANGE_SPECIALWEAPON_CONDITION_080313_KUKURI	// ������ �������� ����
	#define _XDEF_LEVELEVENT_080319_MAGVIPER //Author : ����� //breif : ���� �޼� �̺�Ʈ
	// --------------��

	// 2008.04.03 ---��
	#define _XDEF_INCREASE_COLLECTION_INDEX_080328_KUKURI	// ���� �ε��� Ȯ��
	// --------------��

	// 2008.04.24 ---��
	#define _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI		// ������ ���� ���� �������� �ֱ�
	#define ALL_RB_SELECT_CLOTHES_080404_KUKURI			// �ǻ� ��ü �ý��� : ��Ŷ ���� ��(MSG_CHARAC_INFO/MSG_ZONE_CHARAC_INFO/MSG_STATE_CHANGE)
	#define _XDEF_NPCBUFF_CASH_080410_KUKURI			// �����ħ : ������ ħ�� ������
	// --------------��

	// 2008.05.02 ---��
	#define _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI		// �̺�Ʈ�� ��� �ֱ� (��� �̺�Ʈ) -_XDEF_EVENT_RENEWAL_080201_MAGVIPER�ʿ�
	#define _XDEF_EXPAND_INCHANT_TABLE_080425_KUKURI	// ��æƮ ���̺� Ȯ�� (id 2���� �ø�)
	// --------------��

	// 2008.05.29 ---��
    #define _XDEF_MIXWEAPON_PERIODDELETE_080509_MAGVIPER //Author : �����  //Author : ����� //breif : ������ �Ⱓ�� ���־� ��
    #define _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
	#define _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
	#define _XDEF_QUEST_FOR_DUNGEON_080519_KUKURI		// ��� ����Ʈ �и� : MSG_QUEST_AUTH_REQUEST ��Ŷ ����
	#define _XDEF_ADD_MAX_LEARNSKILL_080522_KUKURI		// ��� �� �ִ� ���� �� 60->80���� Ȯ��
	// --------------��


	//#define _XDEF_NEWSPECIALBGMPROCESS_071123_KERYGMA	// Special BGM looping ó��, Special BGM play �� ���� BGM ����.
	
#ifndef _XTESTSERVER // ���ü����� _XDEF_NEXONNEWTESTSERVERLOGIN_071125_KERYGMA�� ���ǵ�.
	#define _XNEXON_WEBLOGIN 
	#define _XDEF_NEXONENDINGADVERTISE_071213_KERYGMA   // Nexon Ending banner, ���ļ����� �����.
#endif
#ifndef _XNEXON_WEBLOGIN
	#define _XDEF_NEWNEXONLOGIN						// Nexon login ��� ����, ���� �α���.
#endif

	#define _XDEF_NPROTECT_GUARD					// GameGuard CS ����
	//#define _XDEF_NPROTECT_GUARD_TIME_LOG			// �޽��� ���� �ð��� ����ֱ����ؼ� LocalTime ���� ���

	// 2008.07.31 ---��
	// 2008.7.16, ���Ȱ��� �߰�
	#define _XDEF_CLIENT_NEWVERSIONCHECK_071112_KERYGMA		// ���������� ����� �ٸ� ��� ������ ��Ŷ ������ ���� ������ �����ϵ��� ��.
	#define _XDEF_SERVERTIMECHECK_071112_KERYGMA			// ������ �ð��� ������ �� �ֵ��� ������������ �ð� ��Ŷ ����.
	#define _XDEF_HACKTOOLCHECK_071112_KERYGMA				// ����Ʈ�� ��ϵ� ������ �����Ͽ� �����ʿ� �α׷� ����.
	#define _XDEF_NEW_PACKET_CRYPT_20071115_KERYGMA			// ��Ŷ ��ȣȭ�� ������ �������� �����Ͽ���.
	#define _XUSEENCRYPTPATTERN								// Packet ��ȣȭ pattern ó��

	#define _XDEF_SELECT_ANOTHERCHARACTER_080721_KUKURI		// �ٸ� ĳ���� ���� ��� �߰� - ���� ���� �ٲ��� ��
	#define _XDEF_AVOID_OVERLAPPING_CASHITEM_080721_KUKURI	// ���� ������ �ߺ� ��� ����

	#define _XDEF_ALLOW_INCHANTITEM_MIXWEAPON_MAGVIPER_080731 //Author : ����� //breif : ���� ������ �������� ���

//	#define _XDEF_MATCH_EVENT_PREVENT_ABUSE_080718_KUKURI	// ������ȸ ���¡ ����
	// --------------��

	//#define _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER		//Author : ����� //breif : ���� �ý���

	// -------------------------------------------------------------------------------------------------------------��
#endif

////////////////////////////////////////////////////////////////
// ���� - ���� ���� Rollback��
//
//#define ALL_RB_NPCSCRIPT_DEATH_070611_KUKURI			// NPC ��ȭ �� �׾��� �� ó��
//
////////////////////////////////////////////////////////////////

#ifdef _XRUSSIAN
	/////////
	#define _XTS_NEWLIFEWINDOW						// ��Ȱâ : ��ǰ,�ڷ� ���� �������̽� ����
	#define _XDEF_NEWVERSONFILETERING				// ���͸� ����Ʈ ���׷��̵�

/* 2006. 11. 14 ---------------------------------------�� */ 
	#define _XDEF_EXPAND_ZONENAME					/* �ؿ��� ��� �����̸��� �� ���� �̸� �̹����� �ÿ��� �Ѵ�.*/ 
/* ----------------------------------------------------�� */ 

	// 2006.12.15 ---------------------------------------��

	// ĳ�� ������ ��� ����  - �ؿܹ������� ĳ�� �������� ��븸 �� ��� �Ʒ��� define�鸸 �������ָ� ��.
	#define _XDEF_CASHITEM_SHOUT					// ��ġ�� (������)
	#define _XTS_MOVEVILLAGE						// ǥ�� �̵�
	#define _XDEF_CLOTHMASK_DIVIDE					// ġ���ǻ��� ĳ�� �����۰� �Ϲ� ���������� ������.- icon ó��
	#define _XDEF_PERIODITEM_20061027				// �Ⱓ�� ������ : �Ⱓ�� ���� TimeValue �߰�
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#define _XTS_ITEM_V22							// item table v2.2
	#define _XDEF_MONSTERREF_14						// ���� ���̺� 1.4

	#define	_XDEF_CHRISTMASEVENT					// ũ�������� �̺�Ʈ
	// --------------------------------------------------��

	
	#define _XTS_SAFETYZONE							// �������� ����
	#define _XTS_PKZONE_SERVERSETTING				// pk server setting
//	#define _XDEF_MOVE_PKSERVER_SELECT				// pk zone���� �̵��� �� ���� �ϵ��� ���� - pk���� ���� �ִ� ���� Ȱ��ȭ�ϸ� �ȵ�
	
	#define _XTS_ALIANCE_20061018					// ������ ���� 
	#define _XDEF_SEARCHPARTYMEMBER					// ���� ã�� ���
	#define _XDEF_MONEYDROP_NO_20070102				// ���� ������ �Ұ���
	#define _XDEF_MONSTERWARNING_MESSAGE_20070102	// ���� ��� ���� -> �ý��� �޽����� �̵�
	#define _XDEF_QUICKSLOTSERVERSAVE				// Quick slot server side save.
	#define _XDEF_HELPSYSTEMSERVERSAVE				// Help log server side save.


	//Author : ����� 
	#define _XDEF_NPROTECT_GUARD					// GameGuard CS ����
	//#define _XDEF_NPROTECT_GUARD_TIME_LOG			// �޽��� ���� �ð��� ����ֱ����ؼ� LocalTime ���� ���

	#define _XUSEENCRYPTPATTERN						// Packet ��ȣȭ pattern ó��
	//Last updated: 07/02/2	

	#define _XDEF_NEWSTRINGDBFORMAT					// ND Text DB Format ����

	#define _XDEF_SHOWEVENTNOTICE					// �̺�Ʈ â �˾� ON/OFF
	#define _XDEF_EVENTWINDOW_WEBPAGE_20070214		// �̺�Ʈ â �� �������� �����

//	#define _XDEF_CASTLEBATTLE						// �����
//	#define _XDEF_CASTLEBATTLE_MOD_CHARACINFO		// Charac info packet ����


//��������۸� 
	#define _XTS_USECASHITEM						// cash item ��� - ĳ�� ������ border�� message â border ó��
	#define _XTS_ITEMMALLBROWSER					// �����۸� - ������ �� browser�� cash item package
	#define _XDEF_COMMONITEMMALL_20070402			// ���� �����۸�
	#define _XDEF_EXPANDSTRINGDBCATEGORY_20070321	// Item mall ���� category �߰�. 

	#define _ACCLAIM_IGAADSYSTEM_DISABLEDUPLICATE_AD_VIEW	// �׾����� �ߺ� ���� ����.
	#define _XDEF_VEQUALITYCONTROL							// Visual effect quality control

	#define	US_RB_NEWSPLITTINGNOTICETEXT_070529_KERYGMA	// ������ �ؽ�Ʈ �и� ��� ���� ( \n �и� ) : �̱��� ����.

	#define _XDEF_RENEWAL_CHATWINDOW_20070503			// ä��â ��� Ȯ��.
	#define ALL_RB_RENEWALCHAT_BUGFIXED_070611_KERYGMA	// ä�� ������ �� (����)���� �κ� ���� ����.

	#define _XDEF_NEW_WORLDMAP_070528_MAGVIPER			/*!< @author ����� @breif �ű�������*/

	#define _XDEF_ESCAPE_20070205						// Ż�� ��� 07/09/04
	#define _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI	// ���� �κ� ���� ��Ŷ ���� 

	#define _XDEF_MODIFY_UI_MONSTER_070710_KUKURI		// ���� ���� ǥ��
	#define _XDEF_NOTICEPOPUPCONTROL__070910_POPUP		// Control popup window
	
	// 2007.10.02 -------------------------------��
	#define _XDEF_EXPANSION_POTION_070709_KUKURI		// ���� ��� Ȯ��
	#define _XDEF_ITEM_V23								// item table v2.3
	// ------------------------------------------��

	// 2007.10.17 -------------------------------��

	#define _XDEF_CASTLEBATTLE							// �����
	#define _XDEF_CASTLEBATTLE_MOD_CHARACINFO			// Charac info packet ����
	#define _XDEF_CASTLEBATTLE_SECONDEDITION			// ����� - 2�� ������Ʈ
	#define _XDEF_CASTLEBATTLE_MAPTAXRATE				// ���� ���� ����
	#define _XDEF_CASTLEBATTLE_CASTLECAPTAIN_20070130	// ����� - ���� �̸�ǥ
	#define _XDEF_CASTLEBATTLE_RECORD					// ����� - ���� 
	#define _XDEF_CASTLEBATTLE_MINIMAP					// ����� - �̴ϸ�
	#define _XDEF_CASTLEBATTLE_SCHEDULE_RENEWAL_US_KUKURI	// ����� - ������ ������ (US)

	#define	_XDEF_SKILLDEALY_MODIFY	/*!< @author ����� @breif ��ų ������ ����Ǵ� �κ� ����*/
	// ------------------------------------------��

	// 2007.11.06 -------------------------------��
	#define VN_RB_GAMBLEBOX_070607_KUKURI				// �пձ� ���� ������ ���� ���� ����(MSG_SM_EQUIP_RESULT.. ��Ŷ ����)
	#define VN_RB_BOSSROOM_BUGFIX_070615_KUKURI			// ����� ���� ������ ���� �����ϴ� ���� ����
	#define _XDEF_DUNGEONTICKET_070615_KUKURI			// �δ� �����
	//#define _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI	// �δ� ����� UI �� �� : �δ������ ���� item ���� ��Ŷ(MSG_EX_BATTLE_ROOM_REQ..)�� ����, �δ������ �� �� �����ؾ� ��
	#define _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI	// �Ⱓ�� ������ â���� ���� ó�� : ������ǰ���� ��Ŷ �����(MSG_CHARAC_MORE_INFO)
	// ------------------------------------------��

	// 2007.11.19 -------------------------------��
	#define _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI		// ǥ���̵��� ������
	//#define _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK����ġ����, ������ PK����
	//#define _XDEF_PK_TRACINGSERVICES_20070402 /*!< @author ����� @breif PK ��ġ���� ����Ʈ */
	// ------------------------------------------��

	// 2007.11.26 ---�� No.38
	#define _XDEF_CLIENT_NEWVERSIONCHECK_071112_KERYGMA
	#define _XDEF_SERVERTIMECHECK_071112_KERYGMA
	#define _XDEF_HACKTOOLCHECK_071112_KERYGMA
	#define _XDEF_NEW_PACKET_CRYPT_20071115_KERYGMA
	// --------------��

	#define	_XDEF_INPUTPRESS	/*!< @author ����� @breif �ʽı� ���� */
	#define _XDEF_BLOCK_MOVINGATTACK	/*!< @author ����� @breif �ʽı� ���� �ɾ� ��ũ�� : _XDEF_INPUTPRESS�� ���� ����Ǿ�� ��*/
										//Author : ����� //breif : �Ѵ�ġ�� ���ϸ��̼��� ����� ��� //date 07/12/03
	#define _XDEF_LOCK_F12BUTTON_071203_MAGVIPER //Author : ����� //breif : �׾����� F12 �� ������ ���ϰ��� Date : 07/12/03

	#define ALL_RB_REBIRTH_USERITEM_071108_KUKURI		// ȭŸ�Ŵ� ���� ����
	#define _XDEF_RECALLSYSTEM_070604_MAGVIPER /*!< @author ����� @breif ģ����ȯ*/

	#define _XDEF_NEWENCRYPTION_PACKDATA	// ndcore.dsc ��� ����.

	// 2008.03.12 ---�� 
	#define _XDEF_MODELTHREADLOADING				// Character model multithread load ó��:���� ����ȭ
	#define ALL_RB_DRAW_CASHCHECK_070820_KUKURI			// ĳ�� ������ �׵θ� �׸��� ������ : cashcheck==1, 2�϶�
	#define _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : ����� //breif : ���� �и� // item table 2.3 �ʿ�
	// --------------��

    // 2008.04.28  --��
    #define _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : ����� //breif : ������ ü�� �޽��� ��� (�븸�� ���Ѵٰ� �� )
    // --------------��

    #define _XTS_COLLECTION_PACKAGE_071023_KUKURI		// ��ǰ ���� - package ���� �߰�
#endif

// ���� ������ ���� �����ϴ� �� 
#ifdef _XENGLISH 
/////////
	#define _XTS_NEWLIFEWINDOW						// ��Ȱâ : ��ǰ,�ڷ� ���� �������̽� ����
	#define _XDEF_NEWVERSONFILETERING				// ���͸� ����Ʈ ���׷��̵�

/* 2006. 11. 14 ---------------------------------------�� */ 
	#define _XDEF_EXPAND_ZONENAME					/* �ؿ��� ��� �����̸��� �� ���� �̸� �̹����� �ÿ��� �Ѵ�.*/ 
/* ----------------------------------------------------�� */ 

	// 2006.12.15 ---------------------------------------��

	// ĳ�� ������ ��� ����  - �ؿܹ������� ĳ�� �������� ��븸 �� ��� �Ʒ��� define�鸸 �������ָ� ��.
	#define _XDEF_CASHITEM_SHOUT					// ��ġ�� (������)
	#define _XTS_MOVEVILLAGE						// ǥ�� �̵�
	#define _XDEF_CLOTHMASK_DIVIDE					// ġ���ǻ��� ĳ�� �����۰� �Ϲ� ���������� ������.- icon ó��
	#define _XDEF_PERIODITEM_20061027				// �Ⱓ�� ������ : �Ⱓ�� ���� TimeValue �߰�
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#define _XTS_ITEM_V22							// item table v2.2
	#define _XDEF_MONSTERREF_14						// ���� ���̺� 1.4

	#define	_XDEF_CHRISTMASEVENT					// ũ�������� �̺�Ʈ
	// --------------------------------------------------��



	// 1�� 15�� �̱� ���� No.2 Patch ����
	#define _XTS_SAFETYZONE							// �������� ����
	#define _XTS_PKZONE_SERVERSETTING				// pk server setting
//	#define _XDEF_MOVE_PKSERVER_SELECT				// pk zone���� �̵��� �� ���� �ϵ��� ���� - pk���� ���� �ִ� ���� Ȱ��ȭ�ϸ� �ȵ�
	
	#define _XTS_ALIANCE_20061018					// ������ ���� 
	#define _XDEF_SEARCHPARTYMEMBER					// ���� ã�� ���
	#define _XDEF_MONEYDROP_NO_20070102				// ���� ������ �Ұ���
	#define _XDEF_MONSTERWARNING_MESSAGE_20070102	// ���� ��� ���� -> �ý��� �޽����� �̵�
	#define _XDEF_QUICKSLOTSERVERSAVE				// Quick slot server side save.
	#define _XDEF_HELPSYSTEMSERVERSAVE				// Help log server side save.


	//Author : ����� 
	#define _XDEF_NPROTECT_GUARD					// GameGuard CS ����
	//#define _XDEF_NPROTECT_GUARD_TIME_LOG			// �޽��� ���� �ð��� ����ֱ����ؼ� LocalTime ���� ���

	#define _XUSEENCRYPTPATTERN						// Packet ��ȣȭ pattern ó��
	//Last updated: 07/02/2	

	#define _XDEF_NEWSTRINGDBFORMAT					// ND Text DB Format ����

	#define _XDEF_SHOWEVENTNOTICE					// �̺�Ʈ â �˾� ON/OFF
	#define _XDEF_EVENTWINDOW_WEBPAGE_20070214		// �̺�Ʈ â �� �������� �����

//	#define _XDEF_CASTLEBATTLE						// �����
//	#define _XDEF_CASTLEBATTLE_MOD_CHARACINFO		// Charac info packet ����


//��������۸� 
	#define _XTS_USECASHITEM						// cash item ��� - ĳ�� ������ border�� message â border ó��
	#define _XTS_ITEMMALLBROWSER					// �����۸� - ������ �� browser�� cash item package
	#define _XDEF_COMMONITEMMALL_20070402			// ���� �����۸�
	#define _XDEF_EXPANDSTRINGDBCATEGORY_20070321	// Item mall ���� category �߰�. 

	#define _ACCLAIM_IGAADSYSTEM_DISABLEDUPLICATE_AD_VIEW	// �׾����� �ߺ� ���� ����.
	#define _XDEF_VEQUALITYCONTROL							// Visual effect quality control

	#define	US_RB_NEWSPLITTINGNOTICETEXT_070529_KERYGMA	// ������ �ؽ�Ʈ �и� ��� ���� ( \n �и� ) : �̱��� ����.

	#define _XDEF_RENEWAL_CHATWINDOW_20070503			// ä��â ��� Ȯ��.
	#define ALL_RB_RENEWALCHAT_BUGFIXED_070611_KERYGMA	// ä�� ������ �� (����)���� �κ� ���� ����.

	#define _XDEF_NEW_WORLDMAP_070528_MAGVIPER			/*!< @author ����� @breif �ű�������*/

	#define _XDEF_ESCAPE_20070205						// Ż�� ��� 07/09/04
	#define _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI	// ���� �κ� ���� ��Ŷ ���� 

	#define _XDEF_MODIFY_UI_MONSTER_070710_KUKURI		// ���� ���� ǥ��
	#define _XDEF_NOTICEPOPUPCONTROL__070910_POPUP		// Control popup window
	
	// 2007.10.02 -------------------------------��
	#define _XDEF_EXPANSION_POTION_070709_KUKURI		// ���� ��� Ȯ��
	#define _XDEF_ITEM_V23								// item table v2.3
	// ------------------------------------------��

	// 2007.10.17 -------------------------------��

	#define _XDEF_CASTLEBATTLE							// �����
	#define _XDEF_CASTLEBATTLE_MOD_CHARACINFO			// Charac info packet ����
	#define _XDEF_CASTLEBATTLE_SECONDEDITION			// ����� - 2�� ������Ʈ
	#define _XDEF_CASTLEBATTLE_MAPTAXRATE				// ���� ���� ����
	#define _XDEF_CASTLEBATTLE_CASTLECAPTAIN_20070130	// ����� - ���� �̸�ǥ
	#define _XDEF_CASTLEBATTLE_RECORD					// ����� - ���� 
	#define _XDEF_CASTLEBATTLE_MINIMAP					// ����� - �̴ϸ�
	#define _XDEF_CASTLEBATTLE_SCHEDULE_RENEWAL_US_KUKURI	// ����� - ������ ������ (US)

	#define	_XDEF_SKILLDEALY_MODIFY	/*!< @author ����� @breif ��ų ������ ����Ǵ� �κ� ����*/
	// ------------------------------------------��

	// 2007.11.06 -------------------------------��
	#define VN_RB_GAMBLEBOX_070607_KUKURI				// �пձ� ���� ������ ���� ���� ����(MSG_SM_EQUIP_RESULT.. ��Ŷ ����)
	#define VN_RB_BOSSROOM_BUGFIX_070615_KUKURI			// ����� ���� ������ ���� �����ϴ� ���� ����
	#define _XDEF_DUNGEONTICKET_070615_KUKURI			// �δ� �����
	//#define _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI	// �δ� ����� UI �� �� : �δ������ ���� item ���� ��Ŷ(MSG_EX_BATTLE_ROOM_REQ..)�� ����, �δ������ �� �� �����ؾ� ��
	#define _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI	// �Ⱓ�� ������ â���� ���� ó�� : ������ǰ���� ��Ŷ �����(MSG_CHARAC_MORE_INFO)
	// ------------------------------------------��

	// 2007.11.19 -------------------------------��
	#define _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI		// ǥ���̵��� ������
	//#define _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK����ġ����, ������ PK����
	//#define _XDEF_PK_TRACINGSERVICES_20070402 /*!< @author ����� @breif PK ��ġ���� ����Ʈ */
	// ------------------------------------------��

	// 2007.11.26 ---�� No.38
	#define _XDEF_CLIENT_NEWVERSIONCHECK_071112_KERYGMA
	#define _XDEF_SERVERTIMECHECK_071112_KERYGMA
	#define _XDEF_HACKTOOLCHECK_071112_KERYGMA
	#define _XDEF_NEW_PACKET_CRYPT_20071115_KERYGMA
	// --------------��

	#define	_XDEF_INPUTPRESS	/*!< @author ����� @breif �ʽı� ���� */
	#define _XDEF_BLOCK_MOVINGATTACK	/*!< @author ����� @breif �ʽı� ���� �ɾ� ��ũ�� : _XDEF_INPUTPRESS�� ���� ����Ǿ�� ��*/
										//Author : ����� //breif : �Ѵ�ġ�� ���ϸ��̼��� ����� ��� //date 07/12/03
	#define _XDEF_LOCK_F12BUTTON_071203_MAGVIPER //Author : ����� //breif : �׾����� F12 �� ������ ���ϰ��� Date : 07/12/03

	#define ALL_RB_REBIRTH_USERITEM_071108_KUKURI		// ȭŸ�Ŵ� ���� ����
	#define _XDEF_RECALLSYSTEM_070604_MAGVIPER /*!< @author ����� @breif ģ����ȯ*/

	#define _XDEF_NEWENCRYPTION_PACKDATA	// ndcore.dsc ��� ����.

	// 2008.03.12 ---�� 
	#define _XDEF_MODELTHREADLOADING				// Character model multithread load ó��:���� ����ȭ
	#define ALL_RB_DRAW_CASHCHECK_070820_KUKURI			// ĳ�� ������ �׵θ� �׸��� ������ : cashcheck==1, 2�϶�
	#define _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : ����� //breif : ���� �и� // item table 2.3 �ʿ�
	#define _XDEF_ITEM_V24								// item table v2.4
	// --------------��

	// 2008.04.17 ---��
	#define _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI		// ���� ���� 
	#define _XDEF_INCREASE_COLLECTION_INDEX_080328_KUKURI	// ���� �ε��� Ȯ��
	#define _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : ����� //breif : �̺�Ʈ ������ ������(MSG_EVENT_RESULT ��Ŷ ����)
	#define _XDEF_LEVELEVENT_080319_MAGVIPER //Author : ����� //breif : ������ �̺�Ʈ	
	#define _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI		// �̺�Ʈ�� ��� �ֱ� (ģ�� ��õ �̺�Ʈ) -_XDEF_EVENT_RENEWAL_080201_MAGVIPER�ʿ�
	// --------------��

    // 2008.04.28  --��
    #define _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : ����� //breif : ������ ü�� �޽��� ��� (�븸�� ���Ѵٰ� �� )
    // --------------��

	// 2008.05.16  --��
	#define _XDEF_EXPAND_INCHANT_TABLE_080425_KUKURI	// ��æƮ ���̺� Ȯ�� (id 2���� �ø�) - ������ �̺�Ʈ
	// --------------��

    #define _XTS_COLLECTION_PACKAGE_071023_KUKURI		// ��ǰ ���� - package ���� �߰�

	// 2008.07.02  --��
	#define _XDEF_WASTEBASKET_20070226				// ������ ��� �߰�
	// --------------��
	
	// 2008.07.29  --��
	#define _XDEF_QUEST_FOR_DUNGEON_080519_KUKURI		// ��� ����Ʈ �и� : MSG_QUEST_AUTH_REQUEST ��Ŷ ����
	// --------------��

	// 2008.08.08  --��
	#define _XDEF_ITEM_V25								// item table v2.5
	#define _XDEF_SETITEM_080118_MAGVIPER //Author : ����� //breif : ��Ʈ ������
	#define _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
	#define _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
	#define _XDEF_SMELT_ITEM //< @author ����� @breif ���/�븰 ���� ������ 
	#define _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI		// 2�� ���� : ���� ������ ����
	#define _XDEF_MASTERY_TOTALPOINT_080313_KUKURI		// ���� ��Ż ����Ʈ ��� ���� : _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI�� ��ĥ ��

	#define ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI	// ��Ȱâ ������
	#define _XDEF_ITEM_ADDVALUE_071024_KUKURI			// ������ - ����, ����, ȸ�Ǽ��� ���� ����ʿ� �ݿ�
	#define _XDEF_CB_SCHEDULE_RENEWAL_071211_KUKURI		// ����� ������ ������
	// --------------��

	// 2008.08.22  --��
	#define _XDEF_SELECT_ANOTHERCHARACTER_080721_KUKURI		// �ٸ� ĳ���� ���� ��� �߰� - ���� ���� �ٲ��� ��
	#define _XDEF_RESOURCE_RENEWAL_071018_KUKURI		// �Ϲ� ���� ������ ��ġ ����	// _XDEF_EVENTWINDOW �ʿ�
	// --------------��

	// 2008.09.23  --��
	#define _XDEF_SAME_CHAT_MESSAGE_080923_KUKURI		// ���� ä�� �޽��� ������ ����
//	#define _ACCLAIM_IGAADSYSTEM		// test
	// --------------��

#endif


// ��Ʈ�� ������ ���� �����ϴ� �� 
#ifdef _XVIETNAMESE
	//-----��Ʈ�� ����-----//
	#define _XDEF_SHUTDOWN_20061027					// ��Ʈ�� �˴ٿ����� ���� 
	#define _XDEF_VINA_PCROOM_INFO						// ��Ʈ�� pc�� : Charac_List ��Ŷ ����
	#define _XDEF_SDS_ONOFF_SYSTEM_070914_KUKURI		// SDS on/off system
	#define _XDEF_TREADE_MESSAGE_ADDITION_070809_MAGVIPER /*!< @author ����� @breif ��ȯ ���� �� �����۰ŷ��ÿ� �޽��� ���*/

	//-----���� �����۸�-------//
	#define _XTS_USECASHITEM						// cash item ��� - ĳ�� ������ border�� message â border ó��
	#define _XTS_ITEMMALLBROWSER					// �����۸� - ������ �� browser�� cash item package
	#define _XDEF_COMMONITEMMALL_20070402			// ���� �����۸�
	#define _XDEF_EXPANDSTRINGDBCATEGORY_20070321	// Item mall ���� category �߰�. 

	//-----���� �� ���α�� ����-------//
	#define _XDEF_MODELTHREADLOADING				// Character model multithread load ó��:���� ����ȭ
	#define _XDEF_CLIENT_NEWVERSIONCHECK_071112_KERYGMA
	#define _XDEF_SERVERTIMECHECK_071112_KERYGMA
	#define _XDEF_HACKTOOLCHECK_071112_KERYGMA
	#define _XDEF_NEW_PACKET_CRYPT_20071115_KERYGMA
	#define _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI	// ���� �κ� ���� ��Ŷ ���� 	
	#define _XDEF_NEWSTRINGDBFORMAT					// ND Text DB Format ����
	#define _XUSEENCRYPTPATTERN						// Packet ��ȣȭ pattern ó��
	#define _XDEF_NPROTECT_GUARD					// GameGuard CS ����
	#define _XDEF_QUICKSLOTSERVERSAVE				// Quick slot server side save.
	#define _XDEF_HELPSYSTEMSERVERSAVE				// Help log server side save.
	#define _XDEF_VEQUALITYCONTROL					// Visual effect quality control
	#define _XDEF_NEWENCRYPTION_PACKDATA	// ndcore.dsc ��� ����.
	
	//-----������ ���̺�-------//
	#define _XTS_ITEM_V22							// item table v2.2
	#define _XDEF_ITEM_V23								// item table v2.3
	#define _XDEF_MONSTERREF_14						// ���� ���̺� 1.4

	//-----���� ����-------//
	#define _XDEF_LOSTWEAPON_070821_MAGVIPER //Author : ����� //breif ���� ������� ����
	#define	_XDEF_SKILLDEALY_MODIFY	/*!< @author ����� @breif ��ų ������ ����Ǵ� �κ� ����*/
	#define	_XDEF_INPUTPRESS	/*!< @author ����� @breif �ʽı� ���� */
	#define _XDEF_BLOCK_MOVINGATTACK	/*!< @author ����� @breif �ʽı� ���� �ɾ� ��ũ�� : _XDEF_INPUTPRESS�� ���� ����Ǿ�� ��*/
										//Author : ����� //breif : �Ѵ�ġ�� ���ϸ��̼��� ����� ��� //date 07/12/03
	#define VN_RB_BOSSROOM_BUGFIX_070615_KUKURI			// ����� ���� ������ ���� �����ϴ� ���� ����
	#define VN_RB_GAMBLEBOX_070607_KUKURI			// �пձ� ���� ������ ���� ���� ���� (MSG_SM_EQUIP_RESULT.. ��Ŷ ����)

	//-----���� ���------//
	#define	_XDEF_CHRISTMASEVENT					// ũ�������� �̺�Ʈ
	#define _XDEF_EXPAND_ZONENAME					/* �ؿ��� ��� �����̸��� �� ���� �̸� �̹����� �ÿ��� �Ѵ�.*/ 
	#define _XTS_NEWLIFEWINDOW						// ��Ȱâ : ��ǰ,�ڷ� ���� �������̽� ����
	#define _XDEF_NEWVERSONFILETERING				// ���͸� ����Ʈ ���׷��̵�
	#define _XTS_SAFETYZONE							// �������� ����
	#define _XTS_PKZONE_SERVERSETTING				// pk server setting
	#define _XDEF_MOVE_PKSERVER_SELECT				// pk zone���� �̵��� �� ���� �ϵ��� ���� - pk���� ���� �ִ� ���� Ȱ��ȭ�ϸ� �ȵ�
	#define _XTS_ALIANCE_20061018					// ������ ���� 
	#define _XDEF_SEARCHPARTYMEMBER					// ���� ã�� ���
	#define _XDEF_MONEYDROP_NO_20070102				// ���� ������ �Ұ���
	#define _XDEF_MONSTERWARNING_MESSAGE_20070102	// ���� ��� ���� -> �ý��� �޽����� �̵�
	#define _XDEF_NOTICEPOPUPCONTROL__070910_POPUP		// Control popup window
//	#define _XDEF_NOTICE_STRING_PROCESS				// ���� �� �������� Ŭ���� �߰� Author : ����� Last updated: 07/01/04
	#define _XDEF_PARTY_NOTAVAILABLE				// �ڸ���� & ���� ��ȭ
	#define _XDEF_WASTEBASKET_20070226				// ������ ��� �߰�
	#define _XTS_MASTERY_04							// masterytable.bms ver 0.4
	#define _XTS_MASTERYOPTIONAL					// ���� ���� - Ȯ��
	#define _XDEF_EXPANSION_POTION_070709_KUKURI		// ���� ��� Ȯ��
	#define _XDEF_MODIFY_UI_MONSTER_070710_KUKURI		// ���� ���� ǥ�� 
	#define _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI	// �Ⱓ�� ������ â���� ���� ó�� : ������ǰ���� ��Ŷ �����(MSG_CHARAC_MORE_INFO)
	#define ALL_RB_DRAW_CASHCHECK_070820_KUKURI			// ĳ�� ������ �׵θ� �׸��� ������ : cashcheck==1, 2�϶�
	#define _XDEF_NPC_ATTACK_CHECK					// ���/��� Shift key ����ϵ��� ó��
	#define _XDEF_NEW_WORLDMAP_070528_MAGVIPER	/*!< @author ����� @breif �ű������� 07_08_28*/
	#define ALL_RB_REMOVEWEAPON_EMOTIONANI_070622_KERYGMA // �λ�� ������ ����.
	#define _XDEF_SHOWEVENTNOTICE					// �̺�Ʈ â �˾� ON/OFF
	#define _XDEF_EVENTWINDOW_WEBPAGE_20070214		// �̺�Ʈ â �� �������� �����

	//-----����ȥ��-----//
	#define	_XDEF_RESIST_DISPLAY					// ����ȥ�� ���׷� ǥ��
	#define _XDEF_4EFF_SYSTEM_070625_KUKURI				// ����ȥ�� �ý��� : _XDEF_NEWSTRINGMANAGER/_XTS_PK �ʿ�
	
	//-----���� ������ ����------//
	#define _XDEF_CASHITEM_SHOUT					// ��ġ�� (������)
	#define _XTS_MOVEVILLAGE						// ǥ�� �̵�
	#define _XDEF_CLOTHMASK_DIVIDE					// ġ���ǻ��� ĳ�� �����۰� �Ϲ� ���������� ������.- icon ó��
	#define _XDEF_PERIODITEM_20061027				// �Ⱓ�� ������ : �Ⱓ�� ���� TimeValue �߰�
	#define _XDEF_SMELT_ITEM /*!< @author ����� @breif ���/�븰 ���� ������ */
	#define _XDEF_RECALLSYSTEM_070604_MAGVIPER /*!< @author ����� @breif ģ����ȯ*/
	#define _XDEF_ESCAPE_20070205					// Ż�� ��� 
	#define _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : ����� //breif : ���� �и� // item table 2.3 �ʿ�
	#define _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI		// ǥ���̵��� ������
	#define _XDEF_DUNGEONTICKET_070615_KUKURI			// �δ� �����
//	#define _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI	// �δ� ����� UI �� ��(���� ����x) : �δ������ ���� item ���� ��Ŷ(MSG_EX_BATTLE_ROOM_REQ..)�� ����
	
	//-----PK ����-----//
	#define _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK����ġ����, ������ PK����
	#define _XDEF_PK_TRACINGSERVICES_20070402 /*!< @author ����� @breif PK ��ġ���� ����Ʈ */

	//-----ä�� ����------//
	#define _XDEF_RENEWAL_CHATWINDOW_20070503			// ä��â ��� Ȯ��.
	#define _XDEF_ZONECHATTING_20070108					// �� ä�� (���� ��ȭ)
	#define ALL_RB_RENEWALCHAT_BUGFIXED_070611_KERYGMA	// ä�� ������ �� (����)���� �κ� ���� ����.
	#define ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA		// ä�� �Է� ���� ����

	//-----����� ����------//
	#define _XDEF_CASTLEBATTLE						// �����
	#define _XDEF_CASTLEBATTLE_MOD_CHARACINFO		// Charac info packet ����
	#define _XDEF_CASTLEBATTLE_CASTLECAPTAIN_20070130	// ����� - ���� �̸�ǥ
	#define _XDEF_CASTLEBATTLE_RECORD					// ����� - ���� 
	#define _XDEF_CASTLEBATTLE_MINIMAP					// ����� - �̴ϸ�
	#define _XDEF_CASTLEBATTLE_SECONDEDITION		// ����� - 2�� ������Ʈ
	#define _XDEF_CASTLEBATTLE_MAPTAXRATE			// ���� ���� ����
	//#define _XDEF_ALIANCE_WEBBOARD_20070130	// ����� - ������,����ȸ �Խ��� �ٷΰ��� 

	//#define _XDEF_WAREHOSE_RESTRAINT_OF_TRADE	/*!< @author ����� @breif Ư�� ������ â�� �ŷ� ����*/
	//#define KR_RB_DELAYSHOWNEXONNOTICEWINDOW_070530_KERYGMA	// 15�� �̿밡 ���� Delay.

	//��������������������������������������������������������������������������������������������������������������������	
	//�� ������ ���� , ������ ���� �ű� �߰�																			��
	//��������������������������������������������������������������������������������������������������������������������
	
	//-----������ �� ���Ŀ� ��------//
	#define _XDEF_ITEM_V24								// item table v2.4
	#define _XDEF_ITEM_V25								// item table v2.5
	#define _XDEF_SETITEM_080118_MAGVIPER //Author : ����� //breif : ��Ʈ ������
	#define _XDEF_MATCH_EVENT_071025_KUKURI				// ���� ��ȸ

	// 2008.08.12  --��
	#define _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI		// 2�� ���� : ���� ������ ����
	#define _XDEF_MASTERY_TOTALPOINT_080313_KUKURI		// ���� ��Ż ����Ʈ ��� ���� : _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI�� ��ĥ ��
//	#define _XDEF_CHANGE_SPECIALWEAPON_CONDITION_080313_KUKURI	// ������ �������� ����

	#define ALL_RB_REBIRTH_USERITEM_071108_KUKURI		// ȭŸ�Ŵ� ���� ����
	#define ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI	// ��Ȱâ ������
	#define _XDEF_ITEM_ADDVALUE_071024_KUKURI			// ������ - ����, ����, ȸ�Ǽ��� ���� ����ʿ� �ݿ�
//	#define _XDEF_CB_SCHEDULE_RENEWAL_071211_KUKURI		// ����� ������ ������
	// --------------��

	//#define _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI	// ���� ����
	//#define _XDEF_INCREASE_COLLECTION_INDEX_080328_KUKURI	// ���� �ε��� Ȯ��

	// 2008.09.18  --��
	#define _XDEF_INTERFACE_RENEWAL_071002_KUKURI		// �������̽� ������ : Jinstatusâ ����,����ġâ �߰�
	// --------------��

	//-----������ ���� ------//
//	#define _XDEF_MONSTERREF_15							// ���� ���̺� v 1.5
//	#define _XDEF_MIXWEAPON_071009_KUKURI				// ���� ���� �ý���	:_XDEF_SMELT_ITEM�� ������ ��ȿ
//	#define _XTS_NEWBATTLEPARAMETER						// ND_Battle_Parameter ���� Ȯ��. 2007.10.10
//	#define _XDEF_RESOURCE_RENEWAL_071018_KUKURI		// �Ϲ� ���� ������ ��ġ ����	// _XDEF_EVENTWINDOW �ʿ�
//	#define _XTS_COLLECTION_PACKAGE_071023_KUKURI		// ��ǰ ���� - package ���� �߰�
//	#define _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER  //Author : ����� //breif : �̴ϸ� ����
//	#define _XDEF_NEW_SOCKET_071031_KUKURI				// ���� ���� ���� �߰�	: item table v2.4 �ʿ�
//	#define _XDEF_4EFF_MOBNAME_DISPLAY_071106_KUKURI	// ����ȥ�� ���ݷ� ���� �̸� ǥ��
	
	#define _XDEF_LOCK_F12BUTTON_071203_MAGVIPER //Author : ����� //breif : �׾����� F12 �� ������ ���ϰ��� Date : 07/12/03
	#define _XDEF_DDAY_CONTRACTITEM_071122_MAGVIPER //Author : ����� //breif : �Ⱓ�� ������ ����Ⱓ �˷���

	//-----���� ���-----//
//	#define _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : ����� //breif : �̺�Ʈ ������ ������
//	#define _XDEF_TOOLTIP_MIXWEAPON_MESSAGE_MAGVIPER //Author : ����� //breif : ���� ���� ���� �޽��� //date : 08/01/17

	//-----������ ���� ��------//

	#define _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI			// ������ ���� ���� �������� �ֱ�
    #define _XDEF_WEBBOARD_20061211							// �� �Խ��� ���	

    // 2008.04.28  --��
    #define _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : ����� //breif : ������ ü�� �޽��� ��� (�븸�� ���Ѵٰ� �� )
    // --------------��

	// 2008.08.26  --��
	#define _XDEF_MASKSYSTEM_SHOW_NAME						// ������ �ᵵ �̸��� ���̵��� - ��Ʈ�� ��û ����
	#define _XDEF_SELECT_ANOTHERCHARACTER_080721_KUKURI		// �ٸ� ĳ���� ���� ��� �߰� - ���� ���� �ٲ��� ��
	// --------------��

#endif



// �븸 ������ ���� �����ϴ� �� 
#ifdef _XTAIWANESE
	#define _XTS_ITEM_V22				// item table v2.2 - 11�� 15�� ����(���ȭ ��)
	#define _XDEF_MONSTERREF_14			// ���� ���̺� 1.4 - 11�� 15�� ����(���ȭ ��)

	// ĳ�� ������ ��� ����  - �ؿܹ������� ĳ�� �������� ��븸 �� ��� �Ʒ��� define�鸸 �������ָ� ��.
	#define _XDEF_CASHITEM_SHOUT					// ��ġ�� (������)
	#define _XTS_MOVEVILLAGE						// ǥ�� �̵�
	#define _XDEF_CLOTHMASK_DIVIDE					// ġ���ǻ��� ĳ�� �����۰� �Ϲ� ���������� ������.- icon ó��
	#define _XDEF_PERIODITEM_20061027				// �Ⱓ�� ������ : �Ⱓ�� ���� TimeValue �߰�
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 2006.12.15 ---------------------------------------��
	//#define	_XDEF_CHRISTMASEVENT					// ũ�������� �̺�Ʈ
	// --------------------------------------------------��
	
	// �븸 No.4 Patch ���Ի��� 2007�� 1�� 22�� ������Ʈ ���׿� ����////////////////////////////////////////////////////////////////////
	#define _XDEF_KEYCHANGE_20070115				// �븸�� ����(�׽�Ʈ�� �������� �ϰ� ������ �븸��) : ŰŸ�� ���� 
	#define _XDEF_MONSTERWARNING_MESSAGE_20070102	// ���� ��� ���� -> �ý��� �޽����� �̵�

	//Author : ����� Last updated: 07/01/18
	//#define _XDEF_NPROTECT_GUARD					// GameGuard CS ���� : �븸 ���Ⱓ ����.
	//#define _XDEF_NPROTECT_GUARD_TIME_LOG			// �޽��� ���� �ð��� ����ֱ����ؼ� LocalTime ���� ���
	#define _XUSEENCRYPTPATTERN						// Packet ��ȣȭ pattern ó��

	#define _XDEF_ZONECHATTING_20070108				// �븸�� ����(�׽�Ʈ�� �������� �ϰ� ������ �븸��) : ���� ��ȭ ���� 

	#define _XDEF_NEWSTRINGDBFORMAT					// ND Text DB Format ����

	// 2�� 28�� �߰� ����
	#define _XDEF_QUICKSLOTSERVERSAVE				// Quick slot server side save.
	#define _XDEF_HELPSYSTEMSERVERSAVE				// Help system server side save.

	#define _XDEF_SEARCHPARTYMEMBER					// ���� ã�� ���

	#define _XTS_REFUSE_ORGANIZATION				// ������ �ʴ� �ź�

	#define _XTS_SAFETYZONE							// �������� ����
	#define _XTS_PKZONE_SERVERSETTING				// pk server setting
	#define _XDEF_MOVE_PKSERVER_SELECT				// pk zone���� �̵��� �� ���� �ϵ��� ����
	//Last updated: 07/02/28

	#define _XDEF_MONEYDROP_NO_20070102				// // ���� ������ �Ұ��� -> �����Ӵ� ��û����.

	// 2007/03/20 ��ȸ �߰�
	#define _XTS_ALIANCE_20061018					// ������ ���� 

	// 2007/04/04 �ʽı� ���� ���� - ���â�� ����
//	#define	_XDEF_INPUTPRESS /*!< @author ����� @breif �ʽı� ���� */
	#define	_XDEF_SKILLDEALY_MODIFY /*!< @author ����� @breif ��ų ������ ����Ǵ� �κ� ����*/
	#define _XDEF_MODELTHREADLOADING				// Character model multithread load ó��. 

	// 2007/04/23
	#define _XDEF_PARTY_NOTAVAILABLE				// �ڸ���� & ���� ��ȭ

	// 2007.07.27 ---��
	#define _XDEF_DUNGEONTICKET_070615_KUKURI			// �δ� �����
	#define _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI	// �δ� ����� UI �� ��(���� ����x) : �δ������ ���� item ���� ��Ŷ(MSG_EX_BATTLE_ROOM_REQ..)�� ����
	#define VN_RB_BOSSROOM_BUGFIX_070615_KUKURI			// ����� ���� ������ ���� �����ϴ� ���� ����
	// --------------��
	
	// 2007.12.05 ---�� No.38
	#define _XDEF_CLIENT_NEWVERSIONCHECK_071112_KERYGMA
	#define _XDEF_SERVERTIMECHECK_071112_KERYGMA
	#define _XDEF_HACKTOOLCHECK_071112_KERYGMA
	#define _XDEF_NEW_PACKET_CRYPT_20071115_KERYGMA

	#define _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI	// ���� �κ� ���� ��Ŷ ���� 
	// --------------��

	// 2008.01.21  ---��  //��ġ ����
	//��������۸� 
	#define _XTS_USECASHITEM						// cash item ��� - ĳ�� ������ border�� message â border ó��
	#define _XTS_ITEMMALLBROWSER					// �����۸� - ������ �� browser�� cash item package
	#define _XDEF_COMMONITEMMALL_20070402			// ���� �����۸�
	#define _XDEF_EXPANDSTRINGDBCATEGORY_20070321	// Item mall ���� category �߰�. 

	#define _XDEF_LOSTWEAPON_070821_MAGVIPER //Author : ����� //breif ���� ������� ����
	#define _XDEF_BLOCK_MOVINGATTACK	/*!< @author ����� @breif �ʽı� ���� �ɾ� ��ũ�� : _XDEF_INPUTPRESS�� ���� ����Ǿ�� ��*/
										//Author : ����� //breif : �Ѵ�ġ�� ���ϸ��̼��� ����� ��� //date 07/12/03

	#define _XDEF_ESCAPE_20070205					// Ż�� ��� 

	#define _XTS_MASTERY_04							// masterytable.bms ver 0.4
	#define _XTS_MASTERYOPTIONAL					// ���� ���� - Ȯ��
	#define	_XDEF_RESIST_DISPLAY					// ����ȥ�� ���׷� ǥ��
	
	#define _XDEF_4EFF_SYSTEM_070625_KUKURI				// ����ȥ�� �ý��� : _XDEF_NEWSTRINGMANAGER/_XTS_PK �ʿ�
	
	#define _XDEF_WASTEBASKET_20070226				// ������ ��� �߰�

	#define _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK����ġ����, ������ PK����
	#define _XDEF_PK_TRACINGSERVICES_20070402 /*!< @author ����� @breif PK ��ġ���� ����Ʈ */

	#define _XDEF_NPC_ATTACK_CHECK	/*!< @author ����� @breif ������� ���ݽ� ����Ʈ Ŭ��*/

	#define _XDEF_RENEWAL_CHATWINDOW_20070503			// ä��â ��� Ȯ��.
	#define _XDEF_ZONECHATTING_20070108					// �� ä�� (���� ��ȭ)
	#define ALL_RB_RENEWALCHAT_BUGFIXED_070611_KERYGMA	// ä�� ������ �� (����)���� �κ� ���� ����.
	#define ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA		// ä�� �Է� ���� ����.

	#define _XDEF_NEW_WORLDMAP_070528_MAGVIPER	/*!< @author ����� @breif �ű�������*/
	#define _XDEF_RECALLSYSTEM_070604_MAGVIPER /*!< @author ����� @breif ģ����ȯ*/

	#define _XDEF_EXPANSION_POTION_070709_KUKURI		// ���� ��� Ȯ��

	#define _XDEF_MODIFY_UI_MONSTER_070710_KUKURI		// ���� ���� ǥ�� 

	#define _XDEF_ITEM_V23								// item table v2.3
	#define _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : ����� //breif : ���� �и� // item table 2.3 �ʿ�

	#define _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI		// ǥ���̵��� ������

	#define _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI	// �Ⱓ�� ������ â���� ���� ó�� : ������ǰ���� ��Ŷ �����(MSG_CHARAC_MORE_INFO)
	#define ALL_RB_DRAW_CASHCHECK_070820_KUKURI			// ĳ�� ������ �׵θ� �׸��� ������ : cashcheck==1, 2�϶�
	#define _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI	// ���� �κ� ���� ��Ŷ ���� 

	//���� ������ ����
	//#define _XDEF_SMELT_ITEM /*!< @author ����� @breif ���/�븰 ���� ������ */

	// --------------��

	// 2008.01.21  ---�� // �ֽ� ������
	#define _XDEF_INTERFACE_RENEWAL_071002_KUKURI		// �������̽� ������ : Jinstatusâ ����,����ġâ �߰�
	#define _XDEF_MONSTERREF_15							// ���� ���̺� v 1.5
	#define _XDEF_MIXWEAPON_071009_KUKURI				// ���� ���� �ý���	:_XDEF_SMELT_ITEM�� ������ ��ȿ
	#define _XTS_NEWBATTLEPARAMETER						// ND_Battle_Parameter ���� Ȯ��. 2007.10.10

	// �븸 ���������� ���غ��� ������ �Ⱥ��̰� �ҽ� �߰��س�����. ���� �߰��� �� �ҽ� �����ؾ� �� (XWindow_LifeSkill.cpp)
	#define _XDEF_RESOURCE_RENEWAL_071018_KUKURI		// �Ϲ� ���� ������ ��ġ ����	// _XDEF_EVENTWINDOW �ʿ�

	#define _XTS_COLLECTION_PACKAGE_071023_KUKURI		// ��ǰ ���� - package ���� �߰�
	#define _XDEF_ITEM_ADDVALUE_071024_KUKURI			// ������ - ����, ����, ȸ�Ǽ��� ���� ����ʿ� �ݿ�
	#define _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER  //Author : ����� //breif : �̴ϸ� ����
	#define _XDEF_ITEM_V24								// item table v2.4
	#define _XDEF_NEW_SOCKET_071031_KUKURI				// ���� ���� ���� �߰�	: item table v2.4 �ʿ�
	#define _XDEF_4EFF_MOBNAME_DISPLAY_071106_KUKURI	// ����ȥ�� ���ݷ� ���� �̸� ǥ��
	#define ALL_RB_REBIRTH_USERITEM_071108_KUKURI		// ȭŸ�Ŵ� ���� ����

	#define ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI	// ��Ȱâ ������
	#define _XDEF_MATCH_EVENT_071025_KUKURI				// ���� ��ȸ
	#define _XDEF_LOCK_F12BUTTON_071203_MAGVIPER //Author : ����� //breif : �׾����� F12 �� ������ ���ϰ��� Date : 07/12/03
	#define _XDEF_DDAY_CONTRACTITEM_071122_MAGVIPER //Author : ����� //breif : �Ⱓ�� ������ ����Ⱓ �˷���

	//#define _XDEF_CB_SCHEDULE_RENEWAL_071211_KUKURI	// ����� ������ ������
	// --------------��

	#define _XDEF_NEWENCRYPTION_PACKDATA	// ndcore.dsc ��� ����.

	// 2008.08.12  --��
	#define _XTS_NEWSKILLWINDOW						// ��ų â ����
	// --------------��


	// 2008.08.20  --��
	#define _XDEF_ALLOW_INCHANTITEM_MIXWEAPON_MAGVIPER_080731 //Author : ����� //breif : ���� ������ �������� ���
	#define _XDEF_TOOLTIP_MIXWEAPON_MESSAGE_MAGVIPER //Author : ����� //breif : ���� ���� ���� �޽��� //date : 08/01/17
	#define _XDEF_MIXWEAPON_PERIODDELETE_080509_MAGVIPER //Author : �����  //Author : ����� //breif : �������� �Ⱓ�� ���־� ��
	// --------------��

	// 2008.08.22  --��
	#define _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : ����� //breif : �̺�Ʈ ������ ������(MSG_EVENT_RESULT ��Ŷ ����)
	#define _XDEF_LEVELEVENT_080319_MAGVIPER //Author : ����� //breif : ������ �̺�Ʈ	
	// --------------��

	// 2008.09.10  --��
	#define _XDEF_ITEM_V25								// item table v2.5
	#define _XDEF_EXPAND_INCHANT_TABLE_080425_KUKURI	// ��æƮ ���̺� Ȯ�� (id 2���� �ø�) - ������ �̺�Ʈ
	// --------------��

#endif

// �Ϻ� ������ ���� �����ϴ� �� 
#ifdef _XJAPANESE
	
//	#define _XTS_NEWLIFEWINDOW						// ��Ȱâ : ��ǰ,�ڷ� ���� �������̽� ����
	#define _XDEF_NEWVERSONFILETERING				// ���͸� ����Ʈ ���׷��̵�

/* 2006. 11. 14 ---------------------------------------�� */ 
	#define _XDEF_EXPAND_ZONENAME					/* �ؿ��� ��� �����̸��� �� ���� �̸� �̹����� �ÿ��� �Ѵ�.*/ 
/* ----------------------------------------------------�� */ 

	// 2006.12.15 ---------------------------------------��

	// ĳ�� ������ ��� ����  - �ؿܹ������� ĳ�� �������� ��븸 �� ��� �Ʒ��� define�鸸 �������ָ� ��.
	#define _XDEF_CASHITEM_SHOUT					// ��ġ�� (������)
	#define _XTS_MOVEVILLAGE						// ǥ�� �̵�
	#define _XDEF_CLOTHMASK_DIVIDE					// ġ���ǻ��� ĳ�� �����۰� �Ϲ� ���������� ������.- icon ó��
	#define _XDEF_PERIODITEM_20061027				// �Ⱓ�� ������ : �Ⱓ�� ���� TimeValue �߰�
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#define _XTS_ITEM_V22							// item table v2.2
	#define _XDEF_MONSTERREF_14						// ���� ���̺� 1.4

	#define	_XDEF_CHRISTMASEVENT					// ũ�������� �̺�Ʈ
	// --------------------------------------------------��

	// 1�� 15�� �̱� ���� No.2 Patch ����
	#define _XTS_SAFETYZONE							// �������� ����
	#define _XTS_PKZONE_SERVERSETTING				// pk server setting
//	#define _XDEF_MOVE_PKSERVER_SELECT				// pk zone���� �̵��� �� ���� �ϵ��� ���� - pk���� ���� �ִ� ���� Ȱ��ȭ�ϸ� �ȵ�
	
	#define _XTS_ALIANCE_20061018					// ������ ���� 
	#define _XDEF_SEARCHPARTYMEMBER					// ���� ã�� ���
	#define _XDEF_MONEYDROP_NO_20070102				// ���� ������ �Ұ���
	#define _XDEF_MONSTERWARNING_MESSAGE_20070102	// ���� ��� ���� -> �ý��� �޽����� �̵�
	#define _XDEF_QUICKSLOTSERVERSAVE				// Quick slot server side save.
	#define _XDEF_HELPSYSTEMSERVERSAVE				// Help log server side save.

	#define _XUSEENCRYPTPATTERN						// Packet ��ȣȭ pattern ó��
	//Last updated: 07/02/2	
	
	#define _XDEF_NPROTECT_GUARD					// GameGuard CS ����
	//#define _XDEF_NPROTECT_GUARD_TIME_LOG			// �޽��� ���� �ð��� ����ֱ����ؼ� LocalTime ���� ���

	#define _XDEF_NEWSTRINGDBFORMAT					// ND Text DB Format ����

//��������۸� 
	#define _XTS_USECASHITEM						// cash item ��� - ĳ�� ������ border�� message â border ó��
	#define _XTS_ITEMMALLBROWSER					// �����۸� - ������ �� browser�� cash item package
	#define _XDEF_COMMONITEMMALL_20070402			// ���� �����۸�
	#define _XDEF_EXPANDSTRINGDBCATEGORY_20070321	// Item mall ���� category �߰�. 

	#define ALL_RB_REMOVEWEAPON_EMOTIONANI_070622_KERYGMA // �λ�� ������ ����.

	#define _XDEF_NEW_WORLDMAP_070528_MAGVIPER	/*!< @author ����� @breif �ű������� 07_08_28*/
	#define _XDEF_TEMP_INVEN_PACKET_MOD_070827_KUKURI	// ���� �κ� ���� ��Ŷ ���� 	

	// 2008.01.15    ---��  //��ġ ����
	#define _XDEF_CLIENT_NEWVERSIONCHECK_071112_KERYGMA
	#define _XDEF_SERVERTIMECHECK_071112_KERYGMA
	#define _XDEF_HACKTOOLCHECK_071112_KERYGMA
	#define _XDEF_NEW_PACKET_CRYPT_20071115_KERYGMA

	#define _XDEF_ITEM_V23								// item table v2.3

	#define _XDEF_MODELTHREADLOADING				// Character model multithread load ó��:���� ����ȭ
	#define VN_RB_GAMBLEBOX_070607_KUKURI			// �пձ� ���� ������ ���� ���� ���� (MSG_SM_EQUIP_RESULT.. ��Ŷ ����)

	#define _XDEF_NOTICEPOPUPCONTROL__070910_POPUP		// Control popup window

	//-----���� ����-------//
	#define _XDEF_LOSTWEAPON_070821_MAGVIPER //Author : ����� //breif ���� ������� ����
	#define	_XDEF_SKILLDEALY_MODIFY	/*!< @author ����� @breif ��ų ������ ����Ǵ� �κ� ����*/
	#define	_XDEF_INPUTPRESS	/*!< @author ����� @breif �ʽı� ���� */
	#define _XDEF_BLOCK_MOVINGATTACK	/*!< @author ����� @breif �ʽı� ���� �ɾ� ��ũ�� : _XDEF_INPUTPRESS�� ���� ����Ǿ�� ��*/
										//Author : ����� //breif : �Ѵ�ġ�� ���ϸ��̼��� ����� ��� //date 07/12/03
	#define VN_RB_BOSSROOM_BUGFIX_070615_KUKURI			// ����� ���� ������ ���� �����ϴ� ���� ����

	//-----��Ÿ ���------//
	#define _XDEF_NOTICE_STRING_PROCESS				// ���� �� �������� Ŭ���� �߰� Author : ����� Last updated: 07/01/04
	#define _XDEF_PARTY_NOTAVAILABLE				// �ڸ���� & ���� ��ȭ
	#define _XDEF_WASTEBASKET_20070226				// ������ ��� �߰�
	#define _XTS_MASTERY_04							// masterytable.bms ver 0.4
	#define _XTS_MASTERYOPTIONAL					// ���� ���� - Ȯ��
	#define _XDEF_EXPANSION_POTION_070709_KUKURI		// ���� ��� Ȯ��
	#define _XDEF_MODIFY_UI_MONSTER_070710_KUKURI		// ���� ���� ǥ�� 
	#define _XDEF_PERIODITEM_USESTORAGE_070813_KUKURI	// �Ⱓ�� ������ â���� ���� ó�� : ������ǰ���� ��Ŷ �����(MSG_CHARAC_MORE_INFO)
	#define ALL_RB_DRAW_CASHCHECK_070820_KUKURI			// ĳ�� ������ �׵θ� �׸��� ������ : cashcheck==1, 2�϶�

	//-----����ȥ��-----//
	#define	_XDEF_RESIST_DISPLAY					// ����ȥ�� ���׷� ǥ��
	#define _XDEF_4EFF_SYSTEM_070625_KUKURI				// ����ȥ�� �ý��� : _XDEF_NEWSTRINGMANAGER/_XTS_PK �ʿ�
	
	//-----���� ������ ����------//
	#define _XDEF_SMELT_ITEM /*!< @author ����� @breif ���/�븰 ���� ������ */
	#define _XDEF_RECALLSYSTEM_070604_MAGVIPER /*!< @author ����� @breif ģ����ȯ*/
	#define _XDEF_ESCAPE_20070205					// Ż�� ��� 
	#define _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER  //Author : ����� //breif : ���� �и� // item table 2.3 �ʿ�
	#define _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI		// ǥ���̵��� ������
	#define _XDEF_DUNGEONTICKET_070615_KUKURI			// �δ� �����
//	#define _XDEF_UI_DELETE_DUNGEONTICKET_070618_KUKURI	// �δ� ����� UI �� ��(���� ����x) : �δ������ ���� item ���� ��Ŷ(MSG_EX_BATTLE_ROOM_REQ..)�� ����
	
	//-----PK ����-----//
	#define _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK����ġ����, ������ PK����
	#define _XDEF_PK_TRACINGSERVICES_20070402 /*!< @author ����� @breif PK ��ġ���� ����Ʈ */

	//-----ä�� ����------//
	#define _XDEF_RENEWAL_CHATWINDOW_20070503			// ä��â ��� Ȯ��.
	#define _XDEF_ZONECHATTING_20070108					// �� ä�� (���� ��ȭ)
	#define ALL_RB_RENEWALCHAT_BUGFIXED_070611_KERYGMA	// ä�� ������ �� (����)���� �κ� ���� ����.
	#define ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA		// ä�� �Է� ���� ����
	// --------------��

	// 2008.01.15  ---�� // �ֽ� ������
	#define _XDEF_INTERFACE_RENEWAL_071002_KUKURI		// �������̽� ������ : Jinstatusâ ����,����ġâ �߰�
	#define _XDEF_MONSTERREF_15							// ���� ���̺� v 1.5
	#define _XDEF_MIXWEAPON_071009_KUKURI				// ���� ���� �ý���	:_XDEF_SMELT_ITEM�� ������ ��ȿ
	#define _XTS_NEWBATTLEPARAMETER						// ND_Battle_Parameter ���� Ȯ��. 2007.10.10
	#define _XDEF_RESOURCE_RENEWAL_071018_KUKURI		// �Ϲ� ���� ������ ��ġ ����	// _XDEF_EVENTWINDOW �ʿ�
	#define _XTS_COLLECTION_PACKAGE_071023_KUKURI		// ��ǰ ���� - package ���� �߰�
	#define _XDEF_ITEM_ADDVALUE_071024_KUKURI			// ������ - ����, ����, ȸ�Ǽ��� ���� ����ʿ� �ݿ�
	#define _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER  //Author : ����� //breif : �̴ϸ� ����
	#define _XDEF_ITEM_V24								// item table v2.4
	#define _XDEF_NEW_SOCKET_071031_KUKURI				// ���� ���� ���� �߰�	: item table v2.4 �ʿ�
	#define _XDEF_4EFF_MOBNAME_DISPLAY_071106_KUKURI	// ����ȥ�� ���ݷ� ���� �̸� ǥ��
	#define ALL_RB_REBIRTH_USERITEM_071108_KUKURI		// ȭŸ�Ŵ� ���� ����

	#define ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI	// ��Ȱâ ������
	#define _XDEF_LOCK_F12BUTTON_071203_MAGVIPER //Author : ����� //breif : �׾����� F12 �� ������ ���ϰ��� Date : 07/12/03
	#define _XDEF_DDAY_CONTRACTITEM_071122_MAGVIPER //Author : ����� //breif : �Ⱓ�� ������ ����Ⱓ �˷���

	//#define _XDEF_CB_SCHEDULE_RENEWAL_071211_KUKURI		// ����� ������ ������
	// --------------��

	// 2008.02.05 ---��
	#define _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : ����� //breif : �̺�Ʈ ������ ������	
	// --------------��


	//��ġ ����?
	//#define _XDEF_MATCH_EVENT_071025_KUKURI			// ���� ��ȸ

	//----�����----
	//#define _XDEF_CASTLEBATTLE						// �����
	//#define _XDEF_CASTLEBATTLE_MOD_CHARACINFO		// Charac info packet ����
	//#define _XDEF_CASTLEBATTLE_CASTLECAPTAIN_20070130	// ����� - ���� �̸�ǥ
	//#define _XDEF_CASTLEBATTLE_RECORD					// ����� - ���� 
	//#define _XDEF_CASTLEBATTLE_MINIMAP					// ����� - �̴ϸ�
	//#define _XDEF_ALIANCE_WEBBOARD_20070130	// ����� - ������,����ȸ �Խ��� �ٷΰ��� 
	//#define _XDEF_CASTLEBATTLE_SECONDEDITION		// ����� - 2�� ������Ʈ
	//#define _XDEF_CASTLEBATTLE_MAPTAXRATE			// ���� ���� ����
	
	//----���----
	//#define _XDEF_WAREHOSE_RESTRAINT_OF_TRADE	/*!< @author ����� @breif Ư�� ������ â�� �ŷ� ����*/
	//#define _XDEF_NPC_ATTACK_CHECK	/*!< @author ����� @breif ������� ���ݽ� ����Ʈ Ŭ��*/
	//#define _XDEF_SHOWEVENTNOTICE					// �̺�Ʈ â �˾� ON/OFF
	//#define _XDEF_EVENTWINDOW_WEBPAGE_20070214		// �̺�Ʈ â �� �������� �����

	//#define KR_RB_DELAYSHOWNEXONNOTICEWINDOW_070530_KERYGMA	// 15�� �̿밡 ���� Delay.

	#define _XDEF_CLIENT_NEWVERSIONCHECK_071112_KERYGMA
	#define _XDEF_SERVERTIMECHECK_071112_KERYGMA
	#define _XDEF_HACKTOOLCHECK_071112_KERYGMA
	#define _XDEF_NEW_PACKET_CRYPT_20071115_KERYGMA

	#define _XDEF_NEWENCRYPTION_PACKDATA	// ndcore.dsc ��� ����.

	// 2008.03.20 ---��
	#define _XDEF_NPC_ATTACK_CHECK					// ���/��� Shift key ����ϵ��� ó��
	// --------------��

    // 2008.04.28  --��
    #define _XDEF_DAMAGECHATMESSAGE_080428_MAGVIPER // Author : ����� //breif : ������ ü�� �޽��� ��� (�븸�� ���Ѵٰ� �� )
    // --------------��

    // 2008.05.08  --��
    #define _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI		// ���� ���� 
	#define _XDEF_INCREASE_COLLECTION_INDEX_080328_KUKURI	// ���� �ε��� Ȯ��
    // --------------��

    // 2008.06.10  --��
    #define _XDEF_MIXWEAPON_PERIODDELETE_080509_MAGVIPER //Author : �����  //Author : ����� //breif : �������� �Ⱓ�� ���־� ��
    // --------------��

	// 2008.08.12  --��
	#define _XTS_NEWSKILLWINDOW						// ��ų â ����
	#define _XDEF_ALLOW_INCHANTITEM_MIXWEAPON_MAGVIPER_080731 //Author : ����� //breif : ���� ������ �������� ���
	// --------------��

	// 2008.08.14  --��
	#define _XDEF_SELECT_ANOTHERCHARACTER_080721_KUKURI		// �ٸ� ĳ���� ���� ��� �߰� - ���� ���� �ٲ��� ��
	#define ALL_RB_SELECT_CLOTHES_080404_KUKURI			// �ǻ� ��ü �ý��� : ��Ŷ ���� ��(MSG_CHARAC_INFO/MSG_ZONE_CHARAC_INFO/MSG_STATE_CHANGE)
	// --------------��

	// 2008.08.28  --��
	#define _XDEF_ITEM_V25								// item table v2.5
	#define _XDEF_SETITEM_080118_MAGVIPER //Author : ����� //breif : ��Ʈ ������
	// --------------��

	// 2008.09.26  --��
	#define _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
	#define _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��
	#define _XDEF_WEBBOARD_20061211							// �� �Խ��� ���	
	// --------------��

#endif


////////////////////////////////////////////////////////////////////////////////////
//���ü��� ���� ------------------------------------------------------------------��

#ifdef _XTESTSERVER 

	// 2007.10.08 ---��
	#define _XDEF_INTERFACE_RENEWAL_071002_KUKURI		// �������̽� ������ : Jinstatusâ ����,����ġâ �߰�
	#define _XDEF_MONSTERREF_15							// ���� ���̺� v 1.5
	#define _XDEF_MIXWEAPON_071009_KUKURI				// ���� ���� �ý���	:_XDEF_SMELT_ITEM�� ������ ��ȿ
	#define _XTS_NEWBATTLEPARAMETER						// ND_Battle_Parameter ���� Ȯ��. 2007.10.10

	#undef _XDEF_WEBBOARD_20061211						// �� �Խ��� ��� ����
	#undef _XDEF_ALIANCE_WEBBOARD_20070130					//������,����ȸ �Խ��� �ٷΰ���  ��� ����
	// --------------��

	// 2007.10.18 ---��
	#define _XDEF_RESOURCE_RENEWAL_071018_KUKURI		// �Ϲ� ���� ������ ��ġ ����	// _XDEF_EVENTWINDOW �ʿ�
	// --------------��

	// 2007.10.29 ---��
	#define _XTS_COLLECTION_PACKAGE_071023_KUKURI		// ��ǰ ���� - package ���� �߰�
	#define _XDEF_ITEM_ADDVALUE_071024_KUKURI			// ������ - ����, ����, ȸ�Ǽ��� ���� ����ʿ� �ݿ�
	#define _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER  //Author : ����� //breif : �̴ϸ� ����
	// --------------��

	// 2007.11.05 ---��
	#define _XDEF_ITEM_V24								// item table v2.4
	#define _XDEF_NEW_SOCKET_071031_KUKURI				// ���� ���� ���� �߰�	: item table v2.4 �ʿ�
	// --------------��

	// 2007.11.05 ---��
	#define _XDEF_4EFF_MOBNAME_DISPLAY_071106_KUKURI	// ����ȥ�� ���ݷ� ���� �̸� ǥ��
	#define ALL_RB_REBIRTH_USERITEM_071108_KUKURI		// ȭŸ�Ŵ� ���� ����
	// --------------��

	// 2007.12.07 ---��
	#define ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI	// ��Ȱâ ������
	#define _XDEF_MATCH_EVENT_071025_KUKURI				// ���� ��ȸ
	#define _XDEF_LOCK_F12BUTTON_071203_MAGVIPER //Author : ����� //breif : �׾����� F12 �� ������ ���ϰ��� Date : 07/12/03
	#define _XDEF_DDAY_CONTRACTITEM_071122_MAGVIPER //Author : ����� //breif : �Ⱓ�� ������ ����Ⱓ �˷���
	// --------------��

	// 2007.12.17 ---��
	#define _XDEF_CB_SCHEDULE_RENEWAL_071211_KUKURI		// ����� ������ ������
	// --------------��

	// 2008.01.15 ---��
	#define _XDEF_NEWENCRYPTION_PACKDATA				// ndcore.dsc ��� ����.
	// --------------��
	
	// 2008.01.28 ---��
	#define _XDEF_TOOLTIP_MIXWEAPON_MESSAGE_MAGVIPER //Author : ����� //breif : ���� ���� ���� �޽��� //date : 08/01/17
	// --------------��

	// 2008.02.18 ---��
	#define _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : ����� //breif : �̺�Ʈ ������ ������	
	#define _XDEF_SETITEM_080118_MAGVIPER //Author : ����� //breif : ��Ʈ ������
	// --------------��

	// 2008.03.10 ---��
	#define _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI	// ���� ����
	#define _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI		// 2�� ���� : ���� ������ ����
	#define _XDEF_ITEM_V25								// item table v2.5
	// --------------��

	// 2008.03.21 ---��
	#define _XDEF_MASTERY_TOTALPOINT_080313_KUKURI		// ���� ��Ż ����Ʈ ��� ���� : _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI�� ��ĥ ��
	#define _XDEF_CHANGE_SPECIALWEAPON_CONDITION_080313_KUKURI	// ������ �������� ����
	// --------------��

	// 2008.03.28 ---��
	#define _XDEF_NPROTECT_GUARD						// GameGuard CS ����
	// --------------��

	// 2008.04.03 ---��
	#define _XDEF_INCREASE_COLLECTION_INDEX_080328_KUKURI	// ���� �ε��� Ȯ��
	#define _XDEF_BW_WAR_ENTER_GATE_080317_KUKURI			// ������ ���� ���� �������� �ֱ�
	// --------------��

	// 2008.04.18 ---��
	#define ALL_RB_SELECT_CLOTHES_080404_KUKURI			// �ǻ� ��ü �ý��� : ��Ŷ ���� ��(MSG_CHARAC_INFO/MSG_ZONE_CHARAC_INFO/MSG_STATE_CHANGE)
	#define _XDEF_NPCBUFF_CASH_080410_KUKURI			// �����ħ : ������ ħ�� ������
	// --------------��

	////////////
	// �۾���...�ڼ���
	//#define _XTS_ITEMRENEWAL				// item �ڵ� ����
	//#define _XTS_MOTIONBLURTEST			// motion blur test
	//
	////////////

	////////////
	// �۾��Ϸ�...�ڼ���
	//#define _XTS_ACCESSORY_NEWTYPE		// �Ǽ��縮 new type �߰�
	//
	///////////

	//#define _XTS_XTGVER_89			// �������� 0.0089f Load�Լ� ���� ( Warp Zone, Functional Object )

	#undef _XTS_SERVERUNIFICATION		// ���ü������� �������� ���� ����
	//#undef _XDEF_NEXONLOGIN				// �ؽ� �α��� - �α��� ��Ŷ ���� ����
	//#undef _XDEF_NEXONLOGINFINAL		// �ؽ� �޽����� connect �ϴ� �κ� ����
	#undef _XDEF_NEXONUNIFICATIONGUIDE	//���ļ��� �ؽ� ���� �ȳ��� ǥ�� ����

	#define _XDEF_NEXONNEWTESTSERVERLOGIN_071125_KERYGMA	// �ؽ� �α��� : �� ���(���ŷ ���) ����, ���ü��������� �����. ������ _XNEXON_WEBLOGIN �� ���Ե�.

	//#define _XDEF_LEVELEVENT_080319_MAGVIPER //Author : ����� //breif : ������ �̺�Ʈ	
	//#define _XDEF_NOTORIOUS_SYSTEM_MAGVIPER //Author : ����� //breif : ���� �ý��� 

	#define _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
	#define _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : ����� //breif : â�� Ȯ��

#endif

//���ü��� �� ---------------------------------------------------------------------��
///////////////////////////////////////////////////////////////////////////////////////


// Define for Inner Test server -----------------------------------��
#ifdef _XINNERTESTSERVER

#ifndef _XTESTSERVER
#define _XTESTSERVER
#endif

#define _XTS_ITEMPENERTY		// ����� ���������� ���� ���� �������� ������ ��ġ�� ���� ��Ŵ
#define _XTS_FURY				// �г�/����

//#define _XTS_NEW_HONOR2		// ��
//#define _XTS_TARGET_STATE		// �ι� ����ȿ�� ǥ��
//#define _XTS_PARTYQUEST		// ��Ƽ����Ʈ ���� ON/OFF
//#define _XTS_MINISTOPEVENT	// MINISTOP ���� ON/OFF
//#define _XTS_CASHITEM			// ���������

#endif	// --------------------------------------------------------��

#endif // __cplusplus
#endif // __XKERNEL_H__
