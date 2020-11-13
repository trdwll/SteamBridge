// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"
#include "UObject/NoExportTypes.h"

#include "SteamInput.generated.h"

/**
 * Steam Input API is a flexible action-based API that supports all major controller types - Xbox, Playstation, Nintendo Switch Pro, and Steam Controllers.
 * https://partner.steamgames.com/doc/api/ISteamInput
 */
UCLASS()
class STEAMBRIDGE_API USteamInput final : public UObject
{
	GENERATED_BODY()

public:
	USteamInput();
	~USteamInput();

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input", meta = (DisplayName = "Steam Input", CompactNodeTitle = "SteamInput"))
	static USteamInput* GetSteamInput() { return USteamInput::StaticClass()->GetDefaultObject<USteamInput>(); }

	/**
	 * Reconfigure the controller to use the specified action set (ie "Menu", "Walk", or "Drive").
	 * This is cheap, and can be safely called repeatedly. It's often easier to repeatedly call it in your state loops, instead of trying to place it in all of your state transitions.
	 *
	 * @param int64 InputHandle
	 * @param int64 ActionSetHandle
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void ActivateActionSet(int64 InputHandle, int64 ActionSetHandle) { SteamInput()->ActivateActionSet(InputHandle, ActionSetHandle); }

	/**
	 * Reconfigure the controller to use the specified action set layer.
	 *
	 * @param int64 InputHandle
	 * @param int64 ActionSetLayerHandle
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void ActivateActionSetLayer(int64 InputHandle, int64 ActionSetLayerHandle) { SteamInput()->ActivateActionSetLayer(InputHandle, ActionSetLayerHandle); }

	/**
	 * Reconfigure the controller to stop using the specified action set layer.
	 *
	 * @param int64 InputHandle
	 * @param int64 ActionSetLayerHandle
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void DeactivateActionSetLayer(int64 InputHandle, int64 ActionSetLayerHandle) { SteamInput()->DeactivateActionSetLayer(InputHandle, ActionSetLayerHandle); }

	/**
	 * Reconfigure the controller to stop using all action set layers.
	 *
	 * @param int64 InputHandle
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void DeactivateAllActionSetLayers(int64 InputHandle) { SteamInput()->DeactivateAllActionSetLayers(InputHandle); }

	/**
	 * Fill an array with all of the currently active action set layers for a specified controller handle.
	 *
	 * @param int64 InputHandle
	 * @param TArray<int64> & Handles
	 * @return int32
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	int32 GetActiveActionSetLayers(int64 InputHandle, TArray<int64>& Handles);

	/**
	 * Lookup the handle for an Action Set. Best to do this once on startup, and store the handles for all future API calls.
	 *
	 * @param const FString & ActionSetName
	 * @return int64
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	int64 GetActionSetHandle(const FString& ActionSetName) const { return SteamInput()->GetActionSetHandle(TCHAR_TO_UTF8(*ActionSetName)); }

	/**
	 * Returns the current state of the supplied analog game action.
	 *
	 * @param int64 InputHandle
	 * @param int64 ActionSetLayerHandle
	 * @return FSteamInputAnalogActionData
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	FSteamInputAnalogActionData GetAnalogActionData(int64 InputHandle, int64 ActionSetLayerHandle) const;

	/**
	 * Get the handle of the specified Analog action.
	 * NOTE: This function does not take an action set handle parameter. That means that each action in your VDF file must have a unique string identifier. In other words, if you use an action called "up" in two -
	 * different action sets, this function will only ever return one of them and the other will be ignored.
	 *
	 * @param const FString & ActionName
	 * @return int64
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	int64 GetAnalogActionHandle(const FString& ActionName) const { return SteamInput()->GetAnalogActionHandle(TCHAR_TO_UTF8(*ActionName)); }

	/**
	 * Get the origin(s) for an analog action within an action set by filling originsOut with EInputActionOrigin handles. Use this to display the appropriate on-screen prompt for the action.
	 *
	 * @param int64 InputHandle
	 * @param int64 ActionSetHandle
	 * @param int64 AnalogActionHandle
	 * @param TArray<ESteamInputActionOrigin> & Origins
	 * @return int32
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	int32 GetAnalogActionOrigins(int64 InputHandle, int64 ActionSetHandle, int64 AnalogActionHandle, TArray<ESteamInputActionOrigin>& Origins);

	/**
	 * Enumerates currently connected controllers by filling handlesOut with controller handles.
	 *
	 * @param TArray<int64> & Handles
	 * @return int32
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	int32 GetConnectedControllers(TArray<int64>& Handles);

	/**
	 * Returns the associated controller handle for the specified emulated gamepad. Can be used with GetInputTypeForHandle to determine the controller type of a controller using Steam Input Gamepad Emulation.
	 *
	 * @param int32 Index
	 * @return int64
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	int64 GetControllerForGamepadIndex(int32 Index) const { return SteamInput()->GetControllerForGamepadIndex(Index); }

	/**
	 * Get the currently active action set for the specified controller.
	 *
	 * @param int64 InputHandle
	 * @return int64
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	int64 GetCurrentActionSet(int64 InputHandle) const { return SteamInput()->GetCurrentActionSet(InputHandle); }

	/**
	 * Returns the current state of the supplied digital game action.
	 *
	 * @param int64 InputHandle
	 * @param int64 DigitalActionHandle
	 * @return FSteamInputDigitalActionData
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	FSteamInputDigitalActionData GetDigitalActionData(int64 InputHandle, int64 DigitalActionHandle) const;

	/**
	 * Get the handle of the specified digital action.
	 * NOTE: This function does not take an action set handle parameter. That means that each action in your VDF file must have a unique string identifier. In other words, if you use an action called "up" in two -
	 * different action sets, this function will only ever return one of them and the other will be ignored.
	 *
	 * @param const FString & ActionName
	 * @return int64
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	int64 GetDigitalActionHandle(const FString& ActionName) const { return SteamInput()->GetDigitalActionHandle(TCHAR_TO_UTF8(*ActionName)); }

	/**
	 * Get the origin(s) for a digital action within an action set by filling originsOut with EInputActionOrigin handles. Use this to display the appropriate on-screen prompt for the action.
	 *
	 * @param int64 InputHandle
	 * @param int64 ActionSetHandle
	 * @param int64 DigitalActionHandle
	 * @param TArray<ESteamInputActionOrigin> & Origins
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	int32 GetDigitalActionOrigins(int64 InputHandle, int64 ActionSetHandle, int64 DigitalActionHandle, TArray<ESteamInputActionOrigin>& Origins);

	/**
	 * Returns the associated gamepad index for the specified controller, if emulating a gamepad.
	 *
	 * @param int64 ControllerHandle
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	int32 GetGamepadIndexForController(int64 ControllerHandle) const { return SteamInput()->GetGamepadIndexForController(ControllerHandle); }

	/**
	 * Get a local path to art for on-screen glyph for a particular origin.
	 *
	 * @param ESteamInputActionOrigin Origin
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	FString GetGlyphForActionOrigin(ESteamInputActionOrigin Origin) const { return UTF8_TO_TCHAR(*SteamInput()->GetGlyphForActionOrigin((EInputActionOrigin)Origin)); }

	/**
	 * Returns the input type (device model) for the specified controller. This tells you if a given controller is a Steam controller, XBox 360 controller, PS4 controller, etc.
	 *
	 * @param int64 InputHandle
	 * @return ESteamInputType_
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	ESteamInputType_ GetInputTypeForHandle(int64 InputHandle) const { return (ESteamInputType_)SteamInput()->GetInputTypeForHandle(InputHandle); }

	/**
	 * Returns raw motion data for the specified controller.
	 *
	 * @param int64 InputHandle
	 * @return FSteamInputMotionData
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	FSteamInputMotionData GetMotionData(int64 InputHandle) const;

	/**
	 * Returns a localized string (from Steam's language setting) for the specified origin.
	 *
	 * @param ESteamInputActionOrigin Origin
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	FString GetStringForActionOrigin(ESteamInputActionOrigin Origin) const { return SteamInput()->GetStringForActionOrigin((EInputActionOrigin)Origin); }

	/**
	 * Must be called when starting use of the ISteamInput interface.
	 *
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	bool Init() { return SteamInput()->Init(); }

	/**
	 * Synchronize API state with the latest Steam Controller inputs available. This is performed automatically by SteamAPI_RunCallbacks, but for the absolute lowest possible latency, you can call this -
	 * directly before reading controller state.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void RunFrame() { SteamInput()->RunFrame(); }

	/**
	 * Set the controller LED color on supported controllers.
	 * NOTE: The VSC does not support any color but white, and will interpret the RGB values as a greyscale value affecting the brightness of the Steam button LED.
	 * The DS4 responds to full color information and uses the values to set the color & brightness of the lightbar.
	 *
	 * @param int64 InputHandle
	 * @param uint8 R
	 * @param uint8 G
	 * @param uint8 B
	 * @param const TArray<ESteamControllerLEDFlag_> & Flags
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void SetLEDColorU(int64 InputHandle, uint8 R, uint8 G, uint8 B, const TArray<ESteamControllerLEDFlag_>& Flags);

	/**
	 * Set the controller LED color on supported controllers.
	 * NOTE: The VSC does not support any color but white, and will interpret the RGB values as a greyscale value affecting the brightness of the Steam button LED.
	 * The DS4 responds to full color information and uses the values to set the color & brightness of the lightbar.
	 *
	 * @param int64 InputHandle
	 * @param const FLinearColor & Color
	 * @param const TArray<ESteamControllerLEDFlag_> & Flags
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void SetLEDColor(int64 InputHandle, const FLinearColor& Color, const TArray<ESteamControllerLEDFlag_>& Flags);

	/**
	 * Invokes the Steam overlay and brings up the binding screen.
	 *
	 * @param int64 InputHandle
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	bool ShowBindingPanel(int64 InputHandle) const { return SteamInput()->ShowBindingPanel(InputHandle); }

	/**
	 * Must be called when ending use of the ISteamInput interface.
	 *
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	bool Shutdown() { return SteamInput()->Shutdown(); }

	/**
	 * Stops the momentum of an analog action (where applicable, ie a touchpad w/ virtual trackball settings).
	 * NOTE: This will also stop all associated haptics. This is useful for situations where you want to indicate to the user that the limit of an action has been reached, such as spinning a carousel or scrolling a webpage.
	 *
	 * @param int64 InputHandle
	 * @param int64 ActionHandle
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void StopAnalogActionMomentum(int64 InputHandle, int64 ActionHandle) { SteamInput()->StopAnalogActionMomentum(InputHandle, ActionHandle); }

	/**
	 * Triggers a (low-level) haptic pulse on supported controllers.
	 * Currently only the VSC supports haptic pulses.
	 * This API call will be ignored for all other controller models.
	 * The typical max value of an unsigned short is 65535, which means the longest haptic pulse you can trigger with this method has a duration of 0.065535 seconds (ie, less than 1/10th of a second).
	 * This function should be thought of as a low-level primitive meant to be repeatedly used in higher-level user functions to generate more sophisticated behavior.
	 *
	 * @param int64 InputHandle
	 * @param ESteamControllerPad_ TargetPad
	 * @param int32 DurationMicroSec
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void TriggerHapticPulse(int64 InputHandle, ESteamControllerPad_ TargetPad, int32 DurationMicroSec) { SteamInput()->TriggerHapticPulse(InputHandle, (ESteamControllerPad)TargetPad, DurationMicroSec); }

	/**
	 * Triggers a repeated haptic pulse on supported controllers.
	 * Currently only the VSC supports haptic pulses.
	 * This API call will be ignored for incompatible controller models.
	 * This is a more user-friendly function to call than TriggerHapticPulse as it can generate pulse patterns long enough to be actually noticed by the user.
	 * Changing the usDurationMicroSec and usOffMicroSec parameters will change the "texture" of the haptic pulse.
	 *
	 * @param int64 InputHandle
	 * @param ESteamControllerPad_ TargetPad
	 * @param int32 DurationMicroSec
	 * @param int32 OffMicroSec
	 * @param int32 Repeat
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void TriggerRepeatedHapticPulse(int64 InputHandle, ESteamControllerPad_ TargetPad, int32 DurationMicroSec, int32 OffMicroSec, int32 Repeat) { SteamInput()->TriggerRepeatedHapticPulse(InputHandle, (ESteamControllerPad)TargetPad, DurationMicroSec, OffMicroSec, Repeat, 0); }

	/**
	 * Trigger a vibration event on supported controllers.
	 * This API call will be ignored for incompatible controller models.
	 * This generates the traditional "rumble" vibration effect.
	 * The VSC will emulate traditional rumble using its haptics.
	 *
	 * @param int64 InputHandle
	 * @param int32 LeftSpeed
	 * @param int32 RightSpeed
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void TriggerVibration(int64 InputHandle, int32 LeftSpeed, int32 RightSpeed) { SteamInput()->TriggerVibration(InputHandle, LeftSpeed, RightSpeed); }

	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	ESteamInputActionOrigin GetActionOriginFromXboxOrigin(int64 InputHandle, ESteamXboxOrigin Origin) const { return (ESteamInputActionOrigin)SteamInput()->GetActionOriginFromXboxOrigin(InputHandle, (EXboxOrigin)Origin); }

	/**
	 * Get the equivalent origin for a given controller type or the closest controller type that existed in the SDK you built into your game if eDestinationInputType is k_ESteamInputType_Unknown. This action origin -
	 * can be used in your glyph look up table or passed into GetGlyphForActionOrigin or GetStringForActionOrigin
	 *
	 * @param ESteamInputType_ DestinationInputType
	 * @param ESteamInputActionOrigin SourceOrigin
	 * @return ESteamInputActionOrigin
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	ESteamInputActionOrigin TranslateActionOrigin(ESteamInputType_ DestinationInputType, ESteamInputActionOrigin SourceOrigin) const { return (ESteamInputActionOrigin)SteamInput()->TranslateActionOrigin((ESteamInputType)DestinationInputType, (EInputActionOrigin)SourceOrigin); }

	/**
	 * Gets the major and minor device binding revisions for Steam Input API configurations. Major revisions are to be used when changing the number of action sets or otherwise reworking configurations to the degree -
	 * that older configurations are no longer usable. When a user's binding disagrees with the major revision of the current official configuration Steam will forcibly update the user to the new configuration.
	 * New configurations will need to be made for every controller when updating the major revision. Minor revisions are for small changes such as adding a new optional action or updating localization in the configuration.
	 * When updating the minor revision you generally can update a single configuration and check the "Use Action Block" to apply the action block changes to the other configurations.
	 *
	 * @param int64 InputHandle
	 * @param int32 & Major
	 * @param int32 & Minor
	 * @return bool
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	bool GetDeviceBindingRevision(int64 InputHandle, int32& Major, int32& Minor) const { return SteamInput()->GetDeviceBindingRevision(InputHandle, &Major, &Minor); }

	/**
	 * Get the Steam Remote Play session ID associated with a device, or 0 if there is no session associated with it. See isteamremoteplay.h for more information on Steam Remote Play sessions
	 *
	 * @param int64 InputHandle
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	int32 GetRemotePlaySessionID(int64 InputHandle) const { return SteamInput()->GetRemotePlaySessionID(InputHandle); }

protected:
private:
};
