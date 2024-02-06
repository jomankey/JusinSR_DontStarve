#pragma once
#include "CUI.h"
#include "CInvenBox.h"
#include "Engine_Macro.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
}
class CInven : public CUI
{
private:
	explicit CInven(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInven();

public:
	virtual HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject() override;

public:
	static CUI* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring pName);

private:
	virtual void Free() override;


//public:
//	CUI* Get_InvenSlot(int _index) { return m_vecInvenSlot[_index]; }
//	void Push_Item(int _ItmeCount, const _tchar* _ItemName);
//
//private:
//	virtual int UI_Collision();
//	int Find_ItemCount(const _tchar* _ItemName);
//	void CallFind_ItemCount(function<void()>func,const _tchar* _ItemName);





private:
	//Engine::CTexture* m_pTextureCom[15];
	//Engine::CRcTex* m_pBufferCom[15];
	//Engine::CTransform* m_pTransformCom[15];


	//æ∆¿Ã≈€ ƒ≠ ΩΩ∑‘ ¿ÃπÃ¡ˆ
	//CUI* m_vecInvenSlot[15];

	//map<const _tchar*, int> m_mapItem;
	//const _tchar* m_pUI_Name = nullptr;
	//const _tchar* pTextureName[15] = {};


	//POINT m_MousePoint;

	//BOOL ItemCreate = false;
	//BOOL ItemPut = false;

	//float m_fX[15];
	//float m_fY[15];

	//int itemCount = 0;

	CInvenBox* m_pInvenBox[INVENCNT];
};
