// XNetwork_PersonalStore.cpp : ���λ��� ����
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"
#include "SoundEffectList.h"
#include "XSR_STRINGHEADER.H"
#include "XMessageWindow.h"
#include "XWindowObjectDefine.h"

#include "XWindow_PersonalStore.h"
#include "XWindow_TradeConfirm.h"
#include "XWindow_CashBox.h"

extern void _XInsertCenterSystemMessage( LPTSTR message , D3DCOLOR messageinfo );

BOOL _XNetwork::Send_MSGPersonalStore(int type, int param1, int param2, int param3, int param4)
{
	if( type == MSG_NO_PS_CTRL)
	{
		switch(param1) {
		case ps_ctrl_start_req:
			{
				VIEWPACKETLOG
					_XDWPACKETLOG( "SEND MESSAGE : Personal Store Start Request Socket Send ---- " );
				
				int size = sizeof(MSG_PS_CTRL_START_REQ);
				MSG_PS_CTRL_START_REQ psstartreq;
				ZeroMemory(&psstartreq, size);
				psstartreq.header.sLength = size;
				psstartreq.ucMessage = MSG_NO_PS_CTRL;	
				psstartreq.ucCtrl = ps_ctrl_start_req;
				psstartreq.cMode = (char)param2;
				
				_XLog("<<<<<<<<<<<<<<<<<<<<     PERSONAL STORE     >>>>>>>>>>>>>>>>>>>>" );	
				if( psstartreq.cMode == 1 )
				{
					_XLog("<<<<<<<<<<<<<<<<<<<<     PERSONAL STORE FIRST    >>>>>>>>>>>>>>>>>>>>" );	
				}
				else
				{
					_XLog("<<<<<<<<<<<<<<<<<<<<     PERSONAL STORE MODIFY    >>>>>>>>>>>>>>>>>>>>" );	
				}
				
				_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
				if( pPersonalStore_Window )
				{
					// ��ϵ� ���λ����̸�. 
					strcpy( psstartreq.PSName, pPersonalStore_Window->m_PersonalStoreName );
					
					pPersonalStore_Window->m_TotalCountTradeItem = 0;
					for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; ++i)
					{
						if( pPersonalStore_Window->m_TradeItem[i].Get_m_sID() != 0 )
						{
							psstartreq.InvenIdx[i]		= pPersonalStore_Window->m_TradeItem[i].m_InventoryNumber;// 9�� ������. 						
							psstartreq.ucItemCount[i]	= (unsigned char)pPersonalStore_Window->m_TradeItem[i].Get_m_ucCount();// ������ ������ �ȷ��� ����
							psstartreq.ItemPrice[i]		= (unsigned int)pPersonalStore_Window->m_TradeItemPrice[i];// 9�� ������ ����.
							
							_XLog("PERSONAL STORE PRICE : index[%d] price[%d]",i, pPersonalStore_Window->m_TradeItemPrice[i] );
							
							pPersonalStore_Window->m_TotalCountTradeItem++;
						}
					}
					psstartreq.ItemNum = (char)pPersonalStore_Window->m_TotalCountTradeItem;// ������ ����.	
				}
				
				return _XSendPacket_Encapsulate( (char*)&psstartreq );
			}
			break;
		case ps_ctrl_broad_req_cast:
			{
				VIEWPACKETLOG
					_XDWPACKETLOG( "SEND MESSAGE : Personal Store Broadcasting Request Socket Send ---- " );
				
				int size = sizeof(MSG_PS_CTRL_BROAD_REQ_CAST);
				MSG_PS_CTRL_BROAD_REQ_CAST psnamebroadcasting;
				ZeroMemory(&psnamebroadcasting, size);
				psnamebroadcasting.header.sLength = size;
				psnamebroadcasting.ucMessage = MSG_NO_PS_CTRL;	
				psnamebroadcasting.ucCtrl = ps_ctrl_broad_req_cast;
				
				// �ڱⰡ ���� ���Գѹ�. 
				psnamebroadcasting.usPSSLotNo = (unsigned short)param2;
				
				return _XSendPacket_Encapsulate( (char*)&psnamebroadcasting );
			}
			break;
		case ps_ctrl_stop_req:
			{
				VIEWPACKETLOG
					_XDWPACKETLOG( "SEND MESSAGE : Personal Store Stop Request Socket Send ---- " );
				
				int size = sizeof(MSG_PS_CTRL_BROAD_REQ_CAST);
				MSG_PS_CTRL_BROAD_REQ_CAST psstopreq;
				ZeroMemory(&psstopreq, size);
				psstopreq.header.sLength = size;
				psstopreq.ucMessage = MSG_NO_PS_CTRL;		
				psstopreq.ucCtrl = ps_ctrl_stop_req;
				
				// �ڱⰡ ���� ���Գѹ�. 
				psstopreq.usPSSLotNo = (unsigned short)param2;
				psstopreq.cMode = (char)param3;
				
				return _XSendPacket_Encapsulate( (char*)&psstopreq );
			}
			break;
		case ps_ctrl_shopitem_list_req:
			{
				VIEWPACKETLOG
					_XDWPACKETLOG( "SEND MESSAGE : Personal Store shopping list Request Socket Send ---- " );
				
				int size = sizeof(MSG_PS_CTRL_SHOPITEM_LIST_REQ);
				MSG_PS_CTRL_SHOPITEM_LIST_REQ psshopitemlist;
				ZeroMemory(&psshopitemlist, size);
				psshopitemlist.header.sLength = size;
				psshopitemlist.ucMessage = MSG_NO_PS_CTRL;		
				psshopitemlist.ucCtrl = ps_ctrl_shopitem_list_req;

				psshopitemlist.usUID = (unsigned short)param2;			
				psshopitemlist.cMode = (char)param3;			
				
				return _XSendPacket_Encapsulate( (char*)&psshopitemlist );
			}
			break;
		case ps_ctrl_shopitem_buy_req:
			{
				VIEWPACKETLOG
					_XDWPACKETLOG( "SEND MESSAGE : Personal Store buy Request Socket Send ---- " );
				
				int size = sizeof(MSG_PS_CTRL_SHOPITEM_BUY_REQ);
				MSG_PS_CTRL_SHOPITEM_BUY_REQ psshopitembuy;
				ZeroMemory(&psshopitembuy, size);
				psshopitembuy.header.sLength = size;
				psshopitembuy.ucMessage = MSG_NO_PS_CTRL;	
				psshopitembuy.ucCtrl = ps_ctrl_shopitem_buy_req;
				
				
				char cType = ((DWORD)param3 & 0xFFFF0000) >> 16;
				short sID = ((DWORD)param3 & 0x0000FFFF);

				psshopitembuy.usUID			 = (unsigned short)param2;
				psshopitembuy.cType			 = cType;
				psshopitembuy.sID			 = sID;
				psshopitembuy.cSlotNoforBuy	 = (char)param4;
				
				return _XSendPacket_Encapsulate( (char*)&psshopitembuy );
			}
			break;
		}

	}

	return TRUE;
	
}


