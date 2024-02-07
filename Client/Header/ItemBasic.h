#pragma once
#include "CItem.h"
class CItemBasic :
    public CItem
{
private:
    explicit CItemBasic(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CItemBasic(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);
    explicit CItemBasic(const CItem& rhs);
    virtual ~CItemBasic();

public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float& fTimeDelta) override;
    void LateUpdate_GameObject() override;
    void Render_GameObject() override;
    
public:
    static CItem* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _szName);

private:
    virtual void Free();
};

