//------------------------------------------------------------------------------
// File name   : CIniFile.h
// Author      : Micro?s - Antoine Villepreux
// Description : CIniFile class definition
// Purpose     : Easy *.ini files management
//------------------------------------------------------------------------------

#ifndef CINIFILE_H
#define CINIFILE_H

//------------------------------------------------------------------------------
// Precompiler options
//------------------------------------------------------------------------------

#define	INI_USE_EXEPTIONS
#define	INI_CASE_INENSITIVE
#undef	INI_SAVE_ON_EXIT

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#pragma once
#include <map>
#include <fstream>
#include "CStdString.h"

//------------------------------------------------------------------------------
// CIniFile defines
//------------------------------------------------------------------------------

#define INI_LINE_MAX_LENGTH 1024

//------------------------------------------------------------------------------
// CFilename definition
//------------------------------------------------------------------------------

#define CFilename CStdString

//------------------------------------------------------------------------------
// CIniFile definition
//------------------------------------------------------------------------------

class CIniFile
{
public:
	// created in memory, no load from disk

	CIniFile();
	CIniFile(const CFilename &, bool bFastRead = false);
	virtual ~CIniFile();

    // I/O - extension sensitive

	bool Load(const CFilename & filename = "", bool bFastRead = false);
	bool Save(const CFilename & filename = "");

    // Clear all

	void Reset();

	// Errors that 'GetValue' functions may return if key or section doesn't exist

	static const CStdString ms_strError;
	static const int        ms_iError;
	static const float      ms_fError;

	// Get/Set values

	CStdString GetValue (const CStdString& strSection, const CStdString& strKey); // #ifdef INI_USE_EXEPTIONS throw CStdString
	CStdString GetValueS(const CStdString& strSection, const CStdString& strKey);
	int        GetValueI(const CStdString& strSection, const CStdString& strKey); 
	bool       GetValueB(const CStdString& strSection, const CStdString& strKey);
	float      GetValueF(const CStdString& strSection, const CStdString& strKey);

	// No error but default value returned if section/key doesn't exists - use at your own risk

	CStdString GetValue (const CStdString& strSection, const CStdString& strKey, const CStdString& strDefault); 
	CStdString GetValueS(const CStdString& strSection, const CStdString& strKey, const CStdString& strDefault); 
	int        GetValueI(const CStdString& strSection, const CStdString& strKey, const int       & iDefault  ); 
	bool       GetValueB(const CStdString& strSection, const CStdString& strKey, const bool      & bDefault  ); 
	float      GetValueF(const CStdString& strSection, const CStdString& strKey, const float     & fDefault  ); 

	// beware!! 'Save' After Setting values (to save to disk)

	bool SetValue (const CStdString& strSection, const CStdString& strKey, const CStdString& strValue, bool bCreate=true);
	bool SetValue (const CStdString& strSection, const CStdString& strKey, const int       & iValue,   bool bCreate=true);
	bool SetValue (const CStdString& strSection, const CStdString& strKey, const bool      & bValue,   bool bCreate=true);
	bool SetValue (const CStdString& strSection, const CStdString& strKey, const float     & fValue,   bool bCreate=true);
	
	bool SetValueS(const CStdString& strSection, const CStdString& strKey, const CStdString& strValue, bool bCreate=true);
	bool SetValueI(const CStdString& strSection, const CStdString& strKey, const int       & iValue,   bool bCreate=true);
	bool SetValueB(const CStdString& strSection, const CStdString& strKey, const bool      & bValue,   bool bCreate=true);
	bool SetValueF(const CStdString& strSection, const CStdString& strKey, const float     & fValue,   bool bCreate=true);

	// Sections/Keys Deletion/Retrieval
	
    bool Delete(const CStdString& strSection);
	bool Delete(const CStdString& strSection, const CStdString& strKey);

	unsigned int GetSectionCount();
	unsigned int GetKeyCount(unsigned int      iSection);
	unsigned int GetKeyCount(const CStdString& strSection);

	CStdString GetSection(unsigned int      iSection);
	CStdString GetKey    (unsigned int      iSection,   unsigned int iKey);
	CStdString GetKey    (const CStdString& strSection, unsigned int iKey);

	int GetSection(const CStdString& strSection);
	int GetKey    (const CStdString& strSection, const CStdString& strKey);
	
    // Extensions

    void SetReadableExtension (const CStdString&);
    void SetEncryptedExtension(const CStdString&);

	// Error messages

    CStdString GetLastErrorMessage();

//------------------------------------------------------------------------------
// Private
//------------------------------------------------------------------------------

protected:

    enum E_INI_ERROR_MESSAGES
    {
        E_READ = 0,
        E_WRITE,
        E_FIND_SECTION,
        E_FIND_KEY,
        E_UNKNOWN_EXTENSION,
		E_END_DELIMITER,
        E_ERROR_MESSAGES_COUNT,
    };

    enum E_INI_COMMENTS
    {
        E_0 = 0,
        E_1,
        E_2,
        E_SPACING,
        E_COMMENTS_COUNT
    };

    enum E_INI_TYPES
    {
        E_INTEGER = 0,
        E_FLOAT,
        E_STRING,
        E_TYPES_COUNT
    };

    enum E_INI_MARKUPS
    {
        E_SECTION_BEGIN = 0,
        E_SECTION_END,
        E_KEY,
        E_MARKUPS_COUNT
    };

	enum E_INI_TRIM
    {
        E_TRIM_0 = 0,
        E_TRIM_1,
        E_TRIM_COUNT
    };

	enum E_INI_EXTENSIONS
    {
        E_READABLE = 0,
        E_ENCRYPTED,
        E_EXTENSIONS_COUNT
    };

private:

	// Constants

	static const CStdString ms_strErrorMsg	[];
	static const CStdString ms_strComment	[];
	static const CStdString ms_strType		[];
	static const CStdString ms_strMarkup	[];
	static const CStdString ms_strTrim		[];
	static const CStdString ms_strExtension	[];
	static const CStdString ms_strStringDelimiter;

	// Data

	typedef std::map<CStdString, CStdString>	CKey;
	typedef std::map<CStdString, CKey>			CSection;

	CSection m_data;

	CSection::iterator	m_itSection;
	CKey::iterator		m_itKey;

	// File

	CStdString m_strPath;
	std::istream & GetLine(std::istream&, CStdString&);

	bool LoadReadableFile (const CFilename& strFilename = "", bool bIsReadable = true);
	bool SaveReadableFile (const CFilename& strFilename = "");
	bool SaveEncryptedFile(const CFilename& strFilename = "");

	// String format

	CStdString ManageComments(CStdString*);
	void CleanString(CStdString*);

    // Encryption

    CStdString m_strEncryptedExtension;
    CStdString m_strReadableExtension;

	void Decrypt(CStdString&);
	CStdString Encrypt(const CStdString&);

    // Various

	CStdString m_strLastError;
	bool m_bFastRead;
};

//------------------------------------------------------------------------------
// Inline functions
//------------------------------------------------------------------------------

#endif // CIniFile
