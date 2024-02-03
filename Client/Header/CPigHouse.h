#pragma once
#include "GameObject.h"
#include "ResObject.h"
namespace Engine {
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CPigHouse :
	public CResObject
{

	enum PIGHOUSE { STANDARD, ONLIGHT, RUNDOWN, PH_END };
private:
	explicit CPigHouse(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPigHouse(const CPigHouse& rhs);
	virtual ~CPigHouse();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						override;

private:
	HRESULT	Add_Component() override;
	void Change_Frame_Event() override;
	void Check_FrameState() override;
	void Ready_Stat() override;

private:
	PIGHOUSE  m_eHouseState;

public:
	static CResObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};