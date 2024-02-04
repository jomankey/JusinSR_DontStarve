#pragma once
#include "CUI.h"
class CSlideUI : public CUI
{
protected:
	explicit CSlideUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name);
	explicit CSlideUI(const CSlideUI& rhs);
	virtual ~CSlideUI();
public:

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject()						 override;

	void SetSlideOnAndOff(bool _State) { m_bSlideOn = _State; };


	 

public:
	static CSlideUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle = 0.f);
	virtual void Free() override;

	queue<CUI*> m_qItemListQueue;

private:

	_float m_fMinSlide = 20.0f;
	_float m_fMaxSlide = 100.0f;
	_float m_fSlideSpeed = 200.0f;
	BOOL m_bSlideOn = false; 
	BOOL m_bSlideOff = false; 

	BOOL m_bIsRender = false;
};

