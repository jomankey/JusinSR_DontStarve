#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CPlayerStatus :
    public CComponent
{
private:
    explicit CPlayerStatus();
    explicit CPlayerStatus(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CPlayerStatus(const CPlayerStatus& rhs);
    virtual ~CPlayerStatus();
public:
    HRESULT		Ready_PlayerStatus();
    virtual _int	Update_Component(const _float& fTimeDelta);
    virtual void	LateUpdate_Component();
public:

    static CPlayerStatus* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual CComponent* Clone(void);
    virtual void Free();
   /* PLAYERSTATE Get_Player_State() { return m_ePreState; }
    void Set_Player_State(PLAYERSTATE _State) { m_eCurState = _State; }
    void Set_Player_Look_Dir(PLAYERLOOK _DIR) { m_ePlayerLookAt = _DIR; }*/
    
//private:
//    void State_Change();
//
//private:
//    PLAYERSTATE m_eCurState;
//    PLAYERSTATE m_ePreState;
//
//    PLAYERLOOK  m_ePlayerLookAt;
//    _int        m_iFrameCnt;
};

END