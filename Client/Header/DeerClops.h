#pragma once
#include "Base.h"
#include "GameObject.h"
#include "Monster.h"

namespace Engine 
{
	class CRcTex;
	class CRvRcTex;
	class CTexture;
	class CTransform;
	class CCalculator;
}

class CDeerClops :
	public CMonster
{
	enum DEERSTATE { IDLE, WALK, ATTACK, SLEEP,SLEEP_PST, TAUNT,LONG_TAUNT, HIT, DEAD, STATE_END };
	enum DEER_PHASE { FIRST, SECOND, THIRD, DIE, PHASE_END};
private:
	explicit CDeerClops(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos);
	explicit CDeerClops(const CDeerClops& rhs);
	virtual ~CDeerClops();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

public:
	void			Set_WakeUp();				//1페이즈 시작 트리거
	void			Set_Phase(_bool _first, _bool _second, _bool _third, _bool _fourth) {
		m_bPhase[FIRST] = _first;
		m_bPhase[SECOND] = _second;
		m_bPhase[THIRD] = _third;
		m_bPhase[DIE] = _fourth;
	}			//페이즈를 임의적으로 클래스 밖에서 조정해줄 수 있는 함수

private:
	virtual HRESULT			Add_Component();
	virtual void	Set_Hit()						override;
	virtual void	Set_ObjStat();
	virtual void	State_Change()				override;
	
	void			Sleep(const _float& fTimeDelta);		//첫 조우하기 전 자고있음
	void			First_Phase(const _float& fTimeDelta);		//페이즈 별 패턴
	void			Second_Phase(const _float& fTimeDelta);
	void			Third_Phase(const _float& fTimeDelta);
	void			Boss_Die(const _float& fTimeDelta);



public:
	static CDeerClops* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRvRcTex* m_pReverseCom;
	Engine::CTexture* m_pTextureCom[LOOKDIR::LOOK_END][DEERSTATE::STATE_END];
	Engine::CCalculator* m_pCalculatorCom;
private:
	virtual void Free() override;
private:
	DEERSTATE		m_eCurState;
	DEERSTATE		m_ePreState;
	_bool		m_bPhase[DEER_PHASE::PHASE_END];		//페이즈 변경용 bool 배열

	
};

