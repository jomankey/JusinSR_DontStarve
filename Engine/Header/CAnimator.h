#pragma once

#include "Component.h"
#include "CAnimation.h"
namespace Engine
{
	class ENGINE_DLL CAnimator :
		public CComponent
	{
	public:
		void CreateAnimation(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _strAnimKey, const _tchar* _strTexturePath, _uint _iFrameCount, _float _fDuration);

		void ChangeAnimation(const _tchar* _strAnimKey)
		{
			m_pCurAnimation = Find_Animation(_strAnimKey);
			m_pCurAnimation->ResetFrame();
		}
		void SetAnimTexture() { m_pCurAnimation->SetCurTexture(); }


	public:
		virtual HRESULT		Ready_Animation();
		virtual _int		Update_Component(const _float& fTimeDelta);
		virtual void		LateUpdate_Component() {}
		virtual CComponent* Clone()override;

		static CAnimator* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	protected:
		explicit CAnimator() = delete;
		explicit CAnimator(LPDIRECT3DDEVICE9 pGraphicDev);
		explicit CAnimator(const CAnimator& rhs);
		virtual ~CAnimator();

	protected:
		virtual void		Free();
	private:
		CAnimation* Find_Animation(const _tchar* _key)
		{
			auto	iter = find_if(m_mapAnimation.begin(), m_mapAnimation.end(), CTag_Finder(_key));

			if (iter == m_mapAnimation.end())
				return nullptr;

			return iter->second;
		}


	private:
		map<const _tchar*, CAnimation*>	m_mapAnimation;
		CAnimation* m_pCurAnimation;
	};

}