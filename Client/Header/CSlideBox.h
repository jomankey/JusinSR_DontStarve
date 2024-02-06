#pragma once
#include "CUI.h"
class CSlideBox :public CUI
{


protected:
	explicit CSlideBox(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name);
	explicit CSlideBox(const CSlideBox& rhs);
	virtual ~CSlideBox();

public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject()						 override;


public:
	void SetShow(bool _bIsShow) { m_bIsShow = _bIsShow; }

public:
	static CSlideBox* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name,  float _Angle = 0.f);
	virtual HRESULT			Add_Component() override;
	virtual void Free() override;


private:
	const _tchar* ItemName;
	BOOL m_bIsShow = true;
};

