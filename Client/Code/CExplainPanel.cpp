#include "..\Include\stdafx.h"

#include "CExplainPanel.h"

#include "Export_Utility.h"
#include "Export_System.h"





CExplainPanel::CExplainPanel(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name)
    :CUI(pGraphicDev, eUIState, _UI_Name)

{
}

CExplainPanel::CExplainPanel(const CExplainPanel& rhs)
    : CUI(rhs)
{
}

CExplainPanel::~CExplainPanel()
{
}

_int CExplainPanel::Update_GameObject(const _float& fTimeDelta)
{

    __super::Update_GameObject(fTimeDelta);



    return 0;
}

CExplainPanel* CExplainPanel::Create(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle)
{
    CExplainPanel* pInstance = new CExplainPanel(pGraphicDev, _State, _UI_Name);
    if (FAILED(pInstance->Ready_GameObject(_pos, _size, _Angle)))
    {
        Safe_Release(pInstance);
        MSG_BOX("UI Create Failed");
        return nullptr;
    }

    return pInstance;
}
void CExplainPanel::Free()
{
    __super::Free();
}