void _XNetwork::MSGPersonalStoreCtrlStartResult(MSG_PS_CTRL_START_RESULT* psstartresult)
{
	switch(psstartresult->cReturn) {
	case -1: // -1�̸� ������ ���� �� ���� ���̴�.
	case -2: // -2�̸� �������� �ø��ٰ� ������ �� ���̴�.  ���� ������ ����.
	case -3: // -3�̸� �÷��̾� ���°� ��� ���°� �ƴ�. 
		{
			// ��� �ִ� ���� ����
			g_pLocalUser->ReloadWeaponModel();
			
			g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
			g_pLocalUser->SetAnimation(_XACTION_IDLE );
			g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE ); 
						
			_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
			if( pPersonalStore_Window )
			{
				for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; ++i)
				{
					if( pPersonalStore_Window->m_TradeItem[i].Get_m_sID() != 0)
					{
						int inventoryindex = pPersonalStore_Window->m_TradeItem[i].m_InventoryNumber;
						if( pPersonalStore_Window->m_TradeItem[i].IsStackable() )
						{
							if( g_pLocalUser->m_UserItemList[inventoryindex].Get_m_ucCount() == 0)
							{
								g_pLocalUser->m_UserItemList[inventoryindex].Copy( pPersonalStore_Window->m_TradeItem[i] );
							}
							else
							{
								g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucCount(g_pLocalUser->m_UserItemList[inventoryindex].Get_m_ucCount() + pPersonalStore_Window->m_TradeItem[i].Get_m_ucCount());
							}
						}
						else
						{
							g_pLocalUser->m_UserItemList[inventoryindex].Copy( pPersonalStore_Window->m_TradeItem[i] );
						}
					}
				}
				
				pPersonalStore_Window->m_btnOpen->EnableWindow(TRUE);
				pPersonalStore_Window->m_btnCancle->EnableWindow(TRUE);
				pPersonalStore_Window->m_btnOpen->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_BOOTH_OPEN );
				pPersonalStore_Window->ResetTradeInfo();
				pPersonalStore_Window->ShowWindow(FALSE);
				g_pInventory_Window->ShowWindow(FALSE);
				
				// ������ ������ Inventory lock ����
				if(g_pInventory_Window)
				{
					if(g_pInventory_Window->m_bLockInventory)
						g_pInventory_Window->m_bLockInventory = FALSE;
				}
				_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);				
				
				InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKPERSONALSTORE_1682), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
				_XInsertCenterSystemMessage(_XGETINTERFACETEXT(ID_STRING_NETWORKPERSONALSTORE_1682) , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}
		}
		break;
	default: // ����
		{
			g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_OWNER_OPEN;
			_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
			if( pPersonalStore_Window )
			{
				pPersonalStore_Window->m_ServerPersonalStoreID = psstartresult->usPSSlotNo;
				pPersonalStore_Window->m_btnOpen->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORKPERSONALSTORE_1683);
				pPersonalStore_Window->m_btnOpen->EnableWindow(TRUE);
				pPersonalStore_Window->m_btnCancle->EnableWindow(TRUE);

				InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKPERSONALSTORE_1684), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
				_XInsertCenterSystemMessage(_XGETINTERFACETEXT(ID_STRING_NETWORKPERSONALSTORE_1684) , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}
		}
		break;
	}

}

void _XNetwork::MSGPersonalStoreCtrlNameBroadCasting(MSG_PS_CTRL_BROAD_CAST* psnamebroadcasting)
{	
	if(g_pLocalUser->m_Uniqueid == psnamebroadcasting->usUID) return;

	_XUser* pUser = g_Userlist.FindUser(psnamebroadcasting->usUID);
	if(pUser)
	{
		pUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_OWNER_OPEN;

		if( strcmp( pUser->m_PersonalStoreName, psnamebroadcasting->PSName ) != 0 ) 
		{
			strcpy( pUser->m_PersonalStoreName, psnamebroadcasting->PSName );
//			_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
//			if( pPersonalStore_Window )
//			{
//				if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_CLIENT_INTEREST || g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_CLIENT_MODIFY )
//				{
//					strcpy( pPersonalStore_Window->m_PersonalStoreName, psnamebroadcasting->PSName );
//				}
//			}

		}
	}
}

void _XNetwork::MSGPersonalStoreCtrlStopResult(MSG_PS_CTRL_STOP_RESULT* psstopresult)
{
	switch(psstopresult->cReturn) {
	case -1: // -1�̸� ������ ���� �� ���� ���̴�.
	case -2: // -2�̸� �������� �ø��ٰ� ������ �� ���̴�.  ���� ������ ����.
	case -3: // -3�̸� �÷��̾� ���°� ��� ���°� �ƴ�. 
		{
			g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_OWNER_OPEN;
			_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
			
			InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKPERSONALSTORE_1685), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
			_XInsertCenterSystemMessage(_XGETINTERFACETEXT(ID_STRING_NETWORKPERSONALSTORE_1685) , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
			if( pPersonalStore_Window )
			{
				pPersonalStore_Window->m_btnOpen->EnableWindow(TRUE);
				pPersonalStore_Window->m_btnCancle->EnableWindow(TRUE);
			}
		}
		break;
	default: // ������ �ݴ´�. ����
		{
			if( psstopresult->cMode == 0 ) //���� �ݱ� ��� 
			{
				// ��� �ִ� ���� ����
				g_pLocalUser->ReloadWeaponModel();

				g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
				g_pLocalUser->SetAnimation(_XACTION_IDLE );
				g_NetworkKernel.SendPacket( MSG_NO_ZONE_MOVE ); 
				
				_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
				if( pPersonalStore_Window )
				{
					for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; ++i)
					{
						if( pPersonalStore_Window->m_TradeItem[i].Get_m_sID() != 0)
						{
							int inventoryindex = pPersonalStore_Window->m_TradeItem[i].m_InventoryNumber;
							if( pPersonalStore_Window->m_TradeItem[i].IsStackable() )
							{
								if( g_pLocalUser->m_UserItemList[inventoryindex].Get_m_ucCount() == 0)
								{
									g_pLocalUser->m_UserItemList[inventoryindex].Copy( pPersonalStore_Window->m_TradeItem[i] );
								}
								else
								{
									g_pLocalUser->m_UserItemList[inventoryindex].Set_m_ucCount(g_pLocalUser->m_UserItemList[inventoryindex].Get_m_ucCount() + pPersonalStore_Window->m_TradeItem[i].Get_m_ucCount());
								}
							}
							else
							{
								g_pLocalUser->m_UserItemList[inventoryindex].Copy( pPersonalStore_Window->m_TradeItem[i] );
							}
						}
					}
					pPersonalStore_Window->ShowWindow(FALSE);
					g_pInventory_Window->ShowWindow(FALSE);
					pPersonalStore_Window->m_btnOpen->EnableWindow(TRUE);
					pPersonalStore_Window->m_btnCancle->EnableWindow(TRUE);
					pPersonalStore_Window->m_btnOpen->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_BOOTH_OPEN);
					pPersonalStore_Window->ResetTradeInfo();
					
					_XWindow_TradeConfirm* pTradeConfirm_Window = (_XWindow_TradeConfirm*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TRADECONFIRM);
					if(pTradeConfirm_Window)
					{
						if( pTradeConfirm_Window->GetShowStatus() )
						{
							if( pTradeConfirm_Window->m_TradeMode == _XPERSONALSTORE_TOTRADE_ITEM )
							{							
								PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_TRADECONFIRM_CLOSE, 0), (LPARAM)gHWnd); 
							}
						}
					}
					
					// ������ ������ Inventory lock ����
					if(g_pInventory_Window)
					{
						if(g_pInventory_Window->m_bLockInventory)
							g_pInventory_Window->m_bLockInventory = FALSE;
					}
					
					_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);					
					InsertChatString( _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_CLOSESTORE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				}
			}
			else // ���� ���� ��� 
			{
				g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_OWNER_MODIFY;
				
				_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
				if( pPersonalStore_Window )
				{
					pPersonalStore_Window->m_btnOpen->EnableWindow(TRUE);
					pPersonalStore_Window->m_btnCancle->EnableWindow(TRUE);
					pPersonalStore_Window->m_btnOpen->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_BOOTH_OPEN);
					
					InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKPERSONALSTORE_1686), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				}
			}
		}
		break;
	}
}

