#include "stdafx.h"
#include "CPigHouse.h"

#include "Export_System.h"
#include "Export_Utility.h"

CPigHouse::CPigHouse(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_eHouseState(STANDARD)
{
}

CPigHouse::CPigHouse(const CPigHouse& rhs)
	:CGameObject(rhs.m_pGraphicDev), m_eHouseState(rhs.m_eHouseState)
{
}

CPigHouse::~CPigHouse()
{
}

void CPigHouse::Billboard()
{
	_matrix	matWorld, matView, matBillY, matBillX;

	m_pTransForm->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBillY);
	D3DXMatrixIdentity(&matBillX);

	matBillY._11 = matView._11;
	matBillY._13 = matView._13;
	matBillY._31 = matView._31;
	matBillY._33 = matView._33;

	matBillX._21 = matView._21;
	matBillX._22 = matView._22;
	matBillX._32 = matView._32;
	matBillX._33 = matView._33;

	D3DXMatrixInverse(&matBillY, NULL, &matBillY);
	D3DXMatrixInverse(&matBillX, NULL, &matBillX);

	m_pTransForm->Set_WorldMatrix(&(matBillX * matBillY * matWorld));

}

HRESULT CPigHouse::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransForm->Set_Pos(_vec3(float(rand() % 20), 1.5f, float(rand() % 20)));
	return S_OK;
}

_int CPigHouse::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	Billboard();
	renderer::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CPigHouse::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
	_vec3	vPos;
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	House_Change_Event(); // 피격 혹은 특정 이벤트 발생시 집의 형태 변환
	__super::Compute_ViewZ(&vPos);
}

void CPigHouse::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((int)m_eHouseState);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CPigHouse::Add_Component()
{
	CComponent* pComponent = nullptr;
	_vec3 vPos;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(proto::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MultiMap[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(proto::Clone_Proto(L"Proto_PigHouse_IDLE"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MultiMap[ID_STATIC].insert({ L"Proto_PigHouse_IDLE", pComponent });

	pComponent = m_pTransForm = dynamic_cast<CTransform*>(proto::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MultiMap[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
	m_pTransForm->Set_Scale(_vec3(2.f, 1.5f, 1.5f));
	m_pTransForm->Get_Info(INFO_POS, &vPos);
	m_pTransForm->Set_Pos(vPos.x, 1.8f, vPos.z);
	return S_OK;
}

void CPigHouse::House_Change_Event()// 피격 혹은 특정 이벤트 발생시 집의 형태 변환
{
	//if () // 충돌 혹은 피격 혹은 아침저녁 바뀔때 메소드
	//{
	//	m_eHouseState = ??		//스테이트 바꾸기
	//	
	//}

}

CPigHouse* CPigHouse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPigHouse* pInstance = new CPigHouse(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CPigHouse::Free()
{
	CGameObject::Free();
}
