#pragma once
#include "CToolUI.h"
class CLightUI :
    public CToolUI
{

protected:
	explicit CLightUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name, eITEMTOOL_TYPE _ItemToolType);
	explicit CLightUI(const CLightUI& rhs);
	virtual ~CLightUI();

public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;

public:
	static CLightUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, eITEMTOOL_TYPE _ItemToolType);
	virtual void Free() override;


};

