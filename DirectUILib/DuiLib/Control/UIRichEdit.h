#ifndef __UIRICHEDIT_H__
#define __UIRICHEDIT_H__

#pragma once

namespace DuiLib {

	class CTxtWinHost;

	class UILIB_API CRichEditUI : public CContainerUI, public IMessageFilterUI
	{
		DECLARE_QKCONTROL(CRichEditUI)
	public:
		CRichEditUI();
		~CRichEditUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		void SetEnabled(bool bEnabled);
		bool IsMultiLine();
		void SetMultiLine(bool bMultiLine);
		bool IsWantTab();
		void SetWantTab(bool bWantTab = true);
		bool IsWantReturn();
		void SetWantReturn(bool bWantReturn = true);
		bool IsWantCtrlReturn();
		void SetWantCtrlReturn(bool bWantCtrlReturn = true);
		bool IsTransparent();
		void SetTransparent(bool bTransparent = true);
		bool IsRich();
		void SetRich(bool bRich = true);
		bool IsReadOnly();
		void SetReadOnly(bool bReadOnly = true);
		bool IsWordWrap();
		void SetWordWrap(bool bWordWrap = true);
		int GetFont();
		void SetFont(LPCTSTR pStrFontId, int parsedId=-1);
		void SetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
		LONG GetWinStyle();
		void SetWinStyle(LONG lStyle);
		DWORD GetTextColor();
		void SetTextColor(DWORD dwTextColor);
		int GetLimitText();
		void SetLimitText(int iChars);
		long GetTextLength(DWORD dwFlags = GTL_DEFAULT) const;
		QkString & GetText();
		void SetText(LPCTSTR pstrText);
		bool IsModify() const;
		void SetModify(bool bModified = true) const;
		void GetSel(CHARRANGE &cr) const;
		void GetSel(long& nStartChar, long& nEndChar) const;
		int SetSel(CHARRANGE &cr);
		int SetSel(long nStartChar, long nEndChar);
		void ReplaceSel(LPCTSTR lpszNewText, bool bCanUndo);
		void ReplaceSelW(LPCWSTR lpszNewText, bool bCanUndo = false);
		QkString GetSelText() const;
		int SetSelAll();
		int SetSelNone();
		WORD GetSelectionType() const;
		bool GetZoom(int& nNum, int& nDen) const;
		bool SetZoom(int nNum, int nDen);
		bool SetZoomOff();
		bool GetAutoURLDetect() const;
		bool SetAutoURLDetect(bool bAutoDetect = true);
		DWORD GetEventMask() const;
		DWORD SetEventMask(DWORD dwEventMask);
		QkString GetTextRange(long nStartChar, long nEndChar) const;
		void HideSelection(bool bHide = true, bool bChangeStyle = false);
		void ScrollCaret();
		int InsertText(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo = false);
		int AppendText(LPCTSTR lpstrText, bool bCanUndo = false);
		DWORD GetDefaultCharFormat(CHARFORMAT2 &cf) const;
		bool SetDefaultCharFormat(CHARFORMAT2 &cf);
		DWORD GetSelectionCharFormat(CHARFORMAT2 &cf) const;
		bool SetSelectionCharFormat(CHARFORMAT2 &cf);
		bool SetWordCharFormat(CHARFORMAT2 &cf);
		DWORD GetParaFormat(PARAFORMAT2 &pf) const;
		bool SetParaFormat(PARAFORMAT2 &pf);
		bool CanUndo();
		bool CanRedo();
		bool CanPaste();
		bool Redo();
		bool Undo();
		void Clear();
		void Copy();
		void Cut();
		void Paste();
		int GetLineCount() const;
		QkString GetLine(int nIndex, int nMaxLength) const;
		int LineIndex(int nLine = -1) const;
		int LineLength(int nLine = -1) const;
		bool LineScroll(int nLines, int nChars = 0);
		CDuiPoint GetCharPos(long lChar) const;
		long LineFromChar(long nIndex) const;
		CDuiPoint PosFromChar(UINT nChar) const;
		int CharFromPos(CDuiPoint pt) const;
		void EmptyUndoBuffer();
		UINT SetUndoLimit(UINT nLimit);
		long StreamIn(int nFormat, EDITSTREAM &es);
		long StreamOut(int nFormat, EDITSTREAM &es);
		void SetAccumulateDBCMode(bool bDBCMode);
		bool IsAccumulateDBCMode();

		RECT GetTextPadding() const;
		void SetTextPadding(RECT rc);
		LPCTSTR GetNormalImage();
		void SetNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetHotImage();
		void SetHotImage(LPCTSTR pStrImage);
		LPCTSTR GetFocusedImage();
		void SetFocusedImage(LPCTSTR pStrImage);
		LPCTSTR GetDisabledImage();
		void SetDisabledImage(LPCTSTR pStrImage);
		void PaintStatusImage(HDC hDC);

		void SetTipValue(LPCTSTR pStrTipValue);
		LPCTSTR GetTipValue();
		void SetTipValueColor(LPCTSTR pStrColor);
		DWORD GetTipValueColor();
		void SetTipValueAlign(UINT uAlign);
		UINT GetTipValueAlign();

		void DoInit();
		bool SetDropAcceptFile(bool bAccept);
		// 注意：TxSendMessage和SendMessage是有区别的，TxSendMessage没有multibyte和unicode自动转换的功能，
		// 而richedit2.0内部是以unicode实现的，在multibyte程序中，必须自己处理unicode到multibyte的转换
		virtual HRESULT TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *plresult) const; 
		IDropTarget* GetTxDropTarget();
		virtual bool OnTxViewChanged();
		virtual void OnTxNotify(DWORD iNotify, void *pv);

		bool SetScrollPos(SIZE szPos, bool bMsg = true);
		bool LineUp();
		bool LineDown();
		bool LineLeft();
		bool LineRight();
		void PageUp();
		void PageDown();
		void HomeUp();
		void EndDown();
		void PageLeft();
		void PageRight();
		void HomeLeft();
		void EndRight();

		SIZE EstimateSize(const SIZE & szAvailable);
		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void Move(SIZE szOffset, bool bNeedInvalidate = true);
		void DoEvent(TEventUI& event);
		bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	protected:
		enum { 
			DEFAULT_TIMERID = 20,
		};

		CTxtWinHost* m_pTwh;
		bool m_bVScrollBarFixing;
		bool m_bWantTab;
		bool m_bWantReturn;
		bool m_bWantCtrlReturn;
		bool m_bTransparent;
		bool m_bRich;
		bool m_bReadOnly;
		bool m_bWordWrap;
		DWORD m_dwTextColor;
		int _font;
		int m_iLimitText;
		LONG m_lTwhStyle;
		bool m_bDrawCaret;
		bool m_bInited;

		bool  m_fAccumulateDBC ; // TRUE - need to cumulate ytes from 2 WM_CHAR msgs
		// we are in this mode when we receive VK_PROCESSKEY
		UINT m_chLeadByte; // use when we are in _fAccumulateDBC mode

		RECT m_rcTextPadding;
		UINT m_uButtonState;
		QkString m_sNormalImage;
		QkString m_sHotImage;
		QkString m_sFocusedImage;
		QkString m_sDisabledImage;
		QkString m_sTipValue;
		DWORD m_dwTipValueColor;
		UINT m_uTipValueAlign;
	};

} // namespace DuiLib

#endif // __UIRICHEDIT_H__
