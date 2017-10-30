#ifndef _XQUESTSCRIPTDEFINE_H_
#define _XQUESTSCRIPTDEFINE_H_

//////////////////////////////////////////////////////////////////////////////////////
/////  Trigger condition ���� ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_CONDITION_COUNT								66

#define ID_CONDITION_CHECKVALUE							0		// �÷��̾�_���Ⱥ�
#define ID_CONDITION_HAVESKILL							1		// �÷��̾�_������
#define ID_CONDITION_CHECKSTATUS						2		// �÷��̾�_���°˻�
#define ID_CONDITION_CHECKGENDER						3		// �÷��̾�_�����˻�
#define ID_CONDITION_CHECKJOINGROUP						4		// �÷��̾�_���İ˻�
#define ID_CONDITION_CHECKAREA							5		// �÷��̾�_�����˻�
#define ID_CONDITION_CHECKMESSAGE						6		// �÷��̾�_�޼�����
#define ID_CONDITION_HAVEITEM							7       // �÷��̾�_�����ۺ�
#define ID_CONDITION_CHECKATTACK						8		// �÷��̾�_��������˻�
#define ID_CONDITION_CHECKMARTIAL						9		// �÷��̾�_���������˻�
#define ID_CONDITION_CHECKPARTYCOUNT					10		// ��Ƽ��_���˻�
#define ID_CONDITION_CHECKPARTYSTATUS					11		// ��Ƽ��_���°˻� <-��������(������, ����)
#define ID_CONDITION_CHECKAREAOTHER						12		// Ÿ�÷��̾�_�����˻�
#define ID_CONDITION_CHECKDISTANCE						13		// Ÿ�÷��̾�_�Ÿ�üũ
#define ID_CONDITION_CHECKTARGETTING					14		// Ÿ�÷��̾�_Ÿ���ð˻�
#define ID_CONDITION_CHECKTIMER							15		// Ÿ�̸�üũ
#define ID_CONDITION_CHECKPASSTIME						16		// �ð����üũ
#define ID_CONDITION_CHECKPROCEDINGTRIGGER				17		// ����Ʈ����üũ
#define ID_CONDITION_CHECKACTIONID						18		// �׼ǽ���üũ
#define ID_CONDITION_CHECKPROCEDINGQUEST				19		// ��������Ʈ���üũ
#define ID_CONDITION_ISPROGRESSQUEST					20		// Ư������Ʈ����üũ
#define ID_CONDITION_PROGRESSQUEST						21		// �������� ����Ʈ ���� üũ
#define ID_CONDITION_CHECKSELECTION						22		// ���������� ������ ��ȣ üũ 
#define ID_CONDITION_CHECKNPCDIALOGMODE					23		// NPC ��ȭ ��� ���� üũ
#define ID_CONDITION_CHECKWEARITEM						24		// ������ ���� üũ  
#define ID_CONDITION_SETQUESTMARKAREA					25		// QUEST��ũ(!) ǥ���ϱ� ����  
#define ID_CONDITION_SETQUESTMARKNPC					26		// QUEST��ũ(!) ǥ���ϱ� NPC   
#define ID_CONDITION_SETHINTMARKAREA					27		// HINT��ũ(?) ǥ���ϱ� ����
#define ID_CONDITION_SETHINTMARKNPC						28		// HINT��ũ(?) ǥ���ϱ� NPC 
#define ID_CONDITION_CHECKSELECTIONEND					29      // ������ üũ ���ᰡ �Ǿ�����.....
#define ID_CONDITION_CHECKCOUNTPOINT					30		// ī���� ����Ʈ üũ�ϱ� 
#define ID_CONDITION_CHECKMOTION						31		// Ư�� �ൿ üũ 
#define ID_CONDITION_CHECKABNORMALSTATE					32		// �÷��̾�_�����̻�üũ 
#define ID_CONDITION_CHECKBATTLERECORD					33      // �÷��̾�_������üũ
#define ID_CONDITION_CHECKPARTYJOINGROUP				34      // ��Ƽ��_����üũ 
#define ID_CONDITION_CHECKPARTYVALUE					35		// ��Ƽ��_���Ⱥ�(��,����ܰ� �˻�, ��,����ܰ� ���հ˻�...)
#define ID_CONDITION_DISPLAYREWARD						36		// ���� ǥ���ϱ� ������ �н�
#define ID_CONDITION_CHECKSKILLSTATE					37		// �÷��̾�_�������º�
#define ID_CONDITION_CHECKCLANSERVICEPOINT				38		// �÷��̾�_���ı⿩��üũ
#define ID_CONDITION_OPTION_ITEMSTATE					39		// ����Ʈ ��ҽ� ������ ���� or ���� ����(�ɼ����ǽĿ����� ���)
#define ID_CONDITION_CHECKQUESTEMAIL					40		// ǳ��� ���� or �ź� üũ (������ �н�, ������ �б��� �������� ���� �� ����) 
#define ID_CONDITION_CHECKBOSSBATTLESTART				41		// ����� ���� üũ
#define ID_CONDITION_CHECKBOSSBATTELSTAGENUMBER			42		// ����� �ܰ� üũ
#define ID_CONDITION_CHECKPARTYMASTER					43		// ��Ƽ��_����˻�
#define ID_CONDITION_CHECKPARTYMEMBERCLASS				44		// ��Ƽ��_��å�˻�
#define ID_CONDITION_CHECKAREAPARTYMEMBER				45		// ��Ƽ��_�����˻�
#define ID_CONDITION_CHECKRECORDNUMBER					46		// �÷��̾�_���ǥ�˻�
#define ID_CONDITION_CHECKLEVEL							47		// �÷��̾�_���뵵�˻�
#define ID_CONDITION_CHECKPLAYERCLASS					48		// �÷��̾�_��å�˻�
#define ID_CONDITION_CHECKINVENSPACE					49		// �೶���������˻�
#define ID_CONDITION_CHECKPARTYMASTERDEAD				50		// ��������˻�(�ɼ����ǽĿ����� ���)
#define ID_CONDITION_CHECKDESTORYITEM					51		// �������ı��˻�(�ɼ����ǽ�OR��������Ÿ��)
#define ID_CONDITION_CHECKSTARTLIMITQUEST				52		// �ο������������˻�
#define ID_CONDITION_CHECKCOMPLETELIMITQUEST			53		// �Ϸἱ���������˻�
#define ID_CONDITION_CHECKPARTYGENDER					54		// ��Ƽ��_����üũ
#define ID_CONDITION_CHECKSERVICETYPE					55		// �÷��̾�_������˻�
#define ID_CONDITION_CHECKPARTYORGANIZATION				56		// ��Ƽ��_�����ܰ˻�
#define ID_CONDITION_PARTYHAVEITEM						57		// ��Ƽ��_�����۰˻�
#define ID_CONDITION_CHECKNICKNAME						58		// �÷��̾�_��ȣ�˻�
#define ID_CONDITION_CHECKGROUPLEADER					59		// �������� �� ���� �˻�
#define ID_CONDITION_CHECKPARTYCLAN						60		// ��Ƽ��_���İ˻�
#define ID_CONDITION_CHECKPARTYPREQUEST					61		// ��Ƽ��_��������Ʈ�˻�
#define ID_CONDITION_CHECKPARTYCLASSOR					62		// ��Ƽ��_��ü��å�˻�
#define ID_CONDITION_CHECK_MND_MASTERPOINT				63		// �����ý���_���±⿩���˻�
#define ID_CONDITION_CHECK_MND_MASTERRANK				64		// �����ý���_���µ�ް˻�
#define ID_CONDITION_CHECK_MND_STATUS					65		// �����ý���_���°˻�



