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
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState);
	explicit CUI(const CUI& rhs);
	virtual ~CUI();

public:
	virtual HRESULT Ready_GameObject(_vec3 _pos, _vec3 _size);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();
	UI_STATE		m_eUIState;
	Engine::CTexture* m_pTextureCom;
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	//sss
	const _tchar* m_pUI_Name;

protected:
	_matrix m_matWorld;
	_matrix m_matProj;
	_matrix m_matView;
	_matrix TransformMatrix;

	_float m_fX;
	_float m_fY;
	_float m_fSizeX;
	_float m_fSizeY;

public:
	static CUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size);

private:
	virtual void Free() override;
};

