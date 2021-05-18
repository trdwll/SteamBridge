// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamHTMLSurface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHTMLBrowserReadyDelegate, FHHTMLBrowser, BrowserHandle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHTMLCanGoBackAndForwardDelegate, FHHTMLBrowser, BrowserHandle, bool, bCanGoBack, bool, bCanGoForward);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHTMLChangedTitleDelegate, FHHTMLBrowser, BrowserHandle, FString, Title);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHTMLCloseBrowserDelegate, FHHTMLBrowser, BrowserHandle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHTMLFileOpenDialogDelegate, FHHTMLBrowser, BrowserHandle, FString, Title, FString, InitialFileName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHTMLFinishedRequestDelegate, FHHTMLBrowser, BrowserHandle, FString, URL, FString, PageTitle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHTMLHideToolTipDelegate, FHHTMLBrowser, BrowserHandle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHTMLHorizontalScrollDelegate, FHHTMLBrowser, BrowserHandle, int32, ScrollMax, int32, ScrollCurrent, float, PageScale, bool, bVisible, int32, PageSize);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHTMLJSAlertDelegate, FHHTMLBrowser, BrowserHandle, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHTMLJSConfirmDelegate, FHHTMLBrowser, BrowserHandle, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHTMLLinkAtPositionDelegate, FHHTMLBrowser, BrowserHandle, FString, URL, bool, bInput, bool, bLiveLink);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_EightParams(FOnHTMLNeedsPaintDelegate, FHHTMLBrowser, BrowserHandle, FString, BGRA, FIntPoint, Size, FIntPoint, Update, FIntPoint, UpdateSize, FIntPoint, ScrollPosition, float, PageScale, int32, PageSerial);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHTMLNewWindowDelegate, FHHTMLBrowser, BrowserHandle, FString, URL, FIntPoint, Position, FIntPoint, Size);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHTMLOpenLinkInNewTabDelegate, FHHTMLBrowser, BrowserHandle, FString, URL);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHTMLSearchResultsDelegate, FHHTMLBrowser, BrowserHandle, int32, Results, int32, CurrentMatch);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHTMLSetCursorDelegate, FHHTMLBrowser, BrowserHandle, ESteamMouseCursor, MouseCursor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHTMLShowToolTipDelegate, FHHTMLBrowser, BrowserHandle, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnHTMLStartRequestDelegate, FHHTMLBrowser, BrowserHandle, FString, URL, FString, Target, FString, PostData, bool, bIsRedirect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHTMLStatusTextDelegate, FHHTMLBrowser, BrowerHandle, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHTMLUpdateToolTipDelegate, FHHTMLBrowser, BrowserHandle, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHTMLURLChangedDelegate, FHHTMLBrowser, BrowserHandle, FString, URL, FString, PostData, bool, bIsRedirect, FString, PageTitle, bool, bNewNavigation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHTMLVerticalScrollDelegate, FHHTMLBrowser, BrowserHandle, int32, ScrollMax, int32, ScrollCurrent, float, PageScale, bool, bVisible, int32, PageSize);

/**
 * Interface for rendering and interacting with HTML pages.
 * You can use this interface to render and display HTML pages directly inside your game or application. You must call Init prior to using this interface, and Shutdown when you're done using it.
 * It is built upon the Chromium Embedded Framework and supports HTML5 markup.
 * https://partner.steamgames.com/doc/api/ISteamHTMLSurface
 */
UCLASS()
class STEAMBRIDGE_API USteamHTMLSurface final : public UObject
{
	GENERATED_BODY()

public:
	USteamHTMLSurface();
	~USteamHTMLSurface();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam HTML Surface", CompactNodeTitle = "SteamHTMLSurface"))
	static USteamHTMLSurface* GetSteamHTMLSurface() { return USteamHTMLSurface::StaticClass()->GetDefaultObject<USteamHTMLSurface>(); }