//////////////////////////////////////////////////////////////////////////////////////
/////  Trigger action ���� ///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#define ID_ACTION_COUNT									74

#define ID_ACTION_ADDSKILL								0		// �÷��̾�_��������
#define ID_ACTION_DELETESKILL							1		// �÷��̾�_��������
#define ID_ACTION_GIVEITEM								2		// �÷��̾�_�������ֱ�
#define ID_ACTION_TAKEITEM								3		// �÷��̾�_�����ۻ���
#define ID_ACTION_GIVEEXPERIENCE						4		// �÷��̾�_����ġ �ֱ�
#define ID_ACTION_MOVE									5		// �÷��̾�_�̵�
#define ID_ACTION_SETSTATUS								6		// �÷��̾�_���ȼ���
#define ID_ACTION_SOUNDSTART							7		// ȿ���� ����
#define ID_ACTION_MUSICSTART							8		// ����� ����
#define ID_ACTION_SETENV								9		// ���� ����
#define ID_ACTION_EFFECTSTART							10		// Ư��ȿ�� ����
#define ID_ACTION_TIMERSTART							11		// Ÿ�̸� ����
#define ID_ACTION_SENDMESSAGE							12		// ��ȭ ���
#define ID_ACTION_AUTOSTART								13		// �ڵ� ����Ʈ ����
#define ID_ACTION_RESULTQUEST							14		// ����Ʈ ��� ����
#define ID_ACTION_MOVECAMERA							15		// ī�޶� �̵�����
#define ID_ACTION_ROTATECAMERA							16		// ī�޶� ȸ������
#define ID_ACTION_CAMERASETTING							17		// ī�޶� �ܼ���
#define ID_ACTION_SHAKECAMERA							18		// ī�޶� ����
#define ID_ACTION_SCREENEFFECT							19		// ȭ�� ����
#define ID_ACTION_SETWEATHER							20		// ��� ����
#define ID_ACTION_BLOCKTRIGGER							21		// Ʈ���� ��
#define ID_ACTION_JOINGROUP								22		// ���İ���
#define ID_ACTION_GOTOTRIGGER							23		// �ش� Ʈ���ŷ� �̵�
#define ID_ACTION_QUESTPROGRESSINFO						24		// ����Ʈ ���� ���� : ����Ʈ â�� ǥ�õ� Ʈ������ ���� �ؽ�Ʈ 
#define ID_ACTION_SELECTION								25		// ������ ����
#define ID_ACTION_STARTMINIGAME							26		// �̴ϰ��� ����( ������� )
#define ID_ACTION_DEFAULT								27		// �ƹ��ൿ�� ���ϴ� Ʈ����
#define ID_ACTION_TALKINGTOONESELF						28		// ȥ�ڸ��ϱ� 
#define ID_ACTION_SETMOVEVELOCITY						29		// �̵��ӵ� ���� ����
#define ID_ACTION_SETWEARITEM							30		// ������ ���� ����
#define ID_ACTION_STARTOPTIONCONDITION					31		// �ɼ� ���� ����Ʈ �����ϱ�
#define ID_ACTION_ENDOPTIONCONDITION					32		// �ɼ� ���� ����Ʈ ������ 
#define ID_ACTION_GIVECOUNTPOINT						33		// ī���� ����Ʈ �ֱ� 
#define ID_ACTION_SETMOBSPAWN							34		// ���� �ҷ�����
#define ID_ACTION_SETOBJECTSPAWN						35		// ������Ʈ �ҷ�����
#define ID_ACTION_SETMOTION								36		// ������ �ൿ ��Ű��
#define ID_ACTION_SETMOVIE								37		// ���� �����ֱ� 
#define ID_ACTION_GIVENICKNAME							38		// ��ȣ�ֱ�		
#define ID_ACTION_SETMAP								39		// ���� ON/OFF	
#define ID_ACTION_MONSTERTALKINGTOONESELF				40		// ���� ȥ�ڸ��ϱ� 	

