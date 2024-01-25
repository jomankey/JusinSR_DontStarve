#pragma once
#include "GameObject.h"

#include "Engine_Define.h"

BEGIN(Engine)
class CTransform;
class CRcTex;
class CTexture;
END

class CToolTree :
    public CGameObject
{
private:
    explicit CToolTree(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CToolTree(const CToolTree& rhs);
    virtual ~CToolTree();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	_vec3 m_vPos;

public:
	static CToolTree* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

private:
	virtual void Free();
};

