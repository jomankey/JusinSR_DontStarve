#include "CEquiment.h"

#include "Export_Utility.h"
#include "Export_System.h"





CEquiment::CEquiment(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name, eITEMTOOL_TYPE _ItemToolType)
	:CToolUI(pGraphicDev, _UI_Name, _ItemToolType)

{
	m_eUIState = eUIState;
}

CEquiment::CEquiment(const CEquiment& rhs)
	: CToolUI(rhs)
{
}

CEquiment::~CEquiment()
{
}

void CEquiment::LateUpdate_GameObject()
{
	if (UI_Collision() && Engine::Get_DIMouseState(DIM_LB) & 0x80)
	{
		Pop_SidePanel();

		//Pop_SidePanel(false);

	}
}


CEquiment* CEquiment::Create(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, eITEMTOOL_TYPE _ItemToolType)
{
	CEquiment* pInstance = new CEquiment(pGraphicDev, _State, _UI_Name, _ItemToolType);
	if (FAILED(pInstance->Ready_GameObject(_pos, _size)))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CEquiment::Free()
{
	//추후 오브젝트 생성, 삭제 기능으로 변경되면 해당 릴리즈 필요 없음
	this->m_pBufferCom->Release();
	this->m_pTextureCom->Release();
	this->m_pTransForm->Release();
	__super::Free();
}