#define ID_ACTION_SETSKILLEXP							41		// �÷��̾�_�������õ�(�ֱ�,���ѱ�)
#define ID_ACTION_SETSKILLSTATE							42		// �÷��̾�_��������(�������,��밡��)
#define ID_ACTION_SETABNORMALSTATE						43		// �÷��̾�_�����̻���(�ֱ�, ����)
#define ID_ACTION_GIVESETTINGITEM						44		// ������ ������ �ֱ�
#define ID_ACTION_RESETVALUE							45		// ���� �ʱ�ȭ

#define ID_ACTION_SETPLAYERCLASS						46		// �÷��̾�_Ŭ��������
#define ID_ACTION_SETCLANSERVICEPOINT					47		// �÷��̾�_���ı⿩��(�ֱ�,���ѱ�)
#define ID_ACTION_SETITMECOUNTERSTART					48		// ������ ī���� �����ϱ�
#define ID_ACTION_STOPCOUNTER							49		// ī���� ���߱�
#define ID_ACTION_SETCONDITIONEFFECT					50		// �÷��̾�_����ȿ���ο�
#define ID_ACTION_SETSYSTEMMESSAGE						51		// �ý��۸޼������
#define ID_ACTION_SETNPCONOFF							52		// NPC ���� & ����
#define ID_ACTION_SENDQUESTEMAIL						53		// ǳ��� ������ 
#define ID_ACTION_PARTYGIVEITEM							54		// ��Ƽ��_�������ֱ�or����(���,���,MVP)
#define ID_ACTION_PARTYGIVEEXPERIENCE					55		// ��Ƽ��_�����ֱ�or����(���,���,MVP)  
#define ID_ACTION_PARTYSETABNORMALSTATE					56		// ��Ƽ��_�����̻��ֱ�
#define ID_ACTION_PARTYGIVEEQUALLYREWARD				57		// ��Ƽ��_�յ��Ѻ����ֱ�(����/��Ƽ����) 
#define ID_ACTION_HOLDITEM								58		// ���������������Ұ�����
#define ID_ACTION_PARTYMESSAGE							59		// ��Ƽ��_�޼������(ȥ�㸻,�ý���)
#define ID_ACTION_INSERTRANK							60		// ��ũ�� ����ϱ�
#define ID_ACTION_CHANGESERVERGROUP						61		// ������ ���� �����ϱ� 
#define ID_ACTION_GIVEEXPERIENCEBYPERCENTAGE			62		// �÷��̾�_n%����ġ�ֱ�
#define ID_ACTION_DISPLAYQUESTRANKING					63		// Ư������Ʈ��ŷ�����ֱ�
#define ID_ACTION_STARTPARTYITMECOUNTER					64      // ��Ƽ������ī���ͽ����ϱ�
#define ID_ACTION_GIVECLASSITEM							65		// ���Һ� �������ֱ�(����,����)
#define ID_ACTION_PARTYGIVENICKNAME						66		// ��Ƽ��_��ȣ�ֱ�
#define ID_ACTION_PARTYTAKEITEM							67		// ��Ƽ��_�����ۻ���(��Ƽ��ü)
#define ID_ACTION_SETVIRTUALMODE						68		// ������������ϱ�
#define ID_ACTION_GIVEEXPERIENCEBYLEVEL					69		// �÷��̾�_����%����ġ�ֱ�
#define ID_ACTION_SETWIDECONDITIONEFFECT				70		// �÷��̾�_��������ȿ���ο�
#define ID_ACTION_SETCLASSGRADE							71		// ��å ��� ����

#define ID_ACTION_MND_GIVEMASTER_RANK					72		// �����ý��� ���µ�޼���
#define ID_ACTION_MND_SETMASTERPOINT					73		// �����ý��� ���±⿩��(�ֱ�,����)





//////////////////////////////////////////////////////////////////////////////////////////
// ����Ʈ���� ����ϴ� �� ����
/////////////////////////////////////////////////////////////////////////////////////////

