#include "CEquiment.h"

#include "Export_Utility.h"
#include "Export_System.h"





CEquiment::CEquiment(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name)
    :CUI(pGraphicDev, eUIState, _UI_Name)

{
}

CEquiment::CEquiment(const CEquiment& rhs)
    : CUI(rhs)
{
}

CEquiment::~CEquiment()
{
}

_int CEquiment::Update_GameObject(const _float& fTimeDelta)
{

    __super::Update_GameObject(fTimeDelta);


    Pop_SidePanel();

    return 0;
}

void CEquiment::Pop_SidePanel()
{
    if (UI_Collision())
    {
        const vector<CGameObject*>&  vecUi= scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI);

        for (size_t i = 0; i < vecUi.size(); i++)
        {
           // vecUi[i]->GetObjName() == 
        }
    }

}




CEquiment* CEquiment::Create(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle)
{
    CEquiment* pInstance = new CEquiment(pGraphicDev, _State, _UI_Name);
    if (FAILED(pInstance->Ready_GameObject(_pos, _size, _Angle)))
    {
        Safe_Release(pInstance);
        MSG_BOX("UI Create Failed");
        return nullptr;
    }

    return pInstance;
}
void CEquiment::Free()
{
    __super::Free();
}