	/**
	 * Add a header to any HTTP requests from this browser.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to add the header to.
	 * @param const FString & Key - The header name to add.
	 * @param const FString & Value - 	The header value to associate with the key.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void AddHeader(FHHTMLBrowser BrowserHandle, const FString& Key, const FString& Value) { SteamHTMLSurface()->AddHeader(BrowserHandle, TCHAR_TO_UTF8(*Key), TCHAR_TO_UTF8(*Value)); }

	/**
	 * Sets whether a pending load is allowed or if it should be canceled.
	 * NOTE:You MUST call this in response to a HTML_StartRequest_t callback.
	 * You can use this feature to limit the valid pages allowed in your HTML surface.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface that is navigating.
	 * @param bool bAllowed - Allow or deny the navigation to the current start request.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void AllowStartRequest(FHHTMLBrowser BrowserHandle, bool bAllowed) { SteamHTMLSurface()->AllowStartRequest(BrowserHandle, bAllowed); }

	/**
	 * Copy the currently selected text from the current page in an HTML surface into the local clipboard.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to copy the text from.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void CopyToClipboard(FHHTMLBrowser BrowserHandle) { SteamHTMLSurface()->CopyToClipboard(BrowserHandle); }

	/**
	 * Create a browser object for displaying of an HTML page.
	 * NOTE: You must have implemented callback handlers for HTML_BrowserReady_t, HTML_StartRequest_t, HTML_JSAlert_t, HTML_JSConfirm_t, and HTML_FileOpenDialog_t!
	 * NOTE: If you do not implement these callback handlers, the browser may appear to hang instead of navigating to new pages or triggering javascript popups!
	 * NOTE: You MUST call RemoveBrowser when you are done using this browser to free up the resources associated with it. Failing to do so will result in a memory leak.
	 * You will want to call SetSize and LoadURL to start using your display surface.
	 *
	 * @param const FString & UserAgent - Appends the string to the general user agent string of the browser, allowing you to detect your client on webservers. Use NULL if you do not require this functionality.
	 * @param const FString & UserCSS - This allows you to set a CSS style to every page displayed by this browser. Use NULL if you do not require this functionality.
	 * @return FSteamAPICall - SteamAPICall_t to be used with a HTML_BrowserReady_t call result.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|HTMLSurface")
	FSteamAPICall CreateBrowser(const FString& UserAgent, const FString& UserCSS) const { return SteamHTMLSurface()->CreateBrowser(TCHAR_TO_UTF8(*UserAgent), TCHAR_TO_UTF8(*UserCSS)); }

	/**
	 * Run a javascript script in the currently loaded page.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface that is navigating.
	 * @param const FString & Script - The javascript script to run.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void ExecuteJavascript(FHHTMLBrowser BrowserHandle, const FString& Script) { SteamHTMLSurface()->ExecuteJavascript(BrowserHandle, TCHAR_TO_UTF8(*Script)); }

	// #TODO FileLoadDialogResponse

	/**
	 * Find a string in the current page of an HTML surface.
	 * This is the equivalent of "ctrl+f" in your browser of choice. It will highlight all of the matching strings.
	 * You should call StopFind when the input string has changed or you want to stop searching.
	 * Triggers a HTML_SearchResults_t callback.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to find the string in.
	 * @param const FString & SearchStr - The string to search for.
	 * @param bool bCurrentlyInFind - Set this to true on subsequent calls to cycle through to the next matching string.
	 * @param bool bReverse - Search from the bottom up?
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void Find(FHHTMLBrowser BrowserHandle, const FString& SearchStr, bool bCurrentlyInFind, bool bReverse) { SteamHTMLSurface()->Find(BrowserHandle, TCHAR_TO_UTF8(*SearchStr), bCurrentlyInFind, bReverse); }

	/**
	 * Retrieves details about a link at a specific position on the current page in an HTML surface.
	 * Triggers a HTML_LinkAtPosition_t callback.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to get a link from.
	 * @param int32 x - The X (width) position in pixels within the surface. (0, 0) is the top left corner.
	 * @param int32 y - The Y (height) position in pixels within the surface. (0, 0) is the top left corner.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void GetLinkAtPosition(FHHTMLBrowser BrowserHandle, int32 x, int32 y) { SteamHTMLSurface()->GetLinkAtPosition(BrowserHandle, x, y); }

	/**
	 * Navigate back in the page history.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to navigate back on.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void GoBack(FHHTMLBrowser BrowserHandle) { SteamHTMLSurface()->GoBack(BrowserHandle); }

	/**
	 * Navigate forward in the page history
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to navigate forward on.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void GoForward(FHHTMLBrowser BrowserHandle) { SteamHTMLSurface()->GoForward(BrowserHandle); }

	/**
	 * Initializes the HTML Surface API.
	 * This must be called prior to using any other functions in this interface.
	 * You MUST call Shutdown when you are done using the interface to free up the resources associated with it. Failing to do so will result in a memory leak!
	 *
	 * @return bool - true if the API was successfully initialized; otherwise, false.
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	bool Init() { return SteamHTMLSurface()->Init(); }

	/**
	 * Allows you to react to a page wanting to open a javascript modal dialog notification.
	 * NOTE:You MUST call this in response to HTML_JSAlert_t and HTML_JSConfirm_t callbacks.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface that is spawning a dialog.
	 * @param bool bResult - Set this to true to simulate pressing the "OK" button, otherwise false for "Cancel".
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void JSDialogResponse(FHHTMLBrowser BrowserHandle, bool bResult) { SteamHTMLSurface()->JSDialogResponse(BrowserHandle, bResult); }

	/**
	 * UnicodeChar is the unicode character point for this keypress (and potentially multiple chars per press)
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to send the interaction to.
	 * @param int32 UnicodeChar - The unicode character point for this keypress; and potentially multiple characters per press.
	 * @param ESteamHTMLKeyModifiers HTMLKeyModifiers - This should be set to a bitmask of the modifier keys that the user is currently pressing.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void KeyChar(FHHTMLBrowser BrowserHandle, int32 UnicodeChar, ESteamHTMLKeyModifiers HTMLKeyModifiers) { SteamHTMLSurface()->KeyChar(BrowserHandle, UnicodeChar, (ISteamHTMLSurface::EHTMLKeyModifiers)HTMLKeyModifiers); }

	/**
	 * keyboard interactions, native keycode is the virtual key code value from your OS
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to send the interaction to.
	 * @param int32 NativeKeyCode - This is the virtual keycode value from the OS.
	 * @param ESteamHTMLKeyModifiers HTMLKeyModifiers - This should be set to a bitmask of the modifier keys that the user is currently pressing.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void KeyDown(FHHTMLBrowser BrowserHandle, int32 NativeKeyCode, ESteamHTMLKeyModifiers HTMLKeyModifiers) { SteamHTMLSurface()->KeyDown(BrowserHandle, NativeKeyCode, (ISteamHTMLSurface::EHTMLKeyModifiers)HTMLKeyModifiers); }

	/**
	 * keyboard interactions, native keycode is the virtual key code value from your OS
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to send the interaction to.
	 * @param int32 NativeKeyCode - This is the virtual keycode value from the OS.
	 * @param ESteamHTMLKeyModifiers HTMLKeyModifiers - This should be set to a bitmask of the modifier keys that the user is currently pressing.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void KeyUp(FHHTMLBrowser BrowserHandle, int32 NativeKeyCode, ESteamHTMLKeyModifiers HTMLKeyModifiers) { SteamHTMLSurface()->KeyUp(BrowserHandle, NativeKeyCode, (ISteamHTMLSurface::EHTMLKeyModifiers)HTMLKeyModifiers); }

	/**
	 * Navigate to a specified URL.
	 * If you send POST data with pchPostData then the data should be formatted as: name1=value1&name2=value2.
	 * You can load any URI scheme supported by Chromium Embedded Framework including but not limited to: http://, https://, ftp://, and file:///. If no scheme is specified then http:// is used.
	 * Triggers a HTML_StartRequest_t callback.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to load this URL in.
	 * @param const FString & URL - The URL to load.
	 * @param const FString & PostData - Optionally send a POST request with this data, set this to NULL to not send any data.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void LoadURL(FHHTMLBrowser BrowserHandle, const FString& URL, const FString& PostData) { SteamHTMLSurface()->LoadURL(BrowserHandle, TCHAR_TO_UTF8(*URL), TCHAR_TO_UTF8(*PostData)); }

	/**
	 * Tells an HTML surface that a mouse button has been double clicked.
	 * The click will occur where the surface thinks the mouse is based on the last call to MouseMove.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to send the interaction to.
	 * @param ESteamHTMLMouseButton MouseButton - The mouse button which was double clicked.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void MouseDoubleClick(FHHTMLBrowser BrowserHandle, ESteamHTMLMouseButton MouseButton) { SteamHTMLSurface()->MouseDoubleClick(BrowserHandle, (ISteamHTMLSurface::EHTMLMouseButton)MouseButton); }

	/**
	 * Tells an HTML surface that a mouse button has been pressed.
	 * The click will occur where the surface thinks the mouse is based on the last call to MouseMove.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to send the interaction to.
	 * @param ESteamHTMLMouseButton MouseButton - 	The mouse button which was pressed.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void MouseDown(FHHTMLBrowser BrowserHandle, ESteamHTMLMouseButton MouseButton) { SteamHTMLSurface()->MouseDown(BrowserHandle, (ISteamHTMLSurface::EHTMLMouseButton)MouseButton); }

	/**
	 * Tells an HTML surface where the mouse is.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to send the interaction to.
	 * @param int32 x - X (width) coordinate in pixels relative to the position of the HTML surface. (0, 0) is the top left.
	 * @param int32 y - Y (height) coordinate in pixels relative to the position of the HTML surface. (0, 0) is the top left.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void MouseMove(FHHTMLBrowser BrowserHandle, int32 x, int32 y) { SteamHTMLSurface()->MouseMove(BrowserHandle, x, y); }

	/**
	 * Tells an HTML surface that a mouse button has been released.
	 * The click will occur where the surface thinks the mouse is based on the last call to MouseMove.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to send the interaction to.
	 * @param ESteamHTMLMouseButton MouseButton - The mouse button which was released.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void MouseUp(FHHTMLBrowser BrowserHandle, ESteamHTMLMouseButton MouseButton) { SteamHTMLSurface()->MouseUp(BrowserHandle, (ISteamHTMLSurface::EHTMLMouseButton)MouseButton); }

	/**
	 * Tells an HTML surface that the mouse wheel has moved.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to send the interaction to.
	 * @param int32 Delta - The number of pixels to scroll.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void MouseWheel(FHHTMLBrowser BrowserHandle, int32 Delta) { SteamHTMLSurface()->MouseWheel(BrowserHandle, Delta); }

	/**
	 * Paste from the local clipboard to the current page in an HTML surface.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to paste into.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void PasteFromClipboard(FHHTMLBrowser BrowserHandle) { SteamHTMLSurface()->PasteFromClipboard(BrowserHandle); }

	/**
	 * Refreshes the current page.
	 * The reload will most likely hit the local cache instead of going over the network. This is equivalent to F5 or Ctrl+R in your browser of choice.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to reload.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void Reload(FHHTMLBrowser BrowserHandle) { SteamHTMLSurface()->Reload(BrowserHandle); }

	/**
	 * You MUST call this when you are done with an HTML surface, freeing the resources associated with it.
	 * Failing to call this will result in a memory leak!
	 *
	 * @param FHHTMLBrowser BrowserHandle - The browser handle to release.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void RemoveBrowser(FHHTMLBrowser BrowserHandle) { SteamHTMLSurface()->RemoveBrowser(BrowserHandle); }

	/**
	 * Enable/disable low-resource background mode, where javascript and repaint timers are throttled, resources are more aggressively purged from memory, and audio/video elements are paused.
	 * When background mode is enabled, all HTML5 video and audio objects will execute ".pause()" and gain the property "._steam_background_paused = 1".
	 * When background mode is disabled, any video or audio objects with that property will resume with ".play()".
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface set background mode on.
	 * @param bool bBackgroundMode - Toggle background mode on or off.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void SetBackgroundMode(FHHTMLBrowser BrowserHandle, bool bBackgroundMode) { SteamHTMLSurface()->SetBackgroundMode(BrowserHandle, bBackgroundMode); }

	/**
	 * Set a webcookie for a specific hostname.
	 *
	 * @param const FString & Hostname - The hostname of the server to set the cookie for. ('Host' attribute)
	 * @param const FString & Key - The cookie name to set.
	 * @param const FString & Value - The cookie value to set.
	 * @param FDateTime Expires - Sets the 'Expires' attribute on the cookie to the specified timestamp in a friendly format.
	 * @param const FString & Path - Sets the 'Path' attribute on the cookie. You can use this to restrict the cookie to a specific path on the domain. e.g. "/accounts"
	 * @param bool bSecure - Sets the 'Secure' attribute.
	 * @param bool bHTTPOnly - Sets the 'HttpOnly' attribute.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void SetCookie(const FString& Hostname, const FString& Key, const FString& Value, FDateTime Expires, const FString& Path = "/", bool bSecure = false, bool bHTTPOnly = false);

	/**
	 * Scroll the current page horizontally.
	 * Triggers a HTML_HorizontalScroll_t callback.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to set the horizontal scroll position.
	 * @param int32 AbsolutePixelScroll - The absolute pixel position to scroll to. 0 is the left and HTML_HorizontalScroll_t.unScrollMax is the right side.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void SetHorizontalScroll(FHHTMLBrowser BrowserHandle, int32 AbsolutePixelScroll) { SteamHTMLSurface()->SetHorizontalScroll(BrowserHandle, AbsolutePixelScroll); }

	/**
	 * Tell a HTML surface if it has key focus currently, controls showing the I-beam cursor in text controls amongst other things.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to set key focus on.
	 * @param bool bHasKeyFocus - Turn key focus on or off?
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void SetKeyFocus(FHHTMLBrowser BrowserHandle, bool bHasKeyFocus) { SteamHTMLSurface()->SetKeyFocus(BrowserHandle, bHasKeyFocus); }

	/**
	 * Zoom the current page in an HTML surface.
	 * The current scale factor is available from HTML_NeedsPaint_t.flPageScale, HTML_HorizontalScroll_t.flPageScale, and HTML_VerticalScroll_t.flPageScale.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to scale.
	 * @param float Zoom - The amount to zoom, this can range from 1 (100% and the default) to 2 (200%).
	 * @param int32 PointX - The X point in pixels to zoom around. Use 0 if you don't care.
	 * @param int32 PointY - The Y point in pixels to zoom around. Use 0 if you don't care.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void SetPageScaleFactor(FHHTMLBrowser BrowserHandle, float Zoom, int32 PointX, int32 PointY) { SteamHTMLSurface()->SetPageScaleFactor(BrowserHandle, Zoom, PointX, PointY); }

	/**
	 * Sets the display size of a surface in pixels.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to set the size of.
	 * @param int32 Width - The width of the surface in pixels.
	 * @param int32 Height - The height of the surface in pixels.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void SetSize(FHHTMLBrowser BrowserHandle, int32 Width, int32 Height) { SteamHTMLSurface()->SetSize(BrowserHandle, Width, Height); }

	/**
	 * Scroll the current page vertically.
	 * Triggers a HTML_VerticalScroll_t callback.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to set the vertical scroll position.
	 * @param int32 AbsolutePixelScroll - The absolute pixel position to scroll to. 0 is the top and HTML_VerticalScroll_t.unScrollMax is the bottom.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void SetVerticalScroll(FHHTMLBrowser BrowserHandle, int32 AbsolutePixelScroll) { SteamHTMLSurface()->SetVerticalScroll(BrowserHandle, AbsolutePixelScroll); }

	/**
	 * Shutdown the ISteamHTMLSurface interface, releasing the memory and handles.
	 * You MUST call this when you are done using this interface to prevent memory and handle leaks. After calling this then all of the functions provided in this interface will fail until you call Init to reinitialize again.
	 *
	 * @return bool - This function currently always returns true.
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	bool Shutdown() { return SteamHTMLSurface()->Shutdown(); }

	/**
	 * Cancel a currently running find.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to stop the find results.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void StopFind(FHHTMLBrowser BrowserHandle) { SteamHTMLSurface()->StopFind(BrowserHandle); }

	/**
	 * Stop the load of the current HTML page.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to stop loading.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void StopLoad(FHHTMLBrowser BrowserHandle) { SteamHTMLSurface()->StopLoad(BrowserHandle); }

	/**
	 * Open the current pages HTML source code in default local text editor, used for debugging.
	 *
	 * @param FHHTMLBrowser BrowserHandle - The handle of the surface to view its current pages source.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|HTMLSurface")
	void ViewSource(FHHTMLBrowser BrowserHandle) { SteamHTMLSurface()->ViewSource(BrowserHandle); }

	/** Delegates */

