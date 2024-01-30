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
};

class CToolItem :
	public CGameObject
{
protected:
	explicit CToolItem(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _key, _vec3 _vPos);
	explicit CToolItem(const CToolItem& rhs);
	virtual ~CToolItem();

public:
	const wstring& GetItemKey() const { return m_strItemKey; }
	void AddItemCount(_uint _num) { m_tItemInfo.ItemCount += _num; }
	void MinusItemCount(_uint _num)
	{
		if (m_tItemInfo.ItemCount > _num)
			m_tItemInfo.ItemCount -= _num;
	}

	void SetPos(const _vec3& _vPos);

	const ItemInfo& GetItemInfo() { return m_tItemInfo; }
	void SetItemInfo(const ItemInfo& _itemInfo) { m_tItemInfo = _itemInfo; }
public:
	//순수가상함수
	virtual _bool UseItem() { return false; }
	virtual _bool DropItem() { return false; }

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject()override;
	virtual void		Render_GameObject()override;

	static CToolItem* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _key, _vec3 _vPos);

private:
	HRESULT			Add_Component();
	virtual void MousePicking();
	void Billboard();

private:
	virtual void Free();

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransForm;
	Engine::CTexture* m_pTextureCom;

private:
	wstring	m_strItemKey;//텍스처 키값 및 아이템 키값
	ItemInfo	m_tItemInfo;	//아이템개수
};

