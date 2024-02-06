#include "stdafx.h"
#include "CSlideBox.h"


#include "Export_Utility.h"
#include "Export_System.h"


#include "CInventoryMgr.h"
#include "CItem.h"
#include "Texture.h"
#include "Scene.h"
CSlideBox::CSlideBox(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name)
	:CUI(pGraphicDev, eUIState, _UI_Name)
	, ItemName(_UI_Name)

{
}

CSlideBox::CSlideBox(const CSlideBox& rhs)
	:CUI(rhs)
	, ItemName(rhs.ItemName)
{

}

CSlideBox::~CSlideBox()
{
}



_int CSlideBox::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);


	if (nullptr != ItemName)
	{
		m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(ItemName));
		NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ ItemName, m_pTextureCom });
	
	}




	return 0;
}

void CSlideBox::Render_GameObject()
{
	if (m_pTextureCom == nullptr)
		return;

	if(!m_bIsShow)
		return;

	__super::Render_GameObject();
}

CSlideBox* CSlideBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle)
{
	CSlideBox* pInstance = new CSlideBox(pGraphicDev, _State, _UI_Name);
	if (FAILED(pInstance->Ready_GameObject(_pos, _size, _Angle)))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CSlideBox::Add_Component()
{
	CComponent* pComponent = nullptr;

	//VIBUFFER
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}


void CSlideBox::Free()
{
	__super::Free();
}
