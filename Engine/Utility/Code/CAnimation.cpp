#include "CAnimation.h"

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strAnimName)
	:m_pGraphicDev(pGraphicDev)
	, m_strAnimName(_strAnimName)
{

}

CAnimation::~CAnimation()
{
}

void CAnimation::Ready_Animtion(const _tchar* _strTexturePath, _uint _iFrameCount, _float _fDuration)
{
	m_vecFrameInfo.reserve(_iFrameCount);
	m_iMaxFrameCount = _iFrameCount;

	tFrameInfo FrameInfo;


	FrameInfo.Texture = nullptr;

	for (_uint i = 0; i < _iFrameCount; ++i)
	{
		TCHAR	szFileName[128] = L"";

		wsprintf(szFileName, _strTexturePath, i);

		D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&FrameInfo.Texture), E_FAIL;
		FrameInfo.Duration = _fDuration;

		m_vecFrameInfo.push_back(FrameInfo);
	}


}

void CAnimation::AnimUpdate(const _float& fTimeDelta)
{

	m_fAccTime += fTimeDelta;

	if (m_vecFrameInfo[m_iCurFrm].Duration < m_fAccTime)
	{
		m_fAccTime = 0.f;
		m_iCurFrm++;
	}

	if (m_iMaxFrameCount < m_iCurFrm)
	{
		m_iCurFrm = 0;
	}

}



CAnimation* CAnimation::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strAnimName, const _tchar* _strTexturePath, _uint _iFrameCount, _float _fDuration)
{
	CAnimation* anim = new CAnimation(pGraphicDev, _strAnimName);
	anim->Ready_Animtion(_strTexturePath, _iFrameCount, _fDuration);

	return anim;
}

void CAnimation::Free()
{
	for (size_t i = 0; i < m_vecFrameInfo.size(); i++)
	{
		m_vecFrameInfo[i].Texture->Release();
	}
}
