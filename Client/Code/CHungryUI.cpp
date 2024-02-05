#include"CHungryUI.h"



#include "Export_Utility.h"
#include "Export_System.h"

#include "Scene.h"


CHungryUI::CHungryUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name)
	: CUI(pGraphicDev, eUIState, _UI_Name)
	, m_fCurentHungry(0.f)
	, m_fMaxHungry(0.f)

{

}

CHungryUI::CHungryUI(const CHungryUI& rhs)
	: CUI(rhs)
{

}

CHungryUI::~CHungryUI()
{
}

_int CHungryUI::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	if (GetAsyncKeyState('L')) // 공격
	{
		MinusCurntHp(10);
	}

	if (GetAsyncKeyState('K')) // 공격
	{
		AddCurntHp(10);
	}


	return 0;
}

void CHungryUI::Render_GameObject()
{
	scenemgr::Get_CurScene()->BeginOrtho();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_fImageCount);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pBufferCom->Render_Buffer();



	scenemgr::Get_CurScene()->EndOrtho();



}

CHungryUI* CHungryUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle)
{
	CHungryUI* pInstance = new CHungryUI(pGraphicDev, _State, _UI_Name);
	if (FAILED(pInstance->Ready_GameObject(_pos, _size, _Angle)))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CHungryUI::Free()
{
	CUI::Free();
}

void CHungryUI::AddCurntHp(const _float _value)
{


	if (_value < 0)
		return;

	if (m_fImageCount <= 0)
	{
		m_fImageCount = 0;
		return;
	}


	if (_value / 10 > m_fImageCount)
	{
		m_fImageCount = 0;
		return;
	}

	if (m_fImageCount <= 19.f)
	{
		m_fImageCount -= _value / 10.f;
	}

}

void CHungryUI::MinusCurntHp(const _float _value)
{
	if (_value < 0)
		return;

	if (m_fImageCount > m_fImageMaxCount)
	{
		m_fImageCount = m_fImageMaxCount;
		return;
	}
	if (_value / 10 > (m_fImageMaxCount - m_fImageCount))
	{
		m_fImageCount = m_fImageMaxCount;
		return;
	}




	if (m_fImageCount == 19)
		return;

	if (_value >= m_fImageMaxCount * 10.f)
	{
		m_fImageCount = 19.f;
		return;
	}

	if (m_fImageCount < 19.f)
	{
		m_fImageCount += _value / 10.f;
	}

}
