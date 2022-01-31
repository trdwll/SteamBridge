// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "Core/SteamHTMLSurface.h"

#include "SteamBridgeUtils.h"

USteamHTMLSurface::USteamHTMLSurface()
{
	OnHTMLBrowserReadyCallback.Register(this, &USteamHTMLSurface::OnHTMLBrowserReady);
	OnHTMLCanGoBackAndForwardCallback.Register(this, &USteamHTMLSurface::OnHTMLCanGoBackAndForward);
	OnHTMLChangedTitleCallback.Register(this, &USteamHTMLSurface::OnHTMLChangedTitle);
	OnHTMLCloseBrowserCallback.Register(this, &USteamHTMLSurface::OnHTMLCloseBrowser);
	OnHTMLFileOpenDialogCallback.Register(this, &USteamHTMLSurface::OnHTMLFileOpenDialog);
	OnHTMLFinishedRequestCallback.Register(this, &USteamHTMLSurface::OnHTMLFinishedRequest);
	OnHTMLHideToolTipCallback.Register(this, &USteamHTMLSurface::OnHTMLHideToolTip);
	OnHTMLHorizontalScrollCallback.Register(this, &USteamHTMLSurface::OnHTMLHorizontalScroll);
	OnHTMLJSAlertCallback.Register(this, &USteamHTMLSurface::OnHTMLJSAlert);
	OnHTMLJSConfirmCallback.Register(this, &USteamHTMLSurface::OnHTMLJSConfirm);
	OnHTMLLinkAtPositionCallback.Register(this, &USteamHTMLSurface::OnHTMLLinkAtPosition);
	OnHTMLNeedsPaintCallback.Register(this, &USteamHTMLSurface::OnHTMLNeedsPaint);
	OnHTMLNewWindowCallback.Register(this, &USteamHTMLSurface::OnHTMLNewWindow);
	OnHTMLOpenLinkInNewTabCallback.Register(this, &USteamHTMLSurface::OnHTMLOpenLinkInNewTab);
	OnHTMLSearchResultsCallback.Register(this, &USteamHTMLSurface::OnHTMLSearchResults);
	OnHTMLSetCursorCallback.Register(this, &USteamHTMLSurface::OnHTMLSetCursor);
	OnHTMLShowToolTipCallback.Register(this, &USteamHTMLSurface::OnHTMLShowToolTip);
	OnHTMLStartRequestCallback.Register(this, &USteamHTMLSurface::OnHTMLStartRequest);
	OnHTMLStatusTextCallback.Register(this, &USteamHTMLSurface::OnHTMLStatusText);
	OnHTMLUpdateToolTipCallback.Register(this, &USteamHTMLSurface::OnHTMLUpdateToolTip);
	OnHTMLURLChangedCallback.Register(this, &USteamHTMLSurface::OnHTMLURLChanged);
	OnHTMLVerticalScrollCallback.Register(this, &USteamHTMLSurface::OnHTMLVerticalScroll);
}

USteamHTMLSurface::~USteamHTMLSurface()
{
	OnHTMLBrowserReadyCallback.Unregister();
	OnHTMLCanGoBackAndForwardCallback.Unregister();
	OnHTMLChangedTitleCallback.Unregister();
	OnHTMLCloseBrowserCallback.Unregister();
	OnHTMLFileOpenDialogCallback.Unregister();
	OnHTMLFinishedRequestCallback.Unregister();
	OnHTMLHideToolTipCallback.Unregister();
	OnHTMLHorizontalScrollCallback.Unregister();
	OnHTMLJSAlertCallback.Unregister();
	OnHTMLJSConfirmCallback.Unregister();
	OnHTMLLinkAtPositionCallback.Unregister();
	OnHTMLNeedsPaintCallback.Unregister();
	OnHTMLNewWindowCallback.Unregister();
	OnHTMLOpenLinkInNewTabCallback.Unregister();
	OnHTMLSearchResultsCallback.Unregister();
	OnHTMLSetCursorCallback.Unregister();
	OnHTMLShowToolTipCallback.Unregister();
	OnHTMLStartRequestCallback.Unregister();
	OnHTMLStatusTextCallback.Unregister();
	OnHTMLUpdateToolTipCallback.Unregister();
	OnHTMLURLChangedCallback.Unregister();
	OnHTMLVerticalScrollCallback.Unregister();
}

