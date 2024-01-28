#pragma once
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;

END
class CUI;



class CInven : public Engine::CGameObject
{
public:

private:
	explicit CInven(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState);
	explicit CInven(const CInven& rhs);
	virtual ~CInven();

public:
	virtual HRESULT Ready_GameObject();
	virtual HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						override;

	void Set_InvenSlot();

private:
	virtual HRESULT			Add_Component();
	CUI* m_vecInvenSlot[15];

public:
	static CInven* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size);

private:
	virtual void Free() override;
};
