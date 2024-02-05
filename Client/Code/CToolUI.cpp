#include "stdafx.h"


#include "CToolUI.h"

#include "Export_Utility.h"
#include "Export_System.h"


#include "Scene.h"
#include"CSlideUI.h"
#include"CAliveUI.h"
#include"CEquiment.h"
#include"CLightUI.h"
#include"CScienceUI.h"
#include"CWeaponUI.h"
#include"CClothUI.h"


CUI* CToolUI::m_pEquimentUI = nullptr;
CUI* CToolUI::m_pLightUI = nullptr;
CUI* CToolUI::m_pAliveUI = nullptr;
CUI* CToolUI::m_pScienceUI = nullptr;
CUI* CToolUI::m_pWeaponUI = nullptr;
CUI* CToolUI::m_pClothUI = nullptr;
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


void CToolUI::Pop_SidePanel()
{
        for (int i = 0; i < 5; i++)
        {
            _bool bUI = dynamic_cast<CSlideUI*>(m_pSlideUI[i])->GetSlideOn();
            dynamic_cast<CSlideUI*>(m_pSlideUI[i])->SetSlideOnAndOff(!bUI);
        }

    
 
}

CToolUI* CToolUI::Create(LPDIRECT3DDEVICE9	pGraphicDev, const _tchar* _UI_Name)
{
    CToolUI* pInstance = new CToolUI(pGraphicDev, _UI_Name);

    if (m_pWeaponUI == nullptr)
    {
        m_pWeaponUI = CWeaponUI::Create(pGraphicDev, UI_STATE::UI_STATIC, _vec3(20.f, 130.f, 0.f), _vec3(20.f, 20.f, 0.f), L"Proto_UI_Weapon");
        CreateObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, m_pWeaponUI);
    }
    if (m_pLightUI == nullptr)
    {

        //상단에 헤더파일 없어서 오류 뜸/ 문제 없음
        m_pLightUI = CLightUI::Create(pGraphicDev, UI_STATE::UI_STATIC, _vec3(20.f, 170.f, 0.f), _vec3(20.f, 20.f, 0.f), L"Proto_UI_Light");
        CreateObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, m_pLightUI);
    }
    if (m_pAliveUI == nullptr)
    {
        //상단에 헤더파일 없어서 오류 뜸/ 문제 없음
        m_pAliveUI = CAliveUI::Create(pGraphicDev, UI_STATE::UI_STATIC, _vec3(20.f, 210.f, 0.f), _vec3(20.f, 20.f, 0.f), L"Proto_UI_Alive");
        CreateObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, m_pAliveUI);
    }
    if (m_pScienceUI == nullptr)
    {
        m_pScienceUI = CScienceUI::Create(pGraphicDev, UI_STATE::UI_STATIC, _vec3(20.f, 270.f, 0.f), _vec3(20.f, 20.f, 0.f), L"Proto_UI_Science");
        CreateObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, m_pScienceUI);

    }

    if (m_pEquimentUI == nullptr)
    {

        //상단에 헤더파일 없어서 오류 뜸/ 문제 없음
        m_pEquimentUI = CEquiment::Create(pGraphicDev, UI_STATE::UI_STATIC, _vec3(20.f, 310.f, 0.f), _vec3(20.f, 20.f, 0.f), L"Proto_UI_Equipment");
        CreateObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, m_pEquimentUI);
    }
    


    
    
  


    if (m_pClothUI == nullptr)
    {
        m_pClothUI = CClothUI::Create(pGraphicDev, UI_STATE::UI_STATIC, _vec3(20.f, 470.f, 0.f), _vec3(20.f, 20.f, 0.f), L"Proto_UI_Cloth");
        CreateObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, m_pClothUI);
    }



    for (int i = 0; i < 5; i++)
    {
        m_pSlideUI[i]= CSlideUI::Create(pGraphicDev, UI_STATE::UI_STATIC, _vec3(20.f, 400.f - i * 60.f, 0.f), _vec3(30.f, 30.f, 0.f), L"Proto_UI_Item_Panel");
    	CreateObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, m_pSlideUI[i]);
    
    }





    
    return pInstance;
}

void CToolUI::Free()
{
    CToolUI::~CToolUI();
}

