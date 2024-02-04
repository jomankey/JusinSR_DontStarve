#pragma once
#include "CToolUI.h"
class CClothUI :
    public CToolUI
{


protected:
	explicit CClothUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name);
	explicit CClothUI(const CClothUI& rhs);
	virtual ~CClothUI();

public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

public:
	static CClothUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle = 0.f);
	virtual void Free() override;

};

