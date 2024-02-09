#include "stdafx.h"
#include "CTent.h"

#include "Export_System.h"
#include "Export_Utility.h"

CTent::CTent(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResObject(pGraphicDev)
{
}

CTent::CTent(const CTent& rhs)
	: CResObject(rhs.m_pGraphicDev)
{

}

CTent::~CTent()
{
}

HRESULT CTent::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eObject_id = TENT;

	m_fFrame = 0.0f;
	return S_OK;
}

_int CTent::Update_GameObject(const _float& fTimeDelta)
{


	




	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);



	return 0;
}

void CTent::LateUpdate_GameObject()
{



	Check_FrameState();
	_vec3	vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

	__super::LateUpdate_GameObject();
}

void CTent::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);



	m_pTentTexCom[m_eTentCurState]->Set_Texture((_uint)m_fFrame);
	FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pBufferCom->Render_Buffer();
	


	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}




HRESULT CTent::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;



	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTentTexCom[CTENT_IDLE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Tent_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Tent_Idle", pComponent });


	pComponent = m_pTentTexCom[CTENT_HIT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Tent_Hit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Tent_Hit", pComponent });

	pComponent = m_pTentTexCom[CTENT_ENTER] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Tent_Enter"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Tent_Enter", pComponent });

	pComponent = m_pTentTexCom[CTENT_DESTROY] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Tent_Destroy"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Tent_Destroy", pComponent });

	pComponent = m_pTentTexCom[CTENT_BURNT] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Tent_Burnt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Tent_Burnt", pComponent });

	pComponent = m_pTentTexCom[CTENT_PLACE] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Tent_Place"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Tent_Place", pComponent });

	pComponent = m_pTentTexCom[CTENT_END] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_Tent_Defalut_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_Tent_Defalut_Idle", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });







	m_pTransForm->Set_Scale(_vec3(1.0f, 1.0f, 1.0f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 1.0f, vPos.z);

	return S_OK;
}

void CTent::Check_FrameState()
{
	if (m_eTentCurState != m_eTentPrevState)
	{
		switch (m_eTentCurState)
		{
		case CTent::CTENT_IDLE:
			m_fFrameEnd=18.0f;
			break;
		case CTent::CTENT_HIT:
			m_fFrameEnd=3.0f;
			break;
		case CTent::CTENT_ENTER:
			m_fFrameEnd=7.0f;
			break;
		case CTent::CTENT_DESTROY:
			m_fFrameEnd=16.0f;
			break;
		case CTent::CTENT_BURNT:
			m_fFrameEnd=0.0f;
			break;
		case CTent::CTENT_PLACE:
			m_fFrameEnd = 7.0f;
			break;
		case CTent::CTENT_END:
			m_fFrameEnd = 0.0f;
			break;
		default:
			break;
		}
		m_eTentPrevState = m_eTentCurState;
		m_fFrame = 0.0f;
	}

	

}






CTent* CTent::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTent* pInstance = new CTent(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTent::Free()
{
	CGameObject::Free();
}


