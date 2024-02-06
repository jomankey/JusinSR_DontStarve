#include "CInven.h"

CInven::CInven(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CInven::~CInven()
{
}

HRESULT CInven::Ready_GameObject(LPDIRECT3DDEVICE9 pGraphicDev)
{
	for (int i = 0; i < INVENCNT; ++i)
		m_pInvenBox[i] = CInvenBox::Create(pGraphicDev, _vec2(10.f, 10.f), _vec3(200.f, 500.f, 0.f));

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransForm->Set_Pos(_vec3(420.f, 580.f, 0.f));

	return S_OK;
}

_int CInven::Update_GameObject(const _float& fTimeDelta)
{
	for (int i = 0; i < INVENCNT; ++i)
		m_pInvenBox[i]->Update_GameObject(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CInven::LateUpdate_GameObject()
{
	for (int i = 0; i < INVENCNT; ++i)
		m_pInvenBox[i]->LateUpdate_GameObject();

	__super::LateUpdate_GameObject();
}

void CInven::Render_GameObject()
{
	for (int i = 0; i < INVENCNT; ++i)
		m_pInvenBox[i]->Render_GameObject();
}

CUI* CInven::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring pName)
{
	CUI* pInstance = new CInven(pGraphicDev);
	pInstance->SetObjName(pName);

	if (FAILED(pInstance->Ready_GameObject(pGraphicDev)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CInven::Free()
{
}
