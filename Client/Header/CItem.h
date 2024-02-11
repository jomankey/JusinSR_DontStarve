#pragma once
#include "GameObject.h"

namespace Engine 
{
class CRcTex;
class CTexture;
class CTransform;
class CCalculator;
}

struct ItemInfo
{
	_uint ItemCount;//아이템개수 or 장비의 내구도값
	_uint HP;
	_uint Hungry;
	_uint Mental;
	_uint Demage;
	_bool Equipment;
};

class CItem :
	public Engine::CGameObject
{
protected:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev,wstring _strObjName);
	explicit CItem(const CItem& rhs);
	virtual ~CItem();

public:
	void AddItemCount(_uint _num) { m_tItemInfo.ItemCount += _num; }
	void MinusItemCount(_uint _num)
	{
		if (m_tItemInfo.ItemCount >= _num)
			m_tItemInfo.ItemCount -= _num;
	}
	void Set_ItemCount(_uint _iNum) { m_tItemInfo.ItemCount = _iNum; }
	
	_bool			IsEquipment() { return m_tItemInfo.Equipment; }


	const ItemInfo& GetItemInfo() { return m_tItemInfo; }
	void			SetItemInfo(const ItemInfo& _itemInfo) { m_tItemInfo = _itemInfo; }

	_uint Get_ItemCount() { return m_tItemInfo.ItemCount; }

	_float Get_fX() { return m_fX; }
	_float Get_fY() { return m_fY; }
	_float Get_fSizeX() { return m_fSizeX; }
	_float Get_fSizeY() { return m_fSizeY; }

public:
	//순수가상함수
	virtual _bool UseItem() { return false; }
	virtual _bool DropItem() { return false; }

public:
	virtual HRESULT Ready_GameObject()			PURE			 ;
	virtual _int Update_GameObject(const _float& fTimeDelta) ;
	virtual void		LateUpdate_GameObject();
	virtual void		Render_GameObject() PURE;

protected:
	HRESULT			Add_Component();
	virtual void MousePicking();
	void Set_ObjState();

protected:
	virtual void Free();

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCalculator* m_pCalculatorCom;

protected:
	ItemInfo	m_tItemInfo;

	_matrix				m_ViewMatrix, m_ProjMatrix;
	_float m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float m_fPreX, m_fPreY;


};

