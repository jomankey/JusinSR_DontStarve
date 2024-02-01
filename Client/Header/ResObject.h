#pragma once
#include "Export_Utility.h"

class CResObject :
    public CGameObject
{
protected:
    enum RESOBJECTSTATE { RES_IDLE, RES_HIT_1, RES_HIT_2, RES_DEAD, RESOBJECT_END };

protected:
    explicit CResObject(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CResObject(const CResObject& rhs);
    virtual ~CResObject();

public:
    void Set_Attack() { if (!m_bHit) m_Stat.fHP -= 1.f; } // 오브젝트는 -1씩 감소
    void Set_Attack_State(_bool _bAttack) { m_bHit = _bAttack; } // 플레이어 어택 시작 시 true로 변경해주고 플레이어 어택 프레임이 끝날 시 false로 변경해준다.

    _bool Get_AttackState() { return m_bHit; }

public:
    virtual HRESULT Ready_GameObject()						 override;
    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject()					 override;
    virtual void Render_GameObject()					override;

private:
    virtual HRESULT			Add_Component() PURE;
    virtual void			Check_FrameState() PURE;
    virtual void Ready_Stat() PURE;

protected:
    Engine::CRcTex* m_pBufferCom;
    Engine::CTexture* m_pTextureCom[RESOBJECT_END];

    RESOBJECTSTATE m_eCurState;
    RESOBJECTSTATE m_ePreState;

    _float				m_fFrame = 0.f;
    _float				m_fFrameEnd;
    _bool m_bHit;

    OBJSTAT m_Stat;

private:
    virtual void Free();
};