#pragma once
#include "Camera.h"

class CToolCamera :
    public CCamera
{
private:
    explicit CToolCamera(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CToolCamera();

public:
    HRESULT Ready_GameObject(const _vec3* pEye,
        const _vec3* pAt,
        const _vec3* pUp,
        const _float& fFov,
        const _float& fAspect,
        const _float& fNear,
        const _float& fFar);

    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject() override;

private:
    void Key_Input(const _float& fTimeDelta);
    void Mouse_Fix();
    void		Mouse_Move();

private:
    _bool		m_bFix = false;
    _bool		m_bCheck = false;

public:
    static CToolCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
        const _vec3* pEye,
        const _vec3* pAt,
        const _vec3* pUp,
        const _float& fFov,
        const _float& fAspect,
        const _float& fNear,
        const _float& fFar);

private:
    virtual void Free();
};

