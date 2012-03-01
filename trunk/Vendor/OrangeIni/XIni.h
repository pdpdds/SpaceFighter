////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// Ini file read & writer
///
/// ���̼��� : zlib license (http://www.kippler.com/etc/zlib_license/ ����)
/// 
/// @author   parkkh
/// @date     Monday, February 02, 2009  9:37:53 AM
/// 
/// Copyright(C) 2009 Bandisoft, All rights reserved.
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

// Number bases
#define BASE_BINARY			2
#define BASE_OCTAL			8
#define BASE_DECIMAL		10
#define BASE_HEXADECIMAL	16

#include <string>
#include <atlstr.h>
#include <atlcoll.h>

class XIni  
{
public:
	XIni();
	XIni(LPCTSTR szPathName);
	~XIni();

	BOOL	Open(LPCTSTR szPathName);
	BOOL	OpenMemory(LPCTSTR szData, int nStrLen);
	BOOL	OpenMemory(const BYTE* pData, int nDataLen);

	BOOL	IsOpened() { return m_bOpen; }
	void	Clear();

	int		GetInt(LPCTSTR szSection, LPCTSTR szKey, int nDefault=0, int nBase=BASE_DECIMAL);
	BOOL	GetInt2(LPCTSTR szSection, LPCTSTR szKey, int& result, int nDefault=0, int nBase=BASE_DECIMAL);

	CString	GetStr(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault=_T(""));
	BOOL	GetStr2(LPCTSTR szSection, LPCTSTR szKey, CString& sResult, LPCTSTR szDefault=_T(""));
	void	SetCodePage(int nCodePage) { m_nCodePage = nCodePage; }
	BOOL	SetStr(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue);
	BOOL	SetInt(LPCTSTR szSection, LPCTSTR szKey, int nValue);

	BOOL	SaveAs(LPCTSTR szFilePathName);

public :
	// ����Ÿ ����
	struct SData
	{
		//DATA_TYPE	nDataType;
		CString		sKey;
		CString		sStrVal;
	};
	// �ϳ��� ����
	typedef CAtlList<SData> DataList;
	struct SSection
	{
		SSection() {dataList=NULL;}
		void Create() {dataList = new DataList; }
		void Clear() {delete dataList; dataList=NULL; }
		CString sSection;
		DataList *dataList;
	};

	typedef CAtlList<SSection> SectionList;

public :	// ���� ����Ÿ�� �����ϱ�
	const SectionList&	GetSectionList() { return m_sectionList; }


private :
	// ������ Ÿ��
	enum LINE_TYPE
	{
		LINE_TYPE_SECTION,			// [SECTION] ����
		LINE_TYPE_DATA,				// A=B ����
		LINE_TYPE_REMARK,			// ;REMARK ����
		LINE_TYPE_EMPTY,			// ����..
		LINE_TYPE_SINGLELINE,		// �׳� ���� �ؽ�Ʈ ( �׳� = �޺κ��� �����Ǿ��ٰ� �����Ѵ�.. )
		//LINE_TYPE_UNKNOWN,			// �˼� ����?
	};

private :
	void			Init();
	BOOL			ReadALine();
	BOOL			IsUnicodeFileHeader(const BYTE* buf);
	LINE_TYPE		GetLineType(CString sLine);
	XIni::SSection* FindSection(CString sSectionName);
	XIni::SSection* FindOrMakeSection(CString sSectionName);
	CString			ParseSection(CString sLine);
	BOOL			ParseData(CString sLine, SData& data);
	LPCTSTR			SkipWhiteSpace(LPCTSTR p);
	BOOL			FindData(SSection* pSection, LPCTSTR szKey, SData& data);
	BOOL			FindData(SSection* pSection, LPCTSTR szKey, SData** pData);
	LPSTR			Unicode2Ascii(LPCWSTR szInput, int nCodePage);
	LPWSTR			Ascii2Unicode(LPCSTR szInput, int nCodePage);

private :
	SectionList	m_sectionList;		// ������ ���
	BOOL		m_bUnicodeIniFile;					// �����ڵ� ���� ����ΰ�?
	int			m_nCodePage;						// �����ڵ� ������ ���¿���, ASCII ����� ��� �ڵ�������
	BOOL		m_bAutoSave;						// ���°� ���Ҷ����� �����ұ�?
	CString		m_sLine;							// ����..
	BOOL		m_bModified;						// �����Ǿ���?
	BOOL		m_bOpen;							// ���ȳ�?
	LPCTSTR		m_pDataCurrent;						// ���� ó������ ����Ÿ ��ġ
	int			m_nStrLen;							// ���ڿ��� ũ��..
	int			m_nStrPos;							// ó���� ĳ���� ī��Ʈ
};

