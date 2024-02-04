
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
    if (GetAsyncKeyState('K'))
    {
        m_bSlideOn = true;
    }

    if (GetAsyncKeyState('O'))
    {
        m_bSlideOn = false;
    }


    __super::Update_GameObject(fTimeDelta);
    if (m_bSlideOn)
    {
        m_bIsRender = false;
        if (m_fMaxSlide <= m_fX)
        {
            return 0;
        }
        m_fX += fTimeDelta* m_fSlideSpeed;
     
    }
    else
    {
        if(m_fMinSlide>= m_fX)
		{
            m_bIsRender = true;
			return 0;
		}
        m_fX -= fTimeDelta* m_fSlideSpeed;
      
    }
    
    

    return 0;
}

void CSlideUI::Render_GameObject()
{
    if (m_bIsRender)
      return;

    scenemgr::Get_CurScene()->BeginOrtho();
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pTextureCom->Set_Texture(0);

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
    //m_pTransForm->Set_Pos(m_fX - (WINCX >> 1), -m_fY + (WINCY >> 1), 0.f);
    //m_pTransForm->Set_Scale(_vec3{ m_fSizeX, m_fSizeY, 1.f });

    m_pBufferCom->Render_Buffer();



    scenemgr::Get_CurScene()->EndOrtho();


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