void _XNetwork::MSGPersonalStoreCtrlShoppingListResult(MSG_PS_CTRL_SHOPITEM_LIST_RESULT* psshoplistresult)
{	
//	_sHeader            header    ;
//	u_char              ucMessage ;
//	char                cReturn   ;  // -1�̸� ������ ����Ʈ�� ��ϵ� �����̸� ����Ʈ�� ��� �͵� �����Ͽ���. 
//									 // -2�̸� �ڽ��� Ʈ���̵� ���̰ų� Ʈ���̵� ��û�� ���� �����̴�. ���� ���з� ó��. 
//	PSItemList          ItemList[9]; // ���� ������ �ö��ִ� ������ ����Ʈ. 
	
//	char   cItemType   ;               // �ö� ������ Ÿ��.	
//	char   cItemsecond_type;           //
//	short  sItem_Id    ;               //
//	u_char ucSocket[4] ;		       // ��������.
//	u_char ucItemCount ;               // �ȷ��� ������ ����.
//	u_int  uiPrice     ;               // ��ü �������.


	switch(psshoplistresult->cReturn) {
	case -1: // -1�̸� ������ ����Ʈ�� ��ϵ� �����̸� ����Ʈ�� ��� �͵� �����Ͽ���. 
	case -2: // -2�̸� �ڽ��� Ʈ���̵� ���̰ų� Ʈ���̵� ��û�� ���� �����̴�. ���� ���з� ó��. 
		{
			g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;
			_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
			if( pPersonalStore_Window )
			{
				//if( pPersonalStore_Window->GetShowStatus() )
				{
					pPersonalStore_Window->ResetTradeInfo();
					pPersonalStore_Window->ShowWindow(FALSE);
				}				
			}				
		}
		break;
	default: // ����
		{
			g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_CLIENT_INTEREST;
			_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
			if( pPersonalStore_Window )
			{
				
				pPersonalStore_Window->m_ServerPersonalStoreID			= -1;		
				pPersonalStore_Window->m_TotalCountTradeItem			= 0;
				pPersonalStore_Window->m_PrevPacketSendedTime			= 0;
				pPersonalStore_Window->m_Price							= 0;
				memset(pPersonalStore_Window->m_strPrice, 0, sizeof(TCHAR)*16);
				
				for( int i = 0; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER; ++i )
				{
					pPersonalStore_Window->m_TradeItem[i].ResetTradeItem();
					pPersonalStore_Window->m_TradeItemPrice[i] = 0;
				}
				
				pPersonalStore_Window->m_LastDraggingSlot = -1;
				pPersonalStore_Window->m_SelectedSlotNumber = -1;
				
				for( i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; ++i)
				{
					pPersonalStore_Window->m_TradeItem[i].Set_m_cType( psshoplistresult->ItemList[i].cItemType );
					pPersonalStore_Window->m_TradeItem[i].Set_m_cSecond( _XGameItem::GetItemSecond(psshoplistresult->ItemList[i].cItemType, psshoplistresult->ItemList[i].sItem_Id) );
					pPersonalStore_Window->m_TradeItem[i].Set_m_sID( psshoplistresult->ItemList[i].sItem_Id );
					pPersonalStore_Window->m_TradeItem[i].Set_m_ucCount( psshoplistresult->ItemList[i].ucItemCount );
					pPersonalStore_Window->m_TradeItem[i].Set_m_usMaxDur( _XGameItem::GetMaxDurability( pPersonalStore_Window->m_TradeItem[i].Get_m_cType(), pPersonalStore_Window->m_TradeItem[i].Get_m_sID() ) );
					if( pPersonalStore_Window->m_TradeItem[i].Get_m_usMaxDur() == 0 )
					{
						pPersonalStore_Window->m_TradeItem[i].Set_m_usCurDur(0);
					}
					else
					{
						pPersonalStore_Window->m_TradeItem[i].Set_m_usCurDur( psshoplistresult->ItemList[i].usCurDur );
					}
					
					// ����� �ǻ��϶��� ���������� ����. 
#ifdef _XTS_ITEM_OPTIMIZATION
					switch( GetEqualItemType(psshoplistresult->ItemList[i].cItemType) ) 
#else
					switch(psshoplistresult->ItemList[i].cItemType) 
#endif
					{
#ifdef _XTS_ITEM_OPTIMIZATION
					case _XGI_FC_WEAPON :
#else
					case _XGI_FC_WEAPON :
					case _XGI_FC_WEAPON2 :
					case _XGI_FC_WEAPON3 :
#endif
						{
							for(int j = 0; j < 4; ++j)
							{
								pPersonalStore_Window->m_TradeItem[i].Set_m_sSocket( j, psshoplistresult->ItemList[i].sSocket[j] );
							}
							pPersonalStore_Window->m_TradeItem[i].Set_m_ucSocketCount( psshoplistresult->ItemList[i].ucSocketCount );
							pPersonalStore_Window->m_TradeItem[i].Set_m_ucStrength( psshoplistresult->ItemList[i].ucStrength );
#ifdef _XDEF_SMELT_ITEM //Author : �����
							pPersonalStore_Window->m_TradeItem[i].m_usCurProtect = psshoplistresult->ItemList[i].usCurProtect;
							pPersonalStore_Window->m_TradeItem[i].m_usCurMaxProtect = psshoplistresult->ItemList[i].usCurMaxProtect;
							pPersonalStore_Window->m_TradeItem[i].m_usUsedProtect = psshoplistresult->ItemList[i].usUsedProtect;
							//���� // ������
							pPersonalStore_Window->m_TradeItem[i].Set_m_ucWeaponStrength( _XGameItem::GetNature( pPersonalStore_Window->m_TradeItem[i].Get_m_cType(), pPersonalStore_Window->m_TradeItem[i].Get_m_sID() ) );
#endif
						}
						break;
#ifdef _XTS_ITEM_OPTIMIZATION
					case _XGI_FC_CLOTHES :
#else
					case _XGI_FC_CLOTHES :
					case _XGI_FC_CLOTHES2 :
					case _XGI_FC_CLOTHES3 :
#endif
						{
							// �ǻ��϶� �賶�̸� ���������� ���� �ʴ´�.
							if( pPersonalStore_Window->m_TradeItem[i].Get_m_cSecond() != _XGI_SC_CLOTHES_BACKPACK )
							{
								for(int j = 0; j < 4; ++j)
								{
									pPersonalStore_Window->m_TradeItem[i].Set_m_sSocket(j, psshoplistresult->ItemList[i].sSocket[j] );
								}
								pPersonalStore_Window->m_TradeItem[i].Set_m_ucSocketCount( psshoplistresult->ItemList[i].ucSocketCount );
							}
						}
						break;
					}
//					for(int j = 0; j < 4; j++)
//					{
//						pPersonalStore_Window->m_TradeItem[i].m_ucSocket[j] = psshoplistresult->ItemList[i].ucSocket[j];
//					}
//					pPersonalStore_Window->m_TradeItem[i].m_ucSocketCount = psshoplistresult->ItemList[i].ucSocketCount;

					pPersonalStore_Window->m_TradeItemPrice[i] = psshoplistresult->ItemList[i].uiPrice;

					if( pPersonalStore_Window->m_TradeItem[i].Get_m_sID() != 0 )
						pPersonalStore_Window->m_TotalCountTradeItem++;
				}				
				
				pPersonalStore_Window->m_btnOpen->EnableWindow(TRUE);
				pPersonalStore_Window->m_btnCancle->EnableWindow(TRUE);
				pPersonalStore_Window->m_btnOpen->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORKPERSONALSTORE_1687);
				pPersonalStore_Window->ShowWindow(TRUE);
			}
		}
		break;
	}
}