// Ʈ���� Ȱ�� / ��Ȱ�� ---------------------------------------------------------------
#define ID_TRIGGER_DISABLE						        0
#define ID_TRIGGER_ENABLE							    1

// Ʈ���� ������ �ʱ�ȭ ---------------------------------------------------------------
#define ID_TRIGGER_SETDATA_NONE						   -1

// Ʈ���� ���ǹ��� ��� ���� ----------------------------------------------------------
#define ID_TRIGGER_CONDITIONFUNCTION_OR			        0
#define ID_TRIGGER_CONDITIONFUNCTION_AND			    1

// Ʈ���� ����Ʈ ��ũ ���� ----------------------------------------------------------
#define ID_TRIGGER_QUESTMARK_AREA				        0
#define ID_TRIGGER_QUESTMARK_NPC					    1

// Ʈ���� ��Ʈ ��ũ ���� ----------------------------------------------------------
#define ID_TRIGGER_HINTMARK_AREA				        0
#define ID_TRIGGER_HINTMARK_NPC						    1

// Ʈ���� ��Ʈ ��ũ Ÿ�� ----------------------------------------------------------
#define ID_TRIGGER_MARKTYPE_WORLD				        0
#define ID_TRIGGER_MARKTYPE_MAP						    1
#define ID_TRIGGER_MARKTYPE_WORLDANDMAP				    2

// �� ������ ���� -------------------------------------------------------------------
#define ID_TRIGGER_COMPARE_COUNT						3

#define ID_TRIGGER_COMPARE_THELEAST                     0		// <
#define ID_TRIGGER_COMPARE_THEMOST                      1		// >
#define ID_TRIGGER_COMPARE_EXACTLY                      2		// =


// ������ �� ���� ---------------------------------------------------------------------
#define ID_TRIGGER_DATASET_COUNT                        3

#define ID_TRIGGER_DATASET_ADD                          0		// +
#define ID_TRIGGER_DATASET_SUBTRACT                     1		// -
#define ID_TRIGGER_DATASET_SET                          2		// =

// Trigger ���� ----------------------------------------------------------------------
#define ID_TRIGGER_CLASS_NORMAL							0
#define ID_TRIGGER_CLASS_FAIL							1
#define ID_TRIGGER_CLASS_JUMP							2
#define ID_TRIGGER_CLASS_END							3
#define ID_TRIGGER_CLASS_STARTCONDITION					127

// Trigger �Ӽ� ----------------------------------------------------------------------
#define ID_TRIGGER_PROPERTY_CONTINUE					0
#define ID_TRIGGER_PROPERTY_COMPLETE					1

// Check value ------------------------------------------------------------------------
// Condition_CheckValue 
//-------------------------------------------------------------------------------------
#define ID_QUEST_CHECKVALUETYPE_TOTALLEVEL					0	// ���� ����
#define ID_QUEST_CHECKVALUETYPE_VITALITY					1	// ����
#define ID_QUEST_CHECKVALUETYPE_ZEN							2	// ����
#define ID_QUEST_CHECKVALUETYPE_SOUL						3	// ����
#define ID_QUEST_CHECKVALUETYPE_HEART						4	// �ɱ�
#define ID_QUEST_CHECKVALUETYPE_STAMINA						5	// ����
#define ID_QUEST_CHECKVALUETYPE_VITALPOWER					6	// ü��
#define ID_QUEST_CHECKVALUETYPE_INNERPOWER					7	// ����
#define ID_QUEST_CHECKVALUETYPE_SOULPOWER					8	// ����
#define ID_QUEST_CHECKVALUETYPE_HEARTPOWER					9	// �ɷ�
#define ID_QUEST_CHECKVALUETYPE_STAMINAPOWER				10	// ����
#define ID_QUEST_CHECKVALUETYPE_HONOR						11	// ��
#define ID_QUEST_CHECKVALUETYPE_RETRIBUTION					12	// ��
#define ID_QUEST_CHECKVALUETYPE_EXPERIENCE					13	// ����ġ

// Target class -----------------------------------------------------------------------
// Action_EffectStart
//-------------------------------------------------------------------------------------
#define ID_QUEST_TARGETCLASS_PLAYER						0	// �÷��̾�
#define ID_QUEST_TARGETCLASS_STATICNPC					1	// ���� NPC
#define ID_QUEST_TARGETCLASS_DYNAMICNPC					2	// ���� NPC
#define ID_QUEST_TARGETCLASS_AREA						3	// ����

// Target class ------------------------------------------------------------------------
// Condition_CheckAreaOther
// -------------------------------------------------------------------------------------
#define ID_QUEST_TARGETOTHER_STATICNPC					0	// ���� NPC
#define ID_QUEST_TARGETOTHER_DYNAMICNPC					1	// ���� NPC
#define ID_QUEST_TARGETOTHER_MONSTER					2	// ��
#define ID_QUEST_TARGETOTHER_PARTY						3	// ��Ƽ


