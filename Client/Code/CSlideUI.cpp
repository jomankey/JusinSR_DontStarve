
#include "..\Include\stdafx.h"
#include"CSlideUI.h"


#include "Export_Utility.h"
#include "Export_System.h"





CSlideUI::CSlideUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name)
    :CUI(pGraphicDev, eUIState, _UI_Name)
    , m_qItemListQueue()
 
{
}

CSlideUI::CSlideUI(const CSlideUI& rhs)
    : CUI(rhs)
{
}

CSlideUI::~CSlideUI()
{
}

_int CSlideUI::Update_GameObject(const _float& fTimeDelta)
{

    __super::Update_GameObject(fTimeDelta);






    return 0;
}

void CSlideUI::Render_GameObject()
{

  //if (UI_Collision())
  //{
  //
  //
  //
  //}


}

BOOL CSlideUI::UI_Collision(CUI* _TargetUI)
{
    GetCursorPos(&m_MousePoint);
    ScreenToClient(g_hWnd, &m_MousePoint);

    if (nullptr != _TargetUI) {
        if (_TargetUI->GetX() - (_TargetUI->GetSizeX()) < m_MousePoint.x && m_MousePoint.x < _TargetUI->GetX() + (_TargetUI->GetSizeX()))
            if (_TargetUI->GetY() - (_TargetUI->GetSizeY()) < m_MousePoint.y && m_MousePoint.y < _TargetUI->GetY() + (_TargetUI->GetSizeY()))
            {
                return true;
            }
            else
                return false;
        else
            return false;
    }
}

CSlideUI* CSlideUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle)
{
    CSlideUI* pInstance = new CSlideUI(pGraphicDev, _State, _UI_Name);
    if (FAILED(pInstance->Ready_GameObject(_pos, _size, _Angle)))
    {
        Safe_Release(pInstance);
        MSG_BOX("UI Create Failed");
        return nullptr;
    }

    return pInstance;
}
void CSlideUI::Free()
{
    __super::Free();
}