void _XNetwork::MSGPersonalStoreCtrlBuyResult(MSG_PS_CTRL_SHOPITEM_BUY_RESULT* psbuyresult)
{
//	_sHeader            header       ;
//	u_char              ucMessage    ;
//	char                cReturn      ;  // ����� ���  �κ��͸� ���� �Ǵ� �ε��� 
//										// ���� �� ���� �����ڵ� �����ڵ�� ���Ŀ� �ڼ��ϰ� �����ϰ���. 
//	_sUser_InvenItem    BuyItem      ;  // ������ ������ ��.
//	u_int               uiMoney      ;  // ���� ��.	Ȯ�ο�
	
#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : ����� //breif : ĳ�� �೶ �߰�
    switch(psbuyresult->cReturn) {
	case 255: // ����� �ϴ� �������� ���� �������̴�. 
	case 254: // ����� �ϴ� �����۰� ������ ������ ������ ���� �ʴ´�.
	case 253: // ����� �ϴ� �����۰� ������ ������ �ǸŰ����� ���� �ʴ´�
	case 252: // �Ĵ� ����� ���� ���Ƽ� ��� ����� ����� ���� �� ����.
	case 249: // ���� ���̵� Ʋ����. 
		{
			_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
			
			InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKPERSONALSTORE_1688), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
			_XInsertCenterSystemMessage(_XGETINTERFACETEXT(ID_STRING_NETWORKPERSONALSTORE_1688) , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
			if( pPersonalStore_Window )
			{
				pPersonalStore_Window->m_btnOpen->EnableWindow(TRUE);
				pPersonalStore_Window->m_btnCancle->EnableWindow(TRUE);
			}
		}
		break;
	case 251: // ��� ����� �κ��� �����ؼ� ������ �缭 �ű� ���� ����.
		{
			_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_NOTENOUGHINV));
			InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE ); // �೶�� ������ �����մϴ�.
			_XInsertCenterSystemMessage(messagestring , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
//			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
//			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_MESSAGEHANDLER_NOTENOUGHINVEN, TRUE);

			_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
			if( pPersonalStore_Window )
			{
				pPersonalStore_Window->m_btnOpen->EnableWindow(TRUE);
				pPersonalStore_Window->m_btnCancle->EnableWindow(TRUE);
			}
		}
		break;
	case 250: // ������ ������ �������̴� 
		{
			_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_ITEMREFRESH)); // ���������� �Ź��� ���� �� �Դϴ�.
			InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
			_XInsertCenterSystemMessage(messagestring , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

//			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);			
//			TCHAR messagestring[256];
//			memset( messagestring, 0, sizeof(TCHAR)*256 );
//			sprintf(messagestring, "���������� �Ź��� �ٽ� ���� �� �Դϴ�.");
//			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);


			_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
			if( pPersonalStore_Window )
			{
				pPersonalStore_Window->m_btnOpen->EnableWindow(TRUE);
				pPersonalStore_Window->m_btnCancle->EnableWindow(TRUE);
			}
		}
		break;
#else
	switch(psbuyresult->cReturn) {
	case -1: // ����� �ϴ� �������� ���� �������̴�. 
	case -2: // ����� �ϴ� �����۰� ������ ������ ������ ���� �ʴ´�.
	case -3: // ����� �ϴ� �����۰� ������ ������ �ǸŰ����� ���� �ʴ´�
	case -4: // �Ĵ� ����� ���� ���Ƽ� ��� ����� ����� ���� �� ����.
	case -7: // ���� ���̵� Ʋ����. 
		{
			_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
			
			InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKPERSONALSTORE_1688), _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
			_XInsertCenterSystemMessage(_XGETINTERFACETEXT(ID_STRING_NETWORKPERSONALSTORE_1688) , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
			if( pPersonalStore_Window )
			{
				pPersonalStore_Window->m_btnOpen->EnableWindow(TRUE);
				pPersonalStore_Window->m_btnCancle->EnableWindow(TRUE);
			}
		}
		break;
	case -5: // ��� ����� �κ��� �����ؼ� ������ �缭 �ű� ���� ����.
		{
			_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NETWORK_NOTENOUGHINV));
			InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE ); // �೶�� ������ �����մϴ�.
			_XInsertCenterSystemMessage(messagestring , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
//			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
//			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, ID_STRING_MESSAGEHANDLER_NOTENOUGHINVEN, TRUE);

			_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
			if( pPersonalStore_Window )
			{
				pPersonalStore_Window->m_btnOpen->EnableWindow(TRUE);
				pPersonalStore_Window->m_btnCancle->EnableWindow(TRUE);
			}
		}
		break;
	case -6: // ������ ������ �������̴� 
		{
			_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
			TCHAR messagestring[256];
			memset( messagestring, 0, sizeof(messagestring) );
			_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_ITEMREFRESH)); // ���������� �Ź��� ���� �� �Դϴ�.
			InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
			_XInsertCenterSystemMessage(messagestring , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

//			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);			
//			TCHAR messagestring[256];
//			memset( messagestring, 0, sizeof(TCHAR)*256 );
//			sprintf(messagestring, "���������� �Ź��� �ٽ� ���� �� �Դϴ�.");
//			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  _T(""), messagestring, TRUE);


			_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
			if( pPersonalStore_Window )
			{
				pPersonalStore_Window->m_btnOpen->EnableWindow(TRUE);
				pPersonalStore_Window->m_btnCancle->EnableWindow(TRUE);
			}
		}
		break;
