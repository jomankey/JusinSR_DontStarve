#pragma once
#include "Base.h"
#include "Monster.h"

namespace Engine {

class CRcTex;
class CRvRcTex;
class CTexture;
class CTransform;
class CCalculator;

}
class CBeefalo : public CMonster
{
	enum BEEFALOSTATE { IDLE, GRAZE, WALK, RUN, MADRUN, SLEEP, DEAD, STATE_END };

private:
	explicit CBeefalo(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos);
	explicit CBeefalo(const CBeefalo& rhs);
	virtual ~CBeefalo();

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
	Engine::CTexture* m_pTextureCom[LOOKDIR::LOOK_END][BEEFALOSTATE::STATE_END];
	Engine::CCalculator* m_pCalculatorCom;

	_float				m_fFrame = 0.f;
	_float				m_fFrameEnd;
	BEEFALOSTATE		m_eCurState;
	BEEFALOSTATE		m_ePreState;
public:
	static CBeefalo* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);

private:
	virtual void Free() override;
};

