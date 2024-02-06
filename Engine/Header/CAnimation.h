#pragma once
#include "Engine_Define.h"
#include "Base.h"


namespace Engine
{
	class  ENGINE_DLL CAnimation :
		public CBase
	{

	public:
		void Ready_Animtion(const _tchar* _strTexturePath, _uint _iFrameCount, _float _fDuration);
		void AnimUpdate(const _float& fTimeDelta);
		void ResetFrame() { m_iCurFrm = 0.f; m_fAccTime = 0.f; }
		void SetCurTexture() { m_pGraphicDev->SetTexture(0, m_vecFrameInfo[m_iCurFrm].Texture); }
	public:
		static CAnimation* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strAnimName, const _tchar* _strTexturePath, _uint _iFrameCount, _float _fDuration);


	private:
		CAnimation() = delete;
		explicit CAnimation(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strAnimName);
		virtual ~CAnimation();

	private:
		virtual void Free()override;
	private:
		LPDIRECT3DDEVICE9			m_pGraphicDev;
		vector<tFrameInfo>			m_vecFrameInfo;
		_int						m_iMaxFrameCount; //최대프레임수
		_int						m_iCurFrm;		// 현재 프레임
		_float						m_fAccTime;		// 시간누적



		wstring						m_strAnimName;
	};

}