#endif
	default: // 
		{
			DWORD price = g_pLocalUser->m_Money - psbuyresult->uiMoney;
						
			_XLog("PERSONAL STORE BUY RESULT : price[%u]", price );
			
			if( psbuyresult->BuyItem.Item_Money.S_NormalItem.ucCount != 0 )
			{
				g_pLocalUser->m_UserItemList[psbuyresult->cReturn].Set_m_cType( psbuyresult->BuyItem.cType );
				g_pLocalUser->m_UserItemList[psbuyresult->cReturn].Set_m_cSecond(psbuyresult->BuyItem.cSecond);
				g_pLocalUser->m_UserItemList[psbuyresult->cReturn].Set_m_sID(psbuyresult->BuyItem.sID);
				g_pLocalUser->m_UserItemList[psbuyresult->cReturn].m_cIdentify = psbuyresult->BuyItem.Item_Money.S_NormalItem.cIdentify;
				
				g_pLocalUser->m_UserItemList[psbuyresult->cReturn].Set_m_cThird(_XGameItem::GetThird(psbuyresult->BuyItem.cType, psbuyresult->BuyItem.sID));
				g_pLocalUser->m_UserItemList[psbuyresult->cReturn].Set_m_ucCount(psbuyresult->BuyItem.Item_Money.S_NormalItem.ucCount);
				
				g_pLocalUser->m_UserItemList[psbuyresult->cReturn].Set_m_usCurDur(psbuyresult->BuyItem.Item_Money.S_NormalItem.usCurDur);
				g_pLocalUser->m_UserItemList[psbuyresult->cReturn].Set_m_usMaxDur(_XGameItem::GetMaxDurability(psbuyresult->BuyItem.cType, psbuyresult->BuyItem.sID));
			
				g_pLocalUser->m_UserItemList[psbuyresult->cReturn].Set_m_ucSocketCount(psbuyresult->BuyItem.Item_Money.S_NormalItem.ucSocketCount);
				if( g_pLocalUser->m_UserItemList[psbuyresult->cReturn].Get_m_ucSocketCount() > 0 )
				{
					for( int i = 0; i < 4; ++i )
						g_pLocalUser->m_UserItemList[psbuyresult->cReturn].Set_m_sSocket( i, psbuyresult->BuyItem.sSocket[i] );
				}	
				g_pLocalUser->m_UserItemList[psbuyresult->cReturn].Set_m_ucStrength( psbuyresult->BuyItem.ucStrength );
#ifdef _XDEF_SMELT_ITEM //Author : �����
				g_pLocalUser->m_UserItemList[psbuyresult->cReturn].m_usCurProtect = psbuyresult->BuyItem.uselimit.usCurProtect;
				g_pLocalUser->m_UserItemList[psbuyresult->cReturn].m_usCurMaxProtect = psbuyresult->BuyItem.uselimit.usCurMaxProtect;
				g_pLocalUser->m_UserItemList[psbuyresult->cReturn].m_usUsedProtect = psbuyresult->BuyItem.uselimit.usUsedProtect;
				//g_pLocalUser->m_UserItemList[psbuyresult->cReturn].m_ucIncProtect = psbuyresult->BuyItem.ucIncProtect;
				
				g_pLocalUser->m_UserItemList[psbuyresult->cReturn].Set_m_ucWeaponStrength( _XGameItem::GetNature( psbuyresult->BuyItem.cType, psbuyresult->BuyItem.sID) );
#endif
				if( psbuyresult->BuyItem.ucIncludeLimit == 1 )
				{
					g_pLocalUser->m_UserItemList[psbuyresult->cReturn].m_ucFlag1	= psbuyresult->BuyItem.uselimit.ucFlag1;

#ifndef _XDEF_SMELT_ITEM //Author : ����� //breif : �����εǾ������� ������ �ʴ´�
					g_pLocalUser->m_UserItemList[psbuyresult->cReturn].m_ucFlag2	= psbuyresult->BuyItem.uselimit.ucFlag2;	
					g_pLocalUser->m_UserItemList[psbuyresult->cReturn].m_ucFlag3	= psbuyresult->BuyItem.uselimit.ucFlag3;
#endif
					g_pLocalUser->m_UserItemList[psbuyresult->cReturn].m_usValue1	= psbuyresult->BuyItem.uselimit.usValue1;

					if( psbuyresult->BuyItem.uselimit.ucYear > 0 )
					{
						g_pLocalUser->m_UserItemList[psbuyresult->cReturn].Set_m_ucYear(psbuyresult->BuyItem.uselimit.ucYear);	
						g_pLocalUser->m_UserItemList[psbuyresult->cReturn].Set_m_ucMonth(psbuyresult->BuyItem.uselimit.ucMonth);	
						g_pLocalUser->m_UserItemList[psbuyresult->cReturn].Set_m_ucDay(psbuyresult->BuyItem.uselimit.ucDay);	
						g_pLocalUser->m_UserItemList[psbuyresult->cReturn].Set_m_ucHour(psbuyresult->BuyItem.uselimit.ucHour);	
						g_pLocalUser->m_UserItemList[psbuyresult->cReturn].Set_m_ucMinute(psbuyresult->BuyItem.uselimit.ucMinute);
					}				
				}
			}
			
			g_pLocalUser->m_Money = psbuyresult->uiMoney;
			
			_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
			if( pPersonalStore_Window )
			{
				TCHAR		namestring[256];
				memset(namestring, 0, sizeof(TCHAR)*256);
				strcpy(namestring, _XGameItem::GetItemName(psbuyresult->BuyItem.cType, psbuyresult->BuyItem.sID));
				
				TCHAR messagestring[512];
				memset(messagestring, 0, sizeof(TCHAR)*512);
				
				strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NETWORKPERSONALSTORE_1689, 
																		 pPersonalStore_Window->m_OwnerName, namestring, &price ) );

				InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
				
				pPersonalStore_Window->m_btnOpen->EnableWindow(TRUE);
				pPersonalStore_Window->m_btnCancle->EnableWindow(TRUE);
						
				pPersonalStore_Window->m_TradeItem[pPersonalStore_Window->m_SelectedSlotNumber].Set_m_cType(-1);
				pPersonalStore_Window->m_TradeItem[pPersonalStore_Window->m_SelectedSlotNumber].Set_m_sID(0);
				pPersonalStore_Window->m_TradeItemPrice[pPersonalStore_Window->m_SelectedSlotNumber] = 0;			
				pPersonalStore_Window->m_SelectedSlotNumber = -1;	

				pPersonalStore_Window->m_TotalCountTradeItem = 0;
				for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; ++i)
				{
					if( pPersonalStore_Window->m_TradeItem[i].Get_m_sID() != 0 )
					{
						pPersonalStore_Window->m_TotalCountTradeItem++;
					}
				}
			}
		}
		break;
	}
}

void _XNetwork::MSGPersonalStoreCtrlStopBroadCasting(MSG_PS_CTRL_STOP_BROAD_CAST* psstopbroadcasting)
{
//	_sHeader            header    ;
//	u_char              ucMessage ;
//	u_short             usUID     ;  // ���� ������ UID
//	char                cIsComplete; // 0 : ������ ������  
//									 // 1 : ������ �ݴ� ����
									 // 2 : ������ ���¿��� �ٽ� �� ���� 
	
	if(g_pLocalUser->m_Uniqueid == psstopbroadcasting->usUID) // ���� ������ �޾��� �� 
	{
		switch(psstopbroadcasting->cIsComplete) {
		case 0:// 0 : ������ ������
			{
				if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_OWNER_MODIFY ) 
					g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_OWNER_MODIFY;	
			}
			break;
		case 1:// 1 : ������ �ݴ� ���� 
			{
				if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE ) 
					g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;	

				_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
				if( pPersonalStore_Window )
				{
					//if( pPersonalStore_Window->GetShowStatus() )
					{
						_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
						pPersonalStore_Window->ResetTradeInfo();
						pPersonalStore_Window->ShowWindow(FALSE);
						
					}
					
				}				
			}
			break;
		case 2:// 2 : ������ ���¿��� �ٽ� �� ����
			{
				if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_OWNER_OPEN ) 
					g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_OWNER_OPEN;	
			}
			break;
		}		
	}
	else // ���� �޾��� �� 
	{
		switch(psstopbroadcasting->cIsComplete) {
		case 0:// 0 : ������ ������
			{
				_XUser* pUser = g_Userlist.FindUser(psstopbroadcasting->usUID);
				if(pUser)
				{
					if( pUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_OWNER_MODIFY ) 
					{
						pUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_OWNER_MODIFY;
						
						_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
						if( pPersonalStore_Window )
						{
							//if( pPersonalStore_Window->GetShowStatus() )
							{
								if( pPersonalStore_Window->m_OwnerUniqueID == psstopbroadcasting->usUID )
								{
									g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_CLIENT_MODIFY;
									pPersonalStore_Window->m_btnOpen->EnableWindow(FALSE);
								}
							}
						}
					}					
				}
			}
			break;
		case 1:// 1 : ������ �ݴ� ���� 
			{
				_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
				if( pPersonalStore_Window )
				{
					//if( pPersonalStore_Window->GetShowStatus() )
					{
						if( pPersonalStore_Window->m_OwnerUniqueID == psstopbroadcasting->usUID )
						{
							_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
							pPersonalStore_Window->ResetTradeInfo();
							pPersonalStore_Window->ShowWindow(FALSE);
						}
					}
					
				}
				
				_XUser* pUser = g_Userlist.FindUser(psstopbroadcasting->usUID);
				if(pUser)
				{
					if( pUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE ) 
					{
						pUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_NONE;					
					}				
				}
			}
			break;
		case 2:// 2 : ������ ���¿��� �ٽ� �� ����. 
			{
				_XUser* pUser = g_Userlist.FindUser(psstopbroadcasting->usUID);
				if(pUser)
				{
					pUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_OWNER_OPEN;
					
					_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
					if( pPersonalStore_Window )
					{
						//if( pPersonalStore_Window->GetShowStatus() )
						{
							if( pPersonalStore_Window->m_OwnerUniqueID == psstopbroadcasting->usUID )
							{
								g_pLocalUser->m_PersonalStoreState = _XUSER_PERSONALSTORE_CLIENT_INTEREST;
								pPersonalStore_Window->m_btnOpen->EnableWindow(TRUE);
							}
						}
					}
				}
			}
			break;
		}
		
	}
	
}

