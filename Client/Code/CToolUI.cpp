

#include "CToolUI.h"

#include "Export_Utility.h"
#include "Export_System.h"

CToolUI::CToolUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name)
    :CUI(pGraphicDev, eUIState, _UI_Name)
    , m_pToolData{ nullptr }
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
    if (UI_Collision())
    {
        const vector<CGameObject*>& vecUi = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI);

        for (size_t i = 0; i < vecUi.size(); i++)
        {
            // vecUi[i]->GetObjName() == 
        }
    }

}

void CToolUI::Free()
{
    __super::Free();
}

