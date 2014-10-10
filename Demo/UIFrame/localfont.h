#pragma once

#ifndef LOCAOFONT_H_
#define LOCAOFONT_H_
#include "StdAfx.h"


class CAddLocalFont
{
public:  

	CAddLocalFont()
	{

	}

	~CAddLocalFont()
	{
		for (int Index=0;Index<m_fontArray.GetSize();Index++)
		{
			RemoveFontResource(*m_fontArray.GetAt(Index));
			delete m_fontArray.GetAt(Index);
		}

		m_fontArray.Empty();

	}     

	//************************************
	// Method:    AddFont
	// FullName:  CAddLocalFont::AddFont
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: LPCTSTR szfileName:字体文件绝对路径
	//************************************
	bool AddFont(LPCTSTR szFontFilePath)  
	{  
		if (szFontFilePath==NULL)
		{
			return false;
		}

		CDuiString strFontPath = szFontFilePath;
		if (strFontPath.IsEmpty())
		{
			return false;
		}

		int nResult = AddFontResourceEx(strFontPath,FR_PRIVATE,NULL);  
		if (nResult == 0)  
		{  
			return false;  
		}
		else
		{
			m_fontArray.Add(new CDuiString(strFontPath));
		}

		return true;  
	}  


public:

	TStdValArray<CDuiString*> m_fontArray; 
};



#endif









