
#include "..\Include\stdafx.h"
#include"CSlideUI.h"


#include "Export_Utility.h"
#include "Export_System.h"
#include"CExplainPanel.h"




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

	//테스트용
  if (GetAsyncKeyState('K'))
  {
      m_bSlideOn = true;
  }
  
  if (GetAsyncKeyState('O'))
  {
      m_bSlideOn = false;
  }
  
	__super::Update_GameObject(fTimeDelta);
	FindExplainPanel();



	


	if (m_bSlideOn)
	{
		m_bIsRender = false;
		if (m_fMaxSlide <= m_fX)
		{
			if (UI_Collision(this))
			{
				dynamic_cast<CExplainPanel*>(m_pExplainPanel)->Set_Show(true);
				dynamic_cast<CExplainPanel*>(m_pExplainPanel)->SetX(this->m_fX);
				dynamic_cast<CExplainPanel*>(m_pExplainPanel)->SetY(this->m_fY);

			}
			else
			{
				dynamic_cast<CExplainPanel*>(m_pExplainPanel)->Set_Show(false);
			}


			return 0;
		}
		m_fX += fTimeDelta * m_fSlideSpeed;

	}
	else
	{
		if (m_fMinSlide >= m_fX)
		{
			m_bIsRender = true;
			return 0;
		}
		m_fX -= fTimeDelta * m_fSlideSpeed;

	}



	return 0;
}

void CSlideUI::Render_GameObject()
{
	if (m_bIsRender)
		return;

	scenemgr::Get_CurScene()->BeginOrtho();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Set_Texture(0);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	//m_pTransForm->Set_Pos(m_fX - (WINCX >> 1), -m_fY + (WINCY >> 1), 0.f);
	//m_pTransForm->Set_Scale(_vec3{ m_fSizeX, m_fSizeY, 1.f });

	m_pBufferCom->Render_Buffer();



	scenemgr::Get_CurScene()->EndOrtho();


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

void CSlideUI::FindExplainPanel()
{
	if (m_pExplainPanel == nullptr)
	{
		vector<CGameObject*> vecUI = scenemgr::Get_CurScene()->GetGroupObject(eLAYER_TYPE::FORE_GROUND, eOBJECT_GROUPTYPE::UI);
		for (auto iter : vecUI)
		{
			if (iter->GetObjName() == L"Proto_UI_Explain_PopUp_Panel")
			{
				m_pExplainPanel = iter;
				break;
			}

		}

	}

}
