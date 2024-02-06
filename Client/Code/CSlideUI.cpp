
#include "..\Include\stdafx.h"
#include"CSlideUI.h"


#include "Export_Utility.h"
#include "Export_System.h"

#include"CExplainPanel.h"
#include"CSlideBox.h"




CSlideUI::CSlideUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name)
	:CUI(pGraphicDev, eUIState, _UI_Name)
	, m_qItemListQueue()

{
}

CSlideUI::CSlideUI(const CSlideUI& rhs)
	: CUI(rhs)
{
}

CSlideUI::~CSlideUI()
{
}


_int CSlideUI::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	FindExplainPanel();


	if (m_bSlideOn)
	{
		m_bIsRender = false;
		if (m_fMaxSlide <= m_fX)
		{
			if (UI_Collision())
			{
				m_pExplainPanel->Set_Show(true);
				m_pExplainPanel->SetX(this->m_fX+110);
				m_pExplainPanel->SetY(this->m_fY);
			}
			else
			{
				m_pExplainPanel->Set_Show(false);
			}
			return 0;
		}
		m_fX += fTimeDelta * m_fSlideSpeed;
		if (m_pSlideBox == nullptr)
		{
				if (m_pBoxItemName != nullptr)
				{

				CSlideBox* TestSlideBox;
				m_pSlideBox = CSlideBox::Create(m_pGraphicDev, UI_STATE::UI_STATIC, _vec3(m_fX, m_fY-10.f, 0.f), _vec3(this->m_fSizeX-10.f, this->m_fSizeY - 10.f, 0.f), m_pBoxItemName);
				CreateObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI, m_pSlideBox);
				}

		}
		else 
		{
			m_pSlideBox->SetX(m_fX);
			m_pSlideBox->SetY(m_fY);
			m_pSlideBox->SetShow(true);
		}

	}
	else
	{
		if (m_fMinSlide >= m_fX)
		{
			if (m_pSlideBox != nullptr)
			{
				m_pSlideBox->SetShow(false);
			}
			m_bIsRender = true;
			return 0;
		}
		m_fX -= fTimeDelta * m_fSlideSpeed;
		if (m_pSlideBox != nullptr)
		{
			m_pSlideBox->SetX(m_fX);
			m_pSlideBox->SetY(m_fY);
			m_pSlideBox->SetShow(false);
		}

	}



	return 0;
}

void CSlideUI::Render_GameObject()
{
	if (m_bIsRender)
		return;
	__super::Render_GameObject();

}



CSlideUI* CSlideUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle)
{
	CSlideUI* pInstance = new CSlideUI(pGraphicDev, _State, _UI_Name);

	if (FAILED(pInstance->Ready_GameObject(_pos, _size, _Angle)))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}


	return pInstance;
}
void CSlideUI::Free()
{
	__super::Free();
}

void CSlideUI::SetSlideBoxItemName(_tchar* _Name)
{
	m_pBoxItemName = _Name;
}

void CSlideUI::FindExplainPanel()
{
	if (m_pExplainPanel == nullptr)
	{
		vector<CGameObject*> vecUI = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI);
		for (auto iter : vecUI)
		{
			if (iter->GetObjName() == L"Proto_UI_Explain_PopUp_Panel")
			{
				m_pExplainPanel = dynamic_cast<CExplainPanel*>(iter);
				break;
			}

		}

	}

}
