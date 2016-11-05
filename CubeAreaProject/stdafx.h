// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
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
#include <icrsint.h>
#include <iomanip>
#include <regex>

#include "_CALog.h"
#include "_CACharConversion.h"
#include "_CAObject.h"
#include "_CAAreaManager.h"
#include "_CACubeArea.h"
#include "_CAAnime.h"
#include "_CACodeLab.h"
#include "_CAMD5.h"
#include "SHA1.h"
#include "_CAFileBase.h"
#include "_CAFile.h"
#include "_CAFolder.h"
#include "_CADataBlock.h"
#include "_CAFileStream.h"
#include "_CABencode.h"
#include "_CABencodeParser.h"
#include "_CABencodeInteger.h"
#include "_CABencodeString.h"
#include "_CABencodeList.h"
#include "_CABencodeDictionaries.h"
#include "_CATorrent.h"
#include "_CATestSpace.h"
#include "_CADB.h"

#define DATABLOCK_MAX 12884901888 //12GB
//#define DATABLOCK_MAX 1073741824 //1GB
//#define DATABLOCK_MAX 209715200 //200MB