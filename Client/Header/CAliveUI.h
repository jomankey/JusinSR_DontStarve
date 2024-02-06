#pragma once
#include "CToolUI.h"
class CAliveUI :  public CToolUI
{

protected:
	explicit CAliveUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name, eITEMTOOL_TYPE _ItemToolType);
	explicit CAliveUI(const CAliveUI& rhs);
	virtual ~CAliveUI();

public:
	virtual void LateUpdate_GameObject()					 override;

public:
	static CAliveUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, eITEMTOOL_TYPE _ItemToolType);
	virtual void Free() override;



};

