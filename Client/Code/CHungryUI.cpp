#include"CHungryUI.h"



#include "Export_Utility.h"
#include "Export_System.h"

#include "Scene.h"
#include <Player.h>


CHungryUI::CHungryUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CStateUI(pGraphicDev)

{
}

CHungryUI::~CHungryUI()
{
}

HRESULT CHungryUI::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fX = 725.f;
	m_fY = 130.f;
	
	m_fSizeX = 30.f;
	m_fSizeY = 30.f;

	__super::Ready_GameObject();

	m_frameEnd = 20.f;

	return S_OK;
}

_int CHungryUI::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CHungryUI::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	Check_State();
}

void CHungryUI::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CHungryUI::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_UI_Hungry"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UI_Hungry", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CHungryUI::Check_State()
{
	CGameObject* pP = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::PLAYER)[0];
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pP);

	_float fHungry = pPlayer->Get_PlayerHungry();
	_float fMaxHungry = pPlayer->Get_PlayerMaxHangry();

	_float fFrame = (fMaxHungry - fHungry) / 10.f;

	if (fFrame < m_frameEnd)
		m_frame = fFrame;
}

CHungryUI* CHungryUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHungryUI* pInstance = new CHungryUI(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CHungryUI::Free()
{
	__super::Free();
}