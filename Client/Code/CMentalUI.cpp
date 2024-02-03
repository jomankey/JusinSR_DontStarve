#include"CMentalUI.h"



#include "Export_Utility.h"
#include "Export_System.h"

#include "Scene.h"


CMentalUI::CMentalUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name)
	: CUI(pGraphicDev, eUIState, _UI_Name)
{


}

CMentalUI::CMentalUI(const CMentalUI& rhs)
	: CUI(rhs)
{

}

CMentalUI::~CMentalUI()
{
}

_int CMentalUI::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);




	return 0;
}

void CMentalUI::Render_GameObject()
{
	scenemgr::Get_CurScene()->BeginOrtho();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_fImageCount);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pBufferCom->Render_Buffer();



	scenemgr::Get_CurScene()->EndOrtho();



}

CMentalUI* CMentalUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle)
{
	CMentalUI* pInstance = new CMentalUI(pGraphicDev, _State, _UI_Name);
	if (FAILED(pInstance->Ready_GameObject(_pos, _size, _Angle)))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMentalUI::Free()
{
	CUI::Free();
}
