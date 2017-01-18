#include "stdafx.h"
#include "_CACodeLab.h"


int _CACodeLab::THREAD_MAX_NUM = 5;
//int _CACodeLab::THREAD_NUM = 0;
_CACodeLab::_CACodeLab()
{
}


_CACodeLab::~_CACodeLab()
{
}


void _CACodeLab::GetCurrentWorkDirectory()
{
	char buffer[_MAX_PATH];
	_getcwd(buffer, _MAX_PATH);
	std::cout << "The current directory is " << buffer << std::endl;
	wchar_t wbuffer[_MAX_PATH];
	_wgetcwd(wbuffer, _MAX_PATH);
	std::wcout << L"The current directory is " << wbuffer << "(unicode)" << std::endl;
}


void _CACodeLab::WriteFileOnLength()
{
	std::ofstream fout(L"d:\\test.txt");
	//Ver.1
	//int length = 1 * 1024 * 1024 *1024;
	//for (size_t i = 0; i < length; i++)
	//{
	//	fout << buffer;
	//}
	//Ver.2
	std::string buffer(1024 * 1024 * 1024, 'x');
	fout << buffer;
	fout.close();
}


void _CACodeLab::ReadFileWithBuffer()
{
	char * buffer = new char[1 * 1024 * 1024 + 1];
	std::ifstream fin(L"d:\\test.txt");
	int length = 0;
	while (!fin.eof())
	{
		fin.get(buffer, 1 * 1024 * 1024 + 1);
		length += (int)strlen(buffer);
		std::cout << "cached 1MB. length of buffer is " << length << std::endl;
	}
	delete[] buffer;
	fin.close();
}


bool _CACodeLab::GetFileLength(size_t & filelength, const std::wstring & filename)
{
	LARGE_INTEGER fileSize;
	HANDLE handle = CreateFile(filename.c_str(),
		FILE_READ_EA,
		FILE_SHARE_READ,
		0,
		OPEN_EXISTING,
		0,
		0);
	if (handle != INVALID_HANDLE_VALUE)
	{
		//filelength = GetFileSize(handle, NULL);
		if (!GetFileSizeEx(handle, &fileSize))
		{
			CloseHandle(handle);
			return false;
		}
		filelength = (size_t)fileSize.QuadPart;
		CloseHandle(handle);
		return true;
	}
	else
	{
		DWORD lastError = GetLastError();
		DWORD t1 = lastError;
	}
	return false;
}

size_t _CACodeLab::GetFileLength(const std::wstring & filePath)
{
	size_t fileLength;
	if (!GetFileLength(fileLength, filePath))
	{
		_CALog::Log(L"Can not get file length.", L"D:\\CALog.txt");
		throw std::exception("Error");
	}
	return fileLength;
}


void _CACodeLab::CountTime()
{
	//#include <ctime>
	clock_t start, finish;
	start = clock();
	finish = clock();
	std::cout << "Time : " << (double)(finish - start) / CLOCKS_PER_SEC << "s" << std::endl;
}


