// Wininetter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "wininet.lib")

void CALLBACK SessionStatusCallback(
	_In_ HINTERNET hInternet,
	_In_ DWORD_PTR dwContext,
	_In_ DWORD     dwInternetStatus,
	_In_ LPVOID    lpvStatusInformation,
	_In_ DWORD     dwStatusInformationLength )
{
	switch (dwInternetStatus)
	{
	case INTERNET_STATUS_REQUEST_COMPLETE:
		puts("INTERNET_STATUS_REQUEST_COMPLETE");
		::SetEvent(reinterpret_cast<HANDLE>(dwContext));
		break;

	case INTERNET_STATUS_CLOSING_CONNECTION:
		puts("INTERNET_STATUS_CLOSING_CONNECTION");
		break;

	case INTERNET_STATUS_CONNECTED_TO_SERVER:
		puts("INTERNET_STATUS_CONNECTED_TO_SERVER");
		break;

	case INTERNET_STATUS_CONNECTING_TO_SERVER:
		puts("INTERNET_STATUS_CONNECTING_TO_SERVER");
		break;

	case INTERNET_STATUS_CONNECTION_CLOSED:
		puts("INTERNET_STATUS_CONNECTION_CLOSED");
		break;

	case INTERNET_STATUS_COOKIE_HISTORY:
		puts("INTERNET_STATUS_COOKIE_HISTORY");
		break;

	case INTERNET_STATUS_COOKIE_RECEIVED:
		puts("INTERNET_STATUS_COOKIE_RECEIVED");
		break;

	case INTERNET_STATUS_COOKIE_SENT:
		puts("INTERNET_STATUS_COOKIE_SENT");
		break;

	case INTERNET_STATUS_CTL_RESPONSE_RECEIVED:
		puts("INTERNET_STATUS_CTL_RESPONSE_RECEIVED");
		break;

	case INTERNET_STATUS_DETECTING_PROXY:
		puts("INTERNET_STATUS_DETECTING_PROXY");
		break;

	case INTERNET_STATUS_HANDLE_CLOSING:
		puts("INTERNET_STATUS_HANDLE_CLOSING");
		break;

	case INTERNET_STATUS_HANDLE_CREATED:
		puts("INTERNET_STATUS_HANDLE_CREATED");
		break;

	case INTERNET_STATUS_INTERMEDIATE_RESPONSE:
		puts("INTERNET_STATUS_INTERMEDIATE_RESPONSE");
		break;

	case INTERNET_STATUS_NAME_RESOLVED:
		puts("INTERNET_STATUS_NAME_RESOLVED");
		break;

	case INTERNET_STATUS_P3P_HEADER:
		puts("INTERNET_STATUS_P3P_HEADER");
		break;

	case INTERNET_STATUS_P3P_POLICYREF:
		puts("INTERNET_STATUS_P3P_POLICYREF");
		break;

	case INTERNET_STATUS_PREFETCH:
		puts("INTERNET_STATUS_PREFETCH");
		break;

	case INTERNET_STATUS_PRIVACY_IMPACTED:
		puts("INTERNET_STATUS_PRIVACY_IMPACTED");
		break;

	case INTERNET_STATUS_RECEIVING_RESPONSE:
		puts("INTERNET_STATUS_RECEIVING_RESPONSE");
		break;

	case INTERNET_STATUS_REDIRECT:
		puts("INTERNET_STATUS_REDIRECT");
		break;

	case INTERNET_STATUS_REQUEST_SENT:
		puts("INTERNET_STATUS_REQUEST_SENT");
		break;

	case INTERNET_STATUS_RESOLVING_NAME:
		puts("INTERNET_STATUS_RESOLVING_NAME");
		break;

	case INTERNET_STATUS_RESPONSE_RECEIVED:
		puts("INTERNET_STATUS_RESPONSE_RECEIVED");
		break;

	case INTERNET_STATUS_SENDING_REQUEST:
		puts("INTERNET_STATUS_SENDING_REQUEST");
		break;

	case INTERNET_STATUS_STATE_CHANGE:
		puts("INTERNET_STATUS_STATE_CHANGE");
		break;
	}
}

static void CallSession(HINTERNET hintSession, HANDLE hcomplete)
{
	HINTERNET hintReq = ::HttpOpenRequest(hintSession, TEXT("GET"), TEXT(""), TEXT("HTTP/1.1"), NULL, NULL,
		INTERNET_FLAG_NO_AUTH|INTERNET_FLAG_NO_COOKIES|INTERNET_FLAG_RELOAD|INTERNET_FLAG_SECURE,
		reinterpret_cast<DWORD_PTR>(hcomplete));

	if (NULL != hintReq)
	{
		::InternetSetStatusCallback(hintReq, SessionStatusCallback);

		if (!::HttpSendRequest(hintReq, NULL, 0, NULL, 0))
		{
			DWORD error = ::GetLastError();

			printf("ERROR=%lu\n", error);

			if (ERROR_IO_PENDING == error)
			{
				::WaitForSingleObject(hcomplete, INFINITE);
			}
		}

		::InternetCloseHandle(hintReq);
	}
}

static void QueryInternet(HINTERNET hintRoot)
{
	HANDLE hcomplete = ::CreateEvent(NULL, TRUE, FALSE, NULL);

	if (NULL != hcomplete)
	{
		HINTERNET hintSession = ::InternetConnect(hintRoot, TEXT("www.microsoft.com"), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, reinterpret_cast<DWORD_PTR>(hcomplete));

		if (NULL != hintSession)
		{
			CallSession(hintSession, hcomplete);
			::InternetCloseHandle(hintSession);
		}

		::CloseHandle(hcomplete);
	}
}

int main()
{
	if (ERROR_SUCCESS != ::InternetAttemptConnect(0))
	{
		//
		//	...
		//
	}
	else
	{
		HINTERNET hintRoot = ::InternetOpen(TEXT("Don Pedro Test Application"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_ASYNC);

		if (NULL != hintRoot)
		{
			QueryInternet(hintRoot);
			::InternetCloseHandle(hintRoot);
		}
	}

    return 0;
}
