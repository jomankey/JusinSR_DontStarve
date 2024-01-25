#pragma once
#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
END
class CToolGrass :
    public CGameObject
{
private:
    explicit CToolGrass(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
    explicit CToolGrass(const CToolGrass& rhs);
    virtual ~CToolGrass();

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
    static CToolGrass* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

private:
    virtual void Free();
};

