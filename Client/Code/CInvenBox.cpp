#include "CInvenBox.h"

#include "CInventoryMgr.h"
#include "CItem.h"
#include "Texture.h"

CInvenBox::CInvenBox(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name, _uint _index)
	:CUI(pGraphicDev, eUIState, _UI_Name)
	, m_iInvenIndex(_index)
{
}

CInvenBox::CInvenBox(const CInvenBox& rhs)
	:CUI(rhs)
{
}

CInvenBox::~CInvenBox()
{
}



_int CInvenBox::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	CItem* pItemBox = nullptr;
	pItemBox = CInventoryMgr::GetInstance()->GetItemBox(m_iInvenIndex);

	if (nullptr != pItemBox)
	{
		IDirect3DBaseTexture9* pTexture = dynamic_cast<CTexture*>(pItemBox->Find_Component(COMPONENTID::ID_STATIC, pItemBox->GetObjName().c_str()))->Get_Texture(0);

		m_pGraphicDev->SetTexture(1, pTexture);
		pItemBox->GetItemInfo().ItemCount;
	}

	return 0;
}

CInvenBox* CInvenBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, _uint _Index, float _Angle)
{
	CInvenBox* pInstance = new CInvenBox(pGraphicDev, _State, _UI_Name, _Index);
	if (FAILED(pInstance->Ready_GameObject(_pos, _size, _Angle)))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}


void CInvenBox::Free()
{
	__super::Free();
}
