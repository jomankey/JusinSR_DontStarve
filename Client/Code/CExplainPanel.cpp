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
   // if (m_bShow)
   // {
   //     m_fX = m_MousePoint.x + 50.f;
   //     m_fY= m_MousePoint.y;
   //
   // }




    return 0;
}

void CExplainPanel::Render_GameObject()
{
    if (!m_bShow)
		return;
    m_pToolItemName= L"TestItemName";
    m_pExplainName = L"TestExplainName";
    Engine::Render_Font(L"Font_Count", m_pToolItemName, &_vec2(m_fX-40, m_fY-40.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    Engine::Render_Font(L"Font_Count", m_pExplainName, &_vec2(m_fX-40, m_fY), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

    __super::Render_GameObject();



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
