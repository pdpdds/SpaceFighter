#include "stdafx.h"
#include "XIni.h"
#include <assert.h>

#define LEN_OF_BOM	2

XIni::XIni()
{
	Init();
}

XIni::XIni(LPCTSTR szPathName)
{
	Init();
	Open(szPathName);
}

void XIni::Init()
{
	m_bUnicodeIniFile = TRUE;
	m_nCodePage = CP_ACP;
	m_bModified = FALSE;
	m_bOpen = FALSE;
	m_pDataCurrent = NULL;
	m_nStrLen = 0;
	m_nStrPos = 0;
}

XIni::~XIni()
{
	Clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          .ini 파일 열기
/// @param   szPathName  
/// @return  
/// @date    2006-11-08 오전 11:01:14
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL XIni::Open(LPCTSTR szPathName)
{
	// 이전 데이타는 지우고.
	Clear();

	BYTE*		pData;							// 데이타 내용
	int			nDataLen;						// 데이타 크기
	HANDLE		hFile;							// 파일 핸들
	DWORD		read;

	// 파일 열기
	hFile = ::CreateFile(szPathName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if(hFile==INVALID_HANDLE_VALUE) {assert(0); return FALSE; }

	// 메모리 잡기.
	nDataLen = ::GetFileSize(hFile, NULL);			// 파일 크기
	if(nDataLen>1000*1000) {assert(0); CloseHandle(hFile); return FALSE;}	// 너무 크다!!!
	if(nDataLen==0) {CloseHandle(hFile); return TRUE;}						// 0 바이트이다..
	pData = (BYTE*)calloc(1, nDataLen+sizeof(WCHAR));
	if(pData==NULL){assert(0); CloseHandle(hFile); return FALSE;}

	// 파일 읽기
	if(::ReadFile(hFile, pData, nDataLen, &read, NULL)==FALSE || read!=(DWORD)nDataLen)
	{
		assert(0);
		//ASSERT(0);
		free(pData);
		CloseHandle(hFile);
		return FALSE;
	}

	// 메모리 열기로 처리
	BOOL ret;
	ret = OpenMemory(pData, nDataLen);

	// 해제
	CloseHandle(hFile);
	free(pData);

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///          메모리로 ini 내용을 읽는다... (BOM 이 있을경우 유니코드 처리 가능)
///          데이타 내용의 끝이 반드시 NULL로 끝나야 한다.!! -> 없어도 되도록 수정했따..
/// @param   pData  
/// @param   nDataLen  
/// @return  
/// @date    2006-11-20 오후 3:21:09
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL XIni::OpenMemory(const BYTE* pData, int nDataLen)
{
	if(nDataLen<LEN_OF_BOM) {assert(0); return FALSE;}

	LPCTSTR		pNewData = NULL;
	LPCTSTR		szData;
	int			nStrLen = 0;									// 문자열 카운트

	if(IsUnicodeFileHeader(pData))								// 유니코드 파일에서 읽은 데이타 인가?
	{
#ifdef _UNICODE													// unicode -> unicode
		szData = (LPCTSTR)(pData+LEN_OF_BOM);					// bom 빼기
#else															// unicode -> ascii
		pNewData = Unicode2Ascii((LPCWSTR)(pData+LEN_OF_BOM), m_nCodePage);
		if(pNewData==NULL){assert(0); return FALSE;}
		szData = pNewData;
#endif
		nStrLen = (nDataLen-LEN_OF_BOM) / sizeof(WCHAR);
	}
	else														// BOM 이 없다? == ASII
	{
#ifdef _UNICODE													// ascii -> unicode
		pNewData = Ascii2Unicode((LPCSTR)pData, m_nCodePage);
		if(pNewData==NULL){assert(0); return FALSE;}
		szData = pNewData;
#else															// ascii -> ascii
		szData = (LPCTSTR)(pData);
#endif

		nStrLen = nDataLen;

	}

	// 문자열로 데이타 열기
	m_bOpen = OpenMemory(szData, nStrLen);

	// 메모리 해제.
	if(pNewData) free((void*)pNewData);

	return m_bOpen;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          메모리로 열기 - 끝이 NULL 로 안끝나도 된다..
/// @param   szData  
/// @return  
/// @date    2006-11-20 오후 4:16:02
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL XIni::OpenMemory(LPCTSTR szData, int nStrLen)
{
	m_pDataCurrent = szData;

	m_nStrLen = nStrLen;
	m_nStrPos = 0;

	LINE_TYPE nLineType;
	SSection* pCurSection=NULL;
	for(;;)
	{
		if(ReadALine()==FALSE) break;				// read and check eof

		nLineType = GetLineType(m_sLine);
		if(nLineType==LINE_TYPE_SECTION)
		{
			CString sSectionName = ParseSection(m_sLine);
			if(sSectionName.IsEmpty())				// 문제가 있는 섹션!
			{
				assert(0);
				pCurSection = NULL;					// 에러 상태로 만든다.
			}
			else
			{
				pCurSection = FindOrMakeSection(sSectionName);	// 현재 섹션 바꾸기.
			}

		}
		else if(nLineType==LINE_TYPE_DATA)
		{
			if(pCurSection==NULL)
			{
				assert(0);						// 에러 상황
			}
			else
			{
				SData data;
				if(ParseData(m_sLine, data)==FALSE)
				{
					assert(0);					// 에러 상황..
				}
				else
				{
					pCurSection->dataList->AddTail(data);		// 데이타 추가
				}
			}
		}
		else if(nLineType==LINE_TYPE_SINGLELINE)		// = 없이 그냥 한줄짜리
		{
			if(pCurSection==NULL)
			{
				assert(0);						// 에러 상황
			}
			else
			{
				SData data;
				data.sKey = m_sLine;					// 키값만 넣는다.
				pCurSection->dataList->AddTail(data);	// 데이타 추가
			}
		}
		else
		{
			// todo : 주석처리, 빈칸처리, 기타 등등 처리..
		}
	}

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///          내부 데이타 다 지우기.
/// @return  
/// @date    2006-11-08 오전 10:56:45
////////////////////////////////////////////////////////////////////////////////////////////////////
void XIni::Clear()
{
	POSITION p=m_sectionList.GetHeadPosition();
	while(p)
	{
		SSection section = m_sectionList.GetNext(p);
		section.Clear();								// 각각의 section 은 명시적으로 Clear() 해줘야 한다.
	}
	m_sectionList.RemoveAll();

	m_bUnicodeIniFile = FALSE;
	m_bModified = FALSE;
	m_bOpen = FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          한줄 읽어서 결과를 m_sLine 에 저장하기
/// @return  EOF 이면 FALSE
/// @date    2006-11-08 오전 11:04:47
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL XIni::ReadALine()
{
	TCHAR	c;
	BOOL	bEof=FALSE;


	m_sLine.Empty();
	for(;;)
	{
		m_nStrPos++;
		if(m_nStrPos>m_nStrLen)
		{
			bEof = TRUE;									// eof!
			break;
		}

		c = *m_pDataCurrent;
		if(c==0)
		{
			bEof = TRUE;									// eof!
			break;
		}
		m_pDataCurrent ++;

		if(c=='\n') break;									// end of line
		if(c=='\r') continue;								// skip
		m_sLine += c;
	}

	if(m_sLine.GetLength()==0 && bEof==TRUE) return FALSE;		// end of file!

	// 라인의 공백은 인정하지 않는다!!!!!!
	m_sLine.TrimLeft(); 
	m_sLine.TrimRight();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          BOM 체크하기.
/// @param   buf  
/// @return  
/// @date    2006-11-08 오전 11:06:04
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL XIni::IsUnicodeFileHeader(const BYTE* buf)
{
	if(buf[0]==0xff && buf[1]==0xfe) return TRUE;	// little endian BOM 만 처리한다.
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          한줄이 무슨 내용인감?
/// @param   sLine  
/// @return  
/// @date    2006-11-08 오후 1:35:36
////////////////////////////////////////////////////////////////////////////////////////////////////
XIni::LINE_TYPE XIni::GetLineType(CString sLine)
{
	if(sLine.GetLength()==0) return LINE_TYPE_EMPTY;

	if(sLine.Left(1)==_T("[") && sLine.Right(1)==_T("]")) 
		return LINE_TYPE_SECTION;

	if(sLine.Left(1)==_T(";"))
		return LINE_TYPE_REMARK;

	if(sLine.Find('=')>=0) 
		return LINE_TYPE_DATA;

	return LINE_TYPE_SINGLELINE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          "[HELLO]" 에서 "HELLO" 를 추출한다.
/// @param   sLine  
/// @return  실패시 "" 리턴
/// @date    2006-11-08 오후 1:46:14
////////////////////////////////////////////////////////////////////////////////////////////////////
CString XIni::ParseSection(CString sLine)
{
	CString sRet = sLine.Mid(1, sLine.GetLength()-2);
	sRet.TrimLeft(); sRet.TrimRight();
	return sRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          이름으로 섹션을 찾는다.
/// @param   sSectionName  
/// @return  
/// @date    2006-11-08 오후 2:17:52
////////////////////////////////////////////////////////////////////////////////////////////////////
XIni::SSection* XIni::FindSection(CString sSectionName)
{
	POSITION p = m_sectionList.GetHeadPosition();
	while(p)
	{
		SSection& section = m_sectionList.GetNext(p);
		if(section.sSection.CompareNoCase(sSectionName)==0)
		{
			return &section;
		}
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          SECTION NAME 을 가지고, 섹션을 만들던가, 기존 섹션을 찾는다..
/// @param   sSectionName  
/// @return  
/// @date    2006-11-08 오후 1:45:27
////////////////////////////////////////////////////////////////////////////////////////////////////
XIni::SSection* XIni::FindOrMakeSection(CString sSectionName)
{
	SSection* pSection = FindSection(sSectionName);
	if(pSection) return pSection;

	// 기존에 섹션이 없다.. (당연히 없어야 한다.)
	SSection section;
	section.Create();					// 명시적으로 Create() 해야 한다.
	section.sSection = sSectionName;
	POSITION p = m_sectionList.AddTail(section);
	return &(m_sectionList.GetAt(p));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// HELLO=WORLD 와 같은 문장을 파싱한다.
///
/// 아래와 같은 형태도 파싱 가능하다.
///   1) HELLO=WORLD 
///   2) "HELLO"="WORLD"
///   3) HELLO=WORLD ; REMARK
///
/// @param   data  
/// @return  
/// @date    2006-11-08 오후 1:52:01
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL XIni::ParseData(CString sLine, SData& data)
{
	const TCHAR* p = sLine;
	BOOL	bIsQuot = FALSE;

	data.sKey.Empty();
	data.sStrVal.Empty();

	// LEFT 파싱
	if(*p=='"')							// " 로 시작되나?
	{
		bIsQuot = TRUE;
		p++;
	}
	while(*p)
	{
		if(bIsQuot && *p=='"') { p++; break; }// "로 끝났다.
		if(bIsQuot==FALSE && *p=='=') break;	// =를 찾았다.
		data.sKey += *p;
		p++;
	}
	if(*p==0) {assert(0); return FALSE;}	// 파싱 실패

	if(bIsQuot==FALSE)							// 따옴표가 없으면
	{data.sKey.TrimLeft(); data.sKey.TrimRight();}

	if(data.sKey.IsEmpty()) {assert(0); return FALSE;}		// 왼쪽이 공백이다! 에러

	// "aa" = "b" 에서 = 이전의 공백 건너 띄기
	p = SkipWhiteSpace(p);
	if(*p!='=') {assert(0); return FALSE;}					// = 가 없다?
	p++;					// = 건너 띄기

	// = 다음의 공백 건너 띄기
	p = SkipWhiteSpace(p);

	// RIGHT 파싱
	bIsQuot = FALSE;
	if(*p=='"')							// " 로 시작되나?
	{
		bIsQuot = TRUE;
		p++;
	}
	while(*p)
	{
		if(bIsQuot && *p=='"') break;			// "로 끝났다.
		if(bIsQuot==FALSE && *p==';') break;	// ;를 찾았다.
		data.sStrVal+= *p;
		p++;
	}
	if(bIsQuot==FALSE)							// 따옴표가 없으면
	{data.sStrVal.TrimLeft(); data.sStrVal.TrimRight();}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          공백 건너 띄기
/// @param   p  
/// @return  
/// @date    2006-11-08 오후 2:02:03
////////////////////////////////////////////////////////////////////////////////////////////////////
LPCTSTR XIni::SkipWhiteSpace(LPCTSTR p)
{
	while(*p)
	{
		if(*p==' ' || *p=='\t') p++;
		else break;
	}
	return p;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          섹션에서 데이타 찾기.
/// @param   pSection  
/// @param   data  
/// @return  
/// @date    2006-11-08 오후 2:20:50
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL XIni::FindData(SSection* pSection, LPCTSTR szKey, SData& data)
{
	POSITION p = pSection->dataList->GetHeadPosition();
	while(p)
	{
		SData& _data = pSection->dataList->GetNext(p);
		if(_data.sKey.CompareNoCase(szKey)==0)
		{
			data = _data;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL XIni::FindData(SSection* pSection, LPCTSTR szKey, SData** ppData)
{
	POSITION p = pSection->dataList->GetHeadPosition();
	while(p)
	{
		*ppData = &(pSection->dataList->GetNext(p));
		if((*ppData)->sKey.CompareNoCase(szKey)==0)
		{
			return TRUE;
		}
	}
	return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///          INT 값을 찾아서 가져온다.
/// @param   szSection  
/// @param   szKey  
/// @param   nDefault  
/// @param   BASE_DECIMAL  
/// @return  
/// @date    2006-11-08 오후 2:16:16
////////////////////////////////////////////////////////////////////////////////////////////////////
int XIni::GetInt(LPCTSTR szSection, LPCTSTR szKey, int nDefault, int nBase) 
{
	SSection* pSection = FindSection(szSection);
	if(pSection==NULL) return nDefault;

	SData data;
	if(FindData(pSection, szKey, data)==FALSE) return nDefault;

	int ret;
	ret = _tcstoul(data.sStrVal, NULL, nBase);

	return ret;
}

BOOL XIni::GetInt2(LPCTSTR szSection, LPCTSTR szKey, int& result, int nDefault, int nBase)
{
	result = nDefault;

	SSection* pSection = FindSection(szSection);
	if(pSection==NULL) return FALSE;

	SData data;
	if(FindData(pSection, szKey, data)==FALSE) return FALSE;

	result = _tcstoul(data.sStrVal, NULL, nBase);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          STRING 값을 찾는다.
/// @param   szSection  
/// @param   szKey  
/// @param   szDefault  
/// @return  
/// @date    2006-11-08 오후 3:04:51
////////////////////////////////////////////////////////////////////////////////////////////////////
CString	XIni::GetStr(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault)
{
	SSection* pSection = FindSection(szSection);
	if(pSection==NULL) return szDefault;

	SData data;
	if(FindData(pSection, szKey, data)==FALSE) return szDefault;

	return data.sStrVal;
}

BOOL XIni::GetStr2(LPCTSTR szSection, LPCTSTR szKey, CString& sResult, LPCTSTR szDefault)
{
	SSection* pSection = FindSection(szSection);
	if(pSection==NULL) { sResult= szDefault; return FALSE; }

	SData data;
	if(FindData(pSection, szKey, data)==FALSE) { sResult= szDefault; return FALSE; }

	sResult = data.sStrVal;
	return TRUE;
}


LPSTR XIni::Unicode2Ascii(LPCWSTR szInput, int nCodePage)
{
	LPSTR	ret=NULL;
	int		wlen = int((wcslen(szInput)+1)*sizeof(WCHAR));
	int		asciilen;

	asciilen = wlen;					// asciilen 은 wlen 보다 크지 않다.
	ret = (LPSTR)malloc(asciilen);		// ASCII 용 버퍼
	if(ret==NULL) {assert(0); return NULL;}
	if(WideCharToMultiByte(nCodePage, 0, szInput,  -1, ret, asciilen, NULL, NULL)==0)	// UCS2->ascii
	{assert(0); free(ret); return NULL;}
	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          ASCII TO UNICODE
/// @return  결과값은 반드시 free() 로 해제
/// @date    2006-07-20 오후 3:50:40
////////////////////////////////////////////////////////////////////////////////////////////////////
LPWSTR XIni::Ascii2Unicode(LPCSTR szInput, int nCodePage)
{
	LPWSTR ret;
	int	   ucs2len = int(strlen(szInput)+sizeof(CHAR));
	ret = (LPWSTR)malloc(ucs2len*sizeof(WCHAR));				// UCS2 용 버퍼 잡기
	if(ret==NULL){assert(0); return NULL;}

	if(MultiByteToWideChar(nCodePage, 0, szInput, -1, ret, ucs2len)==0)	// MBCS -> UCS2
	{assert(0); free(ret); return NULL;}
	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///         파일로 저장하기.
/// @param  
/// @return 
/// @date   Friday, April 11, 2008  5:03:29 PM
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL XIni::SaveAs(LPCTSTR szFilePathName)
{
	//FILE* fp = _tfopen(szFilePathName, _T("wb"));
	FILE* fp;
	errno_t err = _tfopen_s(&fp, szFilePathName, _T("wb"));
	if(err!=0) {assert(0); return FALSE;}
	if(fp==NULL){assert(0); return FALSE;}


	// bom 쓰기
#ifdef _UNICODE
	fputc(0xff, fp);
	fputc(0xfe, fp);
#endif

	POSITION p = m_sectionList.GetHeadPosition();
	while(p)
	{
		SSection &section =  m_sectionList.GetNext(p);

		::_ftprintf(fp, _T("[%s]\r\n"), (LPCTSTR)section.sSection);

		if(section.dataList)
		{
			POSITION q = section.dataList->GetHeadPosition();
			while(q)
			{
				XIni::SData& data = section.dataList->GetNext(q);
				::_ftprintf(fp, _T("%s = %s\r\n"), (LPCTSTR)data.sKey, (LPCTSTR)data.sStrVal);
			}
		}
		::_ftprintf(fp, _T("\r\n"));
	}
	fclose(fp);
	return TRUE;
}


BOOL XIni::SetStr(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue)
{
	SSection* pSection;

	pSection = FindOrMakeSection(szSection);

	SData *pData;
	if(FindData(pSection, szKey, &pData))
	{
		pData->sKey = szKey;						// 기존 데이타 바꾸기
		pData->sStrVal = szValue;
		return TRUE;
	}

	SData data;										// 새로 데이타 추가하기
	data.sKey = szKey;
	data.sStrVal = szValue;
	pSection->dataList->AddTail(data);

	return TRUE;
}

BOOL XIni::SetInt(LPCTSTR szSection, LPCTSTR szKey, int nValue)
{
	CString str;
	str.Format(_T("%d"), nValue);
	return SetStr(szSection, szKey, str);
}

