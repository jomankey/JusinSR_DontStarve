#pragma once
#include "GameObject.h"

namespace Engine 
{
class CRcTex;
class CTexture;
class CTransform;
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
		if (m_tItemInfo.ItemCount > _num)
			m_tItemInfo.ItemCount -= _num;
	}

	_bool			IsEquipment() { return m_tItemInfo.Equipment; }


	const ItemInfo& GetItemInfo() { return m_tItemInfo; }
	void			SetItemInfo(const ItemInfo& _itemInfo) { m_tItemInfo = _itemInfo; }
public:
	//순수가상함수
	virtual _bool UseItem() { return false; }
	virtual _bool DropItem() { return false; }

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject()override;
	virtual void		Render_GameObject()override;
	static CItem* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName);

private:
	HRESULT			Add_Component();
	virtual void MousePicking();

private:
	virtual void Free();

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;

private:
	ItemInfo	m_tItemInfo;		//아이템개수
};

