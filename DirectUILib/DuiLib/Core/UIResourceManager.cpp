#include "StdAfx.h"
#include "UIResourceManager.h"

namespace DuiLib {
	
	CResourceManager::CResourceManager(void)
	{
		m_pQuerypInterface = NULL;
		
	}

	CResourceManager::~CResourceManager(void)
	{
		ResetResourceMap();
	}

	BOOL CResourceManager::LoadResource(STRINGorID xml, LPCTSTR type)
	{
		if( HIWORD(xml.m_lpstr) != NULL ) 
		{
			if( *(xml.m_lpstr) == _T('<') ) 
			{
				if( !m_xml.Load(xml.m_lpstr) ) return NULL;
			}
			else 
			{
				if( !m_xml.LoadFromFile(xml.m_lpstr) ) return NULL;
			}
		}
		else
		{
			HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), xml.m_lpstr, type);
			if( hResource == NULL ) return NULL;
			HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
			if( hGlobal == NULL ) 
			{
				FreeResource(hResource);
				return NULL;
			}

			if( !m_xml.LoadFromMem((BYTE*)::LockResource(hGlobal), ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource) )) {
				return NULL;
			}
			::FreeResource(hGlobal);
		}

		return LoadResource(m_xml.GetRoot());
	}

	BOOL CResourceManager::LoadResource(XMarkupNode Root)
	{
		if( !Root.IsValid() ) return FALSE;

		LPCTSTR pstrClass = NULL;
		int nAttributes = 0;
		LPCTSTR pstrName = NULL;
		LPCTSTR pstrValue = NULL;
		LPTSTR pstr = NULL;

		//加载图片资源
		LPCTSTR pstrId = NULL;
		LPCTSTR pstrPath = NULL;
		for( XMarkupNode node = Root.GetChild() ; node.IsValid(); node = node.GetSibling() ) 
		{
			pstrClass = node.GetName();
			XMarkupNode ChildNode = node.GetChild();
			if(ChildNode.IsValid()) LoadResource(node);
			else if ((_tcsicmp(pstrClass,_T("Image")) == 0) && node.HasAttributes())
			{
				//加载图片资源
				nAttributes = node.GetAttributeCount();
				for( int i = 0; i < nAttributes; i++ ) 
				{
					pstrName = node.GetAttributeName(i);
					pstrValue = node.GetAttributeValue(i);

					if( _tcsicmp(pstrName, _T("id")) == 0 ) 
					{
						pstrId = pstrValue;
					}
					else if( _tcsicmp(pstrName, _T("path")) == 0 ) 
					{
						pstrPath = pstrValue;
					}
				}
				if( pstrId == NULL ||  pstrPath == NULL) continue;
				QkString * pstrFind = static_cast<QkString *>(m_mImageHashMap.Find(pstrId));
				if(pstrFind != NULL) continue;
				m_mImageHashMap.Insert(pstrId, (LPVOID)new QkString(pstrPath));
			}
			else if( _tcsicmp(pstrClass,_T("Xml")) == 0 && node.HasAttributes()) {
				//加载XML配置文件
				nAttributes = node.GetAttributeCount();
				for( int i = 0; i < nAttributes; i++ ) 
				{
					pstrName = node.GetAttributeName(i);
					pstrValue = node.GetAttributeValue(i);

					if( _tcsicmp(pstrName, _T("id")) == 0 ) 
					{
						pstrId = pstrValue;
					}
					else if( _tcsicmp(pstrName, _T("path")) == 0 ) 
					{
						pstrPath = pstrValue;
					}
				}
				if( pstrId == NULL ||  pstrPath == NULL) continue;
				QkString * pstrFind = static_cast<QkString *>(m_mXmlHashMap.Find(pstrId));
				if(pstrFind != NULL) continue;
				m_mXmlHashMap.Insert(pstrId, (LPVOID)new QkString(pstrPath));
			}
			else continue;
		}
		return TRUE;
	}

	LPCTSTR CResourceManager::MapImagePath(LPCTSTR lpstrId)
	{
		QkString * lpStr = static_cast<QkString *>(m_mImageHashMap.Find(lpstrId));
		return lpStr ? lpStr->GetData():lpstrId;
	}

	LPCTSTR CResourceManager::GetXmlPath(LPCTSTR lpstrId)
	{
		QkString * lpStr = static_cast<QkString *>(m_mXmlHashMap.Find(lpstrId));
		return lpStr == NULL? NULL:lpStr->GetData();
	}

	void CResourceManager::ResetResourceMap()
	{
		QkString* lpStr;
		for( int i = 0; i< m_mImageHashMap.GetSize(); i++ )
		{
			lpStr = static_cast<QkString *>(m_mImageHashMap.GetValueAt(i));
			if(lpStr) delete lpStr;
		}
		m_mImageHashMap.RemoveAll();
		for( int i = 0; i< m_mXmlHashMap.GetSize(); i++ )
		{
			lpStr = static_cast<QkString *>(m_mXmlHashMap.GetValueAt(i));
			if(lpStr) delete lpStr;
		}
		m_mXmlHashMap.RemoveAll();
		for( int i = 0; i< m_mTextResourceHashMap.GetSize(); i++ )
		{
			lpStr = static_cast<QkString *>(m_mTextResourceHashMap.GetValueAt(i));
			if(lpStr) delete lpStr;
		}
		m_mTextResourceHashMap.RemoveAll();
	}

	BOOL CResourceManager::LoadLanguage(LPCTSTR pstrXml)
	{
		XMarkupParser xml;
		if( *(pstrXml) == _T('<') ) {
			if( !xml.Load(pstrXml) ) return FALSE;
		}
		else {
			if( !xml.LoadFromFile(pstrXml) ) return FALSE;
		}
		XMarkupNode Root = xml.GetRoot();
		if( !Root.IsValid() ) return FALSE;

		LPCTSTR pstrClass = NULL;
		int nAttributes = 0;
		LPCTSTR pstrName = NULL;
		LPCTSTR pstrValue = NULL;
		LPTSTR pstr = NULL;

		//加载图片资源
		LPCTSTR pstrId = NULL;
		LPCTSTR pstrText = NULL;
		for( XMarkupNode node = Root.GetChild() ; node.IsValid(); node = node.GetSibling() ) 
		{
			pstrClass = node.GetName();
			if ((_tcsicmp(pstrClass,_T("Text")) == 0) && node.HasAttributes())
			{
				//加载图片资源
				nAttributes = node.GetAttributeCount();
				for( int i = 0; i < nAttributes; i++ ) 
				{
					pstrName = node.GetAttributeName(i);
					pstrValue = node.GetAttributeValue(i);

					if( _tcsicmp(pstrName, _T("id")) == 0 ) 
					{
						pstrId = pstrValue;
					}
					else if( _tcsicmp(pstrName, _T("value")) == 0 ) 
					{
						pstrText = pstrValue;
					}
				}
				if( pstrId == NULL ||  pstrText == NULL) continue;

				QkString *lpstrFind = static_cast<QkString *>(m_mTextResourceHashMap.Find(pstrId));
				if(lpstrFind != NULL) {
					lpstrFind->Assign(pstrText);
				}
				else {
					m_mTextResourceHashMap.Insert(pstrId, (LPVOID)new QkString(pstrText));
				}
			}
			else continue;
		}

		return TRUE;
	}

	QkString & CResourceManager::GetText(LPCTSTR lpstrId, LPCTSTR lpstrType)
	{
		if(lpstrId == NULL) return QkString::EmptyInstance();

		QkString *lpstrFind = static_cast<QkString *>(m_mTextResourceHashMap.Find(lpstrId));
		if (lpstrFind == NULL && m_pQuerypInterface)
		{
			LPCTSTR lpText = m_pQuerypInterface->QueryControlText(lpstrId, lpstrType);
			if(lpText != NULL) {
				lpstrFind = new QkString(lpText);
				m_mTextResourceHashMap.Insert(lpstrId, (LPVOID)lpstrFind);
			}
		}
		if (!lpstrFind)
		{
			return QkString::EmptyInstance(lpstrId);
		}
		return *lpstrFind;
	}

	void CResourceManager::ReloadText()
	{
		if(m_pQuerypInterface == NULL) return;
		//重载文字描述
		LPCTSTR lpstrId = NULL;
		LPCTSTR lpstrText;
		for( int i = 0; i < m_mTextResourceHashMap.GetSize(); i++ )
		{
			const TITEM* key = m_mTextResourceHashMap.GetSlotAt(i);
			if (key) 
			{
				lpstrText = m_pQuerypInterface->QueryControlText(key->Key, NULL);
				if(lpstrText) 
				{
					static_cast<QkString *>(key->Data)->Assign(lpstrText);
				}
			}
		}
	}
	void CResourceManager::ResetTextMap()
	{
		QkString * lpStr;
		for( int i = 0; i< m_mTextResourceHashMap.GetSize(); i++ )
		{
			lpStr = static_cast<QkString *>(m_mTextResourceHashMap.GetValueAt(i));
			if(lpStr) delete lpStr;
		}
		m_mTextResourceHashMap.RemoveAll();
	}

	
} // namespace DuiLib