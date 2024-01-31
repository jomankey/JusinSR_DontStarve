#pragma once
#include "GameObject.h"
#include "Engine_Define.h"

namespace Engine
{
	class CComponent;
	class CRcTex;
	class CTexture;
}
class CObjectRock :
	public Engine::CGameObject
{
private:
	explicit CObjectRock(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CObjectRock(const CObjectRock& rhs);
	virtual ~CObjectRock();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CObjectRock* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

