#include "Mouse.h"
#include "stdafx.h"
#include "Export_System.h"

CMouse::CMouse(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_bColl(false)
{
}

CMouse::~CMouse()
{
}

HRESULT CMouse::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//ShowCursor(NULL);
	//m_pGraphicDev->ShowCursor(TRUE);

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);

	return S_OK;
}

_int CMouse::Update_GameObject(const _float& fTimeDelta)
{
	POINT tPt;
	GetCursorPos(&tPt);
	ScreenToClient(g_hWnd, &tPt);
	m_vMousePos = _vec3(tPt.x, tPt.y, 0.f);

	__super::Update_GameObject(fTimeDelta);

	auto& vecObj = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::GAME_LOGIC, eOBJECT_GROUPTYPE::MONSTER);
	_vec3 vRayPos, vRayDir, vMouseScale, vMonsterPos, vMonsterScale;
	//마우스 좌표 변환
	m_pCalculatorCom->Change_MouseMatrix(g_hWnd, m_vMousePos, &vRayPos, &vRayDir);
	vMouseScale = m_pTransForm->Get_Scale();

	for (auto& iter : vecObj)
	{
		iter->GetTransForm()->Get_Info(INFO_POS, &vMonsterPos);
		vMonsterScale = iter->GetTransForm()->Get_Scale();

		if (Engine::Collision_Mouse_Object(vRayPos, vRayDir, vMonsterPos, vMonsterScale))
		{
			m_bColl = true;
			break;
		}
		else m_bColl = false;
	}
	

	renderer::Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CMouse::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMouse::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	if (m_bColl)
	{
		Engine::Render_Font(L"Mouse_Title", L"이름", &_vec2(m_vMousePos.x + 20.f, m_vMousePos.y - 20.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
		Engine::Render_Font(L"Mouse_Sub", L"정보", &_vec2(m_vMousePos.x + 20.f, m_vMousePos.y - 5.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	}

	scenemgr::Get_CurScene()->EndOrtho();
}

HRESULT CMouse::Add_Component()
{
	CComponent* pComponent = nullptr;

	//VIBUFFER
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	////TEXTURE
	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(GetObjName().c_str()));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ GetObjName().c_str(), pComponent });

	//TransForm
	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	//m_pTransForm->Set_Scale(_vec3(0.5f, 0.3f, 0.5f));

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(proto::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	return S_OK;
}

CMouse* CMouse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMouse* pInstance = new CMouse(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMouse::Free()
{
	__super::Free();
}