	/** A new browser was created and is ready for use. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLBrowserReady"))
	FOnHTMLBrowserReadyDelegate m_OnHTMLBrowserReady;

	/** Called when page history status has changed the ability to go backwards and forward. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLCanGoBackAndForward"))
	FOnHTMLCanGoBackAndForwardDelegate m_OnHTMLCanGoBackAndForward;

	/** Called when the current page in a browser gets a new title. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLChangedTitle"))
	FOnHTMLChangedTitleDelegate m_OnHTMLChangedTitle;

	/** Called when the browser has been requested to close due to user interaction; usually because of a javascript window.close() call. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLCloseBrowser"))
	FOnHTMLCloseBrowserDelegate m_OnHTMLCloseBrowser;

	/** Called when a browser surface has received a file open dialog from a <input type="file"> click or similar, you must call FileLoadDialogResponse with the file(s) the user selected. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLFileOpenDialog"))
	FOnHTMLFileOpenDialogDelegate m_OnHTMLFileOpenDialog;

	/** Called when a browser has finished loading a page. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLFinishedRequest"))
	FOnHTMLFinishedRequestDelegate m_OnHTMLFinishedRequest;

	/** Called when a a browser wants to hide a tooltip. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLHideToolTip"))
	FOnHTMLHideToolTipDelegate m_OnHTMLHideToolTip;

	/** Provides details on the visibility and size of the horizontal scrollbar. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLHorizontalScroll"))
	FOnHTMLHorizontalScrollDelegate m_OnHTMLHorizontalScroll;

	/** Called when the browser wants to display a Javascript alert dialog, call JSDialogResponse when the user dismisses this dialog; or right away to ignore it. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLJSAlert"))
	FOnHTMLJSAlertDelegate m_OnHTMLJSAlert;

	/** Called when the browser wants to display a Javascript confirmation dialog, call JSDialogResponse when the user dismisses this dialog; or right away to ignore it. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLJSConfirm"))
	FOnHTMLJSConfirmDelegate m_OnHTMLJSConfirm;

	/** Result of a call to GetLinkAtPosition */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLLinkAtPosition"))
	FOnHTMLLinkAtPositionDelegate m_OnHTMLLinkAtPosition;

