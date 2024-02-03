#pragma once
#include "CUI.h"
class CWorldUI : public CUI
{

protected:
	explicit CWorldUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name);
	explicit CWorldUI(const CWorldUI& rhs);
	virtual ~CWorldUI();

public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject()						 override;


public:
	static CWorldUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle = 0.f);
	virtual void Free() override;



private:



	_float m_fImageCount = 0.f;

};

