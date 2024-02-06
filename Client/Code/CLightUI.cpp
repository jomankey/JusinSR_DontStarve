#include"CLightUI.h"
#include "Export_Utility.h"
#include "Export_System.h"





CLightUI::CLightUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name, eITEMTOOL_TYPE _ItemToolType)
	:CToolUI(pGraphicDev, _UI_Name, _ItemToolType)

{
	m_eUIState = eUIState;
}

CLightUI::CLightUI(const CLightUI& rhs)
	: CToolUI(rhs)
{
}

CLightUI::~CLightUI()
{
}

_int CLightUI::Update_GameObject(const _float& fTimeDelta)
{

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CLightUI::LateUpdate_GameObject()
{
	if (UI_Collision() && Engine::Get_DIMouseState(DIM_LB) & 0x80)
	{
		Pop_SidePanel();

	}

}

CLightUI* CLightUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, eITEMTOOL_TYPE _ItemToolType)
{
	CLightUI* pInstance = new CLightUI(pGraphicDev, _State, _UI_Name, _ItemToolType);
	if (FAILED(pInstance->Ready_GameObject(_pos, _size)))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CLightUI::Free()
{
	//추후 오브젝트 생성, 삭제 기능으로 변경되면 해당 릴리즈 필요 없음
	this->m_pBufferCom->Release();
	this->m_pTextureCom->Release();
	this->m_pTransForm->Release();
	__super::Free();
}
