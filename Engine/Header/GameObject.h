#pragma once

#include "Engine_Define.h"
#include "Base.h"



namespace Engine
{
	class CComponent;
	class CTransform;

	class ENGINE_DLL CGameObject : public CBase
	{
	public:
		_float				Get_ViewZ() { return m_fViewZ; }
		CTransform* GetTransForm() { return m_pTransForm; }
		HRESULT				SetUp_Material();
		void				Compute_ViewZ(const _vec3* pPos);

	public:
		virtual HRESULT		Ready_GameObject();
		virtual _int		Update_GameObject(const _float& fTimeDelta);
		virtual void		LateUpdate_GameObject();
		virtual void		Render_GameObject();
		virtual CComponent* Find_Component(COMPONENTID eID, const _tchar* pComponentTag);
		virtual void		Free() override;


	protected:
		explicit CGameObject() = delete;
		explicit CGameObject(LPDIRECT3DDEVICE9	pGraphicDev)=delete;
		explicit CGameObject(LPDIRECT3DDEVICE9	pGraphicDev,wstring _wstrName);
		explicit CGameObject(const CGameObject& rhs);
		virtual ~CGameObject();


	protected:
		LPDIRECT3DDEVICE9							m_pGraphicDev;

		multimap<const _tchar*, CComponent*>		m_mapComponent[ID_END];
		CTransform*									m_pTransForm;

		_float										m_fViewZ;	//z버퍼 소팅
		_bool										m_bDelete;	//삭제예정일경우 True
		wstring										m_wstrName;	//Key값

	};
}