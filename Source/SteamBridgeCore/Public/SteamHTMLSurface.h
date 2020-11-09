// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamHTMLSurface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHTMLBrowserReadyDelegate, int32, BrowserHandle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHTMLCanGoBackAndForwardDelegate, int32, BrowserHandle, bool, bCanGoBack, bool, bCanGoForward);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHTMLChangedTitleDelegate, int32, BrowserHandle, FString, Title);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHTMLCloseBrowserDelegate, int32, BrowserHandle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHTMLFileOpenDialogDelegate, int32, BrowserHandle, FString, Title, FString, InitialFileName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHTMLFinishedRequestDelegate, int32, BrowserHandle, FString, URL, FString, PageTitle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHTMLHideToolTipDelegate, int32, BrowserHandle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHTMLHorizontalScrollDelegate, int32, BrowserHandle, int32, ScrollMax, int32, ScrollCurrent, float, PageScale, bool, bVisible, int32, PageSize);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHTMLJSAlertDelegate, int32, BrowserHandle, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHTMLJSConfirmDelegate, int32, BrowserHandle, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHTMLLinkAtPositionDelegate, int32, BrowserHandle, FString, URL, bool, bInput, bool, bLiveLink);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_EightParams(FOnHTMLNeedsPaintDelegate, int32, BrowserHandle, FString, BGRA, FIntPoint, Size, FIntPoint, Update, FIntPoint, UpdateSize, FIntPoint, ScrollPosition, float, PageScale, int32, PageSerial);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHTMLNewWindowDelegate, int32, BrowserHandle, FString, URL, FIntPoint, Position, FIntPoint, Size);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHTMLOpenLinkInNewTabDelegate, int32, BrowserHandle, FString, URL);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHTMLSearchResultsDelegate, int32, BrowserHandle, int32, Results, int32, CurrentMatch);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHTMLSetCursorDelegate, int32, BrowserHandle, ESteamMouseCursor, MouseCursor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHTMLShowToolTipDelegate, int32, BrowserHandle, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnHTMLStartRequestDelegate, int32, BrowserHandle, FString, URL, FString, Target, FString, PostData, bool, bIsRedirect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHTMLStatusTextDelegate, int32, BrowerHandle, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHTMLUpdateToolTipDelegate, int32, BrowserHandle, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHTMLURLChangedDelegate, int32, BrowserHandle, FString, URL, FString, PostData, bool, bIsRedirect, FString, PageTitle, bool, bNewNavigation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHTMLVerticalScrollDelegate, int32, BrowserHandle, int32, ScrollMax, int32, ScrollCurrent, float, PageScale, bool, bVisible, int32, PageSize);

/**
 * Interface for rendering and interacting with HTML pages.
 * You can use this interface to render and display HTML pages directly inside your game or application. You must call Init prior to using this interface, and Shutdown when you're done using it.
 * It is built upon the Chromium Embedded Framework and supports HTML5 markup.
 * https://partner.steamgames.com/doc/api/ISteamHTMLSurface
 */
UCLASS()
class STEAMBRIDGECORE_API USteamHTMLSurface final : public UObject
{
	GENERATED_BODY()

public:
	USteamHTMLSurface();
	~USteamHTMLSurface();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|HTMLSurface", meta = (DisplayName = "Steam HTML Surface", CompactNodeTitle = "SteamHTMLSurface"))
	static USteamHTMLSurface* GetHTMLSurface() { return USteamHTMLSurface::StaticClass()->GetDefaultObject<USteamHTMLSurface>(); }

