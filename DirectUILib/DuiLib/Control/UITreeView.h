#ifdef QkUITreeView

#ifndef UITreeView_h__
#define UITreeView_h__

#include <vector>
using namespace std;

#pragma once

namespace DuiLib
{
	class CTreeViewUI;
	class CCheckBoxUI;
	class CLabelUI;
	class OptionBtn;

	class UILIB_API CTreeNodeUI : public CListContainerElementUI
	{
		DECLARE_QKCONTROL(CTreeNodeUI)
	public:
		CTreeNodeUI(CTreeNodeUI* _ParentNode = NULL);
		~CTreeNodeUI(void);

	public:
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void DoEvent(TEventUI& event);
		void Invalidate();
		bool Select(bool bSelect = true);
		bool SelectMulti(bool bSelect = true);

		bool Add(CControlUI* _pTreeNodeUI);
		bool AddAt(CControlUI* pControl, int iIndex);
		bool Remove(CControlUI* pControl);

		void SetVisibleTag(bool _IsVisible);
		bool GetVisibleTag();
		void SetItemText(LPCTSTR pstrValue);
		QkString GetItemText();
		void CheckBoxSelected(bool _Selected);
		bool IsCheckBoxSelected() const;
		bool IsHasChild() const;
		long GetTreeLevel();
		bool AddChildNode(CTreeNodeUI* _pTreeNodeUI);
		bool RemoveAt(CTreeNodeUI* _pTreeNodeUI);
		void SetParentNode(CTreeNodeUI* _pParentTreeNode);
		CTreeNodeUI* GetParentNode();
		long GetCountChild();
		void SetTreeView(CTreeViewUI* _CTreeViewUI);
		CTreeViewUI* GetTreeView();
		CTreeNodeUI* GetChildNode(int _nIndex);
		void SetVisibleFolderBtn(bool _IsVisibled);
		bool GetVisibleFolderBtn();
		void SetVisibleCheckBtn(bool _IsVisibled);
		bool GetVisibleCheckBtn();
		void SetItemTextColor(DWORD _dwItemTextColor);
		DWORD GetItemTextColor() const;
		void SetItemHotTextColor(DWORD _dwItemHotTextColor);
		DWORD GetItemHotTextColor() const;
		void SetSelItemTextColor(DWORD _dwSelItemTextColor);
		DWORD GetSelItemTextColor() const;
		void SetSelItemHotTextColor(DWORD _dwSelHotItemTextColor);
		DWORD GetSelItemHotTextColor() const;
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		CStdPtrArray GetTreeNodes();
		int			 GetTreeIndex();
		int			 GetNodeIndex();

	public:
		CHorizontalLayoutUI*	GetTreeNodeHoriznotal() const {return pHoriz;};
		CCheckBoxUI*			GetFolderButton() const {return pFolderButton;};
		CLabelUI*				GetDottedLine() const {return pDottedLine;};
		CCheckBoxUI*			GetCheckBox() const {return pCheckBox;};
		OptionBtn*				GetItemButton() const {return pItemButton;};

	private:
		CTreeNodeUI* GetLastNode();
		CTreeNodeUI* CalLocation(CTreeNodeUI* _pTreeNodeUI);

	private:
		long	m_iTreeLavel;
		bool	m_bIsVisable;
		bool	m_bIsCheckBox;
		DWORD	m_dwItemTextColor;
		DWORD	m_dwItemHotTextColor;
		DWORD	m_dwSelItemTextColor;
		DWORD	m_dwSelItemHotTextColor;

		CTreeViewUI*			pTreeView;
		CHorizontalLayoutUI*	pHoriz;
		CCheckBoxUI*			pFolderButton;
		CLabelUI*				pDottedLine;
		CCheckBoxUI*			pCheckBox;
		OptionBtn*				pItemButton;
		CTreeNodeUI*			pParentTreeNode;
		CStdPtrArray			mTreeNodes;
	};

	class UILIB_API CTreeViewUI : public CListUI,public INotifyUI
	{
		DECLARE_QKCONTROL(CTreeViewUI)
	public:
		CTreeViewUI(void);
		~CTreeViewUI(void);

	public:
		virtual LPCTSTR GetClass() const;
		virtual LPVOID	GetInterface(LPCTSTR pstrName);

		virtual UINT GetListType();
		virtual bool Add(CTreeNodeUI* pControl );
		virtual long AddAt(CTreeNodeUI* pControl, int iIndex );
		virtual bool AddAt(CTreeNodeUI* pControl,CTreeNodeUI* _IndexNode);
		virtual bool Remove(CTreeNodeUI* pControl);
		virtual bool RemoveAt(int iIndex);
		virtual void RemoveAll();
		virtual bool OnCheckBoxChanged(void* param);
		virtual bool OnFolderChanged(void* param);
		virtual bool OnDBClickItem(void* param);
		virtual bool SetItemCheckBox(bool _Selected,CTreeNodeUI* _TreeNode = NULL);
		virtual void SetItemExpand(bool _Expanded,CTreeNodeUI* _TreeNode = NULL);
		virtual void Notify(TNotifyUI& msg);
		virtual void SetVisibleFolderBtn(bool _IsVisibled);
		virtual bool GetVisibleFolderBtn();
		virtual void SetVisibleCheckBtn(bool _IsVisibled);
		virtual bool GetVisibleCheckBtn();
		virtual void SetItemMinWidth(UINT _ItemMinWidth);
		virtual UINT GetItemMinWidth();
		virtual void SetItemTextColor(DWORD _dwItemTextColor);
		virtual void SetItemHotTextColor(DWORD _dwItemHotTextColor);
		virtual void SetSelItemTextColor(DWORD _dwSelItemTextColor);
		virtual void SetSelItemHotTextColor(DWORD _dwSelHotItemTextColor);
		
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	private:
		UINT m_uItemMinWidth;
		bool m_bVisibleFolderBtn;
		bool m_bVisibleCheckBtn;
	};
}


#endif // UITreeView_h__


#endif