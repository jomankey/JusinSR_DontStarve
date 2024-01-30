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
class CSpider : public CMonster
{
	enum SPIDERSTATE { IDLE, WALK,ATTACK, SLEEP, DEAD, STATE_END };

private:
	explicit CSpider(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vPos);
	explicit CSpider(const CSpider& rhs);
	virtual ~CSpider();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();
	void			Height_OnTerrain();
	void			BillBoard();
	void			Player_Chase(const _float& fTimeDelta); //플레이어 추격용 테스트 함수

	virtual void			Set_ObjStat()					override;
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRvRcTex* m_pReverseCom;
	Engine::CTransform* m_pTransForm;
	Engine::CTexture* m_pTextureCom[LOOKDIR::LOOK_END];
	Engine::CCalculator* m_pCalculatorCom;

	_float				m_fFrame = 0.f;
	_float				m_fFrameEnd;
	_float				m_fAcctime;

	
public:
	static CSpider* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);

private:
	virtual void Free() override;
};
