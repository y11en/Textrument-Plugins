#include "StdAfx.h"
#include "UIScrollBar.h"
#include "WindowsEx/scrollbar.h"

#include "Core/InsituDebug.h"
namespace DuiLib
{
	IMPLEMENT_QKCONTROL(CScrollBarUI)

	CScrollBarUI::CScrollBarUI() : m_bHorizontal(false), m_nRange(0), m_nScrollPos(0), m_nLineSize(8), 
		m_pOwner(NULL), m_nLastScrollPos(0), m_nLastScrollOffset(0), m_nScrollRepeatDelay(0), m_uButton1State(0), \
		m_uButton2State(0), m_uThumbState(0), m_bShowButton1(true), m_bShowButton2(true)
		, _lastMouseBias(0)
	{
		m_cxyFixed.cx = DEFAULT_SCROLLBAR_SIZE;
		m_ptLastMouse.x = m_ptLastMouse.y = 0;
		_view_states &= ~VIEWSTATEMASK_Focusable;
		::ZeroMemory(&m_rcThumb, sizeof(m_rcThumb));
		::ZeroMemory(&m_rcButton1, sizeof(m_rcButton1));
		::ZeroMemory(&m_rcButton2, sizeof(m_rcButton2));
	}

	LPCTSTR CScrollBarUI::GetClass() const
	{
		return _T("ScrollBarUI");
	}

