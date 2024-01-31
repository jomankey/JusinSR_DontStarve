#pragma once


#include "CUI.h"



class CInvenBox :public CUI
{
protected:
	explicit CInvenBox(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name,_uint _index);
	explicit CInvenBox(const CInvenBox& rhs);
	virtual ~CInvenBox();

public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;



	virtual BOOL IsItemOn() { return m_bItemChek; }
	virtual void SetItemOn(BOOL _ItemPut) { m_bItemChek = _ItemPut; }

public:
	static CInvenBox* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, _uint _Index, float _Angle = 0.f);

public:
	virtual void Free() override;
public:
	_uint	m_iInvenIndex;

};