// Targetting --------------------------------------------------------------------------
// Condition_CheckTargetting
//--------------------------------------------------------------------------------------
#define ID_QUEST_TARGETTING_STATICNPC					0	// ���� NPC
#define ID_QUEST_TARGETTING_DYNAMICNPC					1	// ���� NPC
#define ID_QUEST_TARGETTING_AREA						2	// ����
#define ID_QUEST_TARGETTING_OBJECT						3	// ������Ʈ

// Target object ------------------------------------------------------------------------
// Condition_CheckDistance
//---------------------------------------------------------------------------------------
#define ID_QUEST_TARGETOBJECT_STATICNPC					0	// ���� NPC
#define ID_QUEST_TARGETOBJECT_DYNAMICNPC				1	// ���� NPC
#define ID_QUEST_TARGETOBJECT_AREA						2	// ����

// TargetObject2(monster ����) ----------------------------------------------------------
// Condition_CheckStatusOther
//---------------------------------------------------------------------------------------
#define ID_QUEST_TARGETOTHER_STATICNPC					0	// ���� NPC
#define ID_QUEST_TARGETOTHER_DYNAMICNPC					1	// ���� NPC
#define ID_QUEST_TARGETOTHER_MONSTER					2	// ����

// Item type ----------------------------------------------------------------------------
// Condition_HaveItem/Action_GiveItem/Action_TakeItem
//---------------------------------------------------------------------------------------
#define ID_QUEST_ITEMTYPE_WEAPON						0	// �����
#define ID_QUEST_ITEMTYPE_CLOTHES						1	// �ǻ��
#define ID_QUEST_ITEMTYPE_BOOK							2	// �������
#define ID_QUEST_ITEMTYPE_POTION						3	// ȸ����
#define ID_QUEST_ITEMTYPE_THROW							4	// ��ô
#define ID_QUEST_ITEMTYPE_POISON						5	// �ߵ� �ֹ���
#define ID_QUEST_ITEMTYPE_COUNTERACT					6	// �ص� �ֹ���
#define ID_QUEST_ITEMTYPE_RESOURCE						7	// �ڿ�
#define ID_QUEST_ITEMTYPE_QUEST							8	// ����Ʈ
#define ID_QUEST_ITEMTYPE_ACCESSORY						9	// �Ǽ��縮
#define ID_QUEST_ITEMTYPE_LIFE							10	// ��Ȱ ����
#define ID_QUEST_ITEMTYPE_MUSIC							11	// �Ǳ�
#define ID_QUEST_ITEMTYPE_PASS							12	// ������
#define ID_QUEST_ITEMTYPE_MONEY							13	// ��


// Status type(player) -------------------------------------------------------------------
// Action_SetStatus
//---------------------------------------------------------------------------------------
#define ID_QUEST_STATUSTYPE_VITALITY					0	// ����
#define ID_QUEST_STATUSTYPE_ZEN							1	// ����
#define ID_QUEST_STATUSTYPE_SOUL						2	// ����
#define ID_QUEST_STATUSTYPE_HEART						3	// �ɱ�
#define ID_QUEST_STATUSTYPE_VIGOR						4	// ����
#define ID_QUEST_STATUSTYPE_VITALPOWER					5	// ü��
#define ID_QUEST_STATUSTYPE_INNERPOWER					6	// ����
#define ID_QUEST_STATUSTYPE_HEARTPOWER					7	// �ɷ�
#define ID_QUEST_STATUSTYPE_SOULPOWER					8	// ����
#define ID_QUEST_STATUSTYPE_STAMINAPOWER				9	// ����

// Experience type -----------------------------------------------------------------------
// Action_GiveExperience
// ---------------------------------------------------------------------------------------
#define ID_QUEST_SETEXP_EXPERICENCE						0	// ����ġ
#define ID_QUEST_SETEXP_HONOR							1	// ��
#define ID_QUEST_SETEXP_RETRIBUTION						2	// ����

// Status type(other) ---------------------------------------------------------------------
// Action_SetStatusOther
//-----------------------------------------------------------------------------------------
#define ID_QUEST_STATUSTYPE_DEATH						0	// ����
#define ID_QUEST_STATUSTYPE_REVIVAL						1	// ��Ȱ


// �޼��� �� -----------------------------------------------------------------------------
// Condition_CheckMessage
//-----------------------------------------------------------------------------------------
#define ID_QUEST_MATCHMESSAGE_WHOLEWORD					0	// ��ü ����
#define ID_QUEST_MATCHMESSAGE_PARTIALWORD				1	// �κ� ����

// Ÿ�̸� ----------------------------------------------------------------------------------
// Condition_CheckTimer
//------------------------------------------------------------------------------------------
#define ID_QUEST_CHECKTIMER_ANYTIME						0	// ���� ����
#define ID_QUEST_CHECKTIMER_DAYLIGHT					1	// ��
#define ID_QUEST_CHECKTIMER_NIGHT						2	// ��

// ���� ���� -------------------------------------------------------------------------------
// Action_SetEnv
//------------------------------------------------------------------------------------------
#define ID_QUEST_WEATHER_CLEAR							0	// ����
#define ID_QUEST_WEATHER_RAIN							1	// ��
#define ID_QUEST_WEATHER_LIGHTNING						2	// �� ����
#define ID_QUEST_WEATHER_SNOW							3	// ��
#define ID_QUEST_WEATHER_SANDY							4	// Ȳ��

