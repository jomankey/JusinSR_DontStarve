#pragma once
#include "Component.h"
#include "CAnimation.h"

namespace Engine
{
	class ENGINE_DLL CAnimator :
		public CComponent
	{
	public:
		void SetCurAnimation(const _tchar* _strAnimKey);
		void SetCurAnimationFrame(const _tchar* _strAnimKey,_uint _iFrame);
		_bool GetAnimFinish(const _tchar* _strAnimKey) { return Find_Animation(_strAnimKey)->m_bFinish; }
		void SetAnimTexture();

		//특정 애니메이션일때의 프레임 얻어오기 //걷는상태일때에 특정프레임을얻고싶을떄
		_int	GetAnimFrame(const _tchar* _strAnimKey) { return Find_Animation(_strAnimKey)->m_iCurFrm; }
		_int	GetAnimMaxFrame(const _tchar* _strAnimKey) { return Find_Animation(_strAnimKey)->m_iMaxFrameCount-1; }

		//현재 업데이트중인 애니메이션 프레임
		//현재 이동중일때 프레임을얻어와 회전시켰을때 
		//다시 변경전 프레임값으로 변경한 회전애니메이션에 적용시킬수있음
		// ex) 앞을보고 걷는 프레임이 15였을때 카메라가이동해 오른쪽을 보면 0프레임부터다시 걷는게아닌
		//		15프레임을 오른쪽애니메이션에 15프레임으로 설정하도록
		_int	GetCurAnimFrame() { return m_pCurAnimation->m_iCurFrm; }

		void AddAnimation(const _tchar* _strAnimKey, Engine::CAnimation* _pAnimation) { m_mapAnimation.insert({ _strAnimKey, _pAnimation }); }
		void SetLoopAnimator(_bool _bLoop) { m_bLoop = _bLoop; }
		_bool IsLoop() { return m_bLoop; }

	public:
		virtual HRESULT		Ready_Animation();
		virtual _int		Update_Component(const _float& fTimeDelta);
		virtual void		LateUpdate_Component();
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
		CAnimation* Find_Animation(const _tchar* _key);

	private:
		map<const _tchar*, Engine::CAnimation*>	m_mapAnimation;
		Engine::CAnimation* m_pCurAnimation;
		_bool		m_bLoop;//현재애니메이션 루프상태

	};

}