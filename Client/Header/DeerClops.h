#pragma once
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CRvRcTex;
class CTexture;
class CTransform;
class CCalculator;

END

class CDeerClops :
    public Engine::CGameObject
{

	enum DEERSTATE { IDLE, WALK, ATTACK, SLEEP, TAUNT,HIT, DEAD, STATE_END };
private:
	explicit CDeerClops(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CDeerClops(const CDeerClops& rhs);
	virtual ~CDeerClops();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	virtual HRESULT			Add_Component();
	void			BillBoard();
	void			Set_ObjStat();
	void			Check_State();
	void			Mode_Change();
	void			Patroll(const _float& fTimeDelta);
	void			Fight(const _float& fTimeDelta);
	void			Chase_Player(const _float& fTimeDelta);
	CTransform*   PlayerComponent();
	void		Look_Change();

public:
	virtual void Free() override;
	static CDeerClops* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vPos);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRvRcTex* m_pReverseCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom[LOOKDIR::LOOK_END][DEERSTATE::STATE_END];
	Engine::CCalculator* m_pCalculatorCom;

private:
	_vec3 m_vPos;
	LOOKDIR m_eCurLook;
	LOOKDIR m_ePreLook;
	DEERSTATE		m_eCurState;
	DEERSTATE		m_ePreState;
	_float				m_fFrame = 0.f;
	_float				m_fFrameEnd;
	OBJSTAT m_Stat;
	_bool	m_Dirchange;			//false 일때 오른쪽 보기
	_bool	m_bMode;
	_float	m_fAcctime;
	_float	m_fFrameChange = 0;
};

