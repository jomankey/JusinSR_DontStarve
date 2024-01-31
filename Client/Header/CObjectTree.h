#pragma once
#include "GameObject.h"

namespace Engine {
class CRcTex;
class CTexture;
class CTransform;
}

class CObjectTree :
	public CGameObject
{
private:
	explicit CObjectTree(LPDIRECT3DDEVICE9 pGraphicDev );
	explicit CObjectTree(const CObjectTree& rhs);
	virtual ~CObjectTree();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						override;
private:
	HRESULT			Add_Component();
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CObjectTree* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};


