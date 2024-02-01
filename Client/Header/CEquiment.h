#pragma once
#include "CUI.h"



class CEquiment :public CUI
{
protected:
	explicit CEquiment(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name);
	explicit CEquiment(const CEquiment& rhs);
	virtual ~CEquiment();

public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;


public:
	static CEquiment* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle = 0.f);
	virtual HRESULT			Add_Component() override;
	virtual void Free() override;




};

