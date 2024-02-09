#include "stdafx.h"
#include "CFire.h"

#include "Export_System.h"
#include "Export_Utility.h"

CFire::CFire(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResObject(pGraphicDev)
{
}

CFire::CFire(const CFire& rhs)
	: CResObject(rhs.m_pGraphicDev)
{

}

CFire::~CFire()
{
}

HRESULT CFire::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eObject_id = BON_FIRE;
	m_efireCurState= FIRE_LEVEL_2;
	m_fFrame = 0.0f;
	return S_OK;
}

_int CFire::Update_GameObject(const _float& fTimeDelta)
{

	m_fFrame += m_fFrameEnd * fTimeDelta;
	
	if (m_fFrame> m_fFrameEnd)
	{
		m_fFrame = 0.0f;
	}

	Check_FrameState();

	m_fTimeChek += fTimeDelta;
	if (m_fTimeChek >= 10.f)
	{
		Level_Down();
		m_fTimeChek = 0.f;
	}


	
	CGameObject::Update_GameObject(fTimeDelta);
	renderer::Add_RenderGroup(RENDER_ALPHA, this);



	return 0;
}

void CFire::LateUpdate_GameObject()
{




	_vec3	vPos;
	m_pTransForm->BillBoard();
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

	__super::LateUpdate_GameObject();
}

void CFire::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	//불은 이미지가 꺼져도 항상 있어야함 그래서 이미지만 꺼버림
	if (m_bIsOff==false)
	{
		m_pFireTexCom[m_efireCurState]->Set_Texture((_uint)m_fFrame);
		FAILED_CHECK_RETURN(SetUp_Material(), );
		m_pBufferCom->Render_Buffer();
	}
	

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CFire::Set_Pos(_vec3 _Pos)
{
	m_pTransForm->Set_Pos(_Pos);
}

void CFire::Set_FireLevel(_int iLevel)
{
	if(iLevel<0|| iLevel>4)
		return;

	m_efireCurState = (FIRE_STATE)iLevel;

}

HRESULT CFire::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pFireTexCom[FIRE_LEVEL_1] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_FIre_lev_1"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_FIre_lev_1", pComponent });


	pComponent = m_pFireTexCom[FIRE_LEVEL_2] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_FIre_lev_2"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_FIre_lev_2", pComponent });

	pComponent = m_pFireTexCom[FIRE_LEVEL_3] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_FIre_lev_3"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_FIre_lev_3", pComponent });

	pComponent = m_pFireTexCom[FIRE_LEVEL_4] = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_Object_FIre_lev_4"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Object_FIre_lev_4", pComponent });


	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });







	m_pTransForm->Set_Scale(_vec3(1.0f, 1.0f, 1.0f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 1.0f, vPos.z);

	return S_OK;
}

void CFire::Check_FrameState()
{
	if (m_efireCurState != m_efirerPrevState)
	{

		switch (m_efireCurState)
		{
		case CFire::FIRE_LEVEL_1:
			m_pTransForm->Set_Scale(_vec3(0.3f, 0.3f, 0.3f));
			m_bIsOff = false;
			m_fFrameEnd = 5.f;
			break;
		case CFire::FIRE_LEVEL_2:
			m_pTransForm->Set_Scale(_vec3(0.4f, 0.4f, 0.4f));
			m_bIsOff = false;
			m_fFrameEnd = 5.f;
			break;
		case CFire::FIRE_LEVEL_3:
			m_pTransForm->Set_Scale(_vec3(0.6f, 0.6f, 0.6f));
			m_bIsOff = false;
			m_fFrameEnd = 5.f;
			break;
		case CFire::FIRE_LEVEL_4:
			m_pTransForm->Set_Scale(_vec3(0.8f, 0.8f, 0.8f));
			m_bIsOff = false;
			m_fFrameEnd = 5.f;
			break;
		default:
			break;
		}

		m_efirerPrevState = m_efireCurState;
		m_fFrame = 0.f;
	}


}



void CFire::Level_Down()
{
	if (m_efireCurState== FIRE_LEVEL_4)
	{
		m_efireCurState = FIRE_LEVEL_3;
	}
	else if (m_efireCurState== FIRE_LEVEL_3)
	{
		m_efireCurState = FIRE_LEVEL_2;
	}
	else if (m_efireCurState== FIRE_LEVEL_2)
	{
		m_efireCurState = FIRE_LEVEL_1;
	}
	else if(m_efireCurState== FIRE_LEVEL_1)
	{
			m_bIsOff = true;
	}


}

CFire* CFire::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFire* pInstance = new CFire(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFire::Free()
{
	CGameObject::Free();
}


