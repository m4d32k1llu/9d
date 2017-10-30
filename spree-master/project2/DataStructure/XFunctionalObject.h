// XFunctionalObject.h: interface for the _XFunctionalObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFUNCTIONALOBJECT_H__5DBA77A5_F034_4904_B449_346EF38485D9__INCLUDED_)
#define AFX_XFUNCTIONALOBJECT_H__5DBA77A5_F034_4904_B449_346EF38485D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XObjectManager.h"


class _XFunctionalObject : public _XObject, public _XItem
{
public :
	D3DXVECTOR3				m_Position;
		
	LPTSTR	m_ObjectName;
	int		m_UniqueID;
	int		m_ObjectType;			//Ÿ������ ( ���� ������ ����)
 	int		m_ObjectClass;			//���� ( 0:�� 1:�������Ա� 2:TRAP_LPS 3:�������� )
	int		m_ActionIndex;			//���� �׼��� ���� ( ���Ϳ� ����) ���, ����, ���� ...
	
	BOOL	m_bWideViewMode;
	BOOL	m_bReLocateMode;
	
	float	m_CameraDistance;		//ī�޶� �Ÿ� 
	float	m_CameraTargetHeight;	//ī�޶� ��� ���� 
	float	m_CameraPitch;			//ī�޶� ���� 
	float	m_CameraYaw;			//ī�޶� �¿� 
	
	LPTSTR	m_SystemMessage;		//�ý��� �޼���
	LPTSTR	m_Description;			//������ ���� 
	
	_XList	m_listFunctionalObjectTrigger;
	
	BOOL    m_InsideFrustum;
	
	_XModelDescriptor	m_ModelDescriptor;
	
	int		m_AttackalbeTypeUniqueID;	// ���ݰ����� ���� �϶� �������� �Ҵ��� �ִ� ���̵� 
	char	m_HealthPercentage;			// ���ݰ����� �����϶� ü��
	char	m_DisplayHealthPercentage;
	BOOL	m_bAttackable;				// ���ݰ����� �������� �ƴ����� �� �� �ִ� ����
	
	int		m_AttackInfo;				//TRAP LPS�� ��쿡 ���������� ������.
	int		m_AttackTargetObjectType;	// 0:none 1:pc 2:self 3:mob
	_XItem*					m_AttackTarget;
	FLOAT					m_AttackHitValue;
		
	unsigned char  m_DefenderType;
	unsigned short m_DefenderID;
	D3DXMATRIX	   m_matTemp;
	
	DWORD	m_DieStartTime;
	BOOL	m_bDie;

public:
	_XFunctionalObject();
	virtual ~_XFunctionalObject();
	
	BOOL	LoadScript( LPSTR Filename );
	BOOL	LoadScript( FILE* FilePtr );
	BOOL	Load( FILE* fileptr );
	
	void	ProcessAction(void);			// Functional Object�� ���ǽ� �ൿ���� üũ�ϰ� �����ϴ� ���μ��� 
	void	ProcessAnimation(void);			// Functional Object�� Animation�� ó���ϴ� ���μ��� 
	void	CreateEffectManager( void );
	void	SetAnimation( void );
	void	SetIdleAnimation(void);
	void	SetBlowMotion(void);
	void	SetDieAnimation(void);
	void	SetAnimationIndex( int animationindex );
	BOOL	SetAttackableFunctionalObject(void);	// Functional Object�� ���ݰ������� �ƴ����� �����ϴ� �Լ� 
	
	void	ProcessMove();				// Fuctional object���� player move �κ� ó��

};


class _XFunctionalObjectManager
{
public:
	int						m_countFunctionalObject;
	_XFunctionalObject*		m_pFunctionalObjectArray;	
	_XOBBData*				m_pFunctionalOBBDataArray;
	
public:
	_XFunctionalObjectManager();
	virtual ~_XFunctionalObjectManager();
	
	void			disposeList( void  );	
	BOOL			LoadFunctionalObjectList( FILE* Fileptr, LPSTR Filename = NULL );

	void			CheckInsideFrustum( void );
	void			Render( void );
	void			Process( void );
	void			SetAttackableFunctionalObjectID(int objectid, int objecttype, int healthpercentage, D3DXVECTOR3& vObjectPosition );
	
	_XFunctionalObject*	FindAttackableFunctionalObject( unsigned short objectuniqueid	);
};

#endif // !defined(AFX_XFUNCTIONALOBJECT_H__5DBA77A5_F034_4904_B449_346EF38485D9__INCLUDED_)
