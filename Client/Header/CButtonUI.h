#pragma once
#include "CUI.h"
class CButtonUI :
    public CUI
{
public:
	explicit CButtonUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name);
	explicit CButtonUI(const CButtonUI& rhs);
	virtual ~CButtonUI();







};

