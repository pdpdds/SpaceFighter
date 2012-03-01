////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// Ini file read & writer
///
/// 라이선스 : zlib license (http://www.kippler.com/etc/zlib_license/ 참고)
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
	// 데이타 내용
	struct SData
	{
		//DATA_TYPE	nDataType;
		CString		sKey;
		CString		sStrVal;
	};
	// 하나의 섹션
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

public :	// 내부 데이타에 접근하기
	const SectionList&	GetSectionList() { return m_sectionList; }


private :
	// 한줄의 타입
	enum LINE_TYPE
	{
		LINE_TYPE_SECTION,			// [SECTION] 형태
		LINE_TYPE_DATA,				// A=B 형태
		LINE_TYPE_REMARK,			// ;REMARK 형태
		LINE_TYPE_EMPTY,			// 빈줄..
		LINE_TYPE_SINGLELINE,		// 그냥 한줄 텍스트 ( 그냥 = 뒷부분은 생략되었다고 생각한다.. )
		//LINE_TYPE_UNKNOWN,			// 알수 없음?
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
	SectionList	m_sectionList;		// 섹션의 목록
	BOOL		m_bUnicodeIniFile;					// 유니코드 파일 모드인감?
	int			m_nCodePage;						// 유니코드 컴파일 상태에서, ASCII 모드일 경우 코드페이지
	BOOL		m_bAutoSave;						// 상태가 변할때마다 저장할까?
	CString		m_sLine;							// 한줄..
	BOOL		m_bModified;						// 수정되었나?
	BOOL		m_bOpen;							// 열렸나?
	LPCTSTR		m_pDataCurrent;						// 현재 처리중인 데이타 위치
	int			m_nStrLen;							// 문자열의 크기..
	int			m_nStrPos;							// 처리된 캐릭터 카운트
};