void USteamHTMLSurface::SetCookie(const FString& Hostname, const FString& Key, const FString& Value, FDateTime Expires, const FString& Path, bool bSecure, bool bHTTPOnly)
{
	SteamHTMLSurface()->SetCookie(TCHAR_TO_UTF8(*Hostname), TCHAR_TO_UTF8(*Key), TCHAR_TO_UTF8(*Value), TCHAR_TO_UTF8(*Path), Expires.ToUnixTimestamp(), bSecure, bHTTPOnly);
}

void USteamHTMLSurface::OnHTMLBrowserReady(HTML_BrowserReady_t* pParam)
{
	OnHTMLBrowserReadyDelegate.Broadcast(pParam->unBrowserHandle);
}

void USteamHTMLSurface::OnHTMLCanGoBackAndForward(HTML_CanGoBackAndForward_t* pParam)
{
	OnHTMLCanGoBackAndForwardDelegate.Broadcast(pParam->unBrowserHandle, pParam->bCanGoBack, pParam->bCanGoForward);
}

void USteamHTMLSurface::OnHTMLChangedTitle(HTML_ChangedTitle_t* pParam)
{
	OnHTMLChangedTitleDelegate.Broadcast(pParam->unBrowserHandle, UTF8_TO_TCHAR(pParam->pchTitle));
}

void USteamHTMLSurface::OnHTMLCloseBrowser(HTML_CloseBrowser_t* pParam)
{
	OnHTMLCloseBrowserDelegate.Broadcast(pParam->unBrowserHandle);
}

void USteamHTMLSurface::OnHTMLFileOpenDialog(HTML_FileOpenDialog_t* pParam)
{
	OnHTMLFileOpenDialogDelegate.Broadcast(pParam->unBrowserHandle, UTF8_TO_TCHAR(pParam->pchTitle), UTF8_TO_TCHAR(pParam->pchInitialFile));
}

void USteamHTMLSurface::OnHTMLFinishedRequest(HTML_FinishedRequest_t* pParam)
{
	OnHTMLFinishedRequestDelegate.Broadcast(pParam->unBrowserHandle, UTF8_TO_TCHAR(pParam->pchURL), UTF8_TO_TCHAR(pParam->pchPageTitle));
}

void USteamHTMLSurface::OnHTMLHideToolTip(HTML_HideToolTip_t* pParam)
{
	OnHTMLHideToolTipDelegate.Broadcast(pParam->unBrowserHandle);
}

void USteamHTMLSurface::OnHTMLHorizontalScroll(HTML_HorizontalScroll_t* pParam)
{
	OnHTMLHorizontalScrollDelegate.Broadcast(pParam->unBrowserHandle, pParam->unScrollMax, pParam->unScrollCurrent, pParam->flPageScale, pParam->bVisible, pParam->unPageSize);
}

void USteamHTMLSurface::OnHTMLJSAlert(HTML_JSAlert_t* pParam)
{
	OnHTMLJSAlertDelegate.Broadcast(pParam->unBrowserHandle, UTF8_TO_TCHAR(pParam->pchMessage));
}

void USteamHTMLSurface::OnHTMLJSConfirm(HTML_JSConfirm_t* pParam)
{
	OnHTMLJSConfirmDelegate.Broadcast(pParam->unBrowserHandle, UTF8_TO_TCHAR(pParam->pchMessage));
}

void USteamHTMLSurface::OnHTMLLinkAtPosition(HTML_LinkAtPosition_t* pParam)
{
	OnHTMLLinkAtPositionDelegate.Broadcast(pParam->unBrowserHandle, UTF8_TO_TCHAR(pParam->pchURL), pParam->bInput, pParam->bLiveLink);
}

