#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
END

class CObjectTree :
	public CGameObject
{
private:
	explicit CObjectTree(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CObjectTree(const CObjectTree& rhs);
	virtual ~CObjectTree();
public:
	void Billboard();
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
	static CObjectTree* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

private:
	virtual void Free();

};


