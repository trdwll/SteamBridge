// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

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

void USteamHTTP::OnHTTPRequestCompleted(HTTPRequestCompleted_t* pParam)
{
	OnHTTPRequestCompletedDelegate.Broadcast(pParam->m_hRequest, pParam->m_ulContextValue, pParam->m_bRequestSuccessful, (ESteamHTTPStatus::Type)pParam->m_eStatusCode, pParam->m_unBodySize);
}

void USteamHTTP::OnHTTPRequestDataReceived(HTTPRequestDataReceived_t* pParam)
{
	OnHTTPRequestDataReceivedDelegate.Broadcast(pParam->m_hRequest, pParam->m_ulContextValue, pParam->m_cOffset, pParam->m_cBytesReceived);
}

void USteamHTTP::OnHTTPRequestHeadersReceived(HTTPRequestHeadersReceived_t* pParam)
{
	OnHTTPRequestHeadersReceivedDelegate.Broadcast(pParam->m_hRequest, pParam->m_ulContextValue);
}
