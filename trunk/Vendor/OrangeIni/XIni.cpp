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
///          .ini ���� ����
/// @param   szPathName  
/// @return  
/// @date    2006-11-08 ���� 11:01:14
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL XIni::Open(LPCTSTR szPathName)
{
	// ���� ����Ÿ�� �����.
	Clear();

	BYTE*		pData;							// ����Ÿ ����
	int			nDataLen;						// ����Ÿ ũ��
	HANDLE		hFile;							// ���� �ڵ�
	DWORD		read;

	// ���� ����
	hFile = ::CreateFile(szPathName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if(hFile==INVALID_HANDLE_VALUE) {assert(0); return FALSE; }

	// �޸� ���.
	nDataLen = ::GetFileSize(hFile, NULL);			// ���� ũ��
	if(nDataLen>1000*1000) {assert(0); CloseHandle(hFile); return FALSE;}	// �ʹ� ũ��!!!
	if(nDataLen==0) {CloseHandle(hFile); return TRUE;}						// 0 ����Ʈ�̴�..
	pData = (BYTE*)calloc(1, nDataLen+sizeof(WCHAR));
	if(pData==NULL){assert(0); CloseHandle(hFile); return FALSE;}

	// ���� �б�
	if(::ReadFile(hFile, pData, nDataLen, &read, NULL)==FALSE || read!=(DWORD)nDataLen)
	{
		assert(0);
		//ASSERT(0);
		free(pData);
		CloseHandle(hFile);
		return FALSE;
	}

	// �޸� ����� ó��
	BOOL ret;
	ret = OpenMemory(pData, nDataLen);

	// ����
	CloseHandle(hFile);
	free(pData);

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///          �޸𸮷� ini ������ �д´�... (BOM �� ������� �����ڵ� ó�� ����)
///          ����Ÿ ������ ���� �ݵ�� NULL�� ������ �Ѵ�.!! -> ��� �ǵ��� �����ߵ�..
/// @param   pData  
/// @param   nDataLen  
/// @return  
/// @date    2006-11-20 ���� 3:21:09
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL XIni::OpenMemory(const BYTE* pData, int nDataLen)
{
	if(nDataLen<LEN_OF_BOM) {assert(0); return FALSE;}

	LPCTSTR		pNewData = NULL;
	LPCTSTR		szData;
	int			nStrLen = 0;									// ���ڿ� ī��Ʈ

	if(IsUnicodeFileHeader(pData))								// �����ڵ� ���Ͽ��� ���� ����Ÿ �ΰ�?
	{
#ifdef _UNICODE													// unicode -> unicode
		szData = (LPCTSTR)(pData+LEN_OF_BOM);					// bom ����
#else															// unicode -> ascii
		pNewData = Unicode2Ascii((LPCWSTR)(pData+LEN_OF_BOM), m_nCodePage);
		if(pNewData==NULL){assert(0); return FALSE;}
		szData = pNewData;
#endif
		nStrLen = (nDataLen-LEN_OF_BOM) / sizeof(WCHAR);
	}
	else														// BOM �� ����? == ASII
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

	// ���ڿ��� ����Ÿ ����
	m_bOpen = OpenMemory(szData, nStrLen);

	// �޸� ����.
	if(pNewData) free((void*)pNewData);

	return m_bOpen;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          �޸𸮷� ���� - ���� NULL �� �ȳ����� �ȴ�..
/// @param   szData  
/// @return  
/// @date    2006-11-20 ���� 4:16:02
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
			if(sSectionName.IsEmpty())				// ������ �ִ� ����!
			{
				assert(0);
				pCurSection = NULL;					// ���� ���·� �����.
			}
			else
			{
				pCurSection = FindOrMakeSection(sSectionName);	// ���� ���� �ٲٱ�.
			}

		}
		else if(nLineType==LINE_TYPE_DATA)
		{
			if(pCurSection==NULL)
			{
				assert(0);						// ���� ��Ȳ
			}
			else
			{
				SData data;
				if(ParseData(m_sLine, data)==FALSE)
				{
					assert(0);					// ���� ��Ȳ..
				}
				else
				{
					pCurSection->dataList->AddTail(data);		// ����Ÿ �߰�
				}
			}
		}
		else if(nLineType==LINE_TYPE_SINGLELINE)		// = ���� �׳� ����¥��
		{
			if(pCurSection==NULL)
			{
				assert(0);						// ���� ��Ȳ
			}
			else
			{
				SData data;
				data.sKey = m_sLine;					// Ű���� �ִ´�.
				pCurSection->dataList->AddTail(data);	// ����Ÿ �߰�
			}
		}
		else
		{
			// todo : �ּ�ó��, ��ĭó��, ��Ÿ ��� ó��..
		}
	}

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///          ���� ����Ÿ �� �����.
/// @return  
/// @date    2006-11-08 ���� 10:56:45
////////////////////////////////////////////////////////////////////////////////////////////////////
void XIni::Clear()
{
	POSITION p=m_sectionList.GetHeadPosition();
	while(p)
	{
		SSection section = m_sectionList.GetNext(p);
		section.Clear();								// ������ section �� ��������� Clear() ����� �Ѵ�.
	}
	m_sectionList.RemoveAll();

	m_bUnicodeIniFile = FALSE;
	m_bModified = FALSE;
	m_bOpen = FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          ���� �о ����� m_sLine �� �����ϱ�
/// @return  EOF �̸� FALSE
/// @date    2006-11-08 ���� 11:04:47
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

	// ������ ������ �������� �ʴ´�!!!!!!
	m_sLine.TrimLeft(); 
	m_sLine.TrimRight();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          BOM üũ�ϱ�.
/// @param   buf  
/// @return  
/// @date    2006-11-08 ���� 11:06:04
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL XIni::IsUnicodeFileHeader(const BYTE* buf)
{
	if(buf[0]==0xff && buf[1]==0xfe) return TRUE;	// little endian BOM �� ó���Ѵ�.
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          ������ ���� �����ΰ�?
/// @param   sLine  
/// @return  
/// @date    2006-11-08 ���� 1:35:36
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
///          "[HELLO]" ���� "HELLO" �� �����Ѵ�.
/// @param   sLine  
/// @return  ���н� "" ����
/// @date    2006-11-08 ���� 1:46:14
////////////////////////////////////////////////////////////////////////////////////////////////////
CString XIni::ParseSection(CString sLine)
{
	CString sRet = sLine.Mid(1, sLine.GetLength()-2);
	sRet.TrimLeft(); sRet.TrimRight();
	return sRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          �̸����� ������ ã�´�.
/// @param   sSectionName  
/// @return  
/// @date    2006-11-08 ���� 2:17:52
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
///          SECTION NAME �� ������, ������ �������, ���� ������ ã�´�..
/// @param   sSectionName  
/// @return  
/// @date    2006-11-08 ���� 1:45:27
////////////////////////////////////////////////////////////////////////////////////////////////////
XIni::SSection* XIni::FindOrMakeSection(CString sSectionName)
{
	SSection* pSection = FindSection(sSectionName);
	if(pSection) return pSection;

	// ������ ������ ����.. (�翬�� ����� �Ѵ�.)
	SSection section;
	section.Create();					// ��������� Create() �ؾ� �Ѵ�.
	section.sSection = sSectionName;
	POSITION p = m_sectionList.AddTail(section);
	return &(m_sectionList.GetAt(p));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// HELLO=WORLD �� ���� ������ �Ľ��Ѵ�.
///
/// �Ʒ��� ���� ���µ� �Ľ� �����ϴ�.
///   1) HELLO=WORLD 
///   2) "HELLO"="WORLD"
///   3) HELLO=WORLD ; REMARK
///
/// @param   data  
/// @return  
/// @date    2006-11-08 ���� 1:52:01
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL XIni::ParseData(CString sLine, SData& data)
{
	const TCHAR* p = sLine;
	BOOL	bIsQuot = FALSE;

	data.sKey.Empty();
	data.sStrVal.Empty();

	// LEFT �Ľ�
	if(*p=='"')							// " �� ���۵ǳ�?
	{
		bIsQuot = TRUE;
		p++;
	}
	while(*p)
	{
		if(bIsQuot && *p=='"') { p++; break; }// "�� ������.
		if(bIsQuot==FALSE && *p=='=') break;	// =�� ã�Ҵ�.
		data.sKey += *p;
		p++;
	}
	if(*p==0) {assert(0); return FALSE;}	// �Ľ� ����

	if(bIsQuot==FALSE)							// ����ǥ�� ������
	{data.sKey.TrimLeft(); data.sKey.TrimRight();}

	if(data.sKey.IsEmpty()) {assert(0); return FALSE;}		// ������ �����̴�! ����

	// "aa" = "b" ���� = ������ ���� �ǳ� ���
	p = SkipWhiteSpace(p);
	if(*p!='=') {assert(0); return FALSE;}					// = �� ����?
	p++;					// = �ǳ� ���

	// = ������ ���� �ǳ� ���
	p = SkipWhiteSpace(p);

	// RIGHT �Ľ�
	bIsQuot = FALSE;
	if(*p=='"')							// " �� ���۵ǳ�?
	{
		bIsQuot = TRUE;
		p++;
	}
	while(*p)
	{
		if(bIsQuot && *p=='"') break;			// "�� ������.
		if(bIsQuot==FALSE && *p==';') break;	// ;�� ã�Ҵ�.
		data.sStrVal+= *p;
		p++;
	}
	if(bIsQuot==FALSE)							// ����ǥ�� ������
	{data.sStrVal.TrimLeft(); data.sStrVal.TrimRight();}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          ���� �ǳ� ���
/// @param   p  
/// @return  
/// @date    2006-11-08 ���� 2:02:03
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
///          ���ǿ��� ����Ÿ ã��.
/// @param   pSection  
/// @param   data  
/// @return  
/// @date    2006-11-08 ���� 2:20:50
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
///          INT ���� ã�Ƽ� �����´�.
/// @param   szSection  
/// @param   szKey  
/// @param   nDefault  
/// @param   BASE_DECIMAL  
/// @return  
/// @date    2006-11-08 ���� 2:16:16
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
///          STRING ���� ã�´�.
/// @param   szSection  
/// @param   szKey  
/// @param   szDefault  
/// @return  
/// @date    2006-11-08 ���� 3:04:51
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

	asciilen = wlen;					// asciilen �� wlen ���� ũ�� �ʴ�.
	ret = (LPSTR)malloc(asciilen);		// ASCII �� ����
	if(ret==NULL) {assert(0); return NULL;}
	if(WideCharToMultiByte(nCodePage, 0, szInput,  -1, ret, asciilen, NULL, NULL)==0)	// UCS2->ascii
	{assert(0); free(ret); return NULL;}
	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///          ASCII TO UNICODE
/// @return  ������� �ݵ�� free() �� ����
/// @date    2006-07-20 ���� 3:50:40
////////////////////////////////////////////////////////////////////////////////////////////////////
LPWSTR XIni::Ascii2Unicode(LPCSTR szInput, int nCodePage)
{
	LPWSTR ret;
	int	   ucs2len = int(strlen(szInput)+sizeof(CHAR));
	ret = (LPWSTR)malloc(ucs2len*sizeof(WCHAR));				// UCS2 �� ���� ���
	if(ret==NULL){assert(0); return NULL;}

	if(MultiByteToWideChar(nCodePage, 0, szInput, -1, ret, ucs2len)==0)	// MBCS -> UCS2
	{assert(0); free(ret); return NULL;}
	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///         ���Ϸ� �����ϱ�.
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


	// bom ����
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
		pData->sKey = szKey;						// ���� ����Ÿ �ٲٱ�
		pData->sStrVal = szValue;
		return TRUE;
	}

	SData data;										// ���� ����Ÿ �߰��ϱ�
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

