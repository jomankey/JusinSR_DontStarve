#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CCalculator;
class CPlayerStatus;

END

class CPlayer :	public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();
	void			Key_Input(const _float& fTimeDelta);
	void			Height_OnTerrain();
	_vec3			Picking_OnTerrain();
	void			BillBoard();
	void			Check_State();
	void			Adjust_IDLE();
	void			Change_Texture();
private:
	Engine::CRcTex*		m_pBufferCom;
	Engine::CTransform*	m_pTransformCom;
	Engine::CTexture*	m_pTextureCom/*[PLAYERSTATE::STATE_END]*/;
	Engine::CCalculator*	m_pCalculatorCom;
	Engine::CPlayerStatus*  m_pStatusCom;
	_float				m_fFrame = 0.f;
	_float				m_fFrameEnd;

	PLAYERSTATE m_eCurState;
	PLAYERSTATE m_ePreState;

	PLAYERLOOK  m_ePlayerLookAt;
	_tchar*		m_cTex;
public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;
};

