// Telnet Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#define ICON_FOLDER "<img src=\"https://img.icons8.com/color/50/000000/folder-invoices.png\" height=\"17px\">"  
/*"<img src=\"data:image/png;base64,
iVBORw0KGgoAAAANSUhEUgAAADIAAAAyCAYAAAAeP4ixAAA
AAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAA
DsMAAA7DAcdvqGQAAAFfSURBVGhD7ZK9LkRRFEZPodGI+Iu
fKJDcufS8A57Bo5BMLyq9MVHdhJGIQYgCCXM1k4lSotXRoP
uOfWSX+4w7MsZO7JWsbvbJ+nLHGYZhGMZvgIpbwo7b8hWXFZ
F+X0XVjfG5DiiqTHG+U2n4g5oxFLMqRRZVzRj6GjUpsBNVjK
Ehza+gvT7vDwe9Pxr+kTgeecb5ZL1rnk3soj60wJnfQ0Na
vjbg/V3ifZ6qErcJcDq+zantQdb/KD2iSZyMrnFuHFxMvUj
HmsTl9BPnxsHVzLt0rElqfOPcOLie/ZCONRkaOTeODemhhY
b4m7lX6ViV1Mi5cdBIW+KxIkMj58axIT3UhmjThmjThmjThm
jThmjThmjznw3J06Z0rMnQyLlxkJcOpGNNolHa59w4uJ9fkY
41iTxZ5tz20KfboP8hpEf+0tBErnNmMWj1Ig3apAcyDYaW0
MR5hmEYhtFFnPsEarLYUOTpRhkAAAAASUVORK5CYII=\">" */

#define ICON_FILE "<img src=\"https://img.icons8.com/color/50/000000/file.png\" height=\"17px\">"
/*<img src="data:image/png;base64,
iVBORw0KGgoAAAANSUhEUgAAADIAAAAyCAYAAAAeP4ixAAA
AAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZ
cwAADsMAAA7DAcdvqGQAAADpSURBVGhD7doxCsJAEIXh4MU
FL7ARwUYsPYJYCzZZROysgghiY2MjJGHjmMUtphINM5Pm/
fDAQCDz9WYIDVRe1K5bqTJfzcdEo/Qp3ZyvVrmvSXqzffp
dVEsTjBZkdwu0ODafZwuMFuRwb+n0aO0wmpDL82WH0YaYY
SwgJhgriDrGEqKKsYbEqWCGgMRxjPP1JJ3TPy3Iugy0vX7f5
hxo2r0bb0jn9E8L8s8A4QEiOEB4gAgOEB4gggOEB4jgAOEB
IjhAeIAIDhAeIIIDhAeI4ADhASI4QHiACE4Ekmv+heP3uX
QOQrZl2Rv20rt/bMGhFQAAAABJRU5ErkJggg==">*/

int userCount = 0;
char** userName = NULL;
char** userPsw = NULL;
DWORD WINAPI ClientThread(LPVOID arg);
char* html = NULL;

int main()
{	
	WSADATA DATA;
	WSAStartup(MAKEWORD(2, 2), &DATA);
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(5000);
	saddr.sin_addr.S_un.S_addr = 0;
	bind(s, (sockaddr*)&saddr, sizeof(saddr));
	listen(s, 10);
	while (0 == 0)
	{
		SOCKADDR_IN caddr;
		int clen = sizeof(caddr);
		SOCKET c = accept(s, (sockaddr*)&caddr, &clen);
		DWORD ID = 0;
		CreateThread(NULL, 0, ClientThread, (LPVOID)c, 0, &ID);
	}
	
	return 0;
}

void AppendHTML(char* path, char* str)
{
	if (path != NULL)
	{
		char* tmp = (char*)calloc(1024, 1);
		sprintf(tmp, "<a href=\"%s\">%s</a>", path, str);
		int oldLen = html == NULL ? 0 : strlen(html);
		int newSize = oldLen + strlen(tmp) + 1;
		html = (char*)realloc(html, newSize);
		memset(html + oldLen, 0, newSize - oldLen);
		strcpy(html + oldLen, tmp);
		free(tmp);
		tmp = NULL;
	}
	else
	{
		int oldLen = html == NULL ? 0 : strlen(html);
		int newSize = oldLen + strlen(str) + 1;
		html = (char*)realloc(html, newSize);
		memset(html + oldLen, 0, newSize - oldLen);
		strcpy(html + oldLen, str);
	}
}

DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET c = (SOCKET)arg;
	char buffer[1024];
	char verb[1024];
	char path[1024];
	char http[1024];
	memset(buffer, 0, sizeof(buffer));
	recv(c, buffer, 1023, 0);
	memset(verb, 0, sizeof(verb));
	memset(path, 0, sizeof(path));
	memset(http, 0, sizeof(http));
	sscanf(buffer, "%s%s%s", verb, path, http);
	char* tmp = "%20";
	// thay the "%20" thanh dau cach
	while (strstr(path, tmp))
	{
		int len = strlen(path);
		char* found = strstr(path, tmp);
		found[0] = ' ';
		strcpy(found + 1, found + 3);
		memset(path + len - 2, 0, 2);
	}
	if (path[strlen(path)-1]== '/') 
	{
		int n = 0;
		for (int i = strlen(path) - 2; i >= 0; i--)
		{
			n++;
			if (path[i] == '/')
				break;
		}
		memset(path + strlen(path) - n - 1, 0, n+1);
	}

	// Phuong thuc GET
	if (strcmp(verb, "GET") == 0)
	{
		if (strncmp(path, "/FILE_", 6) == 0)
		{
			int n = 0;
			for (int i = strlen(path)-1; i >= 0; --i)
			{
				if (path[i] == '.')
					break;
				n++;
			}
			char typeFile[10];
			strcpy(typeFile, path + strlen(path) - n);
			char* header;
			if (strcmp(typeFile,"mp3") == 0 || strcmp(typeFile, "flac") == 0)
			{
				header = "HTTP/1.1 200\r\nContent-Type: audio/mp3\r\n\r\n";
			}
			else if (strcmp(typeFile, "txt") == 0 || 
					strcmp(typeFile, "html") == 0 || 
					strcmp(typeFile, "doc") == 0 || 
					strcmp(typeFile, "docx") == 0)
			{
				header = "HTTP/1.1 200\r\nContent-Type: text/html\r\n\r\n";
			}
			else if (strcmp(typeFile, "pdf") == 0)
			{
				header = "HTTP/1.1 200\r\nContent-Type: application/pdf\r\n\r\n";
			}
			else if (strcmp(typeFile, "png") == 0 ||
					strcmp(typeFile, "jpg") == 0 || 
					strcmp(typeFile, "jpeg") == 0)
			{
				header = "HTTP/1.1 200\r\nContent-Type: image/jpeg\r\n\r\n";
			}
			else
			{
				header = "HTTP/1.1 200\r\nContent-Type: video/mp4\r\n\r\n";
			}
			send(c, header, strlen(header), 0);
			strcpy(path, path + 6);
			char* fullpath = (char*)calloc(1024, 1);
			sprintf(fullpath, "d:\\%s", path);
			FILE* f = fopen(fullpath, "rb");
			char* buffer = (char*)calloc(1024, 1);
			while (!feof(f))
			{
				int n = fread(buffer, 1, 1024, f);
				if (n > 0)
				{
					send(c, buffer, n, 0);
				}
			}
			fclose(f);
			free(fullpath);
			fullpath = NULL;
		}
		else
		{
			if (html != NULL)
			{
				free(html);
				html = NULL;
			}
			AppendHTML(NULL, "<html>");
			WIN32_FIND_DATAA fData;
			HANDLE hFind = NULL;
			if (strcmp(path, "/") == 0)
			{
				hFind = FindFirstFileA("d:/*.*", &fData);
			}
			else
			{
				char* fullpath = (char*)calloc(1024, 1);
				sprintf(fullpath, "d:/%s/*.*", path);
				hFind = FindFirstFileA(fullpath, &fData);
				free(fullpath);
				fullpath = NULL;
			}
			BOOL next = FALSE;
			do
			{
				char* addTagImg = (char*)calloc(1024, 1);
				char* fullpath = (char*)calloc(1024, 1);
				if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // is folder
				{
					sprintf(addTagImg, "%s%s", ICON_FOLDER, fData.cFileName);
					sprintf(fullpath, "%s/%s", path, fData.cFileName);
				}
				else // is file
				{
					sprintf(addTagImg, "%s%s", ICON_FILE, fData.cFileName);
					sprintf(fullpath, "FILE_%s/%s", path, fData.cFileName);
				}
				AppendHTML(fullpath, addTagImg);
				AppendHTML(NULL, "<br>");
				free(fullpath);
				fullpath = NULL;
				next = FindNextFileA(hFind, &fData);
			} while (next == TRUE);
			AppendHTML(NULL, "</html>");
			send(c, html, strlen(html), 0);
			free(html);
			html = NULL;
		}
	}

	// Phuong thuc POST
	else if(strcmp(verb,"POST") == 0)
	{
		char* data = strstr(buffer, "id_user_name");
		char user_name[64];
		memset(user_name, 0, 64);
		char user_pass[64];
		memset(user_pass, 0, 64);
		int i = 13;
		int idx = 0;
		for (; i < strlen(data); ++i)
		{
			if (data[i] == '&')
				break;
			user_name[idx] = data[i];
			idx++;
		}
		i += 13;
		idx = 0;
		for (; i < strlen(data); ++i)
		{
			if (data[i] == '&')
				break;
			user_pass[idx] = data[i];
			idx++;
		}
		
		char* html_rep = (char*)malloc(1024);
		memset(html_rep, 0, 1024);
		sprintf(html_rep, "<html><p>User Name va User Pass ban da gui len server la:<p>user_name: %s</p><p>user_pass: %s</p></p></html>", user_name, user_pass);
		send(c, html_rep, strlen(html_rep), 0);
	}
	closesocket(c);
	return 0;
}
