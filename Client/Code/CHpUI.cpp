
#include "CHpUI.h"



#include "Export_Utility.h"
#include "Export_System.h"

#include "Scene.h"

CHpUI::CHpUI(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState, const _tchar* _UI_Name)
	: CUI(pGraphicDev, eUIState, _UI_Name)
	, m_fMaxHp(0.f)
	, m_fCurentHp(0.f)
	, m_frame(0.f)
	, m_frameEnd(0.f)

{

}

CHpUI::CHpUI(const CHpUI& rhs)
	: CUI(rhs)
{

}

CHpUI::~CHpUI()
{
}

_int CHpUI::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);


	//if (!m_bFrameStop) {
	//	if (m_bHpPiusMInus) //+메소드
	//	{
	//		m_frame -= fTimeDelta;
	//		if (m_frame <= (m_PreFrame))
	//		{
	//			m_bFrameStop = true;
	//			m_frameEnd = 0.f;
	//		}
	//
	//	}
	//	else    //-메소드
	//	{
	//		m_frame += fTimeDelta;
	//		if (m_frame >= m_PreFrame)
	//		{
	//			m_bFrameStop = true;
	//			m_frameEnd = 0.f;
	//		}
	//	}
	//}


	if (GetAsyncKeyState('L')) // 공격
	{
		m_fImageCount += fTimeDelta*100.f;
		//MinusCurntHp(10);
	}
	return 0;
}

void CHpUI::Render_GameObject()
{

	scenemgr::Get_CurScene()->BeginOrtho();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_fImageCount);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());
	m_pBufferCom->Render_Buffer();



	scenemgr::Get_CurScene()->EndOrtho();

}

CHpUI* CHpUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE _State, _vec3 _pos, _vec3 _size, const _tchar* _UI_Name, float _Angle)
{
	CHpUI* pInstance = new CHpUI(pGraphicDev, _State, _UI_Name);
	if (FAILED(pInstance->Ready_GameObject(_pos, _size, _Angle)))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CHpUI::Free()
{
	CUI::Free();
}

void CHpUI::AddCurntHp(const _int _value)  //0이 풀피임
{



















	//m_fCurentHp += _value/10;
	//if (m_fMaxHp <= m_fCurentHp)
	//{
	//	m_fCurentHp = m_fMaxHp;
	//	return;
	//}
	//else
	//{
	//	m_fCurentHp += _value;
	//	m_frameEnd = _value/10;
	//	m_bHpPiusMInus = true;
	//}

}

void CHpUI::MinusCurntHp(const _int _value)
{















	//_float Scr = m_fCurentHp -_value;
	//m_fCurentHp -= _value;
	//
	//if (0 >= m_fCurentHp)
	//{
	//	m_fCurentHp = 0;
	//	return;
	//}
	//else
	//{
	//	m_bFrameStop = false;
	//	m_frameEnd = _value/10;
	//	m_bHpPiusMInus = true;
	//	m_PreFrame = m_frame + m_frameEnd;
	//}
}
