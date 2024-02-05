#include "CAliveUI.h"

#include "Export_Utility.h"
#include "Export_System.h"





CAliveUI::CAliveUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name)
    :CToolUI(pGraphicDev, _UI_Name)

{
    m_eUIState= eUIState;
}

CAliveUI::CAliveUI(const CAliveUI& rhs)
    : CToolUI(rhs)
{
}

CAliveUI::~CAliveUI()
{
}

_int CAliveUI::Update_GameObject(const _float& fTimeDelta)
{
    __super::Update_GameObject(fTimeDelta);
    if (UI_Collision() && Engine::Get_DIMouseState(DIM_LB) & 0x80)
    {
        Pop_SidePanel(true);

    }

    return 0;
}

CAliveUI* CAliveUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle)
{
    CAliveUI* pInstance = new CAliveUI(pGraphicDev, _State, _UI_Name);
    if (FAILED(pInstance->Ready_GameObject(_pos, _size, _Angle)))
    {
        Safe_Release(pInstance);
        MSG_BOX("UI Create Failed");
        return nullptr;
    }

    return pInstance;
}
void CAliveUI::Free()
{

    //추후 오브젝트 생성, 삭제 기능으로 변경되면 해당 릴리즈 필요 없음
    this->m_pBufferCom->Release();
    this->m_pTextureCom->Release();
    this->m_pTransForm->Release();
    __super::Free();
}