	/**
	 * Add a header to any HTTP requests from this browser.
	 *
	 * @param int32 BrowserHandle
	 * @param const FString & Key
	 * @param const FString & Value
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void AddHeader(int32 BrowserHandle, const FString& Key, const FString& Value) { SteamHTMLSurface()->AddHeader(BrowserHandle, TCHAR_TO_UTF8(*Key), TCHAR_TO_UTF8(*Value)); }

	/**
	 * Sets whether a pending load is allowed or if it should be canceled.
	 * NOTE:You MUST call this in response to a HTML_StartRequest_t callback.
	 * You can use this feature to limit the valid pages allowed in your HTML surface.
	 *
	 * @param int32 BrowserHandle
	 * @param bool bAllowed
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void AllowStartRequest(int32 BrowserHandle, bool bAllowed) { SteamHTMLSurface()->AllowStartRequest(BrowserHandle, bAllowed); }

	/**
	 * Copy the currently selected text from the current page in an HTML surface into the local clipboard.
	 *
	 * @param int32 BrowserHandle
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void CopyToClipboard(int32 BrowserHandle) { SteamHTMLSurface()->CopyToClipboard(BrowserHandle); }

	/**
	 * Create a browser object for displaying of an HTML page.
	 * NOTE: You must have implemented callback handlers for HTML_BrowserReady_t, HTML_StartRequest_t, HTML_JSAlert_t, HTML_JSConfirm_t, and HTML_FileOpenDialog_t!
	 * NOTE: If you do not implement these callback handlers, the browser may appear to hang instead of navigating to new pages or triggering javascript popups!
	 * NOTE: You MUST call RemoveBrowser when you are done using this browser to free up the resources associated with it. Failing to do so will result in a memory leak.
	 * You will want to call SetSize and LoadURL to start using your display surface.
	 *
	 * @param const FString & UserAgent
	 * @param const FString & UserCSS
	 * @return FSteamAPICall
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|HTMLSurface")
	FSteamAPICall CreateBrowser(const FString& UserAgent, const FString& UserCSS) const { return SteamHTMLSurface()->CreateBrowser(TCHAR_TO_UTF8(*UserAgent), TCHAR_TO_UTF8(*UserCSS)); }

	// TODO: FileLoadDialogResponse

	/**
	 * Find a string in the current page of an HTML surface.
	 * This is the equivalent of "ctrl+f" in your browser of choice. It will highlight all of the matching strings.
	 * You should call StopFind when the input string has changed or you want to stop searching.
	 *
	 * @param int32 BrowserHandle
	 * @param const FString & SearchStr
	 * @param bool bCurrentlyInFind
	 * @param bool bReverse
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void Find(int32 BrowserHandle, const FString& SearchStr, bool bCurrentlyInFind, bool bReverse) { SteamHTMLSurface()->Find(BrowserHandle, TCHAR_TO_UTF8(*SearchStr), bCurrentlyInFind, bReverse); }

	/**
	 * Retrieves details about a link at a specific position on the current page in an HTML surface.
	 *
	 * @param int32 BrowserHandle
	 * @param int32 x
	 * @param int32 y
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void GetLinkAtPosition(int32 BrowserHandle, int32 x, int32 y) { SteamHTMLSurface()->GetLinkAtPosition(BrowserHandle, x, y); }

	/**
	 * Navigate back in the page history.
	 *
	 * @param int32 BrowserHandle
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void GoBack(int32 BrowserHandle) { SteamHTMLSurface()->GoBack(BrowserHandle); }

	/**
	 * Navigate forward in the page history
	 *
	 * @param int32 BrowserHandle
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void GoForward(int32 BrowserHandle) { SteamHTMLSurface()->GoForward(BrowserHandle); }

	/**
	 * Initializes the HTML Surface API.
	 * This must be called prior to using any other functions in this interface.
	 * You MUST call Shutdown when you are done using the interface to free up the resources associated with it. Failing to do so will result in a memory leak!
	 *
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	bool Init() { return SteamHTMLSurface()->Init(); }

	/**
	 * Allows you to react to a page wanting to open a javascript modal dialog notification.
	 * NOTE:You MUST call this in response to HTML_JSAlert_t and HTML_JSConfirm_t callbacks.
	 *
	 * @param int32 BrowserHandle
	 * @param bool bResult
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void JSDialogResponse(int32 BrowserHandle, bool bResult) { SteamHTMLSurface()->JSDialogResponse(BrowserHandle, bResult); }

	/**
	 * UnicodeChar is the unicode character point for this keypress (and potentially multiple chars per press)
	 *
	 * @param int32 BrowserHandle
	 * @param int32 UnicodeChar
	 * @param ESteamHTMLKeyModifiers HTMLKeyModifiers
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void KeyChar(int32 BrowserHandle, int32 UnicodeChar, ESteamHTMLKeyModifiers HTMLKeyModifiers) { SteamHTMLSurface()->KeyChar(BrowserHandle, UnicodeChar, (ISteamHTMLSurface::EHTMLKeyModifiers)HTMLKeyModifiers); }

	/**
	 * keyboard interactions, native keycode is the virtual key code value from your OS
	 *
	 * @param int32 BrowserHandle
	 * @param int32 NativeKeyCode
	 * @param ESteamHTMLKeyModifiers HTMLKeyModifiers
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void KeyDown(int32 BrowserHandle, int32 NativeKeyCode, ESteamHTMLKeyModifiers HTMLKeyModifiers) { SteamHTMLSurface()->KeyDown(BrowserHandle, NativeKeyCode, (ISteamHTMLSurface::EHTMLKeyModifiers)HTMLKeyModifiers); }

	/**
	 * Navigate to a specified URL.
	 * If you send POST data with pchPostData then the data should be formatted as: name1=value1&name2=value2.
	 * You can load any URI scheme supported by Chromium Embedded Framework including but not limited to: http://, https://, ftp://, and file:///. If no scheme is specified then http:// is used.
	 *
	 * @param int32 BrowserHandle
	 * @param const FString & URL
	 * @param const FString & PostData
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void LoadURL(int32 BrowserHandle, const FString& URL, const FString& PostData) { SteamHTMLSurface()->LoadURL(BrowserHandle, TCHAR_TO_UTF8(*URL), TCHAR_TO_UTF8(*PostData)); }

	/**
	 * Tells an HTML surface that a mouse button has been double clicked.
	 * The click will occur where the surface thinks the mouse is based on the last call to MouseMove.
	 *
	 * @param int32 BrowserHandle
	 * @param ESteamHTMLMouseButton MouseButton
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void MouseDoubleClick(int32 BrowserHandle, ESteamHTMLMouseButton MouseButton) { SteamHTMLSurface()->MouseDoubleClick(BrowserHandle, (ISteamHTMLSurface::EHTMLMouseButton)MouseButton); }

	/**
	 * Tells an HTML surface that a mouse button has been pressed.
	 * The click will occur where the surface thinks the mouse is based on the last call to MouseMove.
	 *
	 * @param int32 BrowserHandle
	 * @param ESteamHTMLMouseButton MouseButton
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void MouseDown(int32 BrowserHandle, ESteamHTMLMouseButton MouseButton) { SteamHTMLSurface()->MouseDown(BrowserHandle, (ISteamHTMLSurface::EHTMLMouseButton)MouseButton); }

	/**
	 * Tells an HTML surface where the mouse is.
	 *
	 * @param int32 BrowserHandle
	 * @param int32 x
	 * @param int32 y
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void MouseMove(int32 BrowserHandle, int32 x, int32 y) { SteamHTMLSurface()->MouseMove(BrowserHandle, x, y); }

	/**
	 * Tells an HTML surface that a mouse button has been released.
	 * The click will occur where the surface thinks the mouse is based on the last call to MouseMove.
	 *
	 * @param int32 BrowserHandle
	 * @param ESteamHTMLMouseButton MouseButton
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void MouseUp(int32 BrowserHandle, ESteamHTMLMouseButton MouseButton) { SteamHTMLSurface()->MouseUp(BrowserHandle, (ISteamHTMLSurface::EHTMLMouseButton)MouseButton); }

	/**
	 * Tells an HTML surface that the mouse wheel has moved.
	 *
	 * @param int32 BrowserHandle
	 * @param int32 Delta
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void MouseWheel(int32 BrowserHandle, int32 Delta) { SteamHTMLSurface()->MouseWheel(BrowserHandle, Delta); }

	/**
	 * Paste from the local clipboard to the current page in an HTML surface.
	 *
	 * @param int32 BrowserHandle
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void PasteFromClipboard(int32 BrowserHandle) { SteamHTMLSurface()->PasteFromClipboard(BrowserHandle); }

	/**
	 * Refreshes the current page.
	 * The reload will most likely hit the local cache instead of going over the network. This is equivalent to F5 or Ctrl+R in your browser of choice.
	 *
	 * @param int32 BrowserHandle
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void Reload(int32 BrowserHandle) { SteamHTMLSurface()->Reload(BrowserHandle); }

	/**
	 * You MUST call this when you are done with an HTML surface, freeing the resources associated with it.
	 * Failing to call this will result in a memory leak!
	 *
	 * @param int32 BrowserHandle
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void RemoveBrowser(int32 BrowserHandle) { SteamHTMLSurface()->RemoveBrowser(BrowserHandle); }

	/**
	 * Enable/disable low-resource background mode, where javascript and repaint timers are throttled, resources are more aggressively purged from memory, and audio/video elements are paused.
	 * When background mode is enabled, all HTML5 video and audio objects will execute ".pause()" and gain the property "._steam_background_paused = 1".
	 * When background mode is disabled, any video or audio objects with that property will resume with ".play()".
	 *
	 * @param int32 BrowserHandle
	 * @param bool bBackgroundMode
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void SetBackgroundMode(int32 BrowserHandle, bool bBackgroundMode) { SteamHTMLSurface()->SetBackgroundMode(BrowserHandle, bBackgroundMode); }

	/**
	 * Set a webcookie for a specific hostname.
	 *
	 * @param const FString & Hostname
	 * @param const FString & Key
	 * @param const FString & Value
	 * @param const FString & Path
	 * @param int32 Expires
	 * @param bool bSecure
	 * @param bool bHTTPOnly
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void SetCookie(const FString& Hostname, const FString& Key, const FString& Value, const FString& Path = "/", int32 Expires = 0, bool bSecure = false, bool bHTTPOnly = false);

	/**
	 * Scroll the current page horizontally.
	 *
	 * @param int32 BrowserHandle
	 * @param int32 AbsolutePixelScroll
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void SetHorizontalScroll(int32 BrowserHandle, int32 AbsolutePixelScroll) { SteamHTMLSurface()->SetHorizontalScroll(BrowserHandle, AbsolutePixelScroll); }

	/**
	 * Tell a HTML surface if it has key focus currently, controls showing the I-beam cursor in text controls amongst other things.
	 *
	 * @param int32 BrowserHandle
	 * @param bool bHasKeyFocus
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void SetKeyFocus(int32 BrowserHandle, bool bHasKeyFocus) { SteamHTMLSurface()->SetKeyFocus(BrowserHandle, bHasKeyFocus); }

	/**
	 * Zoom the current page in an HTML surface.
	 * The current scale factor is available from HTML_NeedsPaint_t.flPageScale, HTML_HorizontalScroll_t.flPageScale, and HTML_VerticalScroll_t.flPageScale.
	 *
	 * @param int32 BrowserHandle
	 * @param float Zoom
	 * @param int32 PointX
	 * @param int32 PointY
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void SetPageScaleFactor(int32 BrowserHandle, float Zoom, int32 PointX, int32 PointY) { SteamHTMLSurface()->SetPageScaleFactor(BrowserHandle, Zoom, PointX, PointY); }

	/**
	 * Sets the display size of a surface in pixels.
	 *
	 * @param int32 BrowserHandle
	 * @param int32 Width
	 * @param int32 Height
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void SetSize(int32 BrowserHandle, int32 Width, int32 Height) { SteamHTMLSurface()->SetSize(BrowserHandle, Width, Height); }

	/**
	 * Scroll the current page vertically.
	 *
	 * @param int32 BrowserHandle
	 * @param int32 AbsolutePixelScroll
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void SetVerticalScroll(int32 BrowserHandle, int32 AbsolutePixelScroll) { SteamHTMLSurface()->SetVerticalScroll(BrowserHandle, AbsolutePixelScroll); }

	/**
	 * Shutdown the ISteamHTMLSurface interface, releasing the memory and handles.
	 * You MUST call this when you are done using this interface to prevent memory and handle leaks. After calling this then all of the functions provided in this interface will fail until you call Init to reinitialize again.
	 *
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	bool Shutdown() { return SteamHTMLSurface()->Shutdown(); }

	/**
	 * Cancel a currently running find.
	 *
	 * @param int32 BrowserHandle
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void StopFind(int32 BrowserHandle) { SteamHTMLSurface()->StopFind(BrowserHandle); }

	/**
	 * Stop the load of the current HTML page.
	 *
	 * @param int32 BrowserHandle
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void StopLoad(int32 BrowserHandle) { SteamHTMLSurface()->StopLoad(BrowserHandle); }

	/**
	 * Open the current pages HTML source code in default local text editor, used for debugging.
	 *
	 * @param int32 BrowserHandle
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void ViewSource(int32 BrowserHandle) { SteamHTMLSurface()->ViewSource(BrowserHandle); }

	/** Delegates */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLBrowserReady"))
	FOnHTMLBrowserReadyDelegate m_OnHTMLBrowserReady;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLCanGoBackAndForward"))
	FOnHTMLCanGoBackAndForwardDelegate m_OnHTMLCanGoBackAndForward;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLChangedTitle"))
	FOnHTMLChangedTitleDelegate m_OnHTMLChangedTitle;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLCloseBrowser"))
	FOnHTMLCloseBrowserDelegate m_OnHTMLCloseBrowser;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLFileOpenDialog"))
	FOnHTMLFileOpenDialogDelegate m_OnHTMLFileOpenDialog;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLFinishedRequest"))
	FOnHTMLFinishedRequestDelegate m_OnHTMLFinishedRequest;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLHideToolTip"))
	FOnHTMLHideToolTipDelegate m_OnHTMLHideToolTip;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLHorizontalScroll"))
	FOnHTMLHorizontalScrollDelegate m_OnHTMLHorizontalScroll;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLJSAlert"))
	FOnHTMLJSAlertDelegate m_OnHTMLJSAlert;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLJSConfirm"))
	FOnHTMLJSConfirmDelegate m_OnHTMLJSConfirm;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLLinkAtPosition"))
	FOnHTMLLinkAtPositionDelegate m_OnHTMLLinkAtPosition;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLNeedsPaint"))
	FOnHTMLNeedsPaintDelegate m_OnHTMLNeedsPaint;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLNewWindow"))
	FOnHTMLNewWindowDelegate m_OnHTMLNewWindow;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLOpenLinkInNewTab"))
	FOnHTMLOpenLinkInNewTabDelegate m_OnHTMLOpenLinkInNewTab;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLSearchResults"))
	FOnHTMLSearchResultsDelegate m_OnHTMLSearchResults;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLSetCursor"))
	FOnHTMLSetCursorDelegate m_OnHTMLSetCursor;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLShowToolTip"))
	FOnHTMLShowToolTipDelegate m_OnHTMLShowToolTip;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLStartRequest"))
	FOnHTMLStartRequestDelegate m_OnHTMLStartRequest;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLStatusText"))
	FOnHTMLStatusTextDelegate m_OnHTMLStatusText;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLUpdateToolTip"))
	FOnHTMLUpdateToolTipDelegate m_OnHTMLUpdateToolTip;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLURLChanged"))
	FOnHTMLURLChangedDelegate m_OnHTMLURLChanged;

	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLVerticalScroll"))
	FOnHTMLVerticalScrollDelegate m_OnHTMLVerticalScroll;

protected:
private:
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLBrowserReady, HTML_BrowserReady_t, OnHTMLBrowserReadyCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLCanGoBackAndForward, HTML_CanGoBackAndForward_t, OnHTMLCanGoBackAndForwardCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLChangedTitle, HTML_ChangedTitle_t, OnHTMLChangedTitleCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLCloseBrowser, HTML_CloseBrowser_t, OnHTMLCloseBrowserCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLFileOpenDialog, HTML_FileOpenDialog_t, OnHTMLFileOpenDialogCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLFinishedRequest, HTML_FinishedRequest_t, OnHTMLFinishedRequestCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLHideToolTip, HTML_HideToolTip_t, OnHTMLHideToolTipCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLHorizontalScroll, HTML_HorizontalScroll_t, OnHTMLHorizontalScrollCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLJSAlert, HTML_JSAlert_t, OnHTMLJSAlertCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLJSConfirm, HTML_JSConfirm_t, OnHTMLJSConfirmCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLLinkAtPosition, HTML_LinkAtPosition_t, OnHTMLLinkAtPositionCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLNeedsPaint, HTML_NeedsPaint_t, OnHTMLNeedsPaintCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLNewWindow, HTML_NewWindow_t, OnHTMLNewWindowCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLOpenLinkInNewTab, HTML_OpenLinkInNewTab_t, OnHTMLOpenLinkInNewTabCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLSearchResults, HTML_SearchResults_t, OnHTMLSearchResultsCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLSetCursor, HTML_SetCursor_t, OnHTMLSetCursorCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLShowToolTip, HTML_ShowToolTip_t, OnHTMLShowToolTipCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLStartRequest, HTML_StartRequest_t, OnHTMLStartRequestCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLStatusText, HTML_StatusText_t, OnHTMLStatusTextCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLUpdateToolTip, HTML_UpdateToolTip_t, OnHTMLUpdateToolTipCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLURLChanged, HTML_URLChanged_t, OnHTMLURLChangedCallback);
	STEAM_CALLBACK_MANUAL(USteamHTMLSurface, OnHTMLVerticalScroll, HTML_VerticalScroll_t, OnHTMLVerticalScrollCallback);
};