// Status type2(player) -------------------------------------------------------------------
// Condition_CheckStatus
//-----------------------------------------------------------------------------------------
#define ID_QUEST_STATUSMYSELF_BATTLE					0	// ����
#define ID_QUEST_STATUSMYSELF_RIDEHORSE					1	// �¸�
#define ID_QUEST_STATUSMYSELF_INPARTY					2	// ��Ƽ ����
#define ID_QUEST_STATUSMYSELF_NOTPARTY					3	// ��Ƽ �̰���
#define ID_QUEST_STATUSMYSELF_JOINGROUP					4	// ���� ����
#define ID_QUEST_STATUSMYSELF_NOTJOINGROUP				5	// ���� �̰���
#define ID_QUEST_STATUSMYSELF_LEVELUP					6	// ������
#define ID_QUEST_STATUSMYSELF_TRADENPC					7	// NPC�� �ŷ�
#define ID_QUEST_STATUSMYSELF_TALKNPC					8	// NPC�� ��ȭ
#define ID_QUEST_STATUSMYSELF_DEATH						9	// ����
#define ID_QUEST_STATUSMYSELF_REVIVE					10	// ��Ȱ


// Status type2(other) --------------------------------------------------------------------
// Condition_CheckPartyStatus
//-----------------------------------------------------------------------------------------
#define ID_QUEST_STATUSPARTY_LEVELUP					0	// ���� ��
#define ID_QUEST_STATUSPARTY_DEATH						1	// ����


// ����Ʈ ��� -----------------------------------------------------------------------------
// Action_ResultQuest
//-----------------------------------------------------------------------------------------
#define ID_QUEST_RESULT_SUCCESS							0		// ����
#define ID_QUEST_RESULT_FAIL							1		// ����
#define ID_QUEST_RESULT_CANCEL							2		// ���


// �޼��� üũ - �� EditBox�� ���� �ܾ� �� ----------------------------------------------
// Condition_MessageCheck
//-----------------------------------------------------------------------------------------
#define ID_MAX_CHECKMESSAGE								10		// 10�� �ܾ���� ó�� ����


// �ð� ��� üũ - ����/��/����/�� ���� �ð� -----------------------------------------------
// Condition_CheckPassTime
//-----------------------------------------------------------------------------------------
#define ID_QUEST_CHECKTIME_DAWNTIME							0		// ����
#define ID_QUEST_CHECKTIME_DAYTIME							3600	// ��
#define ID_QUEST_CHECKTIME_NIGHTFALLTIME					14400	// ����
#define ID_QUEST_CHECKTIME_NIGHTTIME						18000	// ��
#define ID_QUEST_CHECKTIME_ONEDAYTIME						28800	// �Ϸ� = 28800�� = 8�ð�

#define ID_QUEST_CHECKTIME_DAWN								0
#define ID_QUEST_CHECKTIME_DAY								1
#define ID_QUEST_CHECKTIME_NIGHTFALL						2
#define ID_QUEST_CHECKTIME_NIGHT							3


// ī�޶� ���� ---------------------------------------------------------------------------
// Action_ShakeCamera
//-----------------------------------------------------------------------------------------
#define ID_QUEST_SHAKECAMERA_MAXHEIGHT						0.5
#define ID_QUEST_SHAKECAMERA_UNITHEIGHT						0.1
// ����
#define ID_QUEST_SHAKECAMERA_UP								0
#define ID_QUEST_SHAKECAMERA_DOWN							1
#define ID_QUEST_SHAKECAMERA_LEFT							0
#define ID_QUEST_SHAKECAMERA_RIGHT							1


// ī�޶� ȸ�� -------------------------------------------------------------------------------
// Action_RotateCamera
//-------------------------------------------------------------------------------------------
#define ID_QUEST_ROTATECAMERA_ANGLE							15.0


// ���� -------------------------------------------------------------------------------------
// Condition_HaveSkill/Action_AddSkill/Action_DeleteSkill
//------------------------------------------------------------------------------------------

#define ID_QUEST_JOINGROUP_NANGIN							0
#define ID_QUEST_JOINGROUP_GAEBANG							1
#define ID_QUEST_JOINGROUP_BEEGOONG							2
#define ID_QUEST_JOINGROUP_SORIM							3
#define ID_QUEST_JOINGROUP_NOCKRIM							4
#define ID_QUEST_JOINGROUP_MOODANG							5
#define ID_QUEST_JOINGROUP_MAKYO							6
#define ID_QUEST_JOINGROUP_SEGA								7
#define ID_QUEST_JOINGROUP_NOFIXED							8

// �÷��̾� �����̻� -------------------------------------------------------------------------------------
// Condition_AbnormalState
//------------------------------------------------------------------------------------------

