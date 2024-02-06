#pragma once
#include "CToolUI.h"
class CWeaponUI :
    public CToolUI
{

protected:
	explicit CWeaponUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name, eITEMTOOL_TYPE _ItemToolType);
	explicit CWeaponUI(const CWeaponUI& rhs);
	virtual ~CWeaponUI();

public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

public:
	static CWeaponUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, eITEMTOOL_TYPE _ItemToolType);
	virtual void Free() override;



};

