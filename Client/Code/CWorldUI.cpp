#include"CWorldUI.h"



#include "Export_Utility.h"
#include "Export_System.h"

#include "Scene.h"


CWorldUI::CWorldUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name)
	: CUI(pGraphicDev, eUIState, _UI_Name)
{

}

CWorldUI::CWorldUI(const CWorldUI& rhs)
	: CUI(rhs)
{

}

CWorldUI::~CWorldUI()
{
}

_int CWorldUI::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);




	return 0;
}

void CWorldUI::Render_GameObject()
{

	scenemgr::Get_CurScene()->BeginOrtho();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_fImageCount);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pBufferCom->Render_Buffer();



	scenemgr::Get_CurScene()->EndOrtho();
}

CWorldUI* CWorldUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle)
{
	CWorldUI* pInstance = new CWorldUI(pGraphicDev, _State, _UI_Name);
	if (FAILED(pInstance->Ready_GameObject(_pos, _size, _Angle)))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CWorldUI::Free()
{
	CUI::Free();
}
