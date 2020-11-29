// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SteamBridgeSettings.generated.h"

/**
 * 
 */
UCLASS(config = Engine)
class STEAMBRIDGE_API USteamBridgeSettings final : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, config, Category = General)
	bool bTest;

	// #TODO Implement OSS Steam settings to remove the requirement of setting the info via text editor
};
