#pragma once
#include "CUI.h"



class CExplainPanel : public CUI
{
protected:
	explicit CExplainPanel(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name);
	explicit CExplainPanel(const CExplainPanel& rhs);
	virtual ~CExplainPanel();

public:

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject()						 override;


public:
	void Set_Show(bool _bShow) { m_bShow = _bShow; }


public:
	static CExplainPanel* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle = 0.f);
	virtual void Free() override;



private:
	BOOL m_bShow = false;
	

};

