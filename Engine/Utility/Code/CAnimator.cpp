#include "CAnimator.h"


CAnimator::CAnimator(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
	, m_pCurAnimation(nullptr)
{
}

CAnimator::CAnimator(const CAnimator& rhs)
	:CComponent(rhs)
	, m_pCurAnimation(nullptr)
{
}

CAnimator::~CAnimator()
{
}



void CAnimator::CreateAnimation(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strAnimName, const _tchar* _strTexturePath, _uint _iFrameCount, _float _fDuration)
{

	if (Find_Animation(_strAnimName))
	{
		assert(false, L"이미있는애니메이션키값");
	}


	CAnimation* anim = CAnimation::Create(pGraphicDev, _strAnimName, _strTexturePath, _iFrameCount, _fDuration);
	anim->Ready_Animtion(_strTexturePath, _iFrameCount, _fDuration);

	m_mapAnimation.insert({ _strAnimName, anim });
}



HRESULT CAnimator::Ready_Animation()
{
	return S_OK;
}

_int CAnimator::Update_Component(const _float& fTimeDelta)
{
	if (nullptr != m_pCurAnimation)
	{
		m_pCurAnimation->AnimUpdate(fTimeDelta);
	}
	return 0;
}

CComponent* CAnimator::Clone()
{
	return new CAnimator(*this);
}

CAnimator* CAnimator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAnimator* pInstance = new CAnimator(pGraphicDev);

	if (FAILED(pInstance->Ready_Animation()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Transform Create Failed");
		return nullptr;
	}

	return pInstance;
}





void CAnimator::Free()
{

	for (size_t i = 0; i < ID_END; ++i)
	{
		for_each(m_mapAnimation.begin(), m_mapAnimation.end(), CDeleteMap());
		m_mapAnimation.clear();
	}

	__super::Free();
}
