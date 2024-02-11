#include "WorldHand.h"

CWorldHand::CWorldHand(LPDIRECT3DDEVICE9 pGraphicDev)
	: CStateUI(pGraphicDev)
{
}

CWorldHand::~CWorldHand()
{
}

HRESULT CWorldHand::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fX = 730.f;
	m_fY = 60.f;

	m_fSizeX = 10.f;
	m_fSizeY = 30.f;

	__super::Ready_GameObject();

	m_frameEnd = 0.f;

	return S_OK;
}

_int CWorldHand::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	m_fWorldTime = 0.0003f;
	m_pTransForm->Rotation(Engine::ROT_Z, D3DXToRadian(-(m_fWorldTime / fTimeDelta)));

	return 0;
}

void CWorldHand::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CWorldHand::Render_GameObject()
{
	__super::Render_GameObject();

	scenemgr::Get_CurScene()->EndOrtho();
}

HRESULT CWorldHand::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_UI_World_10"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_World_10", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CWorldHand::Check_State()
{
}

CWorldHand* CWorldHand::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWorldHand* pInstance = new CWorldHand(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CWorldHand::Free()
{
	__super::Free();
}
