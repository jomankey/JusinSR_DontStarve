#pragma once
#include "Base.h"
#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CRvRcTex;
class CTexture;
class CTransform;
class CCalculator;

END
class CPig : public CMonster
{
	enum PiGSTATE { IDLE,ANGRY_IDLE, WALK, RUN,ATTACK, SLEEP,EAT, DEAD, STATE_END };

private:
	explicit CPig(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos);
	explicit CPig(const CPig& rhs);
	virtual ~CPig();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();
	void			Height_OnTerrain();
	void			BillBoard();
	virtual void	Set_ObjStat()					override;
	void			Player_Chase(const _float& fTimeDelta);
	void			State_Change();
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRvRcTex* m_pReverseCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom[LOOKDIR::LOOK_END][PiGSTATE::STATE_END];
	Engine::CCalculator* m_pCalculatorCom;

	_float				m_fFrame = 0.f;
	_float				m_fFrameEnd;
	PiGSTATE		m_eCurState;
	PiGSTATE		m_ePreState;
public:
	static CPig* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);

private:
	virtual void Free() override;
};

