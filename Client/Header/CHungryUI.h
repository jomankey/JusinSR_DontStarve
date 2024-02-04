#pragma once
#include "CUI.h"

class CHungryUI : public CUI
{
protected:
	explicit CHungryUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name);
	explicit CHungryUI(const CHungryUI& rhs);
	virtual ~CHungryUI();

public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject()						 override;


public:
	static CHungryUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle = 0.f);
	virtual void Free() override;




public:
	void AddCurntHp(const _float _value);
	void MinusCurntHp(const _float _value);
private:
	_float m_fCurentHungry;

	_float m_fMaxHungry;





	_float m_fImageMaxCount = 19.f;
	_float m_fImageCount = 0.f;
};
