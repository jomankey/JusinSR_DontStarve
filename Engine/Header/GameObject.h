#pragma once

#include "Component.h"
#include "Base.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	_float			Get_ViewZ() { return m_fViewZ; }
	CComponent*		Get_Component(COMPONENTID eID, const _tchar* pComponentTag);
	CTransform*		Get_TransForm();
	void			Compute_ViewZ(const _vec3* pPos);

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		LateUpdate_GameObject();
	virtual void		Render_GameObject();

protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	map<const _tchar*, CComponent*>		m_mapComponent[ID_END];
	_float								m_fViewZ;

private:
	CComponent*		Find_Component(COMPONENTID eID, const _tchar* pComponentTag);

public:
	virtual void		Free();
};

END