bool _CACodeLab::ReadFileWithMemMapping(std::vector<BYTE> & filestream, const std::wstring & filename)
{
	HANDLE fileH = CreateFile(filename.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (fileH == INVALID_HANDLE_VALUE)
	{
		std::cout << "error in CreateFile" << std::endl;
		return false;
	}
	HANDLE mapfileH = CreateFileMapping(fileH,
		NULL,
		PAGE_READWRITE,
		0,
		0,
		L"Resource ");
	if (mapfileH == NULL)
	{
		std::cout << "Error in CreateFileMapping" << std::endl;
		return false;
	}
	char * mapH = (char *)MapViewOfFile(mapfileH,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0);
	if (mapH == NULL)
	{
		std::cout << "Error in MapViewOfFile" << std::endl;
		return false;
	}

	char * buff = mapH;
	//char * buffer = new char[1 * 1024 * 1024 * 1024 + 1];
	//char * pbuffer = buffer;
	//for (size_t i = 0; i < 1 * 1024 * 1024 * 1024; i++)
	//{
	//	*pbuffer++ = buff[i];
	//}
	//*pbuffer = '\0';
	//filestream.resize(filelength);
	int filepos = 0;
	for (std::vector<BYTE>::iterator i = filestream.begin();i != filestream.end(); i++)
	{
		*i = (BYTE)buff[filepos++];
	}
	UnmapViewOfFile(mapH);
	CloseHandle(mapfileH);
	CloseHandle(fileH);
	return true;
}

bool _CACodeLab::ReadFileWithMemMapping(BYTE * filestream, const size_t & fileLength, const std::wstring & filename)
{
	size_t _fileLength;
	if (!GetFileLength(_fileLength, filename)||(_fileLength < fileLength))
	{
		return false;
	}
	HANDLE fileH = CreateFile(filename.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (fileH == INVALID_HANDLE_VALUE)
	{
		std::cout << "error in CreateFile" << std::endl;
		return false;
	}
	HANDLE mapfileH = CreateFileMapping(fileH,
		NULL,
		PAGE_READWRITE,
		0,
		0,
		L"Resource ");
	if (mapfileH == NULL)
	{
		std::cout << "Error in CreateFileMapping" << std::endl;
		return false;
	}
	char * mapH = (char *)MapViewOfFile(mapfileH,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0);
	if (mapH == NULL)
	{
		std::cout << "Error in MapViewOfFile" << std::endl;
		return false;
	}
	BYTE * buff = (BYTE *)mapH;
	BYTE * pbuffer = filestream;
	for (size_t i = 0; i < fileLength; i++)
	{
		*pbuffer++ = buff[i];
	}
	UnmapViewOfFile(mapH);
	CloseHandle(mapfileH);
	CloseHandle(fileH);
	return true;
}

bool _CACodeLab::ReadFileWithMemMapping(std::string & filestream, const size_t & fileLength, const std::wstring & filename)
{
	size_t _fileLength;
	if (!GetFileLength(_fileLength, filename) || (_fileLength < fileLength))
	{
		return false;
	}
	HANDLE fileH = CreateFile(filename.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (fileH == INVALID_HANDLE_VALUE)
	{
		std::cout << "error in CreateFile" << std::endl;
		return false;
	}
	HANDLE mapfileH = CreateFileMapping(fileH,
		NULL,
		PAGE_READWRITE,
		0,
		0,
		L"Resource ");
	if (mapfileH == NULL)
	{
		std::cout << "Error in CreateFileMapping" << std::endl;
		return false;
	}
	char * mapH = (char *)MapViewOfFile(mapfileH,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0);
	if (mapH == NULL)
	{
		std::cout << "Error in MapViewOfFile" << std::endl;
		return false;
	}
	BYTE * buff = (BYTE *)mapH;
	//BYTE * pbuffer = filestream;
	for (size_t i = 0; i < fileLength; i++)
	{
		filestream.append((char *)buff++);
	}
	UnmapViewOfFile(mapH);
	CloseHandle(mapfileH);
	CloseHandle(fileH);
	return true;
}

//bool _CACodeLab::ReadFileWithMemMapping(_CAFileStream & filestream, const INT64 & fileLength, const std::wstring & filename)
//{
//	INT64 _fileLength;
//	if (!GetFileLength(_fileLength, filename) || (_fileLength < fileLength))
//	{
//		return false;
//	}
//	HANDLE fileH = CreateFile(filename.c_str(),
//		GENERIC_READ | GENERIC_WRITE,
//		FILE_SHARE_READ,
//		NULL,
//		OPEN_EXISTING,
//		FILE_ATTRIBUTE_NORMAL,
//		NULL);
//	if (fileH == INVALID_HANDLE_VALUE)
//	{
//		std::cout << "error in CreateFile" << std::endl;
//		return false;
//	}
//	HANDLE mapfileH = CreateFileMapping(fileH,
//		NULL,
//		PAGE_READWRITE,
//		0,
//		0,
//		L"Resource ");
//	if (mapfileH == NULL)
//	{
//		std::cout << "Error in CreateFileMapping" << std::endl;
//		return false;
//	}
//	char * mapH = (char *)MapViewOfFile(mapfileH,
//		FILE_MAP_ALL_ACCESS,
//		0,
//		0,
//		0);
//	if (mapH == NULL)
//	{
//		std::cout << "Error in MapViewOfFile" << std::endl;
//		return false;
//	}
//	filestream.GetData(mapH, fileLength);
//	UnmapViewOfFile(mapH);
//	CloseHandle(mapfileH);
//	CloseHandle(fileH);
//	return true;
//}


void _CACodeLab::WriteFileWithMemMapping()
{
	HANDLE fileH = CreateFile(L"d:\\test1.txt",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (fileH == INVALID_HANDLE_VALUE)
	{
		std::cout << "error in CreateFile" << std::endl;
		//return -1;
	}
	HANDLE mapfileH = CreateFileMapping(fileH,
		NULL,
		PAGE_READWRITE,
		0,
		1024 * 1024 * 1024,
		L"Resource ");
	if (mapfileH == NULL)
	{
		std::cout << "Error in CreateFileMapping" << std::endl;
		//return -1;
	}
	char * mapH = (char *)MapViewOfFile(mapfileH,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0);
	if (mapH == NULL)
	{
		std::cout << "Error in MapViewOfFile" << std::endl;
		//return -1;
	}

	char * buff = mapH;
	char * buffer = new char[1 * 1024 * 1024 * 1024 + 1];
	char * pbuffer = buffer;
	for (size_t i = 0; i < 1 * 1024 * 1024 * 1024; i++)
	{
		buff[i] = *pbuffer++;
	}
	UnmapViewOfFile(mapH);
	CloseHandle(mapfileH);
	CloseHandle(fileH);
	delete[] buffer;
}



// Get file size.
__int64 _CACodeLab::CLGetFileSize(std::wstring filePath)
{
	HANDLE handle = CreateFile(
		filePath.c_str(),
		FILE_READ_EA,
		FILE_SHARE_READ,
		0,
		OPEN_EXISTING,
		0,
		0);
	if (handle == INVALID_HANDLE_VALUE)
	{
		throw std::exception("Can not get file handle.");
	}
	DWORD size = GetFileSize(handle, NULL);
	CloseHandle(handle);
	return size;
}


void _CACodeLab::GetDirList()
{
	std::vector<std::string> filelist;
	std::string path("D:\\");
	//GetFiles(path, filelist);
	//#include <Windows.h>
	DWORDLONG hFile = NULL;
	//#include <io.h>
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if (fileinfo.attrib & _A_SUBDIR)
			{
				std::cout << "(folder) " << fileinfo.name << " size = " << fileinfo.size << std::endl;
			}
			else
			{
				std::cout << "(file)   " << fileinfo.name << " size = " << fileinfo.size << std::endl;
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	//if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
	//{
	//	do 
	//	{
	//		//如果是目录,迭代之  
	//		//如果不是,加入列表  
	//		if ((fileinfo.attrib   &   _A_SUBDIR))
	//		{
	//			if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
	//				GetFiles(p.assign(path).append("/").append(fileinfo.name), filelist);

	//		}
	//		else {
	//			filelist.push_back(p.assign(path).append("/").append(fileinfo.name));

	//		}

	//	} while (_findnext(hFile, &fileinfo) == 0);

	//	_findclose(hFile);

	//}
	;
	for (std::vector<std::string>::iterator i = filelist.begin(); i < filelist.cend(); i++)
	{
		std::cout << *i << std::endl;
	}
}



// Test virtual function.
void _CACodeLab::TestVirtualFunction()
{
	/*_CAFile File;
	_CAFolder Folder;
	_CAFileBase * pFile;
	std::cout << File.IsFolder() << std::endl;
	std::cout << Folder.IsFolder() << std::endl;
	pFile = &File;
	std::cout << pFile->IsFolder() << std::endl;
	pFile = &Folder;
	std::cout << pFile->IsFolder() << std::endl;*/
}


//std::string _CACodeLab::WChar2Ansi(LPCWSTR pwszSrc)
//{
//	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
//	if (nLen <= 0) return std::string("");
//	char* pszDst = new char[nLen];
//	if (NULL == pszDst) return std::string("");
//	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
//	pszDst[nLen - 1] = 0;
//	std::string strTemp(pszDst);
//	delete[] pszDst;
//	return strTemp;
//}
//
//
//std::wstring _CACodeLab::Ansi2WChar(LPCSTR pszSrc, int nLen)
//{
//	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
//	if (nSize <= 0) return NULL;
//	WCHAR *pwszDst = new WCHAR[nSize + 1];
//	if (NULL == pwszDst) return NULL;
//	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, pwszDst, nSize);
//	pwszDst[nSize] = 0;
//	if (pwszDst[0] == 0xFEFF) // skip Oxfeff
//		for (int i = 0; i < nSize; i++)
//			pwszDst[i] = pwszDst[i + 1];
//	std::wstring wcharString(pwszDst);
//	delete[] pwszDst;
//	return wcharString;
//}
//
//std::wstring _CACharConversion::ansi2unicode(const std::string & src)
//{
//	if (src.length() >(std::numeric_limits<int>::max)())
//	{
//		std::cout << "String(utf8) too long." << std::endl;
//		return std::wstring();
//	}
//	return Ansi2WChar(src.c_str(), (int)src.length());
//}
//
//
//std::string _CACodeLab::WStringToMBytes(const wchar_t* lpwcszWString)
//{
//	char* pElementText;
//	int iTextLen;
//	// wide char to multi char
//	iTextLen = WideCharToMultiByte(CP_ACP, 0, lpwcszWString, -1, NULL, 0, NULL, NULL);
//	pElementText = new char[iTextLen + 1];
//	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
//	WideCharToMultiByte(CP_ACP, 0, lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
//	std::string strReturn(pElementText);
//	delete[] pElementText;
//	return strReturn;
//}
//
// Output info to file.
//void _CACodeLab::FileOut(std::wstring str, std::wstring filename)
//{
	//char szBOM[3] = { (char)0xEF,(char)0xBB,(char)0xBF };
	/*for (size_t i = 0; i < 3; i++)
	{
		fout.put(szBOM[i]);
	}*/
//}
//
//
//std::wstring _CACharConversion::utf82unicode(std::string utf8Str)
//{
//	if (utf8Str.empty())
//	{
//		return std::wstring();
//	}
//	if (utf8Str.length() > (std::numeric_limits<int>::max)())
//	{
//		std::cout << "String(utf8) too long." << std::endl;
//		return std::wstring();
//	}
//	int dwUnicodeLen = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, NULL, 0);
//	wchar_t * pwText = new wchar_t[dwUnicodeLen];
//	memset(pwText, 0, dwUnicodeLen);
//	MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, pwText, dwUnicodeLen);
//	std::wstring wchar(pwText, dwUnicodeLen - 1);
//	delete[] pwText;
//	return wchar;
//}

bool _CACodeLab::EscapeSequence(std::wstring & str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		size_t find = str.find(L"'", i);
		if (find == (std::numeric_limits<size_t>::max)())
			break;
		i = find;
		str.insert(i, 1, L'\'');
		i++;
		//std::cout << i << std::endl;
	}
	return true;
}

void _CACodeLab::CAMoveFile(const std::wstring & srcPath, const std::wstring & destPath)
{
	//_wsystem(std::wstring(L"C:\\FastCopy313_x64\\FastCopy.exe /cmd=move /auto_close /log=FALSE /no_ui \"").append(srcPath).append(L"\" /to=\"").append(destPath).append(L"\"").c_str());
	_wsystem(std::wstring(L"move ").append(L"\"").append(srcPath).append(L"\" \"").append(destPath).append(L"\"").c_str());
}

bool _CACodeLab::CAMoveFileAPI(const std::wstring & srcPath, const std::wstring & destPath)
{
	if (0 == MoveFileEx(srcPath.c_str(), destPath.c_str(), MOVEFILE_COPY_ALLOWED/* | MOVEFILE_REPLACE_EXISTING*/))
		return false;
	return true;
}

void _CACodeLab::CACopyFile(const std::wstring & srcPath, const std::wstring & destPath)
{
}

void _CACodeLab::Rename(const std::wstring & srcPath, const std::wstring & destPath)
{
	//rename(srcPath.c_str(), destPath.c_str());
}

void _CACodeLab::DacapoCheck(const std::wstring & srcPath, const std::wstring & destPath)
{
	_CAFolder srcFolder(srcPath);
	std::vector<_CAFile *> fileList;
	srcFolder.GetFileList(fileList);
	for (auto i : fileList)
	{
		if (i->Diff())
		{

		}
	}
}

bool _CACodeLab::SameStr(const std::wstring & str1, const std::wstring & str2)
{
	return str1.compare(str2) == 0;
}

bool _CACodeLab::StrMatch(const wchar_t & str1, const wchar_t & str2)
{
	if (str1 == L'[' && str2 == L']') return true;
	return false;
}

double _CACodeLab::StrSimilarity(const std::wstring & str1, const std::wstring & str2)
{
	std::wstring strTmp1;
	strTmp1.resize(str1.length());
	std::transform(str1.begin(), str1.end(), strTmp1.begin(), tolower);
	std::wstring strTmp2;
	strTmp2.resize(str2.length());
	std::transform(str2.begin(), str2.end(), strTmp2.begin(), tolower);
	std::vector<double> SimilarityVector;
	SimilarityVector.resize(max(strTmp1.length(), strTmp2.length()));
	//std::cout << "str1 : " << _CACharConversion::unicode2ansi(strTmp1) << std::endl;
	//std::cout << "str2 : " << _CACharConversion::unicode2ansi(strTmp2) << std::endl;
	for (size_t i = 0; i < SimilarityVector.size(); i++)
	{
		// Length not enough
		if ((strTmp1.length() <= i) || (strTmp2.length() <= i))
		{
			SimilarityVector[i] = 0;
			continue;
		}
		SimilarityVector[i] = CharSimilarity(strTmp1[i], strTmp2[i]);
	}
	//for (auto i : SimilarityVector)
	//{
	//	std::cout << i << " ";
	//}
	//std::cout << std::endl;
	double Similarity = 1;
	for (auto i : SimilarityVector)
	{
		if (i == 0)
		{
			Similarity *= (double)(SimilarityVector.size() - 1) / SimilarityVector.size();
		}
		else
			Similarity *= i;
	}
	return Similarity;
}

double _CACodeLab::CharSimilarity(const wchar_t & str1, const wchar_t & str2)
{
	if (!IsCharacter(str1, str2) && (!IsNumber(str1, str2)) && (!IsSymbol(str1, str2)))
	{
		return 0.01;
	}
	if (IsCharacter(str1, str2))
	{
		//return 1 - (double)abs(str1 - str2) / 26;
		if (str1 == str2)
		{
			return 1;
		}
		else return 0.5;
	}
	if (IsNumber(str1, str2))
	{
		return 0.01;
	}
	if (str1 == str2)
		return 1;
	else return 0.01;
}

bool _CACodeLab::IsCharacter(const wchar_t & str1, const wchar_t & str2)
{
	return ((str1 >= L'a' && str1 <= L'z') || (str1 >= L'A' && str1 <= L'Z')) && ((str2 >= L'a' && str2 <= L'z') || (str2 >= L'A' && str2 <= L'Z'));
}

bool _CACodeLab::IsNumber(const wchar_t & str1, const wchar_t & str2)
{
	return (str1 >= L'0' && str1 <= L'9') && (str2 >= L'0' && str2 <= L'9');
}

bool _CACodeLab::IsSymbol(const wchar_t & str1, const wchar_t & str2)
{
	return (!IsCharacter(str1,str2)) && (!IsNumber(str1,str2));
}