void _XNetwork::MSGPersonalStoreCtrlTradeResultBroadCasting(MSG_PS_CTRL_SHOPITEM_SOLD* pstraderesultbroadcasting)
{
//	_sHeader            header       ;
//	u_char              ucMessage    ;
//	char                cSoldSlot    ; 
//	u_short             OwnerUID     ; // ���� ���� UID
//	u_short             UID          ; // �� ���  UID 
//	char                cType        ; // �ȸ� ������ Ÿ��. Ȯ�ο�
//	short               sID          ; // �ȸ� ������ ���̵�.  Ȯ�ο�

	if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_CLIENT_INTEREST )
	{
		_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
		if( pPersonalStore_Window )
		{
			//if( pPersonalStore_Window->GetShowStatus() )
			{
				if( pPersonalStore_Window->m_OwnerUniqueID == pstraderesultbroadcasting->OwnerUID ) //���� ID�� Ȯ���ؾ��Ѵ�. 
				{
					pPersonalStore_Window->m_TradeItem[ pstraderesultbroadcasting->cSoldSlot ].ResetTradeItem();
					pPersonalStore_Window->m_TotalCountTradeItem = 0;
					for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; ++i)
					{
						if( pPersonalStore_Window->m_TradeItem[i].Get_m_sID() != 0 )
						{
							pPersonalStore_Window->m_TotalCountTradeItem++;
						}
					}
				}
			}
		}		
	}
	else if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN )
	{
		if( g_pLocalUser->m_Uniqueid == pstraderesultbroadcasting->OwnerUID )
		{
			_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
			if( pPersonalStore_Window )
			{
				//if( pPersonalStore_Window->GetShowStatus() )
				{			
					int type = pPersonalStore_Window->m_TradeItem[ pstraderesultbroadcasting->cSoldSlot ].Get_m_cType();
					int id = pPersonalStore_Window->m_TradeItem[ pstraderesultbroadcasting->cSoldSlot ].Get_m_sID();
					TCHAR		namestring[256];
					memset(namestring, 0, sizeof(TCHAR)*256);
					strcpy(namestring, _XGameItem::GetItemName(type, id));
					
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(TCHAR)*256);

					int param2 = pPersonalStore_Window->m_TradeItemPrice[pstraderesultbroadcasting->cSoldSlot];
					strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NETWORKPERSONALSTORE_1690, namestring, &param2 ) );
					
					InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
					_XInsertCenterSystemMessage(messagestring , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
					
					pPersonalStore_Window->m_TradeItem[ pstraderesultbroadcasting->cSoldSlot ].ResetTradeItem();
					pPersonalStore_Window->m_TotalCountTradeItem = 0;
					for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; ++i)
					{
						if( pPersonalStore_Window->m_TradeItem[i].Get_m_sID() != 0 )
						{
							pPersonalStore_Window->m_TotalCountTradeItem++;
						}
					}
				}
			}		
		}
	}

}

void _XNetwork::MSGPersonalStoreCtrlTradeResultToOwner(MSG_PS_CTRL_ITEM_BUY_TO_OWNER* pstraderesulttoowner)
{
//	_sHeader            header       ;
//	u_char              ucMessage    ;
//	char                cReturn      ; // ����� ������ 1; 
//	char                cIndex       ; // �κ��͸� �������� �ȸ� ������ �ε���.
//	_sUser_InvenItem    Item         ; // �ȸ��� �� ���� �����. ����ִٸ� ���� �ε����� �ش��ϴ� �������� ���� ���̴�.
//	u_int               uiMoney      ; // �ȸ��� �� ���� ���ݾ׼�. 
	
	if( pstraderesulttoowner->cReturn == 1 )
	{
		if( g_pLocalUser->m_PersonalStoreState == _XUSER_PERSONALSTORE_OWNER_OPEN )
		{			
			_XWindow_PersonalStore* pPersonalStore_Window = (_XWindow_PersonalStore*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_PERSONALSTORE);
			if( pPersonalStore_Window )
			{
				//if( pPersonalStore_Window->GetShowStatus() )
				{
					int type = pPersonalStore_Window->m_TradeItem[ pstraderesulttoowner->cPSWIndex ].Get_m_cType();
					int id = pPersonalStore_Window->m_TradeItem[ pstraderesulttoowner->cPSWIndex ].Get_m_sID();
					TCHAR		namestring[256];
					memset(namestring, 0, sizeof(TCHAR)*256);
					strcpy(namestring, _XGameItem::GetItemName(type, id));
					
					TCHAR messagestring[256];
					memset(messagestring, 0, sizeof(TCHAR)*256);

					int param2 = pPersonalStore_Window->m_TradeItemPrice[pstraderesulttoowner->cPSWIndex];
					strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NETWORKPERSONALSTORE_1690, namestring, &param2 ) );
					
					InsertChatString( messagestring, _XDEF_CHATMESSAGECOLOR_SYSTEMGET );
					
					pPersonalStore_Window->m_TradeItem[ pstraderesulttoowner->cPSWIndex ].ResetTradeItem();	
					pPersonalStore_Window->m_TotalCountTradeItem = 0;
					for(int i = 0 ; i < _XDEF_PERSONALSTORE_TRADEITEM_MAX_SLOTNUMBER ; ++i)
					{
						if( pPersonalStore_Window->m_TradeItem[i].Get_m_sID() != 0 )
						{
							pPersonalStore_Window->m_TotalCountTradeItem++;
						}
					}
					
					if( pstraderesulttoowner->Item.Item_Money.S_NormalItem.ucCount == 0 )
					{
						g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Reset();
						g_pLocalUser->m_Money = pstraderesulttoowner->uiMoney;	
					}
					else
					{
						if( g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Get_m_ucCount() != 0)
						{
							int buynumber = pstraderesulttoowner->Item.Item_Money.S_NormalItem.ucCount - g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Get_m_ucCount();
							
							g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Set_m_cType(pstraderesulttoowner->Item.cType);
							g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Set_m_cSecond( pstraderesulttoowner->Item.cSecond );
							g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Set_m_sID(pstraderesulttoowner->Item.sID);
							g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].m_cIdentify = pstraderesulttoowner->Item.Item_Money.S_NormalItem.cIdentify;
							
							g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Set_m_cThird(_XGameItem::GetThird(pstraderesulttoowner->Item.cType, pstraderesulttoowner->Item.sID));						
							g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Set_m_usCurDur(pstraderesulttoowner->Item.Item_Money.S_NormalItem.usCurDur);
							g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Set_m_usMaxDur(_XGameItem::GetMaxDurability(pstraderesulttoowner->Item.cType, pstraderesulttoowner->Item.sID));
							
							g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Set_m_ucSocketCount( pstraderesulttoowner->Item.Item_Money.S_NormalItem.ucSocketCount );
							if( pstraderesulttoowner->Item.Item_Money.S_NormalItem.ucSocketCount > 0 )
							{
								for( int i = 0; i < 4; ++i )
									g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Set_m_sSocket( i, pstraderesulttoowner->Item.sSocket[i] );
							}
							g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Set_m_ucStrength( pstraderesulttoowner->Item.ucStrength );  
#ifdef _XDEF_SMELT_ITEM //Author : �����
							g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].m_usCurProtect = pstraderesulttoowner->Item.uselimit.usCurProtect;
							g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].m_usCurMaxProtect = pstraderesulttoowner->Item.uselimit.usCurMaxProtect;
							g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].m_usUsedProtect = pstraderesulttoowner->Item.uselimit.usUsedProtect;

							g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Set_m_ucWeaponStrength( _XGameItem::GetNature( pstraderesulttoowner->Item.cType, pstraderesulttoowner->Item.sID) );
#endif
							g_pLocalUser->m_Money = pstraderesulttoowner->uiMoney;
							
							g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].m_ucFlag1 = pstraderesulttoowner->Item.uselimit.ucFlag1;
