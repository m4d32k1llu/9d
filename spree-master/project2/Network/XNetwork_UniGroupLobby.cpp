#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XWindow_ServerUnification.h"
#include "XWindow_SelectCharacter.h"
#include "XSR_STRINGHEADER.H"

void _XNetwork::MSGServerUnification(MSG_UNI_GROUP_LOBBY* serveruniinfo)
{
#ifdef _XTS_NEWCHARACLIST // _XTS_SERVERUNIFICATION �� ������ �������� ĳ���� �����ɸ� �����.
	switch(serveruniinfo->ucMode) 
	{
		case en_ug_change_name_result :
		{
			_XWindowManager* TempWindowMgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_serverunification.GetWindowManagerPtr();
			if( !TempWindowMgr )
				return;			

			MSG_UG_CHANGE_NAME_RESULT* changenameresult = (MSG_UG_CHANGE_NAME_RESULT*)serveruniinfo;
			if( changenameresult )
			{
				if( changenameresult->ucSuccess == 0 )	// ����
				{
					_XWindowManager* selectcharacterWindowMgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.GetWindowManagerPtr();
					if( selectcharacterWindowMgr )
					{
						_XWindow_SelectCharacter* pSelectCharacter_Window = (_XWindow_SelectCharacter *)selectcharacterWindowMgr->FindWindow(_XDEF_WTITLE_SELECTCHARACTER);
						if( pSelectCharacter_Window )
						{
							strcpy( pSelectCharacter_Window->m_Name, changenameresult->ucNewName );
							switch(pSelectCharacter_Window->m_SelectedCharacter) 
							{
							case 0:
								pSelectCharacter_Window->m_Blank01Button.SetButtonText(pSelectCharacter_Window->m_Name, _XFONT_ALIGNTYPE_LEFT, 37, 0 );
								pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFFCF904 );
								break;
							case 1:
								pSelectCharacter_Window->m_Blank02Button.SetButtonText(pSelectCharacter_Window->m_Name, _XFONT_ALIGNTYPE_LEFT, 37, 0 );
								pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFFCF904 );
								break;
							case 2:
								pSelectCharacter_Window->m_Blank03Button.SetButtonText(pSelectCharacter_Window->m_Name, _XFONT_ALIGNTYPE_LEFT, 37, 0 );
								pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFFCF904 );
								break;
							}								
						}
					}				
					strcpy( g_pLocalUser->m_AllCharacterInfo[changenameresult->ucIndex].charactername, changenameresult->ucNewName);
					strcpy( g_pLocalUser->m_CharacterInfo.charactername, changenameresult->ucNewName );
					g_pLocalUser->m_AllCharacterInfo[changenameresult->ucIndex].ucChangeNameFlag = 0;
				}				
				else	// ����
				{
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_EXISTEDCHARACTER) );
				}
			}
		}
		break;
	}
#else

