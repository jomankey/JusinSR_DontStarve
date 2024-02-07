#pragma once
#include "CUI.h"
#include "CInvenBox.h"
#include "Engine_Macro.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
}
class CInven : public CGameObject
{
private:
	explicit CInven(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInven();

public:
	virtual HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component();

public:
	static CInven* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

private:
	Engine::CTexture* m_pTextureCom;
	Engine::CRcTex* m_pBufferCom;

	_matrix				m_ViewMatrix, m_ProjMatrix;
	_float m_fX, m_fY, m_fSizeX, m_fSizeY;
};
