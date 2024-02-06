#pragma once
#include "Base.h"
#include "GameObject.h"

namespace Engine 
{
	class CRcTex;
	class CRvRcTex;
	class CTexture;
	class CTransform;
	class CCalculator;
}

class CDeerClops :
	public Engine::CGameObject
{
	enum DEERSTATE { IDLE, WALK, ATTACK, SLEEP,SLEEP_PST, TAUNT, HIT, DEAD, STATE_END };
	enum DEER_PHASE { FIRST, SECOND, THIRD, DIE, PHASE_END};
private:
	explicit CDeerClops(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CDeerClops(const CDeerClops& rhs);
	virtual ~CDeerClops();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

public:
	void			Set_WakeUp();				//1페이즈 시작


private:
	virtual HRESULT			Add_Component();

	void			Set_ObjStat();
	void			Check_State();
	_bool		IsTarget_Approach(float _fDistance);
	void			Sleep(const _float& fTimeDelta);
	void			First_Phase(const _float& fTimeDelta);
	void			Second_Phase(const _float& fTimeDelta);
	void			Chase_Player(const _float& fTimeDelta);
	CTransform*		PlayerComponent();
	_vec3			Player_Position();
	CGameObject*	Player_Pointer();
	void			Look_Change();

public:
	virtual void Free() override;
	static CDeerClops* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRvRcTex* m_pReverseCom;
	Engine::CTexture* m_pTextureCom[LOOKDIR::LOOK_END][DEERSTATE::STATE_END];
	Engine::CCalculator* m_pCalculatorCom;

private:
	_vec3				m_vPos;
	LOOKDIR m_eCurLook;
	LOOKDIR m_ePreLook;

	DEERSTATE		m_eCurState;
	DEERSTATE		m_ePreState;
	
	OBJSTAT		m_Stat;
	_bool		m_Dirchange;			//false 일때 오른쪽 보기
	_bool		m_bPhase[DEER_PHASE::PHASE_END];		//페이즈 변경용 bool 배열
	_bool		m_bAttacking;			//공격시
	//Frame
	_float				m_fFrame = 0.f;
	_float				m_fFrameEnd;
	_float	m_fAcctime;
	_float	m_fFrameChange = 0;
	_bool	m_bFrameStop;
};

