#include "stdafx.h"
#include "CCookingPot.h"

#include "Export_System.h"
#include "Export_Utility.h"

CCookingPot::CCookingPot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResObject(pGraphicDev)
{
}

CCookingPot::CCookingPot(const CCookingPot& rhs)
	: CResObject(rhs.m_pGraphicDev)
{

}

CCookingPot::~CCookingPot()
{
}

HRESULT CCookingPot::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eObject_id = COOKING_POT;
	
	m_fDiffY = 1.f;
	m_fFrame = 0.0f;
	return S_OK;
}

_int CCookingPot::Update_GameObject(const _float& fTimeDelta)
{





	


	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);



	return 0;
}

void CCookingPot::LateUpdate_GameObject()
{



	Check_FrameState();
	_vec3	vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

	__super::LateUpdate_GameObject();
}

void CCookingPot::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	
	m_pCookingpotTexCom[m_eCookingpotCurState]->Set_Texture((_uint)m_fFrame);
	
	
	FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}



HRESULT CCookingPot::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;



	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	
	pComponent = m_pCookingpotTexCom[COOKINGPOT_IDLE_EMPTY] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Idle_empty"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Idle_empty", pComponent });


	pComponent = m_pCookingpotTexCom[COOKINGPOT_IDLE_FULL] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Idle_full"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Idle_full", pComponent });

	pComponent = m_pCookingpotTexCom[COOKINGPOT_COOKING_LOOP] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_loop"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_loop", pComponent });

	pComponent = m_pCookingpotTexCom[COOKINGPOT_BURNT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Burnt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Burnt", pComponent });

	pComponent = m_pCookingpotTexCom[COOKINGPOT_PLACE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Place"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Place", pComponent });


	pComponent = m_pCookingpotTexCom[COOKINGPOT_HIT_EMPTY] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Hit_Empty"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Hit_Empty", pComponent });

	pComponent = m_pCookingpotTexCom[COOKINGPOT_HIT_COOKING] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Hit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Hit", pComponent });

	pComponent = m_pCookingpotTexCom[COOKINGPOT_END] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_CookingPot_Default"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_CookingPot_Default", pComponent });












	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });







	m_pTransForm->Set_Scale(_vec3(1.0f, 1.0f, 1.0f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 1.0f, vPos.z);

	return S_OK;
}

void CCookingPot::Check_FrameState()
{
	if (m_eCookingpotCurState != m_eCookingpotPrevState)
	{



		switch (m_eCookingpotCurState)
		{
		case CCookingPot::COOKINGPOT_IDLE_EMPTY:
			m_fFrameEnd = 0.0f;
			break;
		case CCookingPot::COOKINGPOT_IDLE_FULL:
			m_fFrameEnd = 0.0f;
			break;
		case CCookingPot::COOKINGPOT_COOKING_LOOP:
			m_fFrameEnd = 6.0f;
			break;
		case CCookingPot::COOKINGPOT_BURNT:
			m_fFrameEnd=0.0f;
			break;
		case CCookingPot::COOKINGPOT_PLACE:
			m_fFrameEnd = 8.0f;
			break;
		case CCookingPot::COOKINGPOT_HIT_EMPTY:
			m_fFrameEnd = 9.0f;
			break;
		case CCookingPot::COOKINGPOT_HIT_COOKING:
			m_fFrameEnd = 9.0f;
			break;
		case CCookingPot::COOKINGPOT_END:
			m_fFrameEnd = 0.0f;
			break;
		default:
			break;
		}
		m_eCookingpotPrevState = m_eCookingpotCurState;
		m_fFrame = 0.0f;
	}
}




CCookingPot* CCookingPot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCookingPot* pInstance = new CCookingPot(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCookingPot::Free()
{
	CGameObject::Free();
}


