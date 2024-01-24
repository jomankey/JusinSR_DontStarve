#include "PlayerStatus.h"

CPlayerStatus::CPlayerStatus()
{
	//s
}

CPlayerStatus::CPlayerStatus(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)/*, m_ePreState(STATE_END), m_eCurState(IDLE), m_iFrameCnt(0.f)*/
{
}

CPlayerStatus::CPlayerStatus(const CPlayerStatus& rhs)
	:CComponent(rhs)/*, m_ePreState(rhs.m_ePreState), m_eCurState(rhs.m_eCurState), m_iFrameCnt(rhs.m_iFrameCnt)*/
{
}

CPlayerStatus::~CPlayerStatus()
{
	
}

HRESULT CPlayerStatus::Ready_PlayerStatus()
{
	
	return S_OK;
}

_int CPlayerStatus::Update_Component(const _float& fTimeDelta)
{
	
	return 0;
}

void CPlayerStatus::LateUpdate_Component()
{

}

CPlayerStatus* CPlayerStatus::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerStatus* pInstance = new CPlayerStatus(pGraphicDev);

	if (FAILED(pInstance->Ready_PlayerStatus()))
	{
		Safe_Release(pInstance);
		MSG_BOX("PlayerStatus Create Failed");
		return nullptr;
	}

	return pInstance;
	
}

CComponent* CPlayerStatus::Clone(void)
{
	return new CPlayerStatus(*this);
}

void CPlayerStatus::Free()
{
	CComponent::Free();
}


