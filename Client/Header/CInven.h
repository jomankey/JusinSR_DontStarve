#pragma once
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;

END
class CUI;
class CItem;



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

	
	virtual int UI_Collision();

	void Push_Item(CItem* _pItem,const _tchar* _ItemName);
	CUI* Get_InvenSlot(int _index) { return m_vecInvenSlot[_index]; }
private:
	virtual HRESULT			Add_Component();

	//æ∆¿Ã≈€ ƒ≠ ΩΩ∑‘ ¿ÃπÃ¡ˆ
	CUI* m_vecInvenSlot[15];

	CItem* m_Item;
private :

	Engine::CTexture* m_pTextureCom[15];
	Engine::CRcTex* m_pBufferCom[15];
	Engine::CTransform* m_pTransformCom[15];

	POINT m_MousePoint;
	const _tchar* m_pUI_Name;


	float m_fX[15];
	float m_fY[15];


public:
	static CInven* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State);

private:
	virtual void Free() override;
};
