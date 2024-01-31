#pragma once

#include "Engine_Define.h"
#include "Base.h"

namespace Engine 
{

	class ENGINE_DLL CGraphicDev : public CBase
	{
		DECLARE_SINGLETON(CGraphicDev)

	private:
		explicit CGraphicDev();
		virtual ~CGraphicDev();

	public:
		LPDIRECT3DDEVICE9		Get_GraphicDev() { return m_pGraphicDev; }

	public:
		HRESULT			Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _ulong& dwSizeX, const _ulong& dwSizeY, CGraphicDev** ppGraphicDev);

		void			Render_Begin(D3DXCOLOR Color);
		void			Render_End(void);

	private:
		LPDIRECT3D9			m_pSDK;
		LPDIRECT3DDEVICE9	m_pGraphicDev;

	private:
		virtual void		Free();
	};

}