#ifndef _XDEF_SMELT_ITEM //author : ����� //breif : ������ �Ǿ������� ������ �ʴ´�
							g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].m_ucFlag2	= pstraderesulttoowner->Item.uselimit.ucFlag2;	
							g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].m_ucFlag3	= pstraderesulttoowner->Item.uselimit.ucFlag3;
#endif

							g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].m_usValue1	= pstraderesulttoowner->Item.uselimit.usValue1;

							if( pstraderesulttoowner->Item.uselimit.ucYear > 0 )
							{								
								g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Set_m_ucYear(pstraderesulttoowner->Item.uselimit.ucYear);	
								g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Set_m_ucMonth(pstraderesulttoowner->Item.uselimit.ucMonth);	
								g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Set_m_ucDay(pstraderesulttoowner->Item.uselimit.ucDay);	
								g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Set_m_ucHour(pstraderesulttoowner->Item.uselimit.ucHour);	
								g_pLocalUser->m_UserItemList[pstraderesulttoowner->cIndex].Set_m_ucMinute(pstraderesulttoowner->Item.uselimit.ucMinute);
							}
						}
						else
						{
							g_pLocalUser->m_Money = pstraderesulttoowner->uiMoney;
						}
					}
				}
			}			
		}
	}
}



#ifdef _XDEF_COMMONITEMMALL_20070402	//���� ������ �� ���� �Լ�
void _XNetwork::MSGCISInvenAck(MSG_COMMON* pCommon)//ĳ�� �ڽ��� ������ ������ �˷��ش�.
{
	//	USHORT  usTotalCnt;     // ĳ���κ��� (��� �������� ������) �� ������ ����; �� �������� �������� ����ϱ� ����
	//	UCHAR	ucRowCount;		// �������� ������ �� ������ ����
	//	// ucRowCount ��ŭ �ݺ�
	//	int		iPkgGuid;	// �������� Guid
	//	int		iPkgIdx;	// ��Ű�� �ε���
	_XWindow_CashBox* pCashBox_Window = (_XWindow_CashBox*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHBOX );	
	if( pCashBox_Window )
	{
		
		if( !pCashBox_Window->m_listCashItem.empty() )
		{
			_XCASHITEMBOX_INFO* cashitem = NULL;
			list <_XCASHITEMBOX_INFO*>::iterator iter_cashitem;
			for(iter_cashitem = pCashBox_Window->m_listCashItem.begin() ; iter_cashitem != pCashBox_Window->m_listCashItem.end() ; )
			{
				cashitem = *iter_cashitem;
				if( cashitem )
				{
					SAFE_DELETE(cashitem);
					iter_cashitem = pCashBox_Window->m_listCashItem.erase(iter_cashitem);
				}
				else
					iter_cashitem++; //Author : ����� //breif : �߰�
			}
		}

		pCashBox_Window->m_listCashItem.clear(); // Author : ����� //breif : clear �߰�

		USHORT Totalcount = 0;
		UCHAR	Rowcount = 0;
		int PackageItemID = 0;
		int PackageIndex = 0;

		int offset = sizeof(_sHeader);
		offset += sizeof(u_char);
		offset += sizeof(WORD);
		
		
		memcpy(&Totalcount, (char*)pCommon + offset, sizeof(USHORT)); offset += sizeof(USHORT);
		memcpy(&Rowcount, (char*)pCommon + offset, sizeof(UCHAR)); offset += sizeof(UCHAR);

		for(int i =0; i < Rowcount ; ++i) 
		{
			memcpy(&PackageItemID, (char*)pCommon + offset, sizeof(int));  offset += sizeof(int);
			memcpy(&PackageIndex, (char*)pCommon + offset, sizeof(int));  offset += sizeof(int);
			
			_XCASHITEMBOX_INFO* pCashPackageItem = new _XCASHITEMBOX_INFO;
			pCashPackageItem->indexPackage = PackageIndex;//��Ű�� �ε���
			pCashPackageItem->OrderNumber = PackageItemID;//�ֹ���ȣ
			pCashPackageItem->bCheck = FALSE;
			
			pCashBox_Window->m_listCashItem.push_back(pCashPackageItem);
		}

		pCashBox_Window->m_nCashItemTotalCount = Totalcount;
		pCashBox_Window->m_Maxium5PageCount = ((Totalcount/9) + 1)/5;

		pCashBox_Window->SetListItem();
		pCashBox_Window->SetPageNumberButton();
		
		//pCashBox_Window->m_btnMoveAllItem->EnableWindow(TRUE);
		pCashBox_Window->m_btnMoveSelectedItem->EnableWindow(TRUE);
		pCashBox_Window->m_btnRenew->EnableWindow(TRUE);
		pCashBox_Window->m_TabButton[0]->EnableWindow(TRUE);
		pCashBox_Window->m_TabButton[1]->EnableWindow(TRUE);
	}
	
	//(char*)pCommon->_CIS_CASH_INVEN_ACK.iPkgGuid
	
}

