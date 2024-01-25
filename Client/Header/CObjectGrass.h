#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
END

class CObjectGrass :
	public CGameObject
{
private:
	explicit CObjectGrass(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
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
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	_vec3 m_vPos;

public:
	static CObjectGrass* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

private:
	virtual void Free();

};


