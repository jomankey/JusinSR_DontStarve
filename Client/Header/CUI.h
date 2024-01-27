#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CUITex;
class CRcTex;
class CTexture;
class CTransform;

END

class CUI : public Engine::CGameObject
{
public:
	enum UI_STATE { UI_STATIC, UI_DYNAMIC, UI_NONE };

private:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name);
	explicit CUI(const CUI& rhs);
	virtual ~CUI();

public:
	virtual HRESULT Ready_GameObject(_vec3 _pos, _vec3 _size);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;


	BOOL UI_Collision();
private:
	virtual HRESULT			Add_Component();
	UI_STATE		m_eUIState;
	Engine::CTexture* m_pTextureCom;
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	//sss

	_matrix m_matWorld;
	const _tchar* m_pUI_Name;


	BOOL m_bIsCollision;
protected:


	_float m_fX;
	_float m_fY;
	_float m_fSizeX;
	_float m_fSizeY;

public:
	static CUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name);

private:
	virtual void Free() override;
};

