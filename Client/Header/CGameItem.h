#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
END


class CGameItem :
	public CGameObject
{
private:
	explicit CGameItem(LPDIRECT3DDEVICE9 pGraphicDev,const _tchar* _key);
	explicit CGameItem(const CGameItem& rhs);
	virtual ~CGameItem();


public:
	const wstring& GetItemKey() { return m_strItemKey; }

	void AddItemCount(_uint _num) { m_iCount += _num; }

	void MinusItemCount(_uint _num)
	{
		if (m_iCount > _num)
			m_iCount -= _num;
		else
			assert(false, L"ItemCountOverflow");
	}

	void SetPos(const _vec3& _vPos);

public:
	static CGameItem* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						override;

private:
	HRESULT			Add_Component();

private:
	virtual void Free();

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	wstring	m_strItemKey;//텍스처 키값 및 아이템 키값
	_uint	m_iCount;	//아이템개수

private:

};

