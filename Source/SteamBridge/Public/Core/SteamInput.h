// Copyright 2020-2022 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <UObject/NoExportTypes.h>

#include "Steam.h"
#include "SteamEnums.h"
#include "SteamStructs.h"

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
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore", meta = (DisplayName = "Steam Input", CompactNodeTitle = "SteamInput"))
	static USteamInput* GetSteamInput() { return USteamInput::StaticClass()->GetDefaultObject<USteamInput>(); }

	/**
	 * Reconfigure the controller to use the specified action set (ie "Menu", "Walk", or "Drive").
	 * This is cheap, and can be safely called repeatedly. It's often easier to repeatedly call it in your state loops, instead of trying to place it in all of your state transitions.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller you want to activate an action set for.
	 * @param FInputActionSetHandle ActionSetHandle - The handle of the action set you want to activate.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void ActivateActionSet(const FInputHandle InputHandle, const FInputActionSetHandle ActionSetHandle) { SteamInput()->ActivateActionSet(InputHandle, ActionSetHandle); }

	/**
	 * Reconfigure the controller to use the specified action set layer.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller you want to activate an action set layer for.
	 * @param FInputActionSetHandle ActionSetHandle - The handle of the action set layer you want to activate.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void ActivateActionSetLayer(const FInputHandle InputHandle, const FInputActionSetHandle ActionSetHandle) { SteamInput()->ActivateActionSetLayer(InputHandle, ActionSetHandle); }

	/**
	 * Reconfigure the controller to stop using the specified action set layer.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller you want to deactivate an action set layer for.
	 * @param FInputActionSetHandle ActionSetHandle - The handle of the action set layer you want to deactivate.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void DeactivateActionSetLayer(const FInputHandle InputHandle, const FInputActionSetHandle ActionSetHandle) { SteamInput()->DeactivateActionSetLayer(InputHandle, ActionSetHandle); }

	/**
	 * Reconfigure the controller to stop using all action set layers.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller you want to deactivate all action set layers for.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void DeactivateAllActionSetLayers(const FInputHandle InputHandle) { SteamInput()->DeactivateAllActionSetLayers(InputHandle); }

	/**
	 * Fill an array with all of the currently active action set layers for a specified controller handle.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller you want to get active action set layers for.
	 * @param TArray<int64> & Handles - This must point to a STEAM_INPUT_MAX_COUNT sized array of InputHandle_t.
	 * @return int32
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	int32 GetActiveActionSetLayers(const FInputHandle InputHandle, TArray<FInputHandle>& Handles);

	/**
	 * Lookup the handle for an Action Set. Best to do this once on startup, and store the handles for all future API calls.
	 *
	 * @param const FString & ActionSetName - The string identifier of an action set defined in the game's VDF file.
	 * @return FInputActionSetHandle - The handle of the specified action set.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	FInputActionSetHandle GetActionSetHandle(const FString& ActionSetName) const { return SteamInput()->GetActionSetHandle(TCHAR_TO_UTF8(*ActionSetName)); }

	/**
	 * Returns the current state of the supplied analog game action.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller you want to query.
	 * @param FInputActionSetHandle ActionSetHandle - The handle of the analog action you want to query.
	 * @return FSteamInputAnalogActionData - The current state of the specified analog action.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	FSteamInputAnalogActionData GetAnalogActionData(const FInputHandle InputHandle, const FInputAnalogActionHandle AnalogActionHandle) const;

	/**
	 * Get the handle of the specified Analog action.
	 * NOTE: This function does not take an action set handle parameter. That means that each action in your VDF file must have a unique string identifier. In other words, if you use an action called "up" in two -
	 * different action sets, this function will only ever return one of them and the other will be ignored.
	 *
	 * @param const FString & ActionName - The string identifier of the analog action defined in the game's VDF file.
	 * @return FInputAnalogActionHandle - The handle of the specified analog action.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	FInputAnalogActionHandle GetAnalogActionHandle(const FString& ActionName) const { return SteamInput()->GetAnalogActionHandle(TCHAR_TO_UTF8(*ActionName)); }

	/**
	 * Get the origin(s) for an analog action within an action set by filling originsOut with EInputActionOrigin handles. Use this to display the appropriate on-screen prompt for the action.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller you want to query.
	 * @param FInputActionSetHandle ActionSetHandle - The handle of the action set you want to query.
	 * @param FInputAnalogActionHandle AnalogActionHandle - The handle of the analog action you want to query.
	 * @param TArray<ESteamInputActionOrigin> & Origins - A STEAM_INPUT_MAX_ORIGINS sized array of EInputActionOrigin handles.
	 * @return int32 - The number of origins supplied in Origins.
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	int32 GetAnalogActionOrigins(const FInputHandle InputHandle, const FInputActionSetHandle ActionSetHandle, const FInputAnalogActionHandle AnalogActionHandle, TArray<ESteamInputActionOrigin>& Origins);

	/**
	 * Enumerates currently connected controllers by filling handlesOut with controller handles.
	 *
	 * @param TArray<int64> & Handles - This must point to a STEAM_INPUT_MAX_COUNT sized array of InputHandle_t.
	 * @return int32 - The number of handles written to Handles.
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	int32 GetConnectedControllers(TArray<FInputHandle>& Handles);

	/**
	 * Returns the associated controller handle for the specified emulated gamepad. Can be used with GetInputTypeForHandle to determine the controller type of a controller using Steam Input Gamepad Emulation.
	 *
	 * @param int32 Index - The index of the emulated gamepad you want to get a controller handle for.
	 * @return FInputHandle
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	FInputHandle GetControllerForGamepadIndex(const int32 Index) const { return SteamInput()->GetControllerForGamepadIndex(Index); }

	/**
	 * Get the currently active action set for the specified controller.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller you want to query.
	 * @return FInputActionSetHandle - The handle of the action set activated for the specified controller.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	FInputActionSetHandle GetCurrentActionSet(const FInputHandle InputHandle) const { return SteamInput()->GetCurrentActionSet(InputHandle); }

	/**
	 * Returns the current state of the supplied digital game action.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller you want to query.
	 * @param FInputDigitalActionHandle DigitalActionHandle - The handle of the digital action you want to query.
	 * @return FSteamInputDigitalActionData - The current state of the specified digital action.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	FSteamInputDigitalActionData GetDigitalActionData(const FInputHandle InputHandle, const FInputDigitalActionHandle DigitalActionHandle) const;

	/**
	 * Get the handle of the specified digital action.
	 * NOTE: This function does not take an action set handle parameter. That means that each action in your VDF file must have a unique string identifier. In other words, if you use an action called "up" in two -
	 * different action sets, this function will only ever return one of them and the other will be ignored.
	 *
	 * @param const FString & ActionName - 	The string identifier of the digital action defined in the game's VDF file.
	 * @return FInputDigitalActionHandle - The handle of the specified digital action.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	FInputDigitalActionHandle GetDigitalActionHandle(const FString& ActionName) const { return SteamInput()->GetDigitalActionHandle(TCHAR_TO_UTF8(*ActionName)); }

	/**
	 * Get the origin(s) for a digital action within an action set by filling originsOut with EInputActionOrigin handles. Use this to display the appropriate on-screen prompt for the action.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller you want to query.
	 * @param FInputActionSetHandle ActionSetHandle - The handle of the action set you want to query.
	 * @param FInputDigitalActionHandle DigitalActionHandle - The handle of the digital aciton you want to query.
	 * @param TArray<ESteamInputActionOrigin> & Origins - A STEAM_INPUT_MAX_ORIGINS sized array of EInputActionOrigin handles.
	 * @return int32 - The number of origins supplied in Origins.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	int32 GetDigitalActionOrigins(const FInputHandle InputHandle, const FInputActionSetHandle ActionSetHandle, const FInputDigitalActionHandle DigitalActionHandle, TArray<ESteamInputActionOrigin>& Origins);

	/**
	 * Returns the associated gamepad index for the specified controller, if emulating a gamepad.
	 *
	 * @param FInputHandle ControllerHandle - The handle of the controller you want to get a gamepad index for.
	 * @return int32
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	int32 GetGamepadIndexForController(const FInputHandle ControllerHandle) const { return SteamInput()->GetGamepadIndexForController(ControllerHandle); }

	/**
	 * Get a local path to art for on-screen glyph for a particular origin.
	 *
	 * @param ESteamInputActionOrigin Origin
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	FString GetGlyphForActionOrigin(const ESteamInputActionOrigin Origin) const { return UTF8_TO_TCHAR(SteamInput()->GetGlyphForActionOrigin((EInputActionOrigin)Origin)); }

	/**
	 * Returns the input type (device model) for the specified controller. This tells you if a given controller is a Steam controller, XBox 360 controller, PS4 controller, etc.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller whose input type (device model) you want to query
	 * @return ESteamInputType_
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	ESteamInputType_ GetInputTypeForHandle(const FInputHandle InputHandle) const { return (ESteamInputType_)SteamInput()->GetInputTypeForHandle(InputHandle); }

	/**
	 * Returns raw motion data for the specified controller.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller you want to get motion data for.
	 * @return FSteamInputMotionData
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	FSteamInputMotionData GetMotionData(const FInputHandle InputHandle) const;

	/**
	 * Returns a localized string (from Steam's language setting) for the specified origin.
	 *
	 * @param ESteamInputActionOrigin Origin
	 * @return FString
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	FString GetStringForActionOrigin(const ESteamInputActionOrigin Origin) const { return SteamInput()->GetStringForActionOrigin((EInputActionOrigin)Origin); }

	/**
	 * Must be called when starting use of the ISteamInput interface.
	 *
	 * @return bool - Always returns true.
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
	 * @param FInputHandle InputHandle - The handle of the controller to affect.
	 * @param uint8 R - The red component of the color to set (0-255).
	 * @param uint8 G - The green component of the color to set (0-255).
	 * @param uint8 B - The blue component of the color to set (0-255).
	 * @param const TArray<ESteamControllerLEDFlag_> & Flags - Bit-masked flags combined from values defined in ESteamControllerLEDFlag.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void SetLEDColorU(const FInputHandle InputHandle, const uint8 R, const uint8 G, const uint8 B, const TArray<ESteamControllerLEDFlag_>& Flags);

	/**
	 * Set the controller LED color on supported controllers.
	 * NOTE: The VSC does not support any color but white, and will interpret the RGB values as a greyscale value affecting the brightness of the Steam button LED.
	 * The DS4 responds to full color information and uses the values to set the color & brightness of the lightbar.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller to affect.
	 * @param const FLinearColor & Color
	 * @param const TArray<ESteamControllerLEDFlag_> & Flags - Bit-masked flags combined from values defined in ESteamControllerLEDFlag.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void SetLEDColor(const FInputHandle InputHandle, const FLinearColor& Color, const TArray<ESteamControllerLEDFlag_>& Flags);

	/**
	 * Invokes the Steam overlay and brings up the binding screen.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller you want to bring up the binding screen for.
	 * @return bool - true for success; false if overlay is disabled/unavailable, or the user is not in Big Picture Mode.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "SteamBridgeCore|Input")
	bool ShowBindingPanel(const FInputHandle InputHandle) const { return SteamInput()->ShowBindingPanel(InputHandle); }

	/**
	 * Must be called when ending use of the ISteamInput interface.
	 *
	 * @return bool - Always returns true.
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	bool Shutdown() { return SteamInput()->Shutdown(); }

	/**
	 * Stops the momentum of an analog action (where applicable, ie a touchpad w/ virtual trackball settings).
	 * NOTE: This will also stop all associated haptics. This is useful for situations where you want to indicate to the user that the limit of an action has been reached, such as spinning a carousel or scrolling a webpage.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller to affect.
	 * @param FInputAnalogActionHandle ActionHandle - The analog action to stop momentum for.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void StopAnalogActionMomentum(const FInputHandle InputHandle, const FInputAnalogActionHandle ActionHandle) { SteamInput()->StopAnalogActionMomentum(InputHandle, ActionHandle); }

	/**
	 * Triggers a (low-level) haptic pulse on supported controllers.
	 * Currently only the VSC supports haptic pulses.
	 * This API call will be ignored for all other controller models.
	 * The typical max value of an unsigned short is 65535, which means the longest haptic pulse you can trigger with this method has a duration of 0.065535 seconds (ie, less than 1/10th of a second).
	 * This function should be thought of as a low-level primitive meant to be repeatedly used in higher-level user functions to generate more sophisticated behavior.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller to affect.
	 * @param ESteamControllerPad_ TargetPad - Which haptic touch pad to affect.
	 * @param int32 DurationMicroSec - Duration of the pulse, in microseconds (1/1,000,000th of a second)
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void TriggerHapticPulse(const FInputHandle InputHandle, const ESteamControllerPad_ TargetPad, const int32 DurationMicroSec) { SteamInput()->TriggerHapticPulse(InputHandle, (ESteamControllerPad)TargetPad, DurationMicroSec); }

	/**
	 * Triggers a repeated haptic pulse on supported controllers.
	 * Currently only the VSC supports haptic pulses.
	 * This API call will be ignored for incompatible controller models.
	 * This is a more user-friendly function to call than TriggerHapticPulse as it can generate pulse patterns long enough to be actually noticed by the user.
	 * Changing the usDurationMicroSec and usOffMicroSec parameters will change the "texture" of the haptic pulse.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller to affect.
	 * @param ESteamControllerPad_ TargetPad - 	Which haptic touch pad to affect.
	 * @param int32 DurationMicroSec - 	Duration of the pulse, in microseconds (1/1,000,000th of a second).
	 * @param int32 OffMicroSec - Duration of the pause between pulses, in microseconds.
	 * @param int32 Repeat - Number of times to repeat the DurationMicroSec / OffMicroSec duty cycle.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void TriggerRepeatedHapticPulse(const FInputHandle InputHandle, const ESteamControllerPad_ TargetPad, const int32 DurationMicroSec, const int32 OffMicroSec, const int32 Repeat) { SteamInput()->TriggerRepeatedHapticPulse(InputHandle, (ESteamControllerPad)TargetPad, DurationMicroSec, OffMicroSec, Repeat, 0); }

	/**
	 * Trigger a vibration event on supported controllers.
	 * This API call will be ignored for incompatible controller models.
	 * This generates the traditional "rumble" vibration effect.
	 * The VSC will emulate traditional rumble using its haptics.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller to affect.
	 * @param int32 LeftSpeed - The intensity value for the left rumble motor.
	 * @param int32 RightSpeed - The intensity value for the right rumble motor.
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "SteamBridgeCore|Input")
	void TriggerVibration(const FInputHandle InputHandle, const int32 LeftSpeed, const int32 RightSpeed) { SteamInput()->TriggerVibration(InputHandle, LeftSpeed, RightSpeed); }

	/**
	 * Get an action origin that you can use in your glyph look up table or passed into GetGlyphForActionOrigin or GetStringForActionOrigin
	 *
	 * @param FInputHandle InputHandle - The handle of the controller to affect. You can use GetControllerForGamepadIndex to get this handle
	 * @param ESteamXboxOrigin Origin - This is the button you want to get the image for ex: k_EXboxOrigin_A
	 * @return ESteamInputActionOrigin
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	ESteamInputActionOrigin GetActionOriginFromXboxOrigin(const FInputHandle InputHandle, const ESteamXboxOrigin Origin) const { return (ESteamInputActionOrigin)SteamInput()->GetActionOriginFromXboxOrigin(InputHandle, (EXboxOrigin)Origin); }

	/**
	 * Get the equivalent origin for a given controller type or the closest controller type that existed in the SDK you built into your game if eDestinationInputType is k_ESteamInputType_Unknown. This action origin -
	 * can be used in your glyph look up table or passed into GetGlyphForActionOrigin or GetStringForActionOrigin
	 *
	 * @param ESteamInputType_ DestinationInputType - 	The controller type you want to translate to. Steam will pick the closest type from your SDK version if k_ESteamInputType_Unknown is used
	 * @param ESteamInputActionOrigin SourceOrigin - This is the button you want to translate
	 * @return ESteamInputActionOrigin
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	ESteamInputActionOrigin TranslateActionOrigin(const ESteamInputType_ DestinationInputType, const ESteamInputActionOrigin SourceOrigin) const { return (ESteamInputActionOrigin)SteamInput()->TranslateActionOrigin((ESteamInputType)DestinationInputType, (EInputActionOrigin)SourceOrigin); }

	/**
	 * Gets the major and minor device binding revisions for Steam Input API configurations. Major revisions are to be used when changing the number of action sets or otherwise reworking configurations to the degree -
	 * that older configurations are no longer usable. When a user's binding disagrees with the major revision of the current official configuration Steam will forcibly update the user to the new configuration.
	 * New configurations will need to be made for every controller when updating the major revision. Minor revisions are for small changes such as adding a new optional action or updating localization in the configuration.
	 * When updating the minor revision you generally can update a single configuration and check the "Use Action Block" to apply the action block changes to the other configurations.
	 *
	 * @param FInputHandle InputHandle - The handle of the controller to query.
	 * @param int32 & Major - Pointer to int that Major binding revision will be populated into
	 * @param int32 & Minor - Pointer to int that Minor binding revision will be populated into
	 * @return bool - true if a device binding was successfully found and false if the binding is still loading.
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	bool GetDeviceBindingRevision(const FInputHandle InputHandle, int32& Major, int32& Minor) const { return SteamInput()->GetDeviceBindingRevision(InputHandle, &Major, &Minor); }

	/**
	 * Get the Steam Remote Play session ID associated with a device, or 0 if there is no session associated with it. See isteamremoteplay.h for more information on Steam Remote Play sessions
	 *
	 * @param FInputHandle InputHandle - The handle of the controller to query.
	 * @return int32 - Steam Remote Play session ID
	 */
	UFUNCTION(BlueprintPure, Category = "SteamBridgeCore|Input")
	int32 GetRemotePlaySessionID(const FInputHandle InputHandle) const { return SteamInput()->GetRemotePlaySessionID(InputHandle); }

protected:
private:
};
