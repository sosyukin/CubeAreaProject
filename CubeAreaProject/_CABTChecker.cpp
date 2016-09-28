#include "stdafx.h"
#include "_CABTChecker.h"

typedef unsigned char BYTE;
typedef long long INT64;
#define SHA1_LEN 20
#define DIR_SEPARATOR '\\'

typedef unsigned int UINT4;


_CABTChecker::_CABTChecker()
{
	torrentFile = NULL;
	contentPath = NULL;
	contentPathLen = 0;
	bytesRead = -1;
	torrentInfo = -1;
	ofs = -1;
	ofs2 = -1;
	torrentPieceLen = -1;
	lastFile = 0;
	torrentFiles = -1;
	thisFileOffset = -1;
	multiFileTorrent = 0;
	filterMode = 0;
	filterHash = NULL;
	torrentPrivate = 0;
	fileBytesExpected = -1;
	fileBytesActual = -1;
	fileBytesRead = -1;
	pieceList = NULL;
	pieceListLen = -1;
	pieceBuf = NULL;
	bytesInBuf = 0;
	bytesToRead = 0;
	rootName = NULL;
	rootNameLen = -1;
	announce = NULL;
	announceLen = -1;
	numPieces = -1;
	numFiles = 0;
	pieceLen = -1;
	fileName = NULL;
	fileNameLen = -1;
	filePathMax = 8192;
	totalBytes = 0;
	totalBytesDone = 0;
	piecesDone = 0;
	errorsFound = 0;
	errorsFoundThisFile = 0;
	piecesChecked = 0;
	firstFileThisPiece = 0;
	currentFile = 0;
	fileRecordList = NULL;
	maxFileRecords = 16;
	showProgressCount = 1;
	showPieceDetail = 0;
	useCommaDot = 0;
	thisPieceBad = 0;
	showProgressChars = 0;
	CSHA1 sha;
	char test[] = "~}|{zyxwvutsrqponmlkjihgfedcba`_^]\\[ZYXWVUTSRQPONMLKJIHGFEDCBA@?>=<;:9876543210/.-,+*)('&%$#\"! !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	sha.Update((unsigned char *)test, 189);
	sha.Final();
	sha.GetHash(sha1hash);
	if (memcmp(sha1hash, "\x11\xe5\x6b\x84\xd8\xda\xb8\x93\xcd\x8e\x2d\x85\xe4\x3c\xc0\x0d\x5a\xd1\xbb\x78", SHA1_LEN) != 0)
	{
		std::cout << "SHA error" << std::endl;
		errorsFound++;
	}
	else
	{
		std::cout << "SHA Ok" << std::endl;
	}
	sha.Reset();
	if ((sizeof(INT64) != 8) || (sizeof(UINT4) != 4)) {
		//printf("Wrong word length UINT4=%i INT64=%i - this build is faulty!\n", sizeof(UINT4), sizeof(INT64));
	}
	torrentFile = "D:\\BT\\[CASO&SumiSora][LoveLive!Sunshine!!][10][GB][720p].mp4.torrent";
	std::cout << torrentFile << std::endl;
	contentPath = "D:\\BT"; 
	contentPathLen = (int)strlen(contentPath);
	std::cout << contentPath << std::endl << contentPathLen << std::endl;
	filePath = new char[filePathMax];
	if (0 == fopen_s(&fp, torrentFile, "rb"))
	{
		std::cout << "Openfile OK." << std::endl;
	}
	fseek(fp, 01, SEEK_END);
	torrentLen = (int)ftell(fp) - 1;
	rewind(fp);
	torrent = new BYTE[torrentLen];
	bytesRead = fread(torrent, 1, torrentLen, fp);
	if (fp != NULL)
	{
		fclose(fp);
	}
	fp = NULL;
	i = beStepOver(torrent, torrentLen, 0);
	if (i != torrentLen) {
		printf("Unable to parse torrent metadata file %s\n", torrentFile);
		return;
	}
	torrentInfo = beFindInDict(torrent, torrentLen, 0, (BYTE *)"info");
	if (torrentInfo < 0) {
		printf("Unable to read \"info\" from torrent\n");
		return;
	}

	ofs = beFindInDict(torrent, torrentLen, torrentInfo, (BYTE *)"name");
	if (ofs >= 0) {
		ofs = beParseString(torrent, torrentLen, ofs, &rootName, &rootNameLen);
	}
	if (ofs < 0) {
		printf("Unable to read \"name\" from torrent\n");
		return;
	}

	ofs = beFindInDict(torrent, torrentLen, torrentInfo, (BYTE *)"private");
	if (ofs >= 0) {
		ofs = beParseInteger(torrent, torrentLen, ofs, &torrentPrivate);
	}

	ofs = beFindInDict(torrent, torrentLen, torrentInfo, (BYTE *)"length");
	if (ofs >= 0) { // single file
		ofs = beParseInteger(torrent, torrentLen, ofs, &fileBytesExpected);
		totalBytes = fileBytesExpected;
	}
	else { // multi file
		multiFileTorrent = 1;
		torrentFiles = beFindInDict(torrent, torrentLen, torrentInfo, (BYTE *)"files");

		// Count files 
		thisFileOffset = torrentFiles;
		if ((thisFileOffset >= 0) && (thisFileOffset < torrentLen - 1) &&
			(torrent[thisFileOffset] == 'l')) thisFileOffset++;
		while ((thisFileOffset >= 0) && (thisFileOffset < torrentLen)) {
			if (torrent[thisFileOffset] != 'd') break;
			thisFileOffset = beStepOver(torrent, torrentLen, thisFileOffset);
			numFiles++;
		}
		fileRecordList = new fileRecord[numFiles];
		if (torrent == NULL) {
			//printf("Unable to malloc %i bytes for file record list\n", numFiles * sizeof(fileRecord));
			return;
		}
		thisFileOffset = torrentFiles;
		currentFile = 0;
		if ((thisFileOffset >= 0) && (thisFileOffset < torrentLen - 1) &&
			(torrent[thisFileOffset] == 'l')) thisFileOffset++;
		while ((thisFileOffset >= 0) && (thisFileOffset < torrentLen)) {
			if (torrent[thisFileOffset] != 'd') break;
			ofs = beFindInDict(torrent, torrentLen, thisFileOffset, (BYTE *)"length");
			if (ofs >= 0) {
				ofs = beParseInteger(torrent, torrentLen, ofs, &fileBytesExpected);
			}
			if (ofs < 0) {
				printf("Unable to read \"length\" from torrent\n");
				return;
			}
			ofs = beFindInDict(torrent, torrentLen, thisFileOffset, (BYTE *)"path");
			if (ofs < 0) {
				printf("Unable to read \"path\" from torrent\n");
				return;
			}

			filePathOfs = 0;
			if (torrent[ofs] == 'l') ofs++;
			while ((ofs >= 0) && (ofs < torrentLen) && (torrent[ofs] != 'e')) {
				ofs = beParseString(torrent, torrentLen, ofs, &fileName, &fileNameLen);
				if (ofs < 0) {
					printf("Unable to read \"path\" from torrent\n");
					return;
				}
				if (filePathOfs != 0) {
					filePath[filePathOfs] = DIR_SEPARATOR;
					filePathOfs++;
				}
				while (fileNameLen + filePathOfs + contentPathLen + rootNameLen + 16 >= filePathMax) {
					int filePathMaxOld = filePathMax;
					filePathMax *= 2;
					char * tempnew = new char[filePathMax];
					memcpy_s(tempnew, filePathMax, filePath, filePathMaxOld);
					delete[] filePath;
					filePath = tempnew;
					if (filePath == NULL) {
						printf("Unable to realloc %i bytes for file path\n", filePathMax);
						return;
					}
				}
				memcpy(filePath + filePathOfs, fileName, fileNameLen);
				filePathOfs += fileNameLen;
				filePath[filePathOfs] = 0;
			}
			//Action! Memory overflow
			fileRecordList[currentFile].filePath = new char[strlen(filePath) + 1];
			if (fileRecordList[currentFile].filePath == NULL) {
				//printf("Unable to malloc %i bytes for file path\n", strlen(filePath) + 1);
				return;
			}
			strcpy_s(fileRecordList[currentFile].filePath, strlen(filePath), filePath);
			//strcpy(fileRecordList[currentFile].filePath, filePath);
			fileRecordList[currentFile].numBytes = fileBytesExpected;
			fileRecordList[currentFile].errorsFound = 0;

			thisFileOffset = beStepOver(torrent, torrentLen, thisFileOffset);
			totalBytes += fileBytesExpected;
			currentFile++;
		}
	}
	filePath2 = new char[filePathMax];
	if (filePath2 == NULL) {
		printf("Unable to malloc %i bytes for file path (2)\n", filePathMax);
		return;
	}

	if ((ofs < 0) && (torrentFiles < 0)) {
		printf("Unable to read \"info->length\" or \"info->files\" from torrent\n");
		return;
	}

	ofs = beFindInDict(torrent, torrentLen, torrentInfo, (BYTE *)"pieces");
	if (ofs >= 0) {
		ofs = beParseString(torrent, torrentLen, ofs, &pieceList, &pieceListLen);
	}
	if (ofs < 0) {
		printf("Unable to read \"pieces\" from torrent\n");
		return;
	}

	numPieces = pieceListLen / SHA1_LEN;
	if (numPieces * SHA1_LEN != pieceListLen) {
		printf("Pieces list length is not a multiple of 20\n");
		return;
	}

	ofs = beFindInDict(torrent, torrentLen, torrentInfo, (BYTE *)"piece length");
	if (ofs >= 0) {
		ofs = beParseInteger(torrent, torrentLen, ofs, &pieceLen);
	}
	if (ofs < 0) {
		printf("Unable to read \"piece length\" from torrent\n");
		return;
	}
	printf("Torrent file  : %s\n", torrentFile);
	printf("Metadata info : %i bytes, %i piece%s, %s bytes per piece%s\n", torrentLen, numPieces, ((numPieces == 1) ? "" : "s"), print64(pieceLen, p64Buf1, useCommaDot), ((torrentPrivate == 1) ? ", private" : ""));
	printf("Torrent name  : ");
	fwrite(rootName, rootNameLen, 1, stdout);
	printf("\n");

	if (multiFileTorrent) {
		printf("Content info  : %i file%s, %s bytes\n", numFiles, ((numFiles == 1) ? "" : "s"), print64(totalBytes, p64Buf1, useCommaDot));
	}
	else {
		printf("Content info  : single file, %s bytes\n", print64(totalBytes, p64Buf1, useCommaDot));
	}

	ofs = beFindInDict(torrent, torrentLen, 0, (BYTE *)"announce");
	if (ofs >= 0) {
		ofs = beParseString(torrent, torrentLen, ofs, &announce, &announceLen);
	}
	if (ofs >= 0) {
		printf("Announce URL  : ");
		fwrite(announce, announceLen, 1, stdout);
		printf("\n");
	}

	if ((multiFileTorrent) && (contentPath == NULL)) {
		printf("\n");
		if (useCommaDot) {
			printf("F#  Bytes         File name\n");
			printf("--- ------------- -------------------------------------------------------------\n");
		}
		else {
			printf("F#  Bytes       File name\n");
			printf("--- ----------- ---------------------------------------------------------------\n");
		}
		for (i = 0;i<numFiles;i++) {
			printf(useCommaDot ? "%3i %13s %s\n" : "%3i %11s %s\n", i + 1, print64(fileRecordList[i].numBytes, p64Buf1, useCommaDot), fileRecordList[i].filePath);
		}
		printf("\n");
	}

	if (contentPath != NULL) {
		pieceBuf = new BYTE[pieceLen];
		if (pieceBuf == NULL) {
			//printf("Unable to malloc %i bytes for piece buffer\n", pieceLen);
			return;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	if ((multiFileTorrent) && (contentPath != NULL)) { // multi-file torrent check
		printf("\n");
		if (useCommaDot) {
			printf("F#  Ok? Bytes         File name\n");
			printf("--- --- ------------- ---------------------------------------------------------\n");
		}
		else {
			printf("F#  Ok? Bytes       File name\n");
			printf("--- --- ----------- -----------------------------------------------------------\n");
		}

		thisFileOffset = torrentFiles;
		bytesRead = 0;
		currentFile = 0;
		while (currentFile < numFiles) {

			memcpy(filePath, contentPath, contentPathLen);
			filePathOfs = contentPathLen;
			if ((filePathOfs > 0) && (filePath[filePathOfs - 1] != DIR_SEPARATOR)) {
				filePath[filePathOfs] = DIR_SEPARATOR;
				filePathOfs++;
			}
			memcpy(filePath + filePathOfs, rootName, rootNameLen);
			filePathOfs += rootNameLen;
			if ((filePathOfs > 0) && (filePath[filePathOfs - 1] != DIR_SEPARATOR)) {
				filePath[filePathOfs] = DIR_SEPARATOR;
				filePathOfs++;
			}
			strcpy_s(filePath + filePathOfs,strlen(fileRecordList[currentFile].filePath), fileRecordList[currentFile].filePath);

			fileBytesExpected = fileRecordList[currentFile].numBytes;

			filePathActual = filePath;
			fopen_s(&fp,filePath, "rb");
			if (fp == NULL) { // Try without root path
				memcpy(filePath2, contentPath, contentPathLen);
				filePathOfs = contentPathLen;
				if ((filePathOfs > 0) && (filePath2[filePathOfs - 1] != DIR_SEPARATOR)) {
					filePath2[filePathOfs] = DIR_SEPARATOR;
					filePathOfs++;
				}
				strcpy_s(filePath2 + filePathOfs,strlen(fileRecordList[currentFile].filePath), fileRecordList[currentFile].filePath);
				filePathActual = filePath2;
				fopen_s(&fp, filePath2, "rb");
			}

			if (fp == NULL) {
				backspaceProgressLine(&showProgressChars);
				printf("Unable to open file %s or %s\n", filePath, filePath2);
				errorsFound++;
				errorsFoundThisFile++;
			}
			else {
				fseek(fp, 0l, SEEK_END);
				fileBytesActual = ftell(fp);
				rewind(fp);
				if (fileBytesActual != fileBytesExpected) {
					backspaceProgressLine(&showProgressChars);
					printf("File %s length mismatch, expected %s bytes, found %s bytes\n", filePathActual, print64(fileBytesExpected, p64Buf1, useCommaDot), print64(fileBytesActual, p64Buf2, useCommaDot));
					errorsFound++;
					errorsFoundThisFile++;
				}
			}
			fileBytesRead = 0;
			while (fileBytesRead < fileBytesExpected) {
				if (fileBytesExpected - fileBytesRead < pieceLen - bytesRead) {
					bytesToRead = (int)(fileBytesExpected - fileBytesRead);
				}
				else {
					bytesToRead = (int)(pieceLen - bytesRead);
				}
				if (fp != NULL) readLen = (int)fread(pieceBuf + bytesRead, 1, bytesToRead, fp);
				else readLen = 0;
				bytesRead += bytesToRead; fileBytesRead += bytesToRead;
				if ((fp != NULL) && (readLen != bytesToRead)) {
					backspaceProgressLine(&showProgressChars);
					printf("Short read, got %i bytes, expected %i bytes at offset %s of %s\n", readLen, bytesToRead, print64(fileBytesRead, p64Buf1, useCommaDot), filePathActual);
					errorsFound++;
					errorsFoundThisFile++;
				}
				if (currentFile + 1 >= numFiles) lastFile = 1;
				if ((bytesRead == pieceLen) || ((lastFile == 1) && (fileBytesRead == fileBytesExpected))) {

					if ((fp != NULL) && (readLen == bytesToRead)) {
						sha.Reset();
						sha.Update(pieceBuf, (int)bytesRead);
						sha.Final();
						sha.GetHash(sha1hash);
						/*SHAInit(&sha1ctx);
						SHAUpdate(&sha1ctx, pieceBuf, bytesRead);
						SHAFinal(sha1hash, &sha1ctx);*/
					}
					totalBytesDone += bytesRead;
					i = piecesDone * SHA1_LEN;

					if ((fp == NULL) || (readLen != bytesToRead)) {
						errorsFound++;
						errorsFoundThisFile++;
						thisPieceBad = 2;
						for (i = firstFileThisPiece;i <= currentFile;i++) {
							fileRecordList[i].errorsFound = errorsFoundThisFile;
						}
					}
					else if (memcmp(pieceList + i, sha1hash, SHA1_LEN) != 0) {
						errorsFound++;
						errorsFoundThisFile++;
						thisPieceBad = 1;
						for (i = firstFileThisPiece;i <= currentFile;i++) {
							fileRecordList[i].errorsFound = errorsFoundThisFile;
						}
					}

					if (thisPieceBad || showPieceDetail) {
						backspaceProgressLine(&showProgressChars);
						if ((showPieceDetail == 1) || (thisPieceBad == 1)) {
							printf("piece %i computed SHA1 %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", piecesDone,
								(int)sha1hash[0], (int)sha1hash[1], (int)sha1hash[2], (int)sha1hash[3],
								(int)sha1hash[4], (int)sha1hash[5], (int)sha1hash[6], (int)sha1hash[7],
								(int)sha1hash[8], (int)sha1hash[9], (int)sha1hash[10], (int)sha1hash[11],
								(int)sha1hash[12], (int)sha1hash[13], (int)sha1hash[14], (int)sha1hash[15],
								(int)sha1hash[16], (int)sha1hash[17], (int)sha1hash[18], (int)sha1hash[19]);

							printf("piece %i expected SHA1 %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", piecesDone,
								(int)pieceList[i + 0], (int)pieceList[i + 1], (int)pieceList[i + 2], (int)pieceList[i + 3],
								(int)pieceList[i + 4], (int)pieceList[i + 5], (int)pieceList[i + 6], (int)pieceList[i + 7],
								(int)pieceList[i + 8], (int)pieceList[i + 9], (int)pieceList[i + 10], (int)pieceList[i + 11],
								(int)pieceList[i + 12], (int)pieceList[i + 13], (int)pieceList[i + 14], (int)pieceList[i + 15],
								(int)pieceList[i + 16], (int)pieceList[i + 17], (int)pieceList[i + 18], (int)pieceList[i + 19]);
						}
						printf("piece %i is files %i-%i, %s bytes, %s total bytes, %i error%s\n", piecesDone, firstFileThisPiece + 1, currentFile + 1, print64(bytesRead, p64Buf1, useCommaDot), print64(totalBytesDone, p64Buf2, useCommaDot), errorsFound, ((errorsFound == 1) ? "" : "s"));
						thisPieceBad = 0;
					}

					for (i = firstFileThisPiece;i<currentFile;i++) {
						backspaceProgressLine(&showProgressChars);
						printf(useCommaDot ? "%3i %-3s %13s %s\n" : "%3i %-3s %11s %s\n", i + 1, ((fileRecordList[i].errorsFound == 0) ? "ok" : "BAD"),
							print64(fileRecordList[i].numBytes, p64Buf1, useCommaDot), fileRecordList[i].filePath);
					}

					piecesDone++;
					firstFileThisPiece = currentFile;
					bytesRead = 0;
					if (showProgressCount) {
						backspaceProgressLine(&showProgressChars);
						sprintf_s(progressBuf, "%3i %s %i/%i (%i%%) ", currentFile + 1, ((errorsFound == 0) ? "ok" : "BAD"), piecesDone, numPieces, 100 * piecesDone / numPieces);
						showProgressChars = (int)strlen(progressBuf);
						fwrite(progressBuf, showProgressChars, 1, stdout);
						fflush(stdout);
					}
				}
			}
			if (fp != NULL) fclose(fp); fp = NULL;
			errorsFoundThisFile = 0;
			currentFile++;
		}

		for (i = firstFileThisPiece;i<currentFile;i++) {
			backspaceProgressLine(&showProgressChars);
			printf(useCommaDot ? "%3i %-3s %13s %s\n" : "%3i %-3s %11s %s\n", i + 1, ((fileRecordList[i].errorsFound == 0) ? "ok" : "BAD"),
				print64(fileRecordList[i].numBytes, p64Buf1, useCommaDot), fileRecordList[i].filePath);
		}

		if (fp != NULL) fclose(fp); fp = NULL;
		backspaceProgressLine(&showProgressChars);
		printf("Total files %i, total bytes %s, total errors %i, %s\n", currentFile, print64(totalBytesDone, p64Buf1, useCommaDot), errorsFound, (errorsFound == 0) ? "torrent is good" : "torrent has errors");
		if (errorsFound > 0) {
			return;
		}
		else {
			return;
		}

		/////////////////////////////////////////////////////////////////////////////////////
	}
	else if (contentPath != NULL) { // single file torrent
		int contentPathLength = (int)strlen(contentPath);
		strcpy_s(filePath, contentPathLength, contentPath);
		filePathOfs = (int)strlen(filePath);
		if ((filePathOfs > 0) && (filePath[filePathOfs - 1] != DIR_SEPARATOR)) {
			filePath[filePathOfs] = DIR_SEPARATOR;
			filePathOfs++;
		}
		memcpy(filePath + filePathOfs, rootName, rootNameLen);
		filePath[filePathOfs + rootNameLen] = 0;

		fopen_s(&fp, filePath, "rb");
		if (fp == NULL) {
			fopen_s(&fp, contentPath, "rb");
			if (fp != NULL) strcpy_s(filePath,strlen(contentPath), contentPath);
		}
		if (fp == NULL) {
			printf("Unable to open file %s or %s\n", contentPath, filePath);
			return;
		}

		fseek(fp, 0l, SEEK_END);
		fileBytesActual = ftell(fp);
		rewind(fp);
		if (fileBytesActual != fileBytesExpected) {
			backspaceProgressLine(&showProgressChars);
			printf("File length mismatch, expected %s bytes, found %s bytes\n", print64(fileBytesExpected, p64Buf1, useCommaDot), print64(fileBytesActual, p64Buf2, useCommaDot));
			errorsFound++;
		}

		for (piecesDone = 0;piecesDone<numPieces;piecesDone++) {

			bytesRead = fread(pieceBuf, 1, pieceLen, fp);
			//SHAInit(&sha1ctx);
			//SHAUpdate(&sha1ctx, pieceBuf, bytesRead);
			//SHAFinal(sha1hash, &sha1ctx);
			//sha.Reset();
			//sha.Update(pieceBuf, bytesRead);
			//sha.Final();
			//sha.GetHash(sha1hash);
			totalBytesDone += bytesRead;
			i = piecesDone * SHA1_LEN;

			if (memcmp(pieceList + i, sha1hash, SHA1_LEN) != 0) {
				errorsFound++;
				thisPieceBad = 1;
			}

			if (thisPieceBad || showPieceDetail) {
				backspaceProgressLine(&showProgressChars);
				printf("piece %i computed SHA1 %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", piecesDone,
					(int)sha1hash[0], (int)sha1hash[1], (int)sha1hash[2], (int)sha1hash[3],
					(int)sha1hash[4], (int)sha1hash[5], (int)sha1hash[6], (int)sha1hash[7],
					(int)sha1hash[8], (int)sha1hash[9], (int)sha1hash[10], (int)sha1hash[11],
					(int)sha1hash[12], (int)sha1hash[13], (int)sha1hash[14], (int)sha1hash[15],
					(int)sha1hash[16], (int)sha1hash[17], (int)sha1hash[18], (int)sha1hash[19]);

				printf("piece %i expected SHA1 %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", piecesDone,
					(int)pieceList[i + 0], (int)pieceList[i + 1], (int)pieceList[i + 2], (int)pieceList[i + 3],
					(int)pieceList[i + 4], (int)pieceList[i + 5], (int)pieceList[i + 6], (int)pieceList[i + 7],
					(int)pieceList[i + 8], (int)pieceList[i + 9], (int)pieceList[i + 10], (int)pieceList[i + 11],
					(int)pieceList[i + 12], (int)pieceList[i + 13], (int)pieceList[i + 14], (int)pieceList[i + 15],
					(int)pieceList[i + 16], (int)pieceList[i + 17], (int)pieceList[i + 18], (int)pieceList[i + 19]);
				printf("piece %i %s bytes, %s total bytes, %i error%s\n", piecesDone, print64(bytesRead, p64Buf1, useCommaDot), print64(totalBytesDone, p64Buf2, useCommaDot), errorsFound, ((errorsFound == 1) ? "" : "s"));
				thisPieceBad = 0;
			}

			if (showProgressCount) {
				backspaceProgressLine(&showProgressChars);
				sprintf_s(progressBuf, "%s %i/%i (%i%%) ", ((errorsFound == 0) ? "ok" : "BAD"), piecesDone, numPieces, 100 * piecesDone / numPieces);
				showProgressChars = (int)strlen(progressBuf);
				fwrite(progressBuf, showProgressChars, 1, stdout);
				fflush(stdout);
			}

		}
		if (fp != NULL) fclose(fp); fp = NULL;

		backspaceProgressLine(&showProgressChars);
		printf("Total bytes %s, total errors %i, %s\n", print64(totalBytesDone, p64Buf1, useCommaDot), errorsFound, (errorsFound == 0) ? "torrent is good" : "torrent has errors");

		if (errorsFound > 0) {
			return;
		}
		else {
			return;
		}
	}
	return; // How did we get here?
	
}


_CABTChecker::~_CABTChecker()
{
	delete[] pieceBuf;
	delete[] filePath2;
	delete[] fileRecordList;
	delete[] torrent;
	delete[] filePath;
}


// Step over an object (including all its embedded objects)
// Returns new offset, or -1 if unable to parse the input
int _CABTChecker::beStepOver(BYTE * benstr, int benstrLen, int benstrOffset)
{
	BYTE* bp;
	int ip;
	if ((benstrOffset < 0) || (benstrOffset >= benstrLen)) return (-1);
	switch (benstr[benstrOffset]) {
	case 'i':
		benstrOffset++;
		while (benstrOffset < benstrLen) {
			switch (benstr[benstrOffset]) {
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
			case '-':
				benstrOffset++;
				break;
			case 'e':
				benstrOffset++;
				if (benstrOffset < benstrLen) return benstrOffset;
				else return (-1);
			default:
				return (-1);
			}
		}
		return (-1);
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		benstrOffset = beParseString(benstr, benstrLen, benstrOffset, &bp, &ip);
		if ((benstrOffset < 0) || (benstrOffset >= benstrLen)) return(-1);
		else return benstrOffset;
	case 'l':
	case 'd':
		benstrOffset++;
		while (benstrOffset < benstrLen) {
			if (benstr[benstrOffset] == 'e') {
				benstrOffset++;
				if (benstrOffset <= benstrLen) return benstrOffset;
				else return (-1);
			}
			else {
				benstrOffset = beStepOver(benstr, benstrLen, benstrOffset);
				if ((benstrOffset < 0) || (benstrOffset >= benstrLen)) return (-1);
			}
		}
		return (-1);
	default:
		return (-1);
	}
}


// Obtain the length of the string and a pointer to its beginning
// Returns the new offset into the input,
// or -1 if unable to parse the input
// Remember this is not a null terminated string. You need to memcpy and terminate it.
int _CABTChecker::beParseString(BYTE * benstr, int benstrLen, int benstrOffset, BYTE ** stringBegin, int * stringLen)
{
	int i;
	BYTE b;
	int foundLen = 0;
	for (i = benstrOffset;i<benstrLen;i++) {
		b = benstr[i];
		switch (b) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			foundLen = foundLen * 10 + (b - '0');
			break;
		case ':':
			if ((i + 1 + foundLen) > benstrLen) {
				return (-1);
			}
			else {
				*stringBegin = benstr + i + 1;
				*stringLen = foundLen;
				return (i + 1 + foundLen);
			}
		default:
			return (-1);
		}
	}
	return (-1);
}


// Return offset of an element in a dict, or -1 if not found
int _CABTChecker::beFindInDict(BYTE * benstr, int benstrLen, int benstrOffset, BYTE * dictKey)
{
	BYTE* stringPtr;
	int stringLen;
	int dictKeyLen;
	if ((benstrOffset < 0) || (benstrOffset >= benstrLen))
		return (-1);
	if (benstr[benstrOffset] != 'd')
		return (-1);
	dictKeyLen = (int)strlen((char *)dictKey);
	benstrOffset++;
	while ((benstrOffset >= 0) && (benstrOffset < benstrLen))
	{
		if (benstr[benstrOffset] == 'e')
			return (-1);
		benstrOffset = beParseString(benstr, benstrLen, benstrOffset, &stringPtr, &stringLen);
		if (benstrOffset < 0)
			return (-1);
		if ((stringLen == dictKeyLen) && (memcmp(stringPtr, dictKey, stringLen) == 0))
		{
			return (benstrOffset);
		}
		else
		{
			benstrOffset = beStepOver(benstr, benstrLen, benstrOffset);
		}
	}
	return (-1);
}


// Extracts the integer
int _CABTChecker::beParseInteger(BYTE * benstr, int benstrLen, int benstrOffset, INT64 * longOut)
{
	INT64 foundValue = 0;
	INT64 negPos = 1;
	int i;
	BYTE b;
	for (i = benstrOffset;i<benstrLen;i++) {
		b = benstr[i];
		switch (b) {
		case 'i':
			if (i != benstrOffset) return (-1);
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			foundValue = foundValue * 10 + (b - '0');
			break;
		case '-':
			negPos = -1;
			break;
		case 'e':
			*longOut = foundValue * negPos;
			return (i + 1);
		default:
			return (-1);
		}
	}
	return (-1);
}

char* _CABTChecker::print64(INT64 val, char* buf, char useCommaDot) {
	INT64 divisor = 1000000000000000000l;
	char* bp = buf;
	int nonzero = 0;
	int digit;
	if (val < 0) {
		*bp = '-'; bp++;
		val = 0 - val;
	}
	while (divisor > 0) {
		digit = int(val / divisor);
		if (digit != 0) nonzero = 1;
		if (nonzero) {
			*bp = '0' + digit; bp++;
			if (useCommaDot && ((divisor == 1000l) || (divisor == 1000000l) ||
				(divisor == 1000000000l) || (divisor == 1000000000000l))) {
				*bp = useCommaDot; bp++;
			}
		}
		val -= digit * divisor;
		divisor /= 10;
	}
	if (nonzero == 0) {
		*bp = '0' + digit; bp++;
	}
	*bp = 0;
	return buf;
}


void _CABTChecker::backspaceProgressLine(int *showProgressChars) { // remove the progress line by backspacing
	if (*showProgressChars > 0) {
		fwrite("\
\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\
\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\
\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010", *showProgressChars, 1, stdout);
		*showProgressChars = 0;
	}
}