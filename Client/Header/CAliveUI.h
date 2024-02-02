#pragma once
#include "CToolUI.h"
class CAliveUI :  public CToolUI
{

protected:
	explicit CAliveUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name);
	explicit CAliveUI(const CAliveUI& rhs);
	virtual ~CAliveUI();

public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

public:
	static CAliveUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle = 0.f);
	virtual void Free() override;



};

