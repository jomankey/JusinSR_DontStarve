#pragma once
#include "GameObject.h"

class CUI;

namespace Engine
{

	class CRcTex;
	class CTexture;


	class CInven : public Engine::CGameObject
	{
	public:

	private:
		explicit CInven(LPDIRECT3DDEVICE9 pGraphicDev, UI_STATE eUIState);
		explicit CInven(const CInven& rhs);
		virtual ~CInven();

	public:
		virtual HRESULT Ready_GameObject();
		virtual HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 pGraphicDev);
		virtual _int Update_GameObject(const _float& fTimeDelta) override;
		virtual void LateUpdate_GameObject()					 override;
		virtual void Render_GameObject()						override;






		CUI* Get_InvenSlot(int _index) { return m_vecInvenSlot[_index]; }
	private:
		virtual HRESULT			Add_Component();
		CUI* m_vecInvenSlot[15];

	public:
		static CInven* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State);

	private:
		virtual void Free() override;
	};
}