	/** Called when a browser surface has a pending paint. This is where you get the actual image data to render to the screen. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLNeedsPaint"))
	FOnHTMLNeedsPaintDelegate m_OnHTMLNeedsPaint;

	/** A browser has created a new HTML window. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLNewWindow"))
	FOnHTMLNewWindowDelegate m_OnHTMLNewWindow;

	/** The browser has requested to load a url in a new tab. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLOpenLinkInNewTab"))
	FOnHTMLOpenLinkInNewTabDelegate m_OnHTMLOpenLinkInNewTab;

	/** Results from a search. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLSearchResults"))
	FOnHTMLSearchResultsDelegate m_OnHTMLSearchResults;

	/** Called when a browser wants to change the mouse cursor. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLSetCursor"))
	FOnHTMLSetCursorDelegate m_OnHTMLSetCursor;

	/** Called when a browser wants to display a tooltip. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLShowToolTip"))
	FOnHTMLShowToolTipDelegate m_OnHTMLShowToolTip;

	/**
	 * Called when a browser wants to navigate to a new page.
	 * NOTE: You MUST call AllowStartRequest in response to this callback!
	 */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLStartRequest"))
	FOnHTMLStartRequestDelegate m_OnHTMLStartRequest;

	/** Called when a browser wants you to display an informational message. This is most commonly used when you hover over links. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLStatusText"))
	FOnHTMLStatusTextDelegate m_OnHTMLStatusText;

	/** Called when the text of an existing tooltip has been updated. */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLUpdateToolTip"))
	FOnHTMLUpdateToolTipDelegate m_OnHTMLUpdateToolTip;

	/** Called when the browser is navigating to a new url */
	UPROPERTY(BlueprintAssignable, Category = "SteamBridgeCore|Friends", meta = (DisplayName = "OnHTMLURLChanged"))
	FOnHTMLURLChangedDelegate m_OnHTMLURLChanged;

	/** Provides details on the visibility and size of the vertical scrollbar. */
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
