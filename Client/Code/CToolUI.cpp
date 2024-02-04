#include "stdafx.h"


#include "CToolUI.h"

#include "Export_Utility.h"
#include "Export_System.h"


#include "Scene.h"
#include"CSlideUI.h"
//#include"CAliveUI.h"

CUI* CToolUI::m_pAliveUI = nullptr;
CUI* CToolUI::m_pEquimentUI = nullptr;
CUI* CToolUI::m_pSlideUI[5] = {};

CToolUI::CToolUI(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _UI_Name)
    :CUI(pGraphicDev, UI_NONE, _UI_Name)
    , m_pToolData{ nullptr }
    , m_bSlideState(false)
{
}

CToolUI::CToolUI(const CToolUI& rhs)
    : CUI(rhs)
{
}

CToolUI::~CToolUI()
{
}


_int CToolUI::Update_GameObject(const _float& fTimeDelta)
{
    __super::Update_GameObject(fTimeDelta);

    return 0;
}


void CToolUI::SetGameData(CItemTool* _ItemTool, eITEMTOOL_TYPE _ItemType)
{
    if (nullptr != _ItemTool)
    {
        m_pToolData[_ItemType] = _ItemTool;
    }

}


void CToolUI::Pop_SidePanel(bool _state)
{
        for (int i = 0; i < 5; i++)
        {
            dynamic_cast<CSlideUI*>(m_pSlideUI[i])->SetSlideOnAndOff(_state);
        }

    
  

}

CToolUI* CToolUI::Create(LPDIRECT3DDEVICE9	pGraphicDev, const _tchar* _UI_Name)
{
    CToolUI* pInstance = new CToolUI(pGraphicDev, _UI_Name);

    if (m_pAliveUI == nullptr)
    {
        //상단에 헤더파일 없어서 오류 뜸/ 문제 없음
        m_pAliveUI = CAliveUI::Create(pGraphicDev, UI_STATE::UI_STATIC, _vec3(20.f, 140.f, 0.f), _vec3(20.f, 20.f, 0.f), L"Proto_UI_Alive");
        CreateObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, m_pAliveUI);
    }
    if (m_pEquimentUI == nullptr)
    {

        //상단에 헤더파일 없어서 오류 뜸/ 문제 없음
        m_pEquimentUI = CEquiment::Create(pGraphicDev, UI_STATE::UI_STATIC, _vec3(20.f, 100.f, 0.f), _vec3(20.f, 20.f, 0.f), L"Proto_UI_Equipment");
        CreateObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, m_pEquimentUI);
    }

    for (int i = 0; i < 5; i++)
    {
        m_pSlideUI[i]= CSlideUI::Create(pGraphicDev, UI_STATE::UI_STATIC, _vec3(20.f, 300.f - i * 50.f, 0.f), _vec3(40.f, 40.f, 0.f), L"Proto_UI_Item_Panel");
    	CreateObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, m_pSlideUI[i]);
    
    }




    return pInstance;
}

void CToolUI::Free()
{
    CToolUI::~CToolUI();
}

