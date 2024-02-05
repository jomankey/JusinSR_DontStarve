#pragma once
#include "CUI.h"
class CMentalUI :public CUI
{

protected:
	explicit CMentalUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name);
	explicit CMentalUI(const CMentalUI& rhs);
	virtual ~CMentalUI();

public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject()						 override;


public:
	static CMentalUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle = 0.f);
	virtual void Free() override;

private:


	_float m_fImageCount = 0.f;

};

