// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <direct.h>
#include <sys/stat.h>
#include <basetsd.h>
#include <io.h>
#include <Windows.h>
#include <ctime>
#include <limits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <mutex>
#include <list>
#include <vector>
#include <map>
#include <stack>
#include <new>

#include "_CAObject.h"
#include "_CAAreaManager.h"
#include "_CACubeArea.h"
#include "_CAAnime.h"
#include "_CACodeLab.h"
#include "_CAMD5.h"
#include "SHA1.h"
#include "_CAFileBase.h"
#include "_CAFile.h"
#include "_CADataBlock.h"
#include "_CAFileStream.h"
#include "_CABencode.h"
#include "_CABencodeParser.h"
#include "_CABencodeInteger.h"
#include "_CABencodeString.h"
#include "_CABencodeList.h"
#include "_CABencodeDictionaries.h"
#include "_CATorrent.h"
#include "BTParser.h"
#include "_CABTChecker.h"
#include "_CABTParser.h"
#include "_CATestSpace.h"

#define DATABLOCK_MAX 12884901888 //12GB

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
