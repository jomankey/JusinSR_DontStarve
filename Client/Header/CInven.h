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
	//virtual HRESULT Ready_GameObject();
	virtual HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						override;

	

	

	CUI* Get_InvenSlot(int _index) { return m_vecInvenSlot[_index]; }
	void Push_Item(int _ItmeCount,const _tchar* _ItemName);
private:
	virtual HRESULT			Add_Component();
	virtual int UI_Collision();

	//æ∆¿Ã≈€ ƒ≠ ΩΩ∑‘ ¿ÃπÃ¡ˆ
	CUI* m_vecInvenSlot[15];


	const _tchar* FindItemName(const _tchar* _ItemName);
	int Find_ItemCount(const _tchar* _ItemName);
	void CallFind_ItemCount(function<void()>func,const _tchar* _ItemName);




	const _tchar* pTextureName[15] = {};
	int itemCount = 0;
	map<const _tchar*, int> m_mapItem;
	int CheckItemSlotCount = 0;
	//CItem* m_Item;
private :
	Engine::CTexture* m_pTextureCom[15];
	Engine::CRcTex* m_pBufferCom[15];
	Engine::CTransform* m_pTransformCom[15];

	POINT m_MousePoint;



	const _tchar* m_pUI_Name = nullptr;



	BOOL ItemCreate = false;
	BOOL ItemPut = false;

	float m_fX[15];
	float m_fY[15];


public:
	static CInven* Create(LPDIRECT3DDEVICE9	pGraphicDev, UI_STATE _State);

private:
	virtual void Free() override;
};
