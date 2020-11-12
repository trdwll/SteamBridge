// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "Core/SteamHTTP.h"

#include "SteamBridgeUtils.h"

USteamHTTP::USteamHTTP()
{
	OnHTTPRequestCompletedCallback.Register(this, &USteamHTTP::OnHTTPRequestCompleted);
	OnHTTPRequestDataReceivedCallback.Register(this, &USteamHTTP::OnHTTPRequestDataReceived);
	OnHTTPRequestHeadersReceivedCallback.Register(this, &USteamHTTP::OnHTTPRequestHeadersReceived);
}

USteamHTTP::~USteamHTTP()
{
	OnHTTPRequestCompletedCallback.Unregister();
	OnHTTPRequestDataReceivedCallback.Unregister();
	OnHTTPRequestHeadersReceivedCallback.Unregister();
}

bool USteamHTTP::GetHTTPResponseBodySize(int32 RequestHandle, int32& BodySize)
{
	uint32 Tmp = 0;
	BodySize = Tmp;
	return SteamHTTP()->GetHTTPResponseBodySize(RequestHandle, &Tmp);
}

bool USteamHTTP::GetHTTPResponseHeaderSize(int32 RequestHandle, const FString& HeaderName, int32& ResponseHeaderSize)
{
	uint32 Tmp = 0;
	ResponseHeaderSize = Tmp;
	return SteamHTTP()->GetHTTPResponseHeaderSize(RequestHandle, TCHAR_TO_UTF8(*HeaderName), &Tmp);
}

bool USteamHTTP::SendHTTPRequest(int32 RequestHandle, FSteamAPICall& CallHandle)
{
	SteamAPICall_t Tmp = 0;
	CallHandle = Tmp;
	return SteamHTTP()->SendHTTPRequest(RequestHandle, &Tmp);
}

bool USteamHTTP::SendHTTPRequestAndStreamResponse(int32 RequestHandle, FSteamAPICall& CallHandle)
{
	SteamAPICall_t Tmp = 0;
	CallHandle = Tmp;
	return SteamHTTP()->SendHTTPRequestAndStreamResponse(RequestHandle, &Tmp);
}

void USteamHTTP::OnHTTPRequestCompleted(HTTPRequestCompleted_t* pParam)
{
	m_OnHTTPRequestCompleted.Broadcast(pParam->m_hRequest, pParam->m_ulContextValue, pParam->m_bRequestSuccessful, (ESteamHTTPStatus::Type)pParam->m_eStatusCode, pParam->m_unBodySize);
}

void USteamHTTP::OnHTTPRequestDataReceived(HTTPRequestDataReceived_t* pParam)
{
	m_OnHTTPRequestDataReceived.Broadcast(pParam->m_hRequest, pParam->m_ulContextValue, pParam->m_cOffset, pParam->m_cBytesReceived);
}

void USteamHTTP::OnHTTPRequestHeadersReceived(HTTPRequestHeadersReceived_t* pParam)
{
	m_OnHTTPRequestHeadersReceived.Broadcast(pParam->m_hRequest, pParam->m_ulContextValue);
}