	LPVOID CScrollBarUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_SCROLLBAR) == 0 ) return static_cast<CScrollBarUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	CContainerUI* CScrollBarUI::GetOwner() const
	{
		return m_pOwner;
	}

	void CScrollBarUI::SetOwner(CContainerUI* pOwner)
	{
		m_pOwner = pOwner;
	}

	void CScrollBarUI::SetVisible(bool bVisible)
	{
		if( bVisible != m_bVisible ) 
		{
			if(bVisible) _view_states |= VIEWSTATEMASK_Visibility;
			else _view_states &= ~VIEWSTATEMASK_Visibility;

			if( m_bFocused ) m_bFocused_NO;
		}
	}

	void CScrollBarUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if( !IsEnabled() ) {
			m_uButton1State = 0;
			m_uButton2State = 0;
			m_uThumbState = 0;
		}
	}

	void CScrollBarUI::SetFocus()
	{
		if( m_pOwner && !m_pOwner->HasFocus() ) 
			m_pOwner->SetFocus();
	}

	bool CScrollBarUI::IsHorizontal()
	{
		return m_bHorizontal;
	}

	void CScrollBarUI::SetHorizontal(bool bHorizontal)
	{
		if( m_bHorizontal == bHorizontal ) return;

		m_bHorizontal = bHorizontal;
		if( m_bHorizontal ) {
			if( m_cxyFixed.cy == 0 ) {
				m_cxyFixed.cx = 0;
				m_cxyFixed.cy = DEFAULT_SCROLLBAR_SIZE;
			}
		}
		else {
			if( m_cxyFixed.cx == 0 ) {
				m_cxyFixed.cx = DEFAULT_SCROLLBAR_SIZE;
				m_cxyFixed.cy = 0;
			}
		}

		if( m_pOwner != NULL ) m_pOwner->NeedUpdate(); else NeedParentUpdate();
	}

	int CScrollBarUI::GetScrollRange() const
	{
		return m_nRange;
	}

	void CScrollBarUI::SetScrollRange(int nRange)
	{
		if( m_nRange == nRange ) return;

		m_nRange = nRange;
		if( m_nRange < 0 ) m_nRange = 0;
		if( m_nScrollPos > m_nRange ) m_nScrollPos = m_nRange;
		SetPos(m_rcItem);
	}

	int CScrollBarUI::GetScrollPos() const
	{
		return m_nScrollPos;
	}

	void CScrollBarUI::SetScrollPos(int nPos)
	{
		if( m_nScrollPos == nPos ) return;

		if( nPos < 0 ) nPos = 0;
		//if( nPos > m_nRange ) LogIs("nPos > m_nRange:: %ld, %ld", nPos, m_nRange);
		if( nPos > m_nRange ) nPos = m_nRange;

		if( m_nScrollPos == nPos ) return;

		m_nScrollPos = nPos;
		SetPos(m_rcItem);
	}

	void CScrollBarUI::SetScrollRangeAndPos(int nRange, int nPos)
	{
		if( nRange < 0 ) nRange = 0;
		if( nPos < 0 ) nPos = 0;
		if( nPos > nRange ) nPos = nRange;

		if( m_nRange != nRange || m_nScrollPos != nPos ) {
			m_nRange = nRange;
			m_nScrollPos = nPos;
			SetPos(m_rcItem);
		}
	}

	int CScrollBarUI::GetLineSize() const
	{
		return m_nLineSize;
	}

	void CScrollBarUI::SetLineSize(int nSize)
	{
		m_nLineSize = nSize;
	}

	bool CScrollBarUI::GetShowButton1()
	{
		return m_bShowButton1;
	}

	void CScrollBarUI::SetShowButton1(bool bShow)
	{
		m_bShowButton1 = bShow;
		SetPos(m_rcItem);
	}

	LPCTSTR CScrollBarUI::GetButton1NormalImage()
	{
		return m_sButton1NormalImage;
	}

	void CScrollBarUI::SetButton1NormalImage(LPCTSTR pStrImage)
	{
		m_sButton1NormalImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetButton1HotImage()
	{
		return m_sButton1HotImage;
	}

	void CScrollBarUI::SetButton1HotImage(LPCTSTR pStrImage)
	{
		m_sButton1HotImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetButton1PushedImage()
	{
		return m_sButton1PushedImage;
	}

	void CScrollBarUI::SetButton1PushedImage(LPCTSTR pStrImage)
	{
		m_sButton1PushedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetButton1DisabledImage()
	{
		return m_sButton1DisabledImage;
	}

	void CScrollBarUI::SetButton1DisabledImage(LPCTSTR pStrImage)
	{
		m_sButton1DisabledImage = pStrImage;
		Invalidate();
	}

	bool CScrollBarUI::GetShowButton2()
	{
		return m_bShowButton2;
	}

	void CScrollBarUI::SetShowButton2(bool bShow)
	{
		m_bShowButton2 = bShow;
		SetPos(m_rcItem);
	}

	LPCTSTR CScrollBarUI::GetButton2NormalImage()
	{
		return m_sButton2NormalImage;
	}

	void CScrollBarUI::SetButton2NormalImage(LPCTSTR pStrImage)
	{
		m_sButton2NormalImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetButton2HotImage()
	{
		return m_sButton2HotImage;
	}

	void CScrollBarUI::SetButton2HotImage(LPCTSTR pStrImage)
	{
		m_sButton2HotImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetButton2PushedImage()
	{
		return m_sButton2PushedImage;
	}

	void CScrollBarUI::SetButton2PushedImage(LPCTSTR pStrImage)
	{
		m_sButton2PushedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetButton2DisabledImage()
	{
		return m_sButton2DisabledImage;
	}

	void CScrollBarUI::SetButton2DisabledImage(LPCTSTR pStrImage)
	{
		m_sButton2DisabledImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetThumbNormalImage()
	{
		return m_sThumbNormalImage;
	}

	void CScrollBarUI::SetThumbNormalImage(LPCTSTR pStrImage)
	{
		m_sThumbNormalImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetThumbHotImage()
	{
		return m_sThumbHotImage;
	}

	void CScrollBarUI::SetThumbHotImage(LPCTSTR pStrImage)
	{
		m_sThumbHotImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetThumbPushedImage()
	{
		return m_sThumbPushedImage;
	}

	void CScrollBarUI::SetThumbPushedImage(LPCTSTR pStrImage)
	{
		m_sThumbPushedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetThumbDisabledImage()
	{
		return m_sThumbDisabledImage;
	}

	void CScrollBarUI::SetThumbDisabledImage(LPCTSTR pStrImage)
	{
		m_sThumbDisabledImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetRailNormalImage()
	{
		return m_sRailNormalImage;
	}

	void CScrollBarUI::SetRailNormalImage(LPCTSTR pStrImage)
	{
		m_sRailNormalImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetRailHotImage()
	{
		return m_sRailHotImage;
	}

	void CScrollBarUI::SetRailHotImage(LPCTSTR pStrImage)
	{
		m_sRailHotImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetRailPushedImage()
	{
		return m_sRailPushedImage;
	}

	void CScrollBarUI::SetRailPushedImage(LPCTSTR pStrImage)
	{
		m_sRailPushedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetRailDisabledImage()
	{
		return m_sRailDisabledImage;
	}

	void CScrollBarUI::SetRailDisabledImage(LPCTSTR pStrImage)
	{
		m_sRailDisabledImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetBkNormalImage()
	{
		return m_sBkNormalImage;
	}

	void CScrollBarUI::SetBkNormalImage(LPCTSTR pStrImage)
	{
		m_sBkNormalImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetBkHotImage()
	{
		return m_sBkHotImage;
	}

	void CScrollBarUI::SetBkHotImage(LPCTSTR pStrImage)
	{
		m_sBkHotImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetBkPushedImage()
	{
		return m_sBkPushedImage;
	}

	void CScrollBarUI::SetBkPushedImage(LPCTSTR pStrImage)
	{
		m_sBkPushedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CScrollBarUI::GetBkDisabledImage()
	{
		return m_sBkDisabledImage;
	}

	void CScrollBarUI::SetBkDisabledImage(LPCTSTR pStrImage)
	{
		m_sBkDisabledImage = pStrImage;
		Invalidate();
	}

	void CScrollBarUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		CControlUI::SetPos(rc, bNeedInvalidate);
		SIZE cxyFixed = m_cxyFixed;
		if (_manager != NULL) {
			GetManager()->GetDPIObj()->Scale(&cxyFixed);
		}
		rc = m_rcItem;
		if( m_bHorizontal ) {
			int cx = rc.right - rc.left;
			if( m_bShowButton1 ) cx -= cxyFixed.cy;
			if( m_bShowButton2 ) cx -= cxyFixed.cy;
			if( cx > cxyFixed.cy ) {
				m_rcButton1.left = rc.left;
				m_rcButton1.top = rc.top;
				if( m_bShowButton1 ) {
					m_rcButton1.right = rc.left + cxyFixed.cy;
					m_rcButton1.bottom = rc.top + cxyFixed.cy;
				}
				else {
					m_rcButton1.right = m_rcButton1.left;
					m_rcButton1.bottom = m_rcButton1.top;
				}

				m_rcButton2.top = rc.top;
				m_rcButton2.right = rc.right;
				if( m_bShowButton2 ) {
					m_rcButton2.left = rc.right - cxyFixed.cy;
					m_rcButton2.bottom = rc.top + cxyFixed.cy;
				}
				else {
					m_rcButton2.left = m_rcButton2.right;
					m_rcButton2.bottom = m_rcButton2.top;
				}

				m_rcThumb.top = rc.top;
				m_rcThumb.bottom = rc.top + cxyFixed.cy;
				if( m_nRange > 0 ) {
					int cxThumb = cx * (rc.right - rc.left) / (m_nRange + rc.right - rc.left);
					if( cxThumb < cxyFixed.cy ) cxThumb = cxyFixed.cy;

					m_rcThumb.left = m_nScrollPos * (cx - cxThumb) / m_nRange + m_rcButton1.right;
					m_rcThumb.right = m_rcThumb.left + cxThumb;
					if( m_rcThumb.right > m_rcButton2.left ) {
						m_rcThumb.left = m_rcButton2.left - cxThumb;
						m_rcThumb.right = m_rcButton2.left;
					}
				}
				else {
					m_rcThumb.left = m_rcButton1.right;
					m_rcThumb.right = m_rcButton2.left;
				}
			}
			else {
				int cxButton = (rc.right - rc.left) / 2;
				if( cxButton > cxyFixed.cy ) cxButton = cxyFixed.cy;
				m_rcButton1.left = rc.left;
				m_rcButton1.top = rc.top;
				if( m_bShowButton1 ) {
					m_rcButton1.right = rc.left + cxButton;
					m_rcButton1.bottom = rc.top + cxyFixed.cy;
				}
				else {
					m_rcButton1.right = m_rcButton1.left;
					m_rcButton1.bottom = m_rcButton1.top;
				}

				m_rcButton2.top = rc.top;
				m_rcButton2.right = rc.right;
				if( m_bShowButton2 ) {
					m_rcButton2.left = rc.right - cxButton;
					m_rcButton2.bottom = rc.top + cxyFixed.cy;
				}
				else {
					m_rcButton2.left = m_rcButton2.right;
					m_rcButton2.bottom = m_rcButton2.top;
				}

				::ZeroMemory(&m_rcThumb, sizeof(m_rcThumb));
			}
		}
		else {
			int cy = rc.bottom - rc.top;
			if( m_bShowButton1 ) cy -= cxyFixed.cx;
			if( m_bShowButton2 ) cy -= cxyFixed.cx;
			if( cy > cxyFixed.cx ) {
				m_rcButton1.left = rc.left;
				m_rcButton1.top = rc.top;
				if( m_bShowButton1 ) {
					m_rcButton1.right = rc.left + cxyFixed.cx;
					m_rcButton1.bottom = rc.top + cxyFixed.cx;
				}
				else {
					m_rcButton1.right = m_rcButton1.left;
					m_rcButton1.bottom = m_rcButton1.top;
				}

				m_rcButton2.left = rc.left;
				m_rcButton2.bottom = rc.bottom;
				if( m_bShowButton2 ) {
					m_rcButton2.top = rc.bottom - cxyFixed.cx;
					m_rcButton2.right = rc.left + cxyFixed.cx;
				}
				else {
					m_rcButton2.top = m_rcButton2.bottom;
					m_rcButton2.right = m_rcButton2.left;
				}

				m_rcThumb.left = rc.left;
				m_rcThumb.right = rc.left + cxyFixed.cx;
				if( m_nRange > 0 ) {
					int cyThumb = cy * (rc.bottom - rc.top) / (m_nRange + rc.bottom - rc.top);
					if( cyThumb < cxyFixed.cx ) cyThumb = cxyFixed.cx;

					m_rcThumb.top = (m_nScrollPos * 1.0f / m_nRange) * (cy - cyThumb) + m_rcButton1.bottom;
					m_rcThumb.bottom = m_rcThumb.top + cyThumb;
					if( m_rcThumb.bottom > m_rcButton2.top ) {
						m_rcThumb.top = m_rcButton2.top - cyThumb;
						m_rcThumb.bottom = m_rcButton2.top;
					}
				}
				else {
					m_rcThumb.top = m_rcButton1.bottom;
					m_rcThumb.bottom = m_rcButton2.top;
				}
			}
			else {
				int cyButton = (rc.bottom - rc.top) / 2;
				if( cyButton > cxyFixed.cx ) cyButton = cxyFixed.cx;
				m_rcButton1.left = rc.left;
				m_rcButton1.top = rc.top;
				if( m_bShowButton1 ) {
					m_rcButton1.right = rc.left + cxyFixed.cx;
					m_rcButton1.bottom = rc.top + cyButton;
				}
				else {
					m_rcButton1.right = m_rcButton1.left;
					m_rcButton1.bottom = m_rcButton1.top;
				}

				m_rcButton2.left = rc.left;
				m_rcButton2.bottom = rc.bottom;
				if( m_bShowButton2 ) {
					m_rcButton2.top = rc.bottom - cyButton;
					m_rcButton2.right = rc.left + cxyFixed.cx;
				}
				else {
					m_rcButton2.top = m_rcButton2.bottom;
					m_rcButton2.right = m_rcButton2.left;
				}

				::ZeroMemory(&m_rcThumb, sizeof(m_rcThumb));
			}
		}
	}

//#define IMM_DRAG_MODE
	void CScrollBarUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pOwner != NULL ) m_pOwner->DoEvent(event);
			else CControlUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_SETFOCUS ) 
		{
			return;
		}
		if( event.Type == UIEVENT_KILLFOCUS ) 
		{
			return;
		}
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
		{
			if( !IsEnabled() ) return;

			m_nLastScrollOffset = 0;
			m_nScrollRepeatDelay = 0;

			if( ::PtInRect(&m_rcButton1, event.ptMouse) )
			{
				m_uButton1State |= UISTATE_PUSHED;
				if( !m_bHorizontal ) {
					if( m_pOwner != NULL ) m_pOwner->LineUp(); 
					else SetScrollPos(m_nScrollPos - m_nLineSize);
				}
				else {
					if( m_pOwner != NULL ) m_pOwner->LineLeft(); 
					else SetScrollPos(m_nScrollPos - m_nLineSize);
				}
				SetTimer(DEFAULT_TIMERID, 50);
			}
			else if( ::PtInRect(&m_rcButton2, event.ptMouse) ) 
			{
				m_uButton2State |= UISTATE_PUSHED;
				if( !m_bHorizontal ) {
					if( m_pOwner != NULL ) m_pOwner->LineDown(); 
					else SetScrollPos(m_nScrollPos + m_nLineSize);
				}
				else {
					if( m_pOwner != NULL ) m_pOwner->LineRight(); 
					else SetScrollPos(m_nScrollPos + m_nLineSize);
				}
				SetTimer(DEFAULT_TIMERID, 50);
			}
			else if( ::PtInRect(&m_rcThumb, event.ptMouse) ) 
			{
				m_uThumbState |= UISTATE_CAPTURED | UISTATE_PUSHED;
				m_ptLastMouse = event.ptMouse;
				m_nLastScrollPos = m_nScrollPos;
#ifndef IMM_DRAG_MODE
				SetTimer(DEFAULT_TIMERID, 10);
				// 120U 分页快速滚动
#endif
			}
			else 
			{
				if (true || (event.wParam&MK_SHIFT))
				{
					// 直接滚动
					m_uThumbState |= UISTATE_CAPTURED | UISTATE_PUSHED;
					m_ptLastMouse = event.ptMouse;
					__int64 scrollPos;
					if (m_bHorizontal)
					{
						scrollPos = GetScrollRange()*1.0*(m_ptLastMouse.x-m_rcItem.left+(m_rcButton1.left - m_rcButton1.right))/(m_rcButton2.left - m_rcButton1.right);
						if( m_pOwner != NULL ) m_pOwner->SetScrollPos(CDuiSize(scrollPos, m_pOwner->GetScrollPos().cy), true, true); 
						else SetScrollPos(scrollPos);
					}
					else
					{
						scrollPos = GetScrollRange()*1.0*(m_ptLastMouse.y-m_rcItem.top+(m_rcButton1.top - m_rcButton1.bottom))/(m_rcButton2.top - m_rcButton1.bottom);
						if( m_pOwner != NULL ) m_pOwner->SetScrollPos(CDuiSize(m_pOwner->GetScrollPos().cx, scrollPos), true, true); 
						else SetScrollPos(scrollPos);
					}
					m_nLastScrollPos = scrollPos;
					SetTimer(DEFAULT_TIMERID, 15);
				}
				else
				{
					if( !m_bHorizontal )
					{
						if( event.ptMouse.y < m_rcThumb.top ) 
						{
							if( m_pOwner != NULL ) m_pOwner->PageUp(); 
							else SetScrollPos(m_nScrollPos + m_rcItem.top - m_rcItem.bottom);
						}
						else if ( event.ptMouse.y > m_rcThumb.bottom )
						{
							if( m_pOwner != NULL ) m_pOwner->PageDown(); 
							else SetScrollPos(m_nScrollPos - m_rcItem.top + m_rcItem.bottom);                    
						}
					}
					else 
					{
						if( event.ptMouse.x < m_rcThumb.left ) 
						{
							if( m_pOwner != NULL ) m_pOwner->PageLeft(); 
							else SetScrollPos(m_nScrollPos + m_rcItem.left - m_rcItem.right);
						}
						else if ( event.ptMouse.x > m_rcThumb.right )
						{
							if( m_pOwner != NULL ) m_pOwner->PageRight(); 
							else SetScrollPos(m_nScrollPos - m_rcItem.left + m_rcItem.right);                    
						}
					}
					SetTimer(DEFAULT_TIMERID, 50);
				}
			}
			if( _manager != NULL) _manager->SendNotify(this, DUI_MSGTYPE_SCROLL);
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP )
		{
			m_nScrollRepeatDelay = 0;
			m_nLastScrollOffset = 0;
			_lastMouseBias = 0;
			if(!_manager->GetRealManager()->_hackTimer)
			KillTimer(DEFAULT_TIMERID);

			if( m_uThumbState & UISTATE_CAPTURED ) {
				m_uThumbState &= ~( UISTATE_CAPTURED | UISTATE_PUSHED );
				Invalidate();
			}
			else if( m_uButton1State & UISTATE_PUSHED ) {
				m_uButton1State &= ~UISTATE_PUSHED;
				Invalidate();
			}
			else if( m_uButton2State & UISTATE_PUSHED ) {
				m_uButton2State &= ~UISTATE_PUSHED;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE )
		{
			if( (m_uThumbState & UISTATE_CAPTURED) != 0 ) {
				if( !m_bHorizontal ) {
					__int64 fMouseRange = (event.ptMouse.y - m_ptLastMouse.y + _lastMouseBias) * m_nRange;
					int nBtnSize = 0;
					if(GetShowButton1()) nBtnSize += m_cxyFixed.cx;
					if(GetShowButton2()) nBtnSize += m_cxyFixed.cx;
					int vRange = m_rcItem.bottom - m_rcItem.top - (m_rcThumb.bottom - m_rcThumb.top) - nBtnSize;
					if (vRange != 0){
						m_nLastScrollOffset = fMouseRange / abs(vRange);
					}
#ifdef IMM_DRAG_MODE
					if( m_pOwner != NULL ) m_pOwner->SetScrollPos(CDuiSize(m_pOwner->GetScrollPos().cx, \
						m_nLastScrollPos + m_nLastScrollOffset)); 
					else SetScrollPos(m_nLastScrollPos + m_nLastScrollOffset);
#endif
				}
				else {
					__int64 fMouseRange = (event.ptMouse.x - m_ptLastMouse.x + _lastMouseBias) * m_nRange;
					int nBtnSize = 0;
					if(GetShowButton1()) nBtnSize += m_cxyFixed.cy;
					if(GetShowButton2()) nBtnSize += m_cxyFixed.cy;
					int hRange = m_rcItem.right - m_rcItem.left - m_rcThumb.right + m_rcThumb.left - nBtnSize;
					if (hRange != 0) m_nLastScrollOffset = fMouseRange / abs(hRange);
				}
			}
			else {
				if( (m_uThumbState & UISTATE_HOT) != 0 ) {
					if( !::PtInRect(&m_rcThumb, event.ptMouse) ) {
						m_uThumbState &= ~UISTATE_HOT;
						Invalidate();
					}
				}
				else {
					if( !IsEnabled() ) return;
					if( ::PtInRect(&m_rcThumb, event.ptMouse) ) {
						m_uThumbState |= UISTATE_HOT;
						Invalidate();
					}
				}
			}
			return;
		}
		if( event.Type == UIEVENT_CONTEXTMENU )
		{
			// todo 菜单
			return;
		}
		if( event.Type == UIEVENT_TIMER && event.wParam == DEFAULT_TIMERID )
		{
			if(m_uThumbState & UISTATE_CAPTURED)
			{
				if( !m_bHorizontal ) {
					if( m_pOwner != NULL ) m_pOwner->SetScrollPos(CDuiSize(m_pOwner->GetScrollPos().cx
						, m_nLastScrollPos + m_nLastScrollOffset), true, true); 
					else SetScrollPos(m_nLastScrollPos + m_nLastScrollOffset);
				}
				else {
					if( m_pOwner != NULL ) m_pOwner->SetScrollPos(CDuiSize(m_nLastScrollPos + m_nLastScrollOffset
						, m_pOwner->GetScrollPos().cy), true, true); 
					else SetScrollPos(m_nLastScrollPos + m_nLastScrollOffset);
				}
				Invalidate();
			}
			else if(m_uButton1State & UISTATE_PUSHED) 
			{
				if( ++m_nScrollRepeatDelay <= 5 ) return;
				if( !m_bHorizontal ) {
					if( m_pOwner != NULL ) m_pOwner->LineUp(); 
					else SetScrollPos(m_nScrollPos - m_nLineSize);
				}
				else {
					if( m_pOwner != NULL ) m_pOwner->LineLeft(); 
					else SetScrollPos(m_nScrollPos - m_nLineSize);
				}
			}
			else if(m_uButton2State & UISTATE_PUSHED) 
			{
				if( ++m_nScrollRepeatDelay <= 5 ) return;
				if( !m_bHorizontal ) {
					if( m_pOwner != NULL ) m_pOwner->LineDown(); 
					else SetScrollPos(m_nScrollPos + m_nLineSize);
				}
				else {
					if( m_pOwner != NULL ) m_pOwner->LineRight(); 
					else SetScrollPos(m_nScrollPos + m_nLineSize);
				}
			}
			else {
				if( ++m_nScrollRepeatDelay <= 5 ) return;
				POINT pt = { 0 };
				::GetCursorPos(&pt);
				::ScreenToClient(_manager->GetPaintWindow(), &pt);
				if( !m_bHorizontal ) {
					if( pt.y < m_rcThumb.top ) {
						if( m_pOwner != NULL ) m_pOwner->PageUp(); 
						else SetScrollPos(m_nScrollPos + m_rcItem.top - m_rcItem.bottom);
					}
					else if ( pt.y > m_rcThumb.bottom ){
						if( m_pOwner != NULL ) m_pOwner->PageDown(); 
						else SetScrollPos(m_nScrollPos - m_rcItem.top + m_rcItem.bottom);                    
					}
				}
				else {
					if( pt.x < m_rcThumb.left ) {
						if( m_pOwner != NULL ) m_pOwner->PageLeft(); 
						else SetScrollPos(m_nScrollPos + m_rcItem.left - m_rcItem.right);
					}
					else if ( pt.x > m_rcThumb.right ){
						if( m_pOwner != NULL ) m_pOwner->PageRight(); 
						else SetScrollPos(m_nScrollPos - m_rcItem.left + m_rcItem.right);                    
					}
				}
			}
			if( _manager != NULL ) _manager->SendNotify(this, DUI_MSGTYPE_SCROLL);
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			if( IsEnabled() ) {
				m_uButton1State |= UISTATE_HOT;
				m_uButton2State |= UISTATE_HOT;
				if( ::PtInRect(&m_rcThumb, event.ptMouse) ) m_uThumbState |= UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( IsEnabled() ) {
				m_uButton1State &= ~UISTATE_HOT;
				m_uButton2State &= ~UISTATE_HOT;
				m_uThumbState &= ~UISTATE_HOT;
				Invalidate();
			}
			return;
		}

		if( m_pOwner != NULL ) m_pOwner->DoEvent(event); else CControlUI::DoEvent(event);
	}

	void CScrollBarUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("button1normalimage")) == 0 ) SetButton1NormalImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("button1hotimage")) == 0 ) SetButton1HotImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("button1pushedimage")) == 0 ) SetButton1PushedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("button1disabledimage")) == 0 ) SetButton1DisabledImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("button2normalimage")) == 0 ) SetButton2NormalImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("button2hotimage")) == 0 ) SetButton2HotImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("button2pushedimage")) == 0 ) SetButton2PushedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("button2disabledimage")) == 0 ) SetButton2DisabledImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("thumbnormalimage")) == 0 ) SetThumbNormalImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("thumbhotimage")) == 0 ) SetThumbHotImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("thumbpushedimage")) == 0 ) SetThumbPushedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("thumbdisabledimage")) == 0 ) SetThumbDisabledImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("railnormalimage")) == 0 ) SetRailNormalImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("railhotimage")) == 0 ) SetRailHotImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("railpushedimage")) == 0 ) SetRailPushedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("raildisabledimage")) == 0 ) SetRailDisabledImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("bknormalimage")) == 0 ) SetBkNormalImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("bkhotimage")) == 0 ) SetBkHotImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("bkpushedimage")) == 0 ) SetBkPushedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("bkdisabledimage")) == 0 ) SetBkDisabledImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("hor")) == 0 ) SetHorizontal(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("linesize")) == 0 ) SetLineSize(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("range")) == 0 ) SetScrollRange(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("value")) == 0 ) SetScrollPos(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("showbutton1")) == 0 ) SetShowButton1(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("showbutton2")) == 0 ) SetShowButton2(_tcsicmp(pstrValue, _T("true")) == 0);
		else CControlUI::SetAttribute(pstrName, pstrValue);
	}

	bool CScrollBarUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
	{
		if (m_sThumbNormalImage.IsEmpty())
		{
			UINT hot_push_mask = UISTATE_HOT | UISTATE_PUSHED;
			UINT pushState = m_uThumbState & hot_push_mask;
			SCROLL_HITTEST hit_item = pushState?SCROLL_THUMB 
				: ((pushState=m_uButton1State) & UISTATE_PUSHED)?SCROLL_TOP_ARROW
				: ((pushState=m_uButton2State) & UISTATE_PUSHED)?SCROLL_BOTTOM_ARROW
				: SCROLL_NOWHERE;
			if (m_bHorizontal)
			{
				UXTHEME_ScrollBarDraw(GetHWND(), hDC, 0
					, hit_item
					, pushState & UISTATE_PUSHED
					, true, true, &m_rcItem, m_rcButton1.right-m_rcButton1.left, m_rcThumb.left - m_rcItem.left
					, m_rcThumb.right-m_rcThumb.left, false);
			}
			else
			{
				UXTHEME_ScrollBarDraw(GetHWND(), hDC, 0
					, hit_item
					, pushState & UISTATE_PUSHED
					, true, true, &m_rcItem, m_rcButton1.bottom-m_rcButton1.top, m_rcThumb.top - m_rcItem.top
					, m_rcThumb.bottom-m_rcThumb.top, true);
			}
		}
		else
		{
			PaintBkColor(hDC);
			PaintBkImage(hDC);
			PaintBk(hDC);
			PaintButton1(hDC);
			PaintButton2(hDC);
			PaintThumb(hDC);
			PaintRail(hDC);
			PaintBorder(hDC);
		}
		return true;
	}

	void CScrollBarUI::PaintBk(HDC hDC)
	{
		if( !IsEnabled() ) m_uThumbState |= UISTATE_DISABLED;
		else m_uThumbState &= ~ UISTATE_DISABLED;

		if( (m_uThumbState & UISTATE_DISABLED) != 0 ) {
			if( !m_sBkDisabledImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sBkDisabledImage) ) {}
				else return;
			}
		}
		else if( (m_uThumbState & UISTATE_PUSHED) != 0 ) {
			if( !m_sBkPushedImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sBkPushedImage) ) {}
				else return;
			}
		}
		else if( (m_uThumbState & UISTATE_HOT) != 0 ) {
			if( !m_sBkHotImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sBkHotImage) ) {}
				else return;
			}
		}

		if( !m_sBkNormalImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sBkNormalImage) ) {}
			else return;
		}
	}

	void CScrollBarUI::PaintButton1(HDC hDC)
	{
		if( !m_bShowButton1 ) return;

		if( !IsEnabled() ) m_uButton1State |= UISTATE_DISABLED;
		else m_uButton1State &= ~ UISTATE_DISABLED;

		drawableTmp.left   = MulDiv(m_rcButton1.left - m_rcItem.left, 100, GetManager()->GetDPIObj()->GetScale());
		drawableTmp.top    = MulDiv(m_rcButton1.top - m_rcItem.top, 100, GetManager()->GetDPIObj()->GetScale());
		drawableTmp.right  = MulDiv(m_rcButton1.right - m_rcItem.left, 100, GetManager()->GetDPIObj()->GetScale());
		drawableTmp.bottom = MulDiv(m_rcButton1.bottom - m_rcItem.top, 100, GetManager()->GetDPIObj()->GetScale());
		//m_sImageModify.Empty();
		//m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), d1, d2, d3, d4);
		

		if( (m_uButton1State & UISTATE_DISABLED) != 0 ) {
			if( !m_sButton1DisabledImage.IsEmpty() ) {
				if( !DrawImageAt(hDC, (LPCTSTR)m_sButton1DisabledImage, &drawableTmp) ) {}
				else return;
			}
		}
		else if( (m_uButton1State & UISTATE_PUSHED) != 0 ) {
			if( !m_sButton1PushedImage.IsEmpty() ) {
				if( !DrawImageAt(hDC, (LPCTSTR)m_sButton1PushedImage, &drawableTmp) ) {}
				else return;
			}
		}
		else if( (m_uButton1State & UISTATE_HOT) != 0 ) {
			if( !m_sButton1HotImage.IsEmpty() ) {
				if( !DrawImageAt(hDC, (LPCTSTR)m_sButton1HotImage, &drawableTmp) ) {}
				else return;
			}
		}

		if( !m_sButton1NormalImage.IsEmpty() ) {
			if( !DrawImageAt(hDC, (LPCTSTR)m_sButton1NormalImage, &drawableTmp) ) {}
			else return;
		}

		DWORD dwBorderColor = 0xFF85E4FF;
		int nBorderSize = 2;
		CRenderEngine::DrawRect(hDC, m_rcButton1, nBorderSize, dwBorderColor);
	}

	void CScrollBarUI::PaintButton2(HDC hDC)
	{
		if( !m_bShowButton2 ) return;

		if( !IsEnabled() ) m_uButton2State |= UISTATE_DISABLED;
		else m_uButton2State &= ~ UISTATE_DISABLED;
		int d1 = MulDiv(m_rcButton2.left - m_rcItem.left, 100, GetManager()->GetDPIObj()->GetScale());
		int d2 = MulDiv(m_rcButton2.top - m_rcItem.top, 100, GetManager()->GetDPIObj()->GetScale());
		int d3 = MulDiv(m_rcButton2.right - m_rcItem.left, 100, GetManager()->GetDPIObj()->GetScale());
		int d4 = MulDiv(m_rcButton2.bottom - m_rcItem.top, 100, GetManager()->GetDPIObj()->GetScale());
		//m_sImageModify.Empty();
		//m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"),d1 ,d2 ,d3 ,d4 );

		if( (m_uButton2State & UISTATE_DISABLED) != 0 ) {
			if( !m_sButton2DisabledImage.IsEmpty() ) {
				if( !DrawImageAt(hDC, (LPCTSTR)m_sButton2DisabledImage, &drawableTmp) ) {}
				else return;
			}
		}
		else if( (m_uButton2State & UISTATE_PUSHED) != 0 ) {
			if( !m_sButton2PushedImage.IsEmpty() ) {
				if( !DrawImageAt(hDC, (LPCTSTR)m_sButton2PushedImage, &drawableTmp) ) {}
				else return;
			}
		}
		else if( (m_uButton2State & UISTATE_HOT) != 0 ) {
			if( !m_sButton2HotImage.IsEmpty() ) {
				if( !DrawImageAt(hDC, (LPCTSTR)m_sButton2HotImage, &drawableTmp) ) {}
				else return;
			}
		}

		if( !m_sButton2NormalImage.IsEmpty() ) {
			if( !DrawImageAt(hDC, (LPCTSTR)m_sButton2NormalImage, &drawableTmp) ){}
			else return;
		}

		DWORD dwBorderColor = 0xFF85E4FF;
		int nBorderSize = 2;
		CRenderEngine::DrawRect(hDC, m_rcButton2, nBorderSize, dwBorderColor);
	}

	void CScrollBarUI::PaintThumb(HDC hDC)
	{
		if( m_rcThumb.left == 0 && m_rcThumb.top == 0 && m_rcThumb.right == 0 && m_rcThumb.bottom == 0 ) return;
		if( !IsEnabled() ) m_uThumbState |= UISTATE_DISABLED;
		else m_uThumbState &= ~ UISTATE_DISABLED;
		drawableTmp.left    = MulDiv(m_rcThumb.left - m_rcItem.left, 100, GetManager()->GetDPIObj()->GetScale());
		drawableTmp.top     = MulDiv(m_rcThumb.top - m_rcItem.top, 100, GetManager()->GetDPIObj()->GetScale());
		drawableTmp.right   = MulDiv(m_rcThumb.right - m_rcItem.left, 100, GetManager()->GetDPIObj()->GetScale());
		drawableTmp.bottom  = MulDiv(m_rcThumb.bottom - m_rcItem.top, 100, GetManager()->GetDPIObj()->GetScale());
		//m_sImageModify.Empty();
		//m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), d1, d2, d3, d4);

		if( (m_uThumbState & UISTATE_DISABLED) != 0 ) {
			if( !m_sThumbDisabledImage.IsEmpty() ) {
				if( !DrawImageAt(hDC, (LPCTSTR)m_sThumbDisabledImage, &drawableTmp) ) {}
				else return;
			}
		}
		else if( (m_uThumbState & UISTATE_PUSHED) != 0 ) {
			if( !m_sThumbPushedImage.IsEmpty() ) {
				if( !DrawImageAt(hDC, (LPCTSTR)m_sThumbPushedImage, &drawableTmp) ) {}
				else return;
			}
		}
		else if( (m_uThumbState & UISTATE_HOT) != 0 ) {
			if( !m_sThumbHotImage.IsEmpty() ) {
				if( !DrawImageAt(hDC, (LPCTSTR)m_sThumbHotImage, &drawableTmp) ) {}
				else return;
			}
		}

		if( !m_sThumbNormalImage.IsEmpty() ) {
			if( !DrawImageAt(hDC, (LPCTSTR)m_sThumbNormalImage, &drawableTmp) ) {}
			else return;
		}

		DWORD dwBorderColor = 0xFF85E4FF;
		int nBorderSize = 2;
		CRenderEngine::DrawRect(hDC, m_rcThumb, nBorderSize, dwBorderColor);

		RECT rc = m_rcItem;
		int number = 1;
		float space = (m_rcItem.bottom-m_rcItem.top)*1.f / number;
		//for (size_t i = 0; i < number; i++)
		//{
		//	rc.top = space*i;
		//	rc.bottom = rc.top+5;
		//	//CRenderEngine::DrawRect(hDC, rc, nBorderSize, dwBorderColor);
		//}
		for (size_t i = 0; i < number; i++)
		{
			rc.bottom = rc.top = space*i;
			//CRenderEngine::DrawLine(hDC, rc, nBorderSize, dwBorderColor);
		}
	}

	void CScrollBarUI::PaintRail(HDC hDC)
	{
		if( m_rcThumb.left == 0 && m_rcThumb.top == 0 && m_rcThumb.right == 0 && m_rcThumb.bottom == 0 ) return;
		if( !IsEnabled() ) m_uThumbState |= UISTATE_DISABLED;
		else m_uThumbState &= ~ UISTATE_DISABLED;

		//m_sImageModify.Empty();
		if( !m_bHorizontal ) {
			drawableTmp.left    = MulDiv(m_rcThumb.left - m_rcItem.left, 100, GetManager()->GetDPIObj()->GetScale());
			drawableTmp.top     = MulDiv((m_rcThumb.top + m_rcThumb.bottom) / 2 - m_rcItem.top - m_cxyFixed.cx / 2, 100, GetManager()->GetDPIObj()->GetScale());
			drawableTmp.right   = MulDiv(m_rcThumb.right - m_rcItem.left, 100, GetManager()->GetDPIObj()->GetScale());
			drawableTmp.bottom  = MulDiv((m_rcThumb.top + m_rcThumb.bottom) / 2 - m_rcItem.top + m_cxyFixed.cx - m_cxyFixed.cx / 2, 100, GetManager()->GetDPIObj()->GetScale());
			//m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), d1, d2, d3,d4);
		}
		else {
			drawableTmp.left    = MulDiv((m_rcThumb.left + m_rcThumb.right) / 2 - m_rcItem.left - m_cxyFixed.cy / 2, 100, GetManager()->GetDPIObj()->GetScale());
			drawableTmp.top     = MulDiv(m_rcThumb.top - m_rcItem.top, 100, GetManager()->GetDPIObj()->GetScale());
			drawableTmp.right   = MulDiv((m_rcThumb.left + m_rcThumb.right) / 2 - m_rcItem.left + m_cxyFixed.cy - m_cxyFixed.cy / 2, 100, GetManager()->GetDPIObj()->GetScale());
			drawableTmp.bottom  = MulDiv(m_rcThumb.bottom - m_rcItem.top, 100, GetManager()->GetDPIObj()->GetScale());
			//m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), d1,d2, d3, d4);
		}

		if( (m_uThumbState & UISTATE_DISABLED) != 0 ) {
			if( !m_sRailDisabledImage.IsEmpty() ) {
				if( !DrawImageAt(hDC, (LPCTSTR)m_sRailDisabledImage, &drawableTmp) ) {}
				else return;
			}
		}
		else if( (m_uThumbState & UISTATE_PUSHED) != 0 ) {
			if( !m_sRailPushedImage.IsEmpty() ) {
				if( !DrawImageAt(hDC, (LPCTSTR)m_sRailPushedImage, &drawableTmp) ) {}
				else return;
			}
		}
		else if( (m_uThumbState & UISTATE_HOT) != 0 ) {
			if( !m_sRailHotImage.IsEmpty() ) {
				if( !DrawImageAt(hDC, (LPCTSTR)m_sRailHotImage, &drawableTmp) ) {}
				else return;
			}
		}

		if( !m_sRailNormalImage.IsEmpty() ) {
			if( !DrawImageAt(hDC, (LPCTSTR)m_sRailNormalImage, &drawableTmp) ) {}
			else return;
		}
	}

	LONG CScrollBarUI::GetThumbPosition()
	{
		return m_bHorizontal?(m_rcThumb.left+m_rcThumb.right)/2:(m_rcThumb.top+m_rcThumb.bottom)/2;
		//return m_bHorizontal?m_rcThumb.right:m_rcThumb.bottom;
		//return m_bHorizontal?m_rcThumb.right:m_rcThumb.top;
	}
}