#define ID_QUEST_STATE_STATE1								0 //����
#define ID_QUEST_STATE_STATE2								1 //���� (�������� ����)
#define ID_QUEST_STATE_STATE3								2 //ħ��
#define ID_QUEST_STATE_STATE4								3 //����
#define ID_QUEST_STATE_STATE5								4 //����
#define ID_QUEST_STATE_STATE6								5 //������
#define ID_QUEST_STATE_STATE7								6 //�Ű浶
#define ID_QUEST_STATE_STATE8								7 //ġ�絶
#define ID_QUEST_STATE_STATE9								8 //ȥ��
#define ID_QUEST_STATE_STATE10								9 //ȯ��
#define ID_QUEST_STATE_STATE11								10 //ȯû
#define ID_QUEST_STATE_STATE12								11 //����
#define ID_QUEST_STATE_STATE13								12 //�Ǹ�
#define ID_QUEST_STATE_STATE14								13 //��
#define ID_QUEST_STATE_STATE15								14 //����
#define ID_QUEST_STATE_STATE16								15 //����
#define ID_QUEST_STATE_STATE17								16 //����
#define ID_QUEST_STATE_STATE18								17 //����
#define ID_QUEST_STATE_STATE19								18 //��ȭ�Ը�
#define ID_QUEST_STATE_STATE20								19 //����

// �÷��̾� ���� ���� ���� ---------------------------------------------------------------------
// Condition_CheckAttack
// ----------------------------------------------------------------------------------------
#define ID_QUEST_CHECKATTACK_TARGETTING						0
#define ID_QUEST_CHECKATTACK_DEATH							1
#define ID_QUEST_CHECKATTACK_HIT							2
#define ID_QUEST_CHECKATTACK_CHANGE							3
#define ID_QUEST_CHECKATTACK_CLIENTMOBDEATH					4


// ����, ��� ���� ���� -------------------------------------------------------------------
// Action_CancelWeather
//------------------------------------------------------------------------------------------
#define ID_QUEST_CANCELWEATHER_ENV							0
#define ID_QUEST_CANCELWEATHER_WEATHER						1

// ���� ��� ���� ------------------------------------------------------------------------
// Action_SoundStart/Action_MusicStart
// -----------------------------------------------------------------------------------------
#define ID_QUEST_SOUNDTARGET_START							0
#define ID_QUEST_SOUNDTARGET_STOP							1


// ��� ��� -------------------------------------------------------------------------------
// Action_SoundStart/Action_MusicStart/Action_EffectStart
//------------------------------------------------------------------------------------------
#define ID_QUEST_PLAYTYPE_ONCE								0
#define ID_QUEST_PLAYTYPE_LOOP								1

// ī�޶� ���� -----------------------------------------------------------------------------
// Action_RotateCamera
//-----------------------------------------------------------------------------------------
#define ID_QUEST_CAMERAHEIGHT_HIGH							0
#define ID_QUEST_CAMERAHEIGHT_MIDDLE						1
#define ID_QUEST_CAMERAHEIGHT_LOW							2

// ī�޶� ȸ�� ���� --------------------------------------------------------------------------
// Action_RotateCamera
//-------------------------------------------------------------------------------------------
#define ID_QUEST_CAMERADIRECTION_LEFT						0
#define ID_QUEST_CAMERADIRECTION_RIGHT						1

// ī�޶� �Ÿ� --------------------------------------------------------------------------
// Action_RotateCamera
//-------------------------------------------------------------------------------------------
#define ID_QUEST_CAMERADISTANCE_HIGH						0
#define ID_QUEST_CAMERADISTANCE_MIDDLE						1
#define ID_QUEST_CAMERADISTANCE_LOW							2

// ī�޶� ���� ���� ------------------------------------------------------------------------
// Action_ShakeCamera
//-------------------------------------------------------------------------------------------
#define ID_QUEST_CAMERASHAKE_TOPDOWN						0
#define ID_QUEST_CAMERASHAKE_LEFTRIGHT						1

// ī�޶� ���� ���� ------------------------------------------------------------------------
// Action_ShakeCamera/Action_RotateCamera 
//-------------------------------------------------------------------------------------------
#define ID_QUEST_CAMERAINTENSITY_FIRST						0
#define ID_QUEST_CAMERAINTENSITY_SECOND						1
#define ID_QUEST_CAMERAINTENSITY_THIRD						2
#define ID_QUEST_CAMERAINTENSITY_FORTH						3
#define ID_QUEST_CAMERAINTENSITY_FIFTH						4


// ī�޶� ���� -----------------------------------------------------------------------------
// Action_CameraSetting
//------------------------------------------------------------------------------------------
#define ID_QUEST_CAMERASETTING_ZOOMIN						0
#define ID_QUEST_CAMERASETTING_ZOOMOUT						1

// ����/�ƿ� ��� ���� ----------------------------------------------------------------------
// Action_CameraSetting
// ------------------------------------------------------------------------------------------
#define ID_QUEST_CAMERASETTING_PLAYER						0
#define ID_QUEST_CAMERASETTING_AREA							1


// ī�޶� �̵� ------------------------------------------------------------------------------
// Action_MoveCamera
//-------------------------------------------------------------------------------------------
#define ID_QUEST_CAMERAMOVE_PASS							0
#define ID_QUEST_CAMERAMOVE_AREA							1

// ���� üũ --------------------------------------------------------------------------------
// Condition_CheckGender
//-------------------------------------------------------------------------------------------
#define ID_QUEST_GENDER_MAN									0
#define ID_QUEST_GENDER_WOMAN								1