void _XNetwork::MSGCISMoveItemAck(MSG_COMMON* pCommon)//ĳ�� �ڽ����� �೶���� �������� �̵��Ѵ�.
{
//	UCHAR	ucPkgCnt;	// �ű�⿡ ������ ��Ű�� ������ ����
//	// ucPkgCnt ��ŭ �ݺ�
//	int		iPkgGuid;	// �������� Guid
//	
//	UCHAR	ucItemCnt;	// �೶���� �Ű��� ������ ����
//	// ucItemCnt ��ŭ �ݺ�
//	char	cSlot;		// �೶�� ���� �ε���
//	char	cType;		// ������ �з�
//	short	sId;
//	UCHAR	ucItemNum;	// �������� ������ ����
	_XWindow_CashBox* pCashBox_Window = (_XWindow_CashBox*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHBOX );	
	if( pCashBox_Window )
	{						
		UCHAR	ucPkgCnt = 0;
		int		iPkgGuid = 0;
		UCHAR	ucItemCnt = 0;
		UCHAR	ucItemNum = 0;
		_sUser_Inven_v1_all Item;

		int offset = sizeof(_sHeader);
		offset += sizeof(u_char);
		offset += sizeof(WORD);

		// ĳ�� �����Կ��� ���� ������
		memcpy(&ucPkgCnt, (char*)pCommon + offset , sizeof(UCHAR)); offset += sizeof(UCHAR);
		if( ucPkgCnt == 0 )
		{							
			// ���� ó��
			InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1698),_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
		}
		else
		{
			for(int i =0; i < ucPkgCnt; ++i) 
			{
				memcpy(&iPkgGuid, (char*)pCommon + offset, sizeof(int));  offset += sizeof(int);
				
				if( !pCashBox_Window->m_listCashItem.empty() )
				{
					_XCASHITEMBOX_INFO* cashitem = NULL;
					list <_XCASHITEMBOX_INFO*>::iterator iter_cashitem;
					for(iter_cashitem = pCashBox_Window->m_listCashItem.begin() ; iter_cashitem != pCashBox_Window->m_listCashItem.end() ; )
					{
						cashitem = *iter_cashitem;
						if( cashitem )
						{
							if( cashitem->OrderNumber == iPkgGuid )
							{
								SAFE_DELETE(cashitem);
								iter_cashitem = pCashBox_Window->m_listCashItem.erase(iter_cashitem);
								break;
							}
							else
							{
								++iter_cashitem;
							}
						}
					}
				}				
			}// end for( int j =0; j < ucItemCnt; ++j)  
				
			// �೶�� �� ������
			memcpy(&ucItemCnt, (char*)pCommon + offset , sizeof(UCHAR)); offset += sizeof(UCHAR);
			for( int j =0; j < ucItemCnt; ++j) 
			{					
				memcpy(&Item , (char*)pCommon + offset, sizeof(_sUser_Inven_v1_all));  offset += sizeof(_sUser_Inven_v1_all);					
				if( Item.sID > 0 )
				{				
					int slotnumber = Item.cSlot;
					
					g_pLocalUser->m_UserItemList[slotnumber].Set_m_cType(Item.cType);
					g_pLocalUser->m_UserItemList[slotnumber].Set_m_cSecond( Item.cSecond );
					g_pLocalUser->m_UserItemList[slotnumber].Set_m_sID( Item.sID );
					
					g_pLocalUser->m_UserItemList[slotnumber].Set_m_cThird(_XGameItem::GetThird(Item.cType, Item.sID));  
					g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucCount(Item.Item_Money.S_NormalItem.ucCount );
					
					g_pLocalUser->m_UserItemList[slotnumber].Set_m_usCurDur(Item.Item_Money.S_NormalItem.usCurDur);
					g_pLocalUser->m_UserItemList[slotnumber].Set_m_usMaxDur(_XGameItem::GetMaxDurability(Item.cType, Item.sID));

					g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucSocketCount( Item.Item_Money.S_NormalItem.ucSocketCount );
					if( Item.Item_Money.S_NormalItem.ucSocketCount > 0 )
					{
						for( int i = 0; i < 4; ++i )
							g_pLocalUser->m_UserItemList[slotnumber].Set_m_sSocket( i, Item.sSocket[i] );
					}									
					g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucStrength( Item.ucStrength );
#ifdef _XDEF_SMELT_ITEM //Author : �����
					g_pLocalUser->m_UserItemList[slotnumber].m_usCurProtect = Item.uselimit.usCurProtect;
					g_pLocalUser->m_UserItemList[slotnumber].m_usCurMaxProtect = Item.uselimit.usCurMaxProtect;
					g_pLocalUser->m_UserItemList[slotnumber].m_usUsedProtect = Item.uselimit.usUsedProtect;
					
					g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucWeaponStrength( _XGameItem::GetNature( Item.cType, Item.sID) );
					g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucSharpness( Item.ucSharpness );
#else
					g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucSharpness(Item.ucSharpness );
#endif
					
					g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag1 = Item.uselimit.ucFlag1;

#ifndef _XDEF_SMELT_ITEM //Author : ����� //breif : ������ �Ǿ������� ������ �ʴ´�
					g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag2		= Item.uselimit.ucFlag2;	
					g_pLocalUser->m_UserItemList[slotnumber].m_ucFlag3		= Item.uselimit.ucFlag3;
#endif
					g_pLocalUser->m_UserItemList[slotnumber].m_usValue1	= Item.uselimit.usValue1;

					if( Item.uselimit.ucYear > 0 )
					{						
						g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucYear(Item.uselimit.ucYear);	
						g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucMonth(Item.uselimit.ucMonth);	
						g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucDay(Item.uselimit.ucDay);	
						g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucHour(Item.uselimit.ucHour);	
						g_pLocalUser->m_UserItemList[slotnumber].Set_m_ucMinute(Item.uselimit.ucMinute);
					}
				}
				else
				{
#ifdef _XDWDEBUG
					_XLog( "Idx : %d : Type %d, Second %d, ID %d, Third %d, Count %d, Dur %d, MaxDur %d", i, Item.cType, Item.cSecond, Item.sID,
						_XGameItem::GetThird(Item.cType, Item.sID), Item.Item_Money.S_NormalItem.ucCount,
						Item.Item_Money.S_NormalItem.usCurDur, _XGameItem::GetMaxDurability(Item.cType, Item.sID) );
#endif				
				}
			}	
			// �೶�� �� ���� �� 
		
			//Author : ����� //breif : �븸 ����, ����Ʈ�������� �������� �����۸���� ���û�Ѵ� Date : 08/03/07
			//��Ŷ ������ 	
			if( pCashBox_Window->m_listCashItem.empty() ) // ����Ʈ�� ����ִ�.
				g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,pCashBox_Window->m_CurrentPageNumber,9); // ���� ������  m_CurrentPageNumber�� 0 ����
			else
				g_NetworkKernel.SendPacket(MSG_NO_COMMOM_REQ,10,pCashBox_Window->m_CurrentPageNumber + 1,9); //���� ������
			pCashBox_Window->m_bShowPresentMessage = FALSE;
			pCashBox_Window->m_btnMsgClose->ShowWindow(FALSE);
			//pCashBox_Window->SetListItem();
		}
		
		
//		pCashBox_Window->m_btnMoveAllItem->EnableWindow(TRUE);
		pCashBox_Window->m_btnMoveSelectedItem->EnableWindow(TRUE);
		pCashBox_Window->m_btnRenew->EnableWindow(TRUE);
		pCashBox_Window->m_TabButton[0]->EnableWindow(TRUE);
		pCashBox_Window->m_TabButton[1]->EnableWindow(TRUE);
	}		
}

void _XNetwork::MSGCISPresentMsg(MSG_COMMON* pCommon)//���� �޼����� ǥ���Ѵ�.
{
//	char		acc[en_max_lil+1];	// ������ ������ Account
//	SYSTEMTIME	time;				// ���� ���� ��¥(��/��/��/��/��/��); DBTIMESTAMP
//	char		msg[100+1];			// ���� �޽���
	
	int offset = sizeof(_sHeader);
	offset += sizeof(u_char);
	offset += sizeof(WORD);

	_XWindow_CashBox* pCashBox_Window = (_XWindow_CashBox*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CASHBOX );	
	if( pCashBox_Window )
	{						
		if( !pCashBox_Window->m_listCashItem.empty() )
		{
			_XCASHITEMBOX_INFO* cashitem = NULL;
			list <_XCASHITEMBOX_INFO*>::iterator iter_cashitem;
			for(iter_cashitem = pCashBox_Window->m_listCashItem.begin() ; iter_cashitem != pCashBox_Window->m_listCashItem.end() ; ++iter_cashitem)
			{
				cashitem = *iter_cashitem;
				if( cashitem )
				{
					if( cashitem->OrderNumber == pCashBox_Window->m_SendOrderNumberPresentMessage )
					{
						memcpy(&cashitem->PresentName, (char*)pCommon + offset , sizeof(char)*(en_max_lil+1) ); offset += sizeof(char)*(en_max_lil+1);
						memcpy(&cashitem->PresentTime, (char*)pCommon + offset , sizeof(SYSTEMTIME) ); offset += sizeof(SYSTEMTIME);
						memcpy(&cashitem->PresentMsg, (char*)pCommon + offset , sizeof(char)*(100+1) ); offset += sizeof(char)*(100+1);

						break;
					}
				}
			}
		}
		pCashBox_Window->SetPresentMessage(pCashBox_Window->m_SendOrderNumberPresentMessage);
		
	}
}

#endif		