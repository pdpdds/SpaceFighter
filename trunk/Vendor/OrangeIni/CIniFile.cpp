
//-----------------------------------------------------------------------------
// File name    : CIniFile.cpp
// Author       : AV (Antoine Villepreux)
// date         : 20/11/2000
// Description  : CIniFile class implementation
//              : Easy IniFile object management
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------
#include "stdafx.h"

	//#include "Common.h"
	#include "CIniFile.h"
//	#include "CCrypto.h"
	#include "float.h"
	#include "limits.h"


//-----------------------------------------------------------------------------

#ifdef  _DEBUG_NEW
#undef  new
#define new DEBUG_NEW
#undef  THIS_FILE
#define THIS_FILE __FILE__
#endif

//-----------------------------------------------------------------------------
// Error constants
//-----------------------------------------------------------------------------

const CStdString CIniFile::ms_strError = "CINIFILE_ERROR_READING_KEY_OR_SECTION";
const int        CIniFile::ms_iError   = INT_MAX;
const float      CIniFile::ms_fError   = FLT_MAX;

const CStdString CIniFile::ms_strErrorMsg[E_ERROR_MESSAGES_COUNT] = 
{

    "Error: Unable to open ini file",
    "Error: Unable to save ini file",
    "Error: Unable to locate specified section",
    "Error: Unable to locate specified key",
    "Warning: unknown extension" ,
	"Warning: end string delimiter not found"
};

//-----------------------------------------------------------------------------
// Internal shortcuts constants
//-----------------------------------------------------------------------------

const CStdString CIniFile::ms_strComment  [E_COMMENTS_COUNT  ] = { ";"   , "//", "/*", "\t\t" };
const CStdString CIniFile::ms_strType     [E_TYPES_COUNT     ] = { "%d"  , "%f", "%s"         };
const CStdString CIniFile::ms_strMarkup   [E_MARKUPS_COUNT   ] = { "["   , "]" , "="          };
const CStdString CIniFile::ms_strTrim     [E_TRIM_COUNT      ] = { " "   , "\t"               };
const CStdString CIniFile::ms_strExtension[CIniFile::E_EXTENSIONS_COUNT] = { ".ini", ".crk"             };

const CStdString CIniFile::ms_strStringDelimiter = "\"";

//-----------------------------------------------------------------------------
// Various defines
//-----------------------------------------------------------------------------

#ifdef	TO_BOOL
#undef	TO_BOOL
#endif
#define	TO_BOOL(i)		(((i)==0)?false:true)

#ifdef	TO_INT
#undef	TO_INT
#endif
#define TO_INT(b)		(((b))?1:0)

#define BREAKPOINT		_asm int 3

//-----------------------------------------------------------------------------
// CIniFile Implementation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name        : CIniFile::CIniFile()
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Construct empty IniFile object
//-----------------------------------------------------------------------------
CIniFile::CIniFile()
{

	SetReadableExtension (ms_strExtension[E_READABLE ]);
    SetEncryptedExtension(ms_strExtension[E_ENCRYPTED]);

	m_bFastRead = false;
}

//-----------------------------------------------------------------------------
// Name        : CIniFile::CIniFile(const CFilename & strFilename, bool bFastRead)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Construct IniFile object based on given file
//             : Optionnal fast read may be used if file doesn't contain any
//             : trailing spaces nor tabs
//-----------------------------------------------------------------------------
CIniFile::CIniFile(const CFilename & strFilename, bool bFastRead)
{

	SetReadableExtension (ms_strExtension[E_READABLE ]);
    SetEncryptedExtension(ms_strExtension[E_ENCRYPTED]);

	Load(strFilename, bFastRead);


}

//-----------------------------------------------------------------------------
// Name        : CIniFile::~CIniFile()
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Destruct IniFile object
//-----------------------------------------------------------------------------
CIniFile::~CIniFile()
{

#ifdef INI_SAVE_ON_EXIT

	Save(m_strPath);

#endif
}

