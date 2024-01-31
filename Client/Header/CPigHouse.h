#pragma once
#include "GameObject.h"

namespace Engine {
class CRcTex;
class CTexture;
class CTransform;
}

class CPigHouse :
	public CGameObject
{

	enum PIGHOUSE{ STANDARD, ONLIGHT,RUNDOWN,PH_END };
private:
	explicit CPigHouse(LPDIRECT3DDEVICE9 pGraphicDev );
	explicit CPigHouse(const CPigHouse& rhs);
	virtual ~CPigHouse();
public:
	void Billboard();
public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						override;
private:
	HRESULT			Add_Component();
	void			House_Change_Event();
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;

	PIGHOUSE  m_eHouseState;

public:
	static CPigHouse* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};


