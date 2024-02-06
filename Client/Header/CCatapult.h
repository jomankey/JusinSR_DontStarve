#pragma once
#include "Export_Utility.h"
#include "GameObject.h"

class CCatapult :
	public CGameObject
{
private:
	explicit CCatapult(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCatapult(const CCatapult& rhs);
	virtual ~CCatapult();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						override;

	virtual HRESULT			Add_Component();
private:

	CRcTex* m_pBufferCom;
	CAnimator* m_pAnimCom;
public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};