//-----------------------------------------------------------------------------
// Name        : bool CIniFile::Load(const CStdString& strFilename, bool bFastRead)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Load ini file from disk or from previous specified file
//             : Optionnal fast read may be used if file doesn't contain any
//             : trailing spaces nor tabs
//-----------------------------------------------------------------------------
bool CIniFile::Load(const CStdString& strFilename, bool bFastRead)
{
	m_bFastRead = bFastRead;

	if (strFilename != _T("")) 
	{
		m_strPath = strFilename;
	}

	// Try reading readable ini file

    if (m_strPath.GetLength() > m_strReadableExtension.GetLength())
    if (!m_strReadableExtension.CompareNoCase(m_strPath.Right(m_strReadableExtension.GetLength())))
    {
		// If *.ini doesn't exist change extension to crypted extension and try again

        if (LoadReadableFile(m_strPath, true)) 
		{
			return true;
		}
		else
		{
			m_strPath = m_strPath.Left(m_strPath.GetLength() - m_strReadableExtension.GetLength());
			m_strPath += m_strEncryptedExtension;
		}
    }

	// Try reading crypted ini file
    
    if (m_strPath.GetLength() > m_strEncryptedExtension.GetLength())
    if (!m_strEncryptedExtension.CompareNoCase(m_strPath.Right(m_strEncryptedExtension.GetLength())))
    {
        return LoadReadableFile(m_strPath, false);
    }

	// No recognized extension found
	// Try reading readable ini file

    m_strLastError = ms_strErrorMsg[E_UNKNOWN_EXTENSION];

    if (LoadReadableFile())
	{
		return true;
	}
	else
	{
		m_strLastError = ms_strErrorMsg[E_READ];

		#ifdef WIN32
			CStdString strMsg = m_strPath;
			strMsg += " file not found !";
			::MessageBox(NULL,strMsg, m_strLastError, MB_OK);
		#endif

		BREAKPOINT;

	#ifdef INI_USE_EXEPTIONS

		throw(strMsg.c_str());

	#endif

		return false;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Name        : bool CIniFile::LoadReadableFile(const CStdString& strFilename, bool bIsReadable)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Internal function - file loading
//-----------------------------------------------------------------------------
bool CIniFile::LoadReadableFile(const CStdString& strFilename, bool bIsReadable)
{
	if (strFilename != _T("")) m_strPath = strFilename;

	// Open stream

	std::ifstream inifile;
	inifile.open(m_strPath);

	if (inifile.fail())
	{
        m_strLastError = ms_strErrorMsg[E_READ];
		return false;
	}

	// Open stream Ok, parse file lines

	CStdString strLine, strSection, strKey, strValue;

	while (GetLine(inifile, strLine))
	{
		if (! bIsReadable) 
		{
			Decrypt(strLine);
		}

		if (!m_bFastRead)
		{
			ManageComments(&strLine);
		}

		if (strLine != _T(""))
		{
            if (	strLine[0]						== (ms_strMarkup[E_SECTION_BEGIN])[0] && 
					strLine[strLine.GetLength()-1]	== (ms_strMarkup[E_SECTION_END]  )[0]	)
			{
				// Section found

				strSection = strLine;

				strSection.TrimLeft (ms_strMarkup[E_SECTION_BEGIN]);
				strSection.TrimRight(ms_strMarkup[E_SECTION_END]);

				if (!m_bFastRead)
				{
					CleanString(&strSection);
				}
			}
			else
			{
				// Key found

				strKey   = strLine.Left (strLine.Find(ms_strMarkup[E_KEY]));
				strValue = strLine.Right(strLine.GetLength()-strKey.GetLength()-1);

				if (!m_bFastRead)
				{
					CleanString(&strKey);
					CleanString(&strValue);
				}

				strValue.TrimLeft(ms_strMarkup[E_KEY]);
				
				SetValue(strSection, strKey, strValue);
			}
		}
	}

	inifile.close();

	return true;	
}

//-----------------------------------------------------------------------------
// Name        : bool CIniFile::Save(const CStdString& strFilename)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Save IniFile object to disk with specified filename
//-----------------------------------------------------------------------------
bool CIniFile::Save(const CStdString& strFilename)
{
	if (strFilename != _T("")) m_strPath = strFilename;

    if (m_strPath.GetLength() > m_strReadableExtension.GetLength())
    if (!m_strReadableExtension.CompareNoCase(m_strPath.Right(m_strReadableExtension.GetLength())))
    {
        return SaveReadableFile();
    }
    
    if (m_strPath.GetLength() > m_strEncryptedExtension.GetLength())
    if (!m_strEncryptedExtension.CompareNoCase(m_strPath.Right(m_strEncryptedExtension.GetLength())))
    {
        return SaveEncryptedFile();
    }

    m_strLastError = ms_strErrorMsg[E_UNKNOWN_EXTENSION];

    SaveReadableFile();

	return false;
}

//-----------------------------------------------------------------------------
// Name        : bool CIniFile::SaveReadableFile(const CStdString& strFilename)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : internal function - save readable file to disk
//-----------------------------------------------------------------------------
bool CIniFile::SaveReadableFile(const CStdString& strFilename)
{
	if (strFilename != _T("")) m_strPath = strFilename;

	std::wofstream inifile;//(strFilename, std::ios::app);;
	inifile.open(m_strPath);

	for(m_itSection=m_data.begin(); m_itSection!=m_data.end(); m_itSection++)
	{
		inifile	<< ms_strMarkup[E_SECTION_BEGIN] << (*m_itSection).first << ms_strMarkup[E_SECTION_END] << "\n";

		for(m_itKey=((*m_itSection).second).begin(); m_itKey!=((*m_itSection).second).end(); m_itKey++)
		{
			inifile	<< (*m_itKey).first << ms_strMarkup[E_KEY] << (*m_itKey).second << "\n";
		}
	}

	inifile.close();

	return true;

}

//-----------------------------------------------------------------------------
// Name        : bool CIniFile::SaveEncryptedFile(const CStdString& strFilename)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : internal function - save crypted file to disk
//-----------------------------------------------------------------------------
bool CIniFile::SaveEncryptedFile(const CStdString& strFilename)
{

	if (strFilename != _T("")) m_strPath = strFilename;

	std::wofstream inifile;
	inifile.open(m_strPath);

	for(m_itSection=m_data.begin(); m_itSection!=m_data.end(); m_itSection++)
	{
		inifile	<< Encrypt(ms_strMarkup[E_SECTION_BEGIN]) << Encrypt((*m_itSection).first) << Encrypt(ms_strMarkup[E_SECTION_END]) << "\n";

		for(m_itKey=((*m_itSection).second).begin(); m_itKey!=((*m_itSection).second).end(); m_itKey++)
		{
			inifile	<< Encrypt((*m_itKey).first) << Encrypt(ms_strMarkup[E_KEY]) << Encrypt((*m_itKey).second) << "\n";
		}
	}

	inifile.close();

	return true;

}

//-----------------------------------------------------------------------------
// Name        : void CIniFile::Reset()
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Reset IniFile object - clears all sections and keys
//-----------------------------------------------------------------------------
void CIniFile::Reset()
{

	for(m_itSection=m_data.begin(); m_itSection!=m_data.end(); m_itSection++)
	{
		((*m_itSection).second).clear();
	}

	m_data.clear();

}

//-----------------------------------------------------------------------------
// Name        : unsigned int CIniFile::GetSectionCount()
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get number of sections
//-----------------------------------------------------------------------------
unsigned int CIniFile::GetSectionCount()
{
	return m_data.size();
}

//-----------------------------------------------------------------------------
// Name        : unsigned int CIniFile::GetKeyCount(const CStdString& strSection)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get number of keys in specified section
//-----------------------------------------------------------------------------
unsigned int CIniFile::GetKeyCount(const CStdString& strSection)
{
	return m_data[strSection].size();
	
}

//-----------------------------------------------------------------------------
// Name        : unsigned int CIniFile::GetKeyCount(unsigned int iSection)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get number of keys in i-th section
//-----------------------------------------------------------------------------
unsigned int CIniFile::GetKeyCount(unsigned int iSection)
{
	m_itSection = m_data.begin();
	std::advance(m_itSection, iSection);

	return (*m_itSection).second.size();	
}

//-----------------------------------------------------------------------------
// Name        : CStdString CIniFile::GetSection(unsigned int iSection)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get name of i-th section
//-----------------------------------------------------------------------------
CStdString CIniFile::GetSection(unsigned int iSection)
{
	m_itSection = m_data.begin();
	std::advance(m_itSection, iSection);

	return (*m_itSection).first;
}

//-----------------------------------------------------------------------------
// Name        : CStdString CIniFile::GetKey(unsigned int iSection, unsigned int iKey)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get name of i-th key from i-th section
//-----------------------------------------------------------------------------
CStdString CIniFile::GetKey(unsigned int iSection, unsigned int iKey)
{
	m_itSection = m_data.begin();
	std::advance(m_itSection, iSection);

	m_itKey = (*m_itSection).second.begin();
	std::advance(m_itKey, iKey);

	return (*m_itKey).first;
}

//-----------------------------------------------------------------------------
// Name        : CStdString CIniFile::GetKey(const CStdString& strSection, unsigned int iKey)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get name of i-th key from specified section
//-----------------------------------------------------------------------------
CStdString CIniFile::GetKey(const CStdString& strSection, unsigned int iKey)
{
	m_itKey = m_data[strSection].begin();
	std::advance(m_itKey, iKey);

	return (*m_itKey).first;

}

//-----------------------------------------------------------------------------
// Name        : int CIniFile::GetSection(const CStdString& strSection)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get id of specified section
//-----------------------------------------------------------------------------
int CIniFile::GetSection(const CStdString& strSection)
{
	int iCurrentSection = 0;

	for(m_itSection=m_data.begin(); m_itSection!=m_data.end(); m_itSection++)
	{
		if ((*m_itSection).first == strSection)
		{
			return iCurrentSection;
		}

		iCurrentSection++;
	}

	return -1;
}

//-----------------------------------------------------------------------------
// Name        : int CIniFile::GetKey(const CStdString& strSection, const CStdString& strKey)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get id of specified key from specified section
//-----------------------------------------------------------------------------
int CIniFile::GetKey(const CStdString& strSection, const CStdString& strKey)
{
	int iCurrentKey = 0;

	for(m_itKey=m_data[strSection].begin(); m_itKey!=m_data[strSection].end(); m_itKey++)
	{
		if ((*m_itKey).first == strKey)
		{
			return iCurrentKey;
		}

		iCurrentKey++;
	}

	return -1;
}

//-----------------------------------------------------------------------------
// Name        : CStdString CIniFile::GetValue(const CStdString& strSection, const CStdString& strKey)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get string value from specified section/key
//-----------------------------------------------------------------------------
CStdString CIniFile::GetValue(const CStdString& strSection, const CStdString& strKey)
{
	if (!m_bFastRead)
	{
		if (m_data.find(strSection) == m_data.end())
		{
			m_strLastError = ms_strErrorMsg[E_FIND_SECTION];
		}
		else if (m_data[strSection].find(strKey) == m_data[strSection].end())
		{
			m_strLastError = ms_strErrorMsg[E_FIND_KEY];
		}
		else
		{
			return m_data[strSection][strKey];
		}

		#ifdef WIN32
			::MessageBox(NULL, m_strLastError, m_strPath, MB_ICONWARNING);
		#endif

		#ifdef INI_USE_EXEPTIONS
			throw(ms_strError.c_str());
		#endif

		return ms_strError;
	}

	return m_data[strSection][strKey];
}

//-----------------------------------------------------------------------------
// Name        : CStdString CIniFile::GetValueS(const CStdString& strSection, const CStdString& strKey)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get string value from specified section/key
//-----------------------------------------------------------------------------
CStdString CIniFile::GetValueS(const CStdString& strSection, const CStdString& strKey)
{
	return GetValue(strSection,strKey);
}

//-----------------------------------------------------------------------------
// Name        : CStdString CIniFile::GetValue(const CStdString& strSection, const CStdString& strKey, const CStdString& strDefault)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get string value from specified section/key and return default value if section or key not found
//-----------------------------------------------------------------------------
CStdString CIniFile::GetValue(const CStdString& strSection, const CStdString& strKey, const CStdString& strDefault)
{
	if (!m_bFastRead)
	{
		if ( (m_data.find(strSection) == m_data.end()) ||
			 (m_data[strSection].find(strKey) == m_data[strSection].end()) )
		{
			return strDefault;
		}
	}

	return GetValue(strSection,strKey);
}

//-----------------------------------------------------------------------------
// Name        : CStdString CIniFile::GetValueS(const CStdString& strSection, const CStdString& strKey, const CStdString& strDefault)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get string value from specified section/key and return default value if section or key not found
//-----------------------------------------------------------------------------
CStdString CIniFile::GetValueS(const CStdString& strSection, const CStdString& strKey, const CStdString& strDefault)
{
	return GetValue(strSection,strKey,strDefault);

}

//-----------------------------------------------------------------------------
// Name        : int CIniFile::GetValueI(const CStdString& strSection, const CStdString& strKey)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get integer value from specified section/key
//-----------------------------------------------------------------------------
int CIniFile::GetValueI(const CStdString& strSection, const CStdString& strKey)
{

	return _tstoi(GetValue(strSection,strKey).c_str());
}

//-----------------------------------------------------------------------------
// Name        : int CIniFile::GetValueI(const CStdString& strSection, const CStdString& strKey, const int & iDefault)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get integer value from specified section/key and return default value if section or key not found
//-----------------------------------------------------------------------------
int CIniFile::GetValueI(const CStdString& strSection, const CStdString& strKey, const int & iDefault)
{

	CStdString strDefault;
	strDefault.Format(ms_strType[E_INTEGER], iDefault);

	return _tstoi(GetValue(strSection,strKey,strDefault).c_str());

}

//-----------------------------------------------------------------------------
// Name        : bool CIniFile::GetValueB(const CStdString& strSection, const CStdString& strKey)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get boolean value from specified section/key
//-----------------------------------------------------------------------------
bool CIniFile::GetValueB(const CStdString& strSection, const CStdString& strKey)
{

	return TO_BOOL(GetValueI(strSection,strKey));

}

//-----------------------------------------------------------------------------
// Name        : bool CIniFile::GetValueB(const CStdString& strSection, const CStdString& strKey, const bool & bDefault)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get boolean value from specified section/key and return default value if section or key not found
//-----------------------------------------------------------------------------
bool CIniFile::GetValueB(const CStdString& strSection, const CStdString& strKey, const bool & bDefault)
{
	return TO_BOOL(GetValueI(strSection,strKey,TO_INT(bDefault)));

}

//-----------------------------------------------------------------------------
// Name        : float CIniFile::GetValueF(const CStdString& strSection, const CStdString& strKey)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get floating point value from specified section/key
//-----------------------------------------------------------------------------
float CIniFile::GetValueF(const CStdString& strSection, const CStdString& strKey)
{
	return (float)_tstof(GetValue(strSection,strKey).c_str());
}

//-----------------------------------------------------------------------------
// Name        : float CIniFile::GetValueF(const CStdString& strSection, const CStdString& strKey, const float & fDefault)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get floating point value from specified section/key and return default value if section or key not found
//-----------------------------------------------------------------------------
float CIniFile::GetValueF(const CStdString& strSection, const CStdString& strKey, const float & fDefault)
{

	CStdString strDefault;
	strDefault.Format(ms_strType[E_FLOAT], fDefault);

	return (float)_tstof(GetValue(strSection,strKey,strDefault).c_str());
}

//-----------------------------------------------------------------------------
// Name        : void CIniFile::SetReadableExtension(const CStdString& ext)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Change readable exension
//-----------------------------------------------------------------------------
void CIniFile::SetReadableExtension(const CStdString& ext)
{

	m_strReadableExtension = ext;

}

//-----------------------------------------------------------------------------
// Name        : void CIniFile::SetEncryptedExtension(const CStdString& ext)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Change crypted exension
//-----------------------------------------------------------------------------
void CIniFile::SetEncryptedExtension(const CStdString& ext)
{
	m_strEncryptedExtension = ext;
}

//-----------------------------------------------------------------------------
// Name        : CStdString CIniFile::GetLastErrorMessage()
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Get last error message
//-----------------------------------------------------------------------------
CStdString CIniFile::GetLastErrorMessage()
{
	return m_strLastError;

}

//-----------------------------------------------------------------------------
// Name        : bool CIniFile::SetValue(const CStdString& strSection, const CStdString& strKey, const CStdString& value, bool bCreate)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Set string value to specified section/key. Possibility to create section/key if doesn't exist
//-----------------------------------------------------------------------------
bool CIniFile::SetValue(const CStdString& strSection, const CStdString& strKey, const CStdString& value, bool bCreate)
{

	if (m_data.find(strSection) == m_data.end())
	{
		if (!bCreate)
		{
			return false;
		}

		m_data.insert(CSection::value_type(strSection,CKey()));
	}

	if (m_data[strSection].find(strKey)	== m_data[strSection].end())
	{
		if (!bCreate)
		{
			return false;
		}

		m_data[strSection].insert(CKey::value_type(strKey,""));
	}

	m_data[strSection][strKey] = value;
	
	return true;

}

//-----------------------------------------------------------------------------
// Name        : bool CIniFile::SetValueS(const CStdString& strSection, const CStdString& strKey, const CStdString& value, bool create)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Set string value to specified section/key. Possibility to create section/key if doesn't exist
//-----------------------------------------------------------------------------
bool CIniFile::SetValueS(const CStdString& strSection, const CStdString& strKey, const CStdString& value, bool create)
{
	SetValue(strSection, strKey, value, create);
	
	return true;

}

//-----------------------------------------------------------------------------
// Name        : bool CIniFile::SetValue(const CStdString& strSection, const CStdString& strKey, const int & value, bool create)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Set integer value to specified section/key. Possibility to create section/key if doesn't exist
//-----------------------------------------------------------------------------
bool CIniFile::SetValue(const CStdString& strSection, const CStdString& strKey, const int & value, bool create)
{

	return SetValueI(strSection, strKey, value, create);
}

//-----------------------------------------------------------------------------
// Name        : bool CIniFile::SetValue(const CStdString& strSection, const CStdString& strKey, const bool & value, bool create)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Set boolean value to specified section/key. Possibility to create section/key if doesn't exist
//-----------------------------------------------------------------------------
bool CIniFile::SetValue(const CStdString& strSection, const CStdString& strKey, const bool & value, bool create)
{

	return SetValueB(strSection, strKey, value, create);

}

//-----------------------------------------------------------------------------
// Name        : bool CIniFile::SetValue(const CStdString& strSection, const CStdString& strKey, const float & value, bool create)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Set floating point value to specified section/key. Possibility to create section/key if doesn't exist
//-----------------------------------------------------------------------------
bool CIniFile::SetValue(const CStdString& strSection, const CStdString& strKey, const float & value, bool create)
{

	return SetValueF(strSection, strKey, value, create); 
}

//-----------------------------------------------------------------------------
// Name        : bool CIniFile::SetValueI(const CStdString& strSection, const CStdString& strKey, const int & value, bool create)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Set integer value to specified section/key. Possibility to create section/key if doesn't exist
//-----------------------------------------------------------------------------
bool CIniFile::SetValueI(const CStdString& strSection, const CStdString& strKey, const int & value, bool create)
{

	CStdString temp;
	temp.Format(ms_strType[E_INTEGER],value);
	return SetValue(strSection, strKey, temp, create);

}

//-----------------------------------------------------------------------------
// Name        : bool CIniFile::SetValueB(const CStdString& strSection, const CStdString& strKey, const bool & value, bool create)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description :Set boolean value to specified section/key. Possibility to create section/key if doesn't exist
//-----------------------------------------------------------------------------
bool CIniFile::SetValueB(const CStdString& strSection, const CStdString& strKey, const bool & value, bool create)
{
	CStdString temp;
	temp.Format(ms_strType[E_INTEGER],TO_INT(value));
	return SetValue(strSection, strKey, temp, create);
}

//-----------------------------------------------------------------------------
// Name        : bool CIniFile::SetValueF(const CStdString& strSection, const CStdString& strKey, const float & value, bool create)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Set floating point value to specified section/key. Possibility to create section/key if doesn't exist
//-----------------------------------------------------------------------------
bool CIniFile::SetValueF(const CStdString& strSection, const CStdString& strKey, const float & value, bool create)
{

	CStdString temp;
	temp.Format(ms_strType[E_FLOAT],value);
	return SetValue(strSection, strKey, temp, create);
}

//-----------------------------------------------------------------------------
// Name        : bool CIniFile::Delete(const CStdString& strSection, const CStdString& strKey)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Delete section/key
//-----------------------------------------------------------------------------
bool CIniFile::Delete(const CStdString& strSection, const CStdString& strKey)
{
	m_data[strSection].erase(strKey);

	return true; 
}

//-----------------------------------------------------------------------------
// Name        : bool CIniFile::Delete(const CStdString& strSection)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : Delete section
//-----------------------------------------------------------------------------
bool CIniFile::Delete(const CStdString& strSection)
{
	m_data.erase(strSection);

	return true;
}

//-----------------------------------------------------------------------------
// Name        : std::istream & CIniFile::GetLine(std::istream & is, CStdString& str)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : internal function - get line from stream
//-----------------------------------------------------------------------------
std::istream & CIniFile::GetLine(std::istream & is, CStdString& str)
{
	char buf[INI_LINE_MAX_LENGTH];
	is.getline(buf, INI_LINE_MAX_LENGTH);
	str = buf;
	return is;

}

//-----------------------------------------------------------------------------
// Name        : void CIniFile::CleanString(CStdString * pString)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : internal function - clean given string (removes trailing characters)
//-----------------------------------------------------------------------------
void CIniFile::CleanString(CStdString * pString)
{
	int iOldLength = pString->GetLength();

	do for(unsigned int iTrim=0; iTrim<E_TRIM_COUNT; iTrim++)
	{
		iOldLength = pString->GetLength();

		pString->TrimLeft (ms_strTrim[iTrim]);
		pString->TrimRight(ms_strTrim[iTrim]);
	}
	while(pString->GetLength() != iOldLength);

	pString->TrimLeft (ms_strStringDelimiter);
	pString->TrimRight(ms_strStringDelimiter);

}

//-----------------------------------------------------------------------------
// Name        : CStdString CIniFile::ManageComments(CStdString * pString)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : internal function - removes (and retrieve) comment from string
//-----------------------------------------------------------------------------
CStdString CIniFile::ManageComments(CStdString * pString)
{

	// First, look for string delimiters

	int iDelimiterBegin = pString->Find       (ms_strStringDelimiter[0]);
	int iDelimiterEnd   = pString->ReverseFind(ms_strStringDelimiter[0]);

	if (iDelimiterEnd < iDelimiterBegin)
	{
		m_strLastError = ms_strErrorMsg[E_END_DELIMITER];
	}

	// then manage comments

	CStdString strComment = "";

	int iPrevPos, iPos, iLength=pString->GetLength();

	iPrevPos = pString->Find(ms_strComment[E_0]);
	if ((iPrevPos<0)||((iPrevPos>iDelimiterBegin)&&(iPrevPos<iDelimiterEnd))) iPrevPos = iLength;

	iPos = pString->Find(ms_strComment[E_1]);
	if ((iPos<0)||((iPos>iDelimiterBegin)&&(iPos<iDelimiterEnd))) iPos = iLength;

	iPos = (iPrevPos<iPos) ? iPrevPos : iPos;

	iPrevPos = pString->Find(ms_strComment[E_2]);
	if ((iPrevPos<0)||((iPrevPos>iDelimiterBegin)&&(iPrevPos<iDelimiterEnd))) iPrevPos = iLength;

	iPos = (iPrevPos<iPos) ? iPrevPos : iPos;

	if (iPos>=0)
	{
		 strComment = pString->Right(iLength-iPos);
		*pString    = pString->Left (iPos);

		 pString->TrimRight();
		 pString->TrimRight(ms_strComment[E_SPACING]);
	}

	return strComment;

}

//-----------------------------------------------------------------------------
// Name        : void CIniFile::Decrypt(CStdString& str)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : internal function - decrypt string (uses CCrypto)
//-----------------------------------------------------------------------------
void CIniFile::Decrypt(CStdString& str)
{

//	CCrypto::Decrypt(&str);

}

//-----------------------------------------------------------------------------
// Name        : CStdString CIniFile::Encrypt(const CStdString& str)
// Author      : AV (Antoine Villepreux)
// Date        : 20/11/2000
// Description : internal function - encrypt string (uses CCrypto)
//-----------------------------------------------------------------------------
CStdString CIniFile::Encrypt(const CStdString& str)
{
	CStdString strTmp = str;

//	CCrypto::Encrypt(&strTmp);

	return strTmp;
	
	return "";
}
