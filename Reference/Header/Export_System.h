#pragma once

#include "Engine_Define.h"
#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"

BEGIN(Engine)

// GraphicDev

inline HRESULT		Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _ulong& dwSizeX, const _ulong& dwSizeY, CGraphicDev** ppGraphicDev);
inline void			Render_Begin(D3DXCOLOR Color);
inline void			Render_End(void);

// TimerMgr

inline _float		Get_TimeDelta(const _tchar* pTimerTag);
inline void		Update_TimeDelta(const _tchar* pTimerTag);
inline HRESULT		Ready_Timer(const _tchar* pTimerTag);

// FrameMgr

inline _bool			IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
inline HRESULT			Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);

// Destroy
inline void			Release_System();


#include "Export_System.inl"

END