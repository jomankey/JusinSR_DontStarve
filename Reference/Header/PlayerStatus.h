#pragma once
#include "ObjStatus.h"

BEGIN(Engine)

class ENGINE_DLL CPlayerStatus :
    public CObjStatus
{
private:
    explicit CPlayerStatus();
    explicit CPlayerStatus(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CPlayerStatus(const CPlayerStatus& rhs);
    virtual ~CPlayerStatus();
public:
    PLAYERSTATE Get_Player_State() { return m_eState; }
    void Set_Player_State(PLAYERSTATE _State) { m_eState = _State; }
    

private:
    PLAYERSTATE m_eState;

};

END