#ifdef _XTS_SERVERUNIFICATION
	switch(serveruniinfo->ucMode) 
	{
	case en_ug_simple_grp_info :
		{
			_XWindowManager* TempWindowMgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_serverunification.GetWindowManagerPtr();
			if( !TempWindowMgr )
				return;

			_XWindow_ServerUnification* pServerUnification_Window = (_XWindow_ServerUnification*)TempWindowMgr->FindWindow(_XDEF_WTITLE_SERVERUNIFICATIONWINDOW);
			if( !pServerUnification_Window )
				return;

			MSG_UG_SIMPLE_GROUP_INFO* groupinfo = (MSG_UG_SIMPLE_GROUP_INFO*)serveruniinfo;
			if( groupinfo )
				pServerUnification_Window->SetSimpleGroupInfo(groupinfo);
		}
		break;
	case en_ug_select_simple_grp_fail :
		break;
	case en_ug_detail_grp_info :
		{
			_XWindowManager* TempWindowMgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_serverunification.GetWindowManagerPtr();
			if( !TempWindowMgr )
				return;

			_XWindow_ServerUnification* pServerUnification_Window = (_XWindow_ServerUnification*)TempWindowMgr->FindWindow(_XDEF_WTITLE_SERVERUNIFICATIONWINDOW);
			if( !pServerUnification_Window )
				return;

			MSG_UG_DETAIL_GRP_INFO* detailinfo = (MSG_UG_DETAIL_GRP_INFO*)serveruniinfo;
			if( detailinfo )
				pServerUnification_Window->SetDetailGroupInfo(detailinfo);
		}
		break;
	case en_ug_move_result_complete :
		{
			_XWindowManager* TempWindowMgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_serverunification.GetWindowManagerPtr();
			if( !TempWindowMgr )
				return;

			_XWindow_ServerUnification* pServerUnification_Window = (_XWindow_ServerUnification*)TempWindowMgr->FindWindow(_XDEF_WTITLE_SERVERUNIFICATIONWINDOW);
			if( !pServerUnification_Window )
				return;

			MSG_UG_MOVE_RESULT_COMPLETE* moveresult = (MSG_UG_MOVE_RESULT_COMPLETE*)serveruniinfo;
			if( moveresult )
				pServerUnification_Window->SetMoveResult(moveresult);
		}
		break;
	case en_ug_move_result_change_name :
		break;
	case en_ug_move_result_fail :
		{
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("���ռ����� ������ �Ұ����մϴ�. �ֹι�ȣ ���ڸ��� Ȯ�� �� �ٽ� �õ� �Ͻñ� �ٶ��ϴ�.") );
		}
		break;
	case en_ug_change_name_result :
		{
			_XWindowManager* TempWindowMgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_serverunification.GetWindowManagerPtr();
			if( !TempWindowMgr )
				return;			

			MSG_UG_CHANGE_NAME_RESULT* changenameresult = (MSG_UG_CHANGE_NAME_RESULT*)serveruniinfo;
			if( changenameresult )
			{
				if( changenameresult->ucSuccess == 0 )	// ����
				{
					int selectedservernum = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
					if( selectedservernum == _GR_INDEX_UNION_ )
					{
						_XWindow_ServerUnification* pServerUnification_Window = (_XWindow_ServerUnification*)TempWindowMgr->FindWindow(_XDEF_WTITLE_SERVERUNIFICATIONWINDOW);
						if( pServerUnification_Window )				
							pServerUnification_Window->SetChangeNameResult(changenameresult);
					}
					else
					{
						_XWindowManager* selectcharacterWindowMgr = ((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.GetWindowManagerPtr();
						if( selectcharacterWindowMgr )
						{
							_XWindow_SelectCharacter* pSelectCharacter_Window = (_XWindow_SelectCharacter *)selectcharacterWindowMgr->FindWindow(_XDEF_WTITLE_SELECTCHARACTER);
							if( pSelectCharacter_Window )
							{
								strcpy( pSelectCharacter_Window->m_Name, changenameresult->ucNewName );
								switch(pSelectCharacter_Window->m_SelectedCharacter) 
								{
								case 0:
									pSelectCharacter_Window->m_Blank01Button.SetButtonText(pSelectCharacter_Window->m_Name, _XFONT_ALIGNTYPE_LEFT, 37, 0 );
									pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFFCF904 );
									break;
								case 1:
									pSelectCharacter_Window->m_Blank02Button.SetButtonText(pSelectCharacter_Window->m_Name, _XFONT_ALIGNTYPE_LEFT, 37, 0 );
									pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFFCF904 );
									break;
								case 2:
									pSelectCharacter_Window->m_Blank03Button.SetButtonText(pSelectCharacter_Window->m_Name, _XFONT_ALIGNTYPE_LEFT, 37, 0 );
									pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFFCF904 );
									break;
								}								
							}
						}				
						strcpy( g_pLocalUser->m_AllCharacterInfo[changenameresult->ucIndex].charactername, changenameresult->ucNewName);
						strcpy( g_pLocalUser->m_CharacterInfo.charactername, changenameresult->ucNewName );
						g_pLocalUser->m_AllCharacterInfo[changenameresult->ucIndex].ucChangeNameFlag = 0;
					}
				}				
				else	// ����
				{
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("�ߺ��� �̸��Դϴ�.") );
				}
			}
		}
		break;
	case en_ug_notyet_move_in_new :		// 30 �ż����� : ���� ������ �Ϸ����� �ʾҴ�.
		{
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.CloseLoginRequestDialog();
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, _T(""), _T("���������� �Ϸ���� �ʾҽ��ϴ�. ���� ������ �Ϸ��� ���� ���� ����Ʈ���� ������ ���ա� ������ �����ϼž� �մϴ�."),
						TRUE, _XDEF_LS_COMPULSORYSTEP2YES );
		}
		break;
	case en_ug_force_use_charac_in_new :	// en_ug_notyet_move_in_new ������ 
		{
		}
		break;
	case en_ug_already_move_in_old :	// 32 �̹� ������ �Ϸ�� ������������ ����. �޼��� �����ְ� ƨ��.
		{
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.CloseLoginRequestDialog();
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("������ �Ϸ�� �����Դϴ�. �� ������ �����Ͽ� �ֽñ� �ٶ��ϴ�.") );
		}
		break;
	case en_ug_force_use_result :
		{
			MSG_UG_FORCE_USE_RESULT* forceuseresult = (MSG_UG_FORCE_USE_RESULT*)serveruniinfo;
			if( forceuseresult )
			{
				switch(forceuseresult->ucResult)	// 0 - Success, 1 - ��� Ȯ��, 2 - ����.
				{
				case 0 :
					break;
				case 1 :
					{
						((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.CloseLoginRequestDialog();
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("�ֹι�ȣ ���ڸ��� Ȯ�� �� �ٽ� �õ� �Ͻñ� �ٶ��ϴ�.") );
					}
					break;
				case 2 :
					{
						((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.CloseLoginRequestDialog();
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("���ӿ� �����Ͽ����ϴ�.") );
					}
					break;
				}			
			}			
		}
		break;
	}	
#endif //#ifdef _XTS_SERVERUNIFICATION

#endif //#ifdef _XTS_NEWCHARACLIST
}