#pragma once

#include "GameObject.h"

namespace Engine
{

	class CUITex;
	class CRcTex;
	class CTexture;
	class CTransform;

}

class CUI
	: public Engine::CGameObject
{
protected:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name);
	explicit CUI(const CUI& rhs);
	virtual ~CUI();

public:
	virtual HRESULT Ready_GameObject(_vec3 _pos, _vec3 _size, float _Angle = 0);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;



	virtual BOOL UI_Collision();
	virtual float Get_fX() { return m_fX; }
	virtual float Get_fY() { return m_fY; }
	virtual float Get_fSizeX() { return m_fSizeX; }
	virtual float Get_fSizeY() { return m_fSizeY; }

	virtual HRESULT			Add_Component();
private:
	Engine::CTexture* m_pTextureCom;
	Engine::CRcTex* m_pBufferCom;
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
	static CUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle = 0.f);

public:
	virtual void Free() override;
};

