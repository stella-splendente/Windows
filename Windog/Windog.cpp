#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <strsafe.h>
#include <stdint.h>
#include <crtdbg.h>
#include <WinBase.h>
#include <fstream>
#include <WTypes.h >
#include <oleauto.h>
#include <streambuf>
#include <string>
#include <conio.h>
#include <iostream>
#include <errno.h>
#include <direct.h>
#include <io.h>
#include <malloc.h>
#include <math.h>
#include <process.h>
#include <search.h>
#include <signal.h>
#include <fcntl.h>
#include <fwpstypes.h>
#include <ctype.h>
#include <stringapiset.h>
#include <fstream>
#include <tchar.h>
#include <locale.h>
#include <fileapi.h>
#include <memory>
#pragma warning(disable:4996)
#define MAX_LINE 1024
using namespace std;

bool read_bob_using_memory_map();
bool create_bob(HANDLE file_handle);
bool copy_bob(LPCWSTR input);
bool delete_bob();
bool read_bob();

bool read_bob_using_memory_map()
{
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		printf("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		printf("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	// current dir \\ test.txt 파일명 생성 
	wchar_t file_name[260];
	wchar_t file_name2[260];
	if (!SUCCEEDED(StringCbPrintfW(
		file_name,
		sizeof(file_name),
		L"%ws\\bob.txt",
		buf)))
	{
		printf("err, can not create file name");
		free(buf);
		return false;
	}
	if (!SUCCEEDED(StringCbPrintfW(
		file_name2,
		sizeof(file_name2),
		L"%ws\\bob2.txt",
		buf)))
	{
		printf("err, can not create file name");
		free(buf);
		return false;
	}
	free(buf); buf = NULL;

	HANDLE file_handle = CreateFileW(
		(LPCWSTR)file_name,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (INVALID_HANDLE_VALUE == file_handle)
	{
		printf("err, CreateFile(%ws) failed, gle = %u", file_name, GetLastError());
		return false;
	}


	if (TRUE != create_bob(file_handle))
	{
		return false;
	}


	if (TRUE != copy_bob(file_name))
	{
		CloseHandle(file_handle);
		return false;
	}

	read_bob();

	HANDLE file_handle2 = CreateFileW(
		(LPCWSTR)file_name2,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);



	CloseHandle(file_handle);
	CloseHandle(file_handle2);
	return true;


}
bool create_bob(HANDLE file_handle)
{
	wchar_t chr[260];
	if (!SUCCEEDED(StringCbPrintfW(
		chr,
		sizeof(chr),
		L"노용환 멘토님 만세 I can give my word!!!")))
	{
		printf("err, can not create file name");
		return false;
	}

	//문자열을 UTF-8로 인코딩 
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, chr, wcslen(chr), NULL, 0, NULL, NULL);
	char* windows;
	windows = (char*)malloc(sizeof(char)*size_needed);

	WideCharToMultiByte(CP_UTF8, 0, chr, size_needed, windows, size_needed, NULL, NULL);

	unsigned char smarker[3] = { 0xEF, 0xBB, 0xBF };
	DWORD numberOfByteWritten;


	if (TRUE != ::WriteFile(file_handle, &smarker, sizeof(smarker), &numberOfByteWritten, NULL))
	{
		printf("err, WriteFile(%s) failed, gle = %u", chr, GetLastError());
		CloseHandle(file_handle);
		return false;
	}
	if (TRUE != ::WriteFile(file_handle, windows, size_needed, &numberOfByteWritten, NULL))
	{
		printf("err, WriteFile(%s) failed, gle = %u", chr, GetLastError());
		CloseHandle(file_handle);
		return false;
	}

	free(windows); //malloc 해제 
	CloseHandle(file_handle);
	return true;
}
bool delete_bob()
{
wchar_t *buf = NULL;
uint32_t len = 0;
len = GetCurrentDirectoryW(len, buf);
if (0 == len)
{
	printf("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
	return false;
}

buf = (PWSTR)malloc(sizeof(WCHAR) * len);
if (0 == GetCurrentDirectoryW(len, buf))
{
	printf("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
	free(buf);
	return false;
}
// current dir \\ test.txt 파일명 생성 
wchar_t file_name[260];
wchar_t file_name2[260];
if (!SUCCEEDED(StringCbPrintfW(
	file_name,
	sizeof(file_name),
	L"%ws\\bob.txt",
	buf)))
{
	printf("err, can not create file name");
	free(buf);
	return false;
}
if (!SUCCEEDED(StringCbPrintfW(
	file_name2,
	sizeof(file_name2),
	L"%ws\\bob2.txt",
	buf)))
{
	printf("err, can not create file name");
	free(buf);
	return false;
}
free(buf); buf = NULL;

DeleteFileW(file_name);
DeleteFileW(file_name2);

return true;
 }
 bool copy_bob(LPCWSTR input) {
	 // current directory 를 구한다. 
	 wchar_t *buf = NULL;
	 uint32_t buflen = 0;
	 buflen = GetCurrentDirectoryW(buflen, buf);
	 if (0 == buflen)
	 {
		 printf("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		 return false;
	 }


	 buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);
	 if (0 == GetCurrentDirectoryW(buflen, buf))
	 {
		 printf("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		 free(buf);
		 return false;
	 }

	 // current dir \\ bob2.txt 파일명 생성 
	 wchar_t file_name[260];
	 if (!SUCCEEDED(StringCbPrintfW(
		 file_name,
		 sizeof(file_name),
		 L"%ws\\bob2.txt",
		 buf)))
	 {
		 printf("err, can not create file name");
		 free(buf);
		 return false;
	 }
	 free(buf);
	 buf = NULL;

	 if (TRUE != CopyFile(input, (LPCWSTR)file_name, false)) //파일 copy 
	 {
		 printf("err, CopyFile() failed. gle: %u", GetLastError());
		 return false;
	 }
	 return true;
 }
 bool read_bob()
 {
	 // current directory 를 구한다. 
	 wchar_t *buf = NULL;
	 uint32_t buflen = 0;
	 buflen = GetCurrentDirectoryW(buflen, buf);
	 if (0 == buflen)
	 {
		 printf("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());

		 return false;
	 }


	 buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);
	 if (0 == GetCurrentDirectoryW(buflen, buf))
	 {
		 printf("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		 free(buf);

		 return false;
	 }


	 // current dir \\ test.txt 파일명 생성 
	 wchar_t file_name[260];
	 if (!SUCCEEDED(StringCbPrintfW(
		 file_name,
		 sizeof(file_name),
		 L"%ws\\bob2.txt",
		 buf)))
	 {
		 printf("err, can not create file name");
		 free(buf);

		 return false;
	 }
	 free(buf); buf = NULL;


	 HANDLE file_handle = CreateFileW(
		 (LPCWSTR)file_name,
		 GENERIC_READ,
		 FILE_SHARE_READ,
		 NULL,
		 OPEN_EXISTING,
		 FILE_ATTRIBUTE_NORMAL,
		 NULL

		 );
	 if (INVALID_HANDLE_VALUE == file_handle)
	 {
		 printf("err, CreateFile(%ws) failed, gle = %u", file_name, GetLastError());
		 CloseHandle(file_handle);
		 return false;
	 }
	 wchar_t* bstrstr = NULL;
	 char multibyteBuffer[MAX_LINE];
	 char* str = NULL;
	 DWORD numberOfByteRead;


	 memset(multibyteBuffer, 0, sizeof(multibyteBuffer));


	 if (TRUE != ReadFile(file_handle, multibyteBuffer, MAX_LINE - 1, &numberOfByteRead, NULL))
	 {
		 printf("err, ReadFile() failed. gle: %u", GetLastError());
		 CloseHandle(file_handle);
		 return false;
	 }
	 int Len1 = ::MultiByteToWideChar(CP_UTF8, 0, multibyteBuffer, -1, bstrstr, 0);
	 bstrstr = (wchar_t*)malloc(sizeof(wchar_t)*(Len1 + 1));
	 memset(bstrstr, 0, sizeof(bstrstr));
	 ::MultiByteToWideChar(CP_UTF8, 0, multibyteBuffer, -1, bstrstr, Len1);


	 int Len2 = WideCharToMultiByte(CP_ACP, 0, bstrstr, -1, str, 0, NULL, NULL);
	 str = (char*)malloc(Len2 + 1);
	 memset(str, 0, sizeof(str));
	 WideCharToMultiByte(CP_ACP, 0, bstrstr, -1, str, Len2, NULL, NULL);

	 printf("%s\n\n", str + 1);

	 free(bstrstr);
	 free(str);
	 CloseHandle(file_handle);
	 return true;
 }
 int _tmain(int argc, _TCHAR* argv[])
 {
	 read_bob_using_memory_map();
	 return 0;
 }
