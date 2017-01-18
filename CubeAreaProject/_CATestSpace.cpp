#include "stdafx.h"
#include "_CATestSpace.h"


_CATestSpace::_CATestSpace()
{
}


_CATestSpace::~_CATestSpace()
{
}


// Test _CAMD5
void _CATestSpace::TestMD5()
{
	std::cout << _CAMD5("abc").toString() << std::endl;
	//std::cout << _CAMD5(std::ifstream("D:\\test.txt")).toString() << std::endl;
	//std::cout << _CAMD5(ifstream("D:\\test.exe", std::ios::binary)).toString() << std::endl;
	std::cout << FileDigest("D:\\test.exe") << std::endl;
	_CAMD5 md5;
	md5.update("");
	PrintMD5("", md5);

	md5.update("a");
	PrintMD5("a", md5);

	md5.update("bc");
	PrintMD5("abc", md5);

	md5.update("defghijklmnopqrstuvwxyz");
	PrintMD5("abcdefghijklmnopqrstuvwxyz", md5);

	md5.reset();
	md5.update("message digest");
	PrintMD5("message digest", md5);

	md5.reset();
	md5.update(ifstream("D:\\test.txt"));
	PrintMD5("D:\\test.txt", md5);

}


void _CATestSpace::PrintMD5(const std::string & str, _CAMD5 & md5)
{
	std::cout << "MD5(\"" << str << "\") = " << md5.toString() << std::endl;
}


std::string _CATestSpace::FileDigest(const string & file)
{
	std::ifstream in(file.c_str(), std::ios::binary);
	if (!in)
	{
		return "";
	}
	_CAMD5 md5;
	std::streamsize length;
	char buffer[1024];
	while (!in.eof())
	{
		in.read(buffer, 1024);
		length = in.gcount();
		if (length > 0)
		{
			md5.update(buffer, length);
		}
	}
	in.close();
	return md5.toString();
}

// Test Thread
void _CATestSpace::TestThread()
{
	int n = 0;
	std::thread t1;
	std::thread t2(f1, n + 1);
	std::thread t3(f2, std::ref(n));
	std::thread t4(std::move(t3));
	std::thread t5(f1, n + 1);
	std::thread t6(f1, n + 1);
	std::thread t7(f1, n + 1);
	std::thread t8(f1, n + 1);
	std::thread t9(f1, n + 1);
	t2.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	t9.join();
	f1(n);
	std::cout << "Final value of n is " << n << "\n";
	//std::thread t1(&threadfunction, 1);
	//std::thread t2(&threadfunction, 2);
	//std::thread t3(&threadfunction, 3);
	//std::thread t4(&threadfunction, 4);
	//std::thread t5(&threadfunction, 5);
	//std::thread t6(&threadfunction, 6);
	//std::thread t7(&threadfunction, 7);
	//std::thread t8(&threadfunction, 8);
	//std::thread t9(&threadfunction, 9);
	std::thread tA(&myThreadA);//创建一个分支线程，回调到myThread函数里
	std::thread tB(&myThreadB);
	tA.join();
	tB.join();
	/*system("dir");
	while (!std::cin.eof())
	{
	std::cout << "cached " << std::cin.get() << std::endl;
	}*/
	/*t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	t9.join();*/
}
int _CATestSpace::publicvar = 100;
int _CATestSpace::tickets = 100;
void _CATestSpace::threadfunction(int i)
{
	//printf("Hello world!\n");
	Sleep(1000);
	publicvar = publicvar + 10;
	std::cout << publicvar << " Hello world!\n";

}
void _CATestSpace::myThreadA()
{
	while (true)
	{
		//mymutex.lock();
		if (tickets>0)
		{
			Sleep(10);
			std::cout << tickets-- << std::endl;
			//CCLOG("A Sell %d", tickets--);//输出售票，每次减1
			//mymutex.unlock();
		}
		else {
			//mymutex.unlock();
			break;

		}
	}
}
void _CATestSpace::myThreadB()
{
	while (true)
	{
		//mymutex.lock();
		if (tickets>0)
		{
			Sleep(10);
			printf("%d\n", tickets--);
			//mymutex.unlock();
			//std::cout << tickets-- << std::endl;
			//CCLOG("B Sell %d", tickets--);
		}
		else
		{
			//mymutex.unlock();
			break;

		}
	}
}
void _CATestSpace::f1(int n)
{
	for (size_t i = 0; i < 500000; i++)
	{
		for (size_t i = 0; i < 500000; i++)
		{
			sin(i);
			//n = i;
		}

		//std::cout << "Thread " << n << " executing\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void _CATestSpace::f2(int& n)
{
	for (size_t i = 0; i < 500000; i++)
	{
		for (size_t i = 0; i < 500000; i++)
		{
			sin(i);
		}
		//std::cout << "Thread 2 executing\n";
		++n;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}


// Test GetFileLength
void _CATestSpace::TestGetFileLength()
{
	size_t filelength;
	std::cout << sizeof(filelength) << std::endl;
	std::cout << sizeof(DWORD) << std::endl;
	std::cout << sizeof(LPDWORD) << std::endl;
	std::cout << sizeof(long) << std::endl;
	std::cout << sizeof(long long) << std::endl;
	//_CACodeLab::GetFileLength(filelength, L"d:\\cn_windows_server_2012_r2_with_update_x64_dvd_6052725.iso");
	_CACodeLab::GetFileLength(filelength, L"d:\\cn_windows_server_2012_r2_with_update_x64_dvd_6052725.iso");
	std::cout << filelength << std::endl;
}


// Test ReadFileWithMemMapping
void _CATestSpace::TestReadFileWithMemMapping()
{
	size_t filelength;
	CSHA1 sha1;
	_CACodeLab::GetFileLength(filelength, L"D:\\BT\\[CASO&SumiSora][LoveLive!Sunshine!!][10][GB][720p].mp4");
	/* ----- ReadFileWithMemMapping -----
	   -----       Version 2.0      -----
	   ---------------------------------- */
	BYTE * fileStream;
	std::wstring strOut;
	fileStream = new BYTE[filelength];
	_CACodeLab::ReadFileWithMemMapping(fileStream, filelength, L"D:\\BT\\[CASO&SumiSora][LoveLive!Sunshine!!][10][GB][720p].mp4");
	//sha1.Update(fileStream, filelength);
	//sha1.Final();
	//sha1.ReportHashStl(strOut, CSHA1::REPORT_HEX_SHORT);
	std::wcout << strOut << L" ";
	delete[] fileStream;
	/* ----- ReadFileWithMemMapping -----
	   -----    Version 2.1(beta)   -----
	   -------------ERROR---------------- */
	sha1.Reset();
	_CAFileStream filestream(DATABLOCK_MAX);
	//std::string filestream;
	//_CACodeLab::ReadFileWithMemMapping(filestream, filelength, L"D:\\BT\\[CASO&SumiSora][LoveLive!Sunshine!!][10][GB][720p].mp4");
	//sha1.Update((unsigned char *)filestream._current, filelength);
	//sha1.Final();
	//sha1.ReportHashStl(strOut, CSHA1::REPORT_HEX_SHORT);
	//std::wcout << strOut << std::endl;
}


// Test FileStream and DataBlock
void _CATestSpace::TestFileStream()
{
	_CAFileStream fileStream(1024*1024*1024);
	fileStream.AddFile(L"D:\\BT\\[CASO&SumiSora][LoveLive!Sunshine!!][10][GB][720p].mp4");
	//fileStream.GetData("Hello World!", 13);
	//fileStream++;
	//fileStream += 2;
}


void _CATestSpace::TestBencodeInteger()
{
	_CABencodeInteger bencodeInteger;
	_CAFileStream fileStream(DATABLOCK_MAX);
	fileStream.GetData("i35214142326e", 14);
	bencodeInteger.Parse(fileStream);
}


void _CATestSpace::TestBencodeString()
{
	_CABencodeString bencodeString;
	_CAFileStream fileStream(DATABLOCK_MAX);
	fileStream.GetData("10:HelloWorld", 14);
	bencodeString.Parse(fileStream);
	std::cout << bencodeString._string << std::endl;
}


void _CATestSpace::TestBencodeList()
{
	_CABencodeList bencodeList;
	_CAFileStream fileStream(DATABLOCK_MAX);
	fileStream.GetData("li3425e3:abce", 14);
	bencodeList.Parse(fileStream);
}


void _CATestSpace::TestBencodeDictionary()
{
	_CABencodeDictionary bencodeDictionary;
	_CAFileStream fileStream(DATABLOCK_MAX);
	size_t fileLength;
	std::wstring fileName(L"D:\\BT\\ebc61e99ca92c16f183392d40d5757e6be7aef9f.torrent");
	std::wstring fileName1(L"D:\\BT\\[CASO&SumiSora][LoveLive!Sunshine!!][10][GB][720p].mp4.torrent");
	_CACodeLab::GetFileLength(fileLength, fileName);
	//_CACodeLab::ReadFileWithMemMapping(fileStream, fileLength, fileName);
	//fileStream.GetData("d1:ai123ee", 11);
	bencodeDictionary.Parse(fileStream);
	//bencodeDictionary.Output(0);
}


void _CATestSpace::TestTorrent()
{
	std::wstring torrentPath(L"D:\\待校验\\BT");
	//std::wstring downloadPath(L"E:\\TDDOWNLOAD");
	std::wstring downloadPath(L"D:\\待校验\\下载");
	_CAFolder torrentFolder(torrentPath);
	_CAFolder downloadFolder(downloadPath);
	std::vector<_CAFile *> torrentFileList;
	//for (auto i = torrentFolder._files.begin(); i != torrentFolder._files.end(); i++)
	//{
	//	_CAFile * pfile = dynamic_cast<_CAFile *>(*i);
	//	if (pfile)
	//	{
	//		if (pfile->Suffix() == std::wstring(L"torrent"))
	//		{
	//			//_CALog::Log(std::wstring(pfile->Path()).append(L"\n"), L"D:\\BTTest\\Log\\TorrentList.txt");
	//			torrentFileList.push_back(pfile);
	//		}
	//	}
	//}
	// [Update]
	torrentFolder.GetFileList(L"torrent", torrentFileList, _CACodeLab::SameStr, _CAFile::ATTR::SUFFIX);
	clock_t start, end;
	start = clock();
	for (auto i = torrentFileList.begin(); i != torrentFileList.end(); i++)
	{
		try
		{
			_CATorrent torrent((*i)->Path());
			
			//torrent.Rename();
			///*
			if (torrent.Check(downloadPath))
			{
				_CACodeLab::CAMoveFile((*i)->Path(), L"D:\\待校验\\TorrentChecked\\");
				_CACodeLab::CAMoveFile(torrent.GetDownloadPath(), L"D:\\待校验\\DownloadChecked\\");
			}
			//*/
			//_CALog::Log(std::wstring((*i)->Path()).append(L" checked.\n"), L"D:\\BTTest\\Log\\TorrentChecked.txt");
		}
		catch (const std::exception& e)
		{
			std::wstring buf((*i)->Path());
			buf.append(L" ").append(_CACharConversion::ansi2unicode(e.what())).append(L"\n");
			_CALog::Log(buf, L"D:\\BTTest\\Log\\TorrentError.txt");
		}
	}
	end = clock();
	double dur = (double)(end - start);
	std::cout << dur << std::endl;
}

void _CATestSpace::TestDataBlock()
{
	_CADataBlock dataBlock(1);
	//long long size = 4294967296*4;
	//dataBlock.CreateBlock(size);
}

void _CATestSpace::TestFile()
{
	
	//_CAFile file(L"D:\\cpTest\\TorrentChecked\\[CASO&SumiSora][Boku_wa_Tomodachi_ga_Sukunai][BDRip].torrent");
	//_CAFile file1(L"D:\\BT\\[CASO&SumiSora][LoveLive!Sunshine!!][10][GB][720p].mp4.torrent");
	//_CAFile file1(L"D:\\BT\\test.txt");
	//std::string buffer;
	//file1.Read(buffer, 0, file1.Size());
	_CAFolder folder(L"D:\\cpTest");
	std::vector<_CAFile *> fileList;
	/*folder.GetFileList(L"[KissSub&FZSD&Xrip][Kono_Subarashii_Sekai_ni_Shukufuku_o!][BDrip][01][1080P][x264_10bit_flac].mkv",
		fileList,
		_CACodeLab::SameStr,
		_CAFile::FILENAME);*/
	folder.GetFileList(L"mkv",
		fileList,
		_CACodeLab::SameStr,
		_CAFile::SUFFIX);
}

void _CATestSpace::TestFullPermutation()
{
	std::stack<int> stack;
	std::vector<int> i;
	i.push_back(1);
	i.push_back(2);
	i.push_back(3);
	i.push_back(4);
	SubPermutation(i, stack);

}

void _CATestSpace::TestSQLServerConnection()
{
	// Init Point

	CoInitialize(NULL);
	_ConnectionPtr pMyConnect = NULL;
	HRESULT hr = pMyConnect.CreateInstance(__uuidof(Connection));
	if (FAILED(hr))
	{
		return;
	}
	// Init parament DESKTOP-EBP9289
	//_bstr_t strConnect = "Provider=SQLOLEDB;Server=DESKTOP-EBP9289\\CUBEAREADB;Database=test;UID=sa;PWD=123456";
	_bstr_t strConnect = "Provider=SQLOLEDB;Server=DESKTOP-EBP9289\\CUBEAREADB;Database=test";
	try
	{
		pMyConnect->Open(strConnect, "sa", "123456", adModeUnknown);
	}
	catch (_com_error &e)
	{
		std::cout << e.Description() << std::endl;
	}
	_RecordsetPtr m_pRecordset;
	if (FAILED(m_pRecordset.CreateInstance(__uuidof(Recordset))))
	{
		return;
	}
	try
	{
		m_pRecordset->Open("select * from AreaMap", (IDispatch *)pMyConnect, adOpenDynamic, adLockOptimistic, adCmdText);
	}
	catch (_com_error &e)
	{
		std::cout << e.Description() << std::endl;
	}
	try
	{
		m_pRecordset->MoveFirst();
		while (!m_pRecordset->adoEOF)
		{
			string AreaName = (char*)(_bstr_t)(m_pRecordset->Fields->GetItem(_variant_t("AreaName"))->Value);
			string AreaPath = (char*)(_bstr_t)(m_pRecordset->Fields->GetItem(_variant_t("AreaPath"))->Value);
			std::cout << AreaName << "\t" << AreaPath << std::endl;
			m_pRecordset->MoveNext();
		}
	}
	catch (_com_error &e)
	{
		std::cout << e.Description() << std::endl;
	}
}

void _CATestSpace::TestDB()
{
	_CADB db;
	//db.Execute("select * from AreaMap where id = 1");
	//std::wstring strlist[] = { L"A", L"B", L"C" };
	//size_t strCount = sizeof(strlist) / sizeof(std::wstring);
	//std::vector<std::wstring> v1(strlist, strlist + strCount);
	_variant_t columnList[2] = { L"column1", L"column2" };
	_variant_t valueList[2] = { L"A", 1 };
	db.Insert(L"TestTable", std::vector<_variant_t>(columnList, columnList + 2), std::vector<_variant_t>(valueList, valueList + 2));
}

void _CATestSpace::TestRegex()
{
	if (std::regex_match("subject", std::regex("(sub)(.*)")))
	{
		std::cout << "string literal matched\n";
	}
	std::string s("subject");
	std::regex e("(sub)(.*)");
	if (std::regex_match(s, e))
	{
		std::cout << "string object matched\n";
	}
	if (std::regex_match(s.begin(), s.end(), e))
	{
		std::cout << "range matched\n";
	}
	std::cmatch cm; // same as std::match_results<const char *=""> cm;
	std::regex_match("subject", cm, e);
	std::cout << "string literal with " << cm.size() << " matches\n";

	std::smatch sm; // same as std::match_results<string::const_iterator>
	std::regex_match(s, sm, e);
	std::cout << "string object with " << sm.size() << " matches\n";

	std::regex_match(s.cbegin(), s.cend(), sm, e);
	std::cout << "range with " << sm.size() << " matches\n";

	// using explicit flags:
	std::regex_match("subject", cm, e, std::regex_constants::match_default);
	std::cout << "the matches were: ";
	for (size_t i = 0; i < sm.size(); i++)
	{
		std::cout << "[" << sm[i] << "] ";
	}
	std::cout << std::endl;
}

void _CATestSpace::EscapeSequence()
{
	std::wstring str(L"hello'w'or'ld!'");
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
}

void _CATestSpace::TestLog()
{
	//_CALog::SetLogFile(L"D:\\TestLog.log");
	_CALog::Log(L"Hello Log!\n");
	_CALog::Log("Hello Log!\n");
}

void _CATestSpace::TestCharConversion()
{
	std::string utf8("Hello World!");
	std::wstring unicode(_CACharConversion::utf82unicode(utf8));
	std::wstring unicode1(L"长门有希");
	std::string utf81(_CACharConversion::unicode2utf8(unicode1));
	std::wstring unicode2(_CACharConversion::utf82unicode(utf81));
	std::wstring unicode3(_CACharConversion::ansi2unicode(utf8));
	std::string ansi(_CACharConversion::unicode2ansi(unicode1));
	std::cout << ansi << std::endl;

}

void _CATestSpace::TestFileOperation()
{
	//_CACodeLab::CAMoveFile(L"d:\\cpTest\\FastCopy.chm", L"d:\\cpTest\\1\\"); // Move FastCopy.chm to folder 1
	//_CACodeLab::CAMoveFile(L"d:\\cpTest\\2", L"d:\\cpTest\\1\\"); // Move Folder 2 to folder 1
	//_CACodeLab::CAMoveFile(L"d:\\cpTest\\2\\", L"d:\\cpTest\\1\\"); // Same as \\2
	_CACodeLab::CAMoveFile(L"d:\\cpTest\\2\\*", L"d:\\cpTest\\1\\"); // Move content of folder 2 to folder 1
}

void _CATestSpace::TestMoveFile()
{
	_CACodeLab::CAMoveFileAPI(L"d:\\cpTest\\4\\*", L"d:\\cpTest\\1\\");
}

void _CATestSpace::TestDownloadManager()
{
	_CADownloadManager downloadManager;
	//downloadManager.TorrentRename();
	downloadManager.TorrentDownloadCheck();
}

std::vector<int> _CATestSpace::DeleteElem(std::vector<int> sub, int subi)
{
	std::vector<int> tmp;
	for (int i = 0; i < sub.size(); i++)
	{
		if (i != subi)
		{
			tmp.push_back(sub.at(i));
		}
	}
	return tmp;
}

void _CATestSpace::SubPermutation(std::vector<int> sub, std::stack<int> stack)
{
	std::stack<int> stack1;
	if (sub.size() == 1)
	{
		stack1 = stack;
		stack.push(sub.at(0));
		StackOutput(stack);
		return;
	}
	for (int i = 0;i < sub.size(); i++)
	{
		stack1 = stack;
		stack1.push(sub[i]);
		std::vector<int> t = DeleteElem(sub, i);
		SubPermutation(t, stack1);
	}

}


void _CATestSpace::StackOutput(std::stack<int> stack)
{
	while (!stack.empty())
	{
		std::cout << stack.top() << " ";
		stack.pop();
	}
	std::cout << std::endl;
}
