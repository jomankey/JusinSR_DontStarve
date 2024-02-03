#pragma once
#include "CUI.h"
class CHpUI : public CUI
{
protected:
	explicit CHpUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name);
	explicit CHpUI(const CHpUI& rhs);
	virtual ~CHpUI();

public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject()						 override;


public:
	static CHpUI* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle = 0.f);
	virtual void Free() override;




public:
	void SetCurntHp(float _CurentHp) { m_fCurentHp = _CurentHp; }
	void SetMaxHp(float _MaxHp) { m_fMaxHp = _MaxHp; }
	void AddCurntHp(const _int _value);
	void MinusCurntHp(const _int _value);
	

private:
	_float m_fCurentHp;
	_float m_fMaxHp;
	_float m_frame; //출력될 사진 인덱스
	_float m_frameEnd; //프레임이 시작되고나서 움직이는 값
	_float m_PreFrame;
	BOOL m_bFrameStop = true;
	BOOL m_bHpPiusMInus = true; //true 체력이 증가 false면 체력이 감소



	_float m_fImageCount=0.f;
};

