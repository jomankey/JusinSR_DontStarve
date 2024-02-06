#include "CButtonUI.h"

CButtonUI::CButtonUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name)
	: CUI(pGraphicDev, eUIState, _UI_Name)
{

}

CButtonUI::CButtonUI(const CButtonUI& rhs)
	: CUI(rhs)
{
}

CButtonUI::~CButtonUI()
{
}