// ȭ�� ���� (Fadein/out) -------------------------------------------------------------------
// Action_ScreenEffect
// ------------------------------------------------------------------------------------------
#define ID_QUEST_SCREENEFFECT_FADEIN						0
#define ID_QUEST_SCREENEFFECT_FADEOUT						1

// ī�޶� ��ũ ���� -------------------------------------------------------------------------
#define ID_QUEST_CAMERA_KEEP								0	// ���� �� ī�޶� �״�� ����
#define ID_QUEST_CAMERA_RESTORE								1	// ���� �� ī�޶� �ǵ�����

// �÷��̾� �̵��ӵ� --------------------------------------------------------------------
// Action_SetMoveVelocity
// ------------------------------------------------------------------------------------------
#define ID_QUEST_MOVEVELOCITY_LOW							0
#define ID_QUEST_MOVEVELOCITY_MIDDLE						1
#define ID_QUEST_MOVEVELOCITY_USUAL							2

// �ൿ�� �ɼ� ���ǽ� ��� ����  --------------------------------------------------------------------
// Action_OptionConditionStart
// ------------------------------------------------------------------------------------------
#define ID_QUEST_OPTIONRESULT_CANCLESUCCESS					0
#define ID_QUEST_OPTIONRESULT_CANCLEFAIL					1

// ����Ʈ ��� ��� ���� --------------------------------------------------------------------
#define ID_QUEST_CANCELCASE_RESET							0
#define ID_QUEST_CANCELCASE_FAIL							1
#define ID_QUEST_CANCELCASE_NO								2

// ����Ʈ ���� ��ũ ���� --------------------------------------------------------------------
#define ID_QUEST_STARTMARK_AREA								0
#define ID_QUEST_STARTMARK_NPC								1

// ����Ʈ �ð� üũ ���� --------------------------------------------------------------------
#define ID_QUEST_CHECKTIME_NEEDLESS							0
#define ID_QUEST_CHECKTIME_NEED								1

// ǳ��� ���� ����  --------------------------------------------------------------------
#define ID_QUEST_QUESTEMAIL_NONE							0
#define ID_QUEST_QUESTEMAIL_OK								1
#define ID_QUEST_QUESTEMAIL_YESNO							2
#define ID_QUEST_QUESTEMAIL_OK_NOSTART						3
#define ID_QUEST_QUESTEMAIL_YESNO_NOSTART					4

// ���ÿ� ������ ������ ����Ʈ �� -----------------------------------------------------------
#define ID_QUEST_SYNC									6
#define ID_MAX_QUESTCOUNT								10   // ����Ʈ�� �ִ� ��
#define ID_MAX_CONDITIONCOUNT							16   // ���ǹ��� �ִ� ��
#define ID_MAX_TRIGGERCOUNT								32	 // Ʈ������ �ִ� ��

// ��ü ����Ʈ �� ---------------------------------------------------------------------------
#define ID_TOTAL_QUESTCOUNT								240

// Ʈ���� ���� ��Ŷ�� cResult ---------------------------------------------------------------
#define _QUEST_RESULT_SUCCESS_							0		// Ʈ���� ���� ����( �Ϲ�, �������� Ʈ���� )
#define _QUEST_FAILRESULT_SUCCESS_						1		// Ʈ���� ���� ����( ���� �׼� Ʈ���� ) ������ ���������� �׷��� ������ �����̴�.

#define _QUEST_CONDITION_RESULT_FAILED_					2		// Ʈ���� ���� ���� 
#define _QUEST_ACTION_RESULT_FAILED_					3		// Ʈ���� True �׼� ����
#define _QUEST_FAILACTION_RESULT_FAILED_				4		// Ʈ���� fail �׼� ����

// Ʈ���� ���� ������ ����� cInfo �� -------------------------------------------------------
#define _QUEST_INFO_ACTION_SUCCESS						0		// �������ο��� ����.

// Ʈ���� ���� �����ϰ�� cinfo�� -----------------------------------------------------------
#define _QUEST_INFO_IMPROPER_ID_						1		// �߸��� Ʈ���ž��̵� 1
#define _QUEST_INFO_PACKET_ERROR_						2		// �߸��� ������Ŷ	   1 	
#define _QUEST_INFO_SCRIPT_ERROR_						3		// Quest Script ����   1,2	
#define _QUEST_INFO_QUEST_SETERROR_						4		// Quest ���� ����	   2
#define _QUEST_INFO_INVEN_FULL_							5		// �κ��� ��ã��	   2	
#define _QUEST_INFO_UNKNOWN_ERROR_						6		// �˼����� ���� try-catch 1,2

// version ver 1.1
#define _QUEST_VERSION_MAJOR							1		
#define _QUEST_VERSION_MINOR							1

// ���� �ڵ�
#define _QUEST_COUNTRYCODE_KR							0x00000001
#define _QUEST_COUNTRYCODE_VN							0x00000002
#define _QUEST_COUNTRYCODE_US							0x00000004
#define _QUEST_COUNTRYCODE_TW							0x00000008
#define _QUEST_COUNTRYCODE_CN							0x00000010
#define _QUEST_COUNTRYCODE_JP							0x00000020
#define _QUEST_COUNTRYCODE_RS							0x00000040

#endif