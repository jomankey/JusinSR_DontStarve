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
protected:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name);
	explicit CUI(const CUI& rhs);
	virtual ~CUI();

public:
	virtual HRESULT Ready_GameObject(_vec3 _pos, _vec3 _size,float _Angle=0);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;



	virtual BOOL UI_Collision();

private:
	virtual HRESULT			Add_Component();
	Engine::CTexture* m_pTextureCom;
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

	UI_STATE		m_eUIState;
protected:
	_float m_fX;
	_float m_fY;
	_float m_fSizeX;
	_float m_fSizeY;
	_float m_fAngle;
	
	_float m_OriginfX;
	_float m_OriginfY;
	_float m_OriginfSizeX;
	_float m_OriginfSizeY;
	_float m_OriginfAngle;


	_matrix m_matWorld;
	const _tchar* m_pUI_Name;

	//¸¶¿ì½º´Â ½Ì±ÛÅæÀ¸·Î??
	POINT m_MousePoint;

	

public:
	static CUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle= 0.f);

private:
	virtual void Free() override;
};