void USteamHTMLSurface::OnHTMLNeedsPaint(HTML_NeedsPaint_t* pParam)
{
	OnHTMLNeedsPaintDelegate.Broadcast(pParam->unBrowserHandle, UTF8_TO_TCHAR(pParam->pBGRA), {(int32)pParam->unTall, (int32)pParam->unWide}, {(int32)pParam->unUpdateX, (int32)pParam->unUpdateY}, {(int32)pParam->unUpdateTall, (int32)pParam->unUpdateWide},
		{(int32)pParam->unScrollX, (int32)pParam->unScrollY}, pParam->flPageScale, pParam->unPageSerial);
}

void USteamHTMLSurface::OnHTMLNewWindow(HTML_NewWindow_t* pParam)
{
	OnHTMLNewWindowDelegate.Broadcast(pParam->unBrowserHandle, UTF8_TO_TCHAR(pParam->pchURL), {(int32)pParam->unX, (int32)pParam->unY}, {(int32)pParam->unTall, (int32)pParam->unWide});
}

void USteamHTMLSurface::OnHTMLOpenLinkInNewTab(HTML_OpenLinkInNewTab_t* pParam)
{
	OnHTMLOpenLinkInNewTabDelegate.Broadcast(pParam->unBrowserHandle, UTF8_TO_TCHAR(pParam->pchURL));
}

void USteamHTMLSurface::OnHTMLSearchResults(HTML_SearchResults_t* pParam)
{
	OnHTMLSearchResultsDelegate.Broadcast(pParam->unBrowserHandle, pParam->unResults, pParam->unCurrentMatch);
}

void USteamHTMLSurface::OnHTMLSetCursor(HTML_SetCursor_t* pParam)
{
	OnHTMLSetCursorDelegate.Broadcast(pParam->unBrowserHandle, (ESteamMouseCursor)pParam->eMouseCursor);
}

void USteamHTMLSurface::OnHTMLShowToolTip(HTML_ShowToolTip_t* pParam)
{
	OnHTMLShowToolTipDelegate.Broadcast(pParam->unBrowserHandle, UTF8_TO_TCHAR(pParam->pchMsg));
}

void USteamHTMLSurface::OnHTMLStartRequest(HTML_StartRequest_t* pParam)
{
	OnHTMLStartRequestDelegate.Broadcast(pParam->unBrowserHandle, UTF8_TO_TCHAR(pParam->pchURL), UTF8_TO_TCHAR(pParam->pchTarget), UTF8_TO_TCHAR(pParam->pchPostData), pParam->bIsRedirect);
}

void USteamHTMLSurface::OnHTMLStatusText(HTML_StatusText_t* pParam)
{
	OnHTMLStatusTextDelegate.Broadcast(pParam->unBrowserHandle, UTF8_TO_TCHAR(pParam->pchMsg));
}

void USteamHTMLSurface::OnHTMLUpdateToolTip(HTML_UpdateToolTip_t* pParam)
{
	OnHTMLUpdateToolTipDelegate.Broadcast(pParam->unBrowserHandle, UTF8_TO_TCHAR(pParam->pchMsg));
}

void USteamHTMLSurface::OnHTMLURLChanged(HTML_URLChanged_t* pParam)
{
	OnHTMLURLChangedDelegate.Broadcast(pParam->unBrowserHandle, UTF8_TO_TCHAR(pParam->pchURL), UTF8_TO_TCHAR(pParam->pchPostData), pParam->bIsRedirect, UTF8_TO_TCHAR(pParam->pchPageTitle), pParam->bNewNavigation);
}

void USteamHTMLSurface::OnHTMLVerticalScroll(HTML_VerticalScroll_t* pParam)
{
	OnHTMLVerticalScrollDelegate.Broadcast(pParam->unBrowserHandle, pParam->unScrollMax, pParam->unScrollCurrent, pParam->flPageScale, pParam->bVisible, pParam->unPageSize);
}
