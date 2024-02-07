#pragma once

#include "Base.h"
#include "GameObject.h"
#include "ResObject.h"

namespace Engine
{
	class CRcTex;
	class CRvRcTex;
	class CTexture;
	class CTransform;
	class CCalculator;
	class CPlayerStatus;
}

class CPlayer : public Engine::CGameObject
{
	
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strName);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;
	void			Set_Attack(int _Atk);			//공격당했을 때 호출되는 함수
private:
	HRESULT			Add_Component();
	void			Key_Input(const _float& fTimeDelta);
	void			Ket_Input_Road(const _float& fTimeDelta);
	HRESULT			SetUp_Material();
	void			Check_State();				//상태 변경용 함수
	void			Set_Scale();				//행동에 따른 스케일을 변경하기 위한 함수
	void			Set_Stat();					
	void			Weapon_Change();			//무기 변경에 따른 변화에 대한 함수
	void			Look_Change();				//바라보는 방향을 바꾸기 위한 함수
	void			ResObj_Mining(RESOBJID _ObjID , CGameObject* _Obj);	//Res오브젝트들을 채굴할 때 사용하는 함수


	HRESULT Ready_Light();	//조명
	void Fire_Light();
	

	CGameObject* Find_NeerObject(float _fRange, eOBJECT_GROUPTYPE _findTarget);

	void BillBoard();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRvRcTex* m_pReverseCom;
	Engine::CTexture* m_pTextureCom[LOOKDIR::LOOK_END][PLAYERSTATE::STATE_END];
	Engine::CCalculator* m_pCalculatorCom;

	_float				m_fFrame = 0.f;
	_float				m_fFrameEnd;

	PLAYERSTATE			m_eCurState;
	PLAYERSTATE			m_ePreState;

	WEAPON				m_eCurWeapon;			//현재 착용하고 있는 무기
	WEAPON				m_ePreWeapon;			// 추후에 무기 스위칭할때 메소드를 위해 생성해둠

	LOOKDIR				m_eCurLook;
	LOOKDIR				m_ePreLook;
	_bool				m_Dirchange;			//Look
	_bool				m_bAttack;
	OBJSTAT				m_Stat;
	_bool				m_KeyLock;				//Key Input 함수를 막는 용도	true 면 막힘
	_bool				m_bFrameLock;			//프레임 축적을 막는 용도 true 면 막힘


	_vec3				m_vTargetPos;			//오브젝트 대상의 위치벡터
	_vec3				m_vTargetDir;			//이동하는 방향
	_bool				m_vPlayerActing;		//오브젝트와 상호작용 할 때 true

	RESOBJID			m_TargetObject;			//Res오브젝트의 종류를 판별
	_bool				m_bIsRoadScene;			//Road 씬이면 true (임시)

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9	pGraphicDev, wstring _strName);
	static CPlayer* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;
};

