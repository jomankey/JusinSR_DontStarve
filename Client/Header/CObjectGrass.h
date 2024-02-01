#pragma once
#include "GameObject.h"

namespace Engine {
class CRcTex;
class CTexture;
}

class CObjectGrass :
	public CGameObject
{
private:
	explicit CObjectGrass(LPDIRECT3DDEVICE9 pGraphicDev );
	explicit CObjectGrass(const CObjectGrass& rhs);
	virtual ~CObjectGrass();
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
	static CObjectGrass* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};


