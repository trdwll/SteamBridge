// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#include "Core/SteamUser.h"

#include "SteamBridgeUtils.h"

USteamUser::USteamUser()
{
	OnClientGameServerDenyCallback.Register(this, &USteamUser::OnClientGameServerDeny);
	OnDurationControlCallback.Register(this, &USteamUser::OnDurationControl);
	OnEncryptedAppTicketResponseCallback.Register(this, &USteamUser::OnEncryptedAppTicketResponse);
	OnGameWebCallback.Register(this, &USteamUser::OnGameWeb);
	OnGetAuthSessionTicketResponseCallback.Register(this, &USteamUser::OnGetAuthSessionTicketResponse);
	OnIPCFailureCallback.Register(this, &USteamUser::OnIPCFailure);
	OnLicensesUpdatedCallback.Register(this, &USteamUser::OnLicensesUpdated);
	OnMicroTxnAuthorizationResponseCallback.Register(this, &USteamUser::OnMicroTxnAuthorizationResponse);
	OnSteamServerConnectFailureCallback.Register(this, &USteamUser::OnSteamServerConnectFailure);
	OnSteamServersConnectedCallback.Register(this, &USteamUser::OnSteamServersConnected);
	OnSteamServersDisconnectedCallback.Register(this, &USteamUser::OnSteamServersDisconnected);
	OnStoreAuthURLResponseCallback.Register(this, &USteamUser::OnStoreAuthURLResponse);
	OnValidateAuthTicketResponseCallback.Register(this, &USteamUser::OnValidateAuthTicketResponse);
}

USteamUser::~USteamUser()
{
	OnClientGameServerDenyCallback.Unregister();
	OnDurationControlCallback.Unregister();
	OnEncryptedAppTicketResponseCallback.Unregister();
	OnGameWebCallback.Unregister();
	OnGetAuthSessionTicketResponseCallback.Unregister();
	OnIPCFailureCallback.Unregister();
	OnLicensesUpdatedCallback.Unregister();
	OnMicroTxnAuthorizationResponseCallback.Unregister();
	OnSteamServerConnectFailureCallback.Unregister();
	OnSteamServersConnectedCallback.Unregister();
	OnSteamServersDisconnectedCallback.Unregister();
	OnStoreAuthURLResponseCallback.Unregister();
	OnValidateAuthTicketResponseCallback.Unregister();
}

void USteamUser::AdvertiseGame(FSteamID SteamID, const FString& IP, int32 Port)
{
	uint32 TmpIP;
	USteamBridgeUtils::ConvertIPStringToUint32(IP, TmpIP);
	SteamUser()->AdvertiseGame(SteamID, TmpIP, FMath::Clamp<uint16>(Port, 0, 65535));
}

ESteamVoiceResult USteamUser::DecompressVoice(const TArray<uint8>& CompressedBuffer, TArray<uint8>& UncompressedBuffer)
{
	ESteamVoiceResult result = ESteamVoiceResult::NotInitialized;
	uint16 BufferSize = 1024;
	for (int i = 0; i < 4; i++)
	{
		uint32 BytesWritten = 0;
		UncompressedBuffer.SetNum(BufferSize);
		result = (ESteamVoiceResult)SteamUser()->DecompressVoice(CompressedBuffer.GetData(), CompressedBuffer.Num(), UncompressedBuffer.GetData(), UncompressedBuffer.Num(), &BytesWritten, 48000);
		UncompressedBuffer.SetNum(BytesWritten);

		if (result == ESteamVoiceResult::OK)
		{
			return result;
		}

		BufferSize = BufferSize * 2 ^ i;
	}
	return result;
}

FHAuthTicket USteamUser::GetAuthSessionTicket(TArray<uint8>& Ticket)
{
	Ticket.SetNum(8192);
	uint32 TempCounter = 0;
	FHAuthTicket bResult = SteamUser()->GetAuthSessionTicket(Ticket.GetData(), 8192, &TempCounter);
	Ticket.SetNum(TempCounter);
	return bResult;
}

bool USteamUser::GetEncryptedAppTicket(TArray<uint8>& Ticket)
{
	uint32 TempCounter = 0;
	Ticket.SetNum(8192);
	bool bResult = SteamUser()->GetEncryptedAppTicket(Ticket.GetData(), 8192, &TempCounter);
	Ticket.SetNum(TempCounter);
	return bResult;
}

ESteamVoiceResult USteamUser::GetVoice(TArray<uint8>& VoiceData)
{
	int32 tmpData = 0;
	ESteamVoiceResult result = GetAvailableVoice(tmpData);
	if (result == ESteamVoiceResult::OK)
	{
		VoiceData.SetNum(tmpData);
		result = (ESteamVoiceResult)SteamUser()->GetVoice(true, VoiceData.GetData(), VoiceData.Num(), (uint32*)&tmpData);
		VoiceData.SetNum(tmpData);
	}
	return result;
}

void USteamUser::OnClientGameServerDeny(ClientGameServerDeny_t* pParam)
{
	FString IP = USteamBridgeUtils::ConvertIPToString(pParam->m_unGameServerIP);
	m_OnClientGameServerDeny.Broadcast(pParam->m_uAppID, IP, pParam->m_usGameServerPort, pParam->m_bSecure == 1, (ESteamDenyReason)pParam->m_uReason);
}

void USteamUser::OnDurationControl(DurationControl_t* pParam)
{
	m_OnDurationControl.Broadcast((ESteamResult)pParam->m_eResult, (int32)pParam->m_appid, pParam->m_bApplicable, pParam->m_csecsLast5h, (ESteamDurationControlProgress)pParam->m_progress,
		(ESteamDurationControlNotification)pParam->m_notification);
}

void USteamUser::OnEncryptedAppTicketResponse(EncryptedAppTicketResponse_t* pParam)
{
	m_OnEncryptedAppTicketResponse.Broadcast((ESteamResult)pParam->m_eResult);
}

void USteamUser::OnGameWeb(GameWebCallback_t* pParam)
{
	m_OnGameWeb.Broadcast(pParam->m_szURL);
}

void USteamUser::OnGetAuthSessionTicketResponse(GetAuthSessionTicketResponse_t* pParam)
{
	m_OnGetAuthSessionTicketResponse.Broadcast(pParam->m_hAuthTicket, (ESteamResult)pParam->m_eResult);
}

void USteamUser::OnIPCFailure(IPCFailure_t* pParam)
{
	m_IPCFailure.Broadcast((ESteamFailureType)pParam->m_eFailureType);
}

void USteamUser::OnLicensesUpdated(LicensesUpdated_t* pParam)
{
	m_OnLicensesUpdated.Broadcast();
}

void USteamUser::OnMicroTxnAuthorizationResponse(MicroTxnAuthorizationResponse_t* pParam)
{
	m_OnMicroTxnAuthorizationResponse.Broadcast(pParam->m_unAppID, FString::FromInt(pParam->m_ulOrderID), pParam->m_bAuthorized == 1);
}

void USteamUser::OnSteamServerConnectFailure(SteamServerConnectFailure_t* pParam)
{
	m_OnSteamServerConnectFailure.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_bStillRetrying);
}

void USteamUser::OnSteamServersConnected(SteamServersConnected_t* pParam)
{
	m_OnSteamServersConnected.Broadcast();
}

void USteamUser::OnSteamServersDisconnected(SteamServersDisconnected_t* pParam)
{
	m_OnSteamServersDisconnected.Broadcast((ESteamResult)pParam->m_eResult);
}

void USteamUser::OnStoreAuthURLResponse(StoreAuthURLResponse_t* pParam)
{
	m_OnStoreAuthURLResponse.Broadcast(pParam->m_szURL);
}

void USteamUser::OnValidateAuthTicketResponse(ValidateAuthTicketResponse_t* pParam)
{
	m_OnValidateAuthTicketResponse.Broadcast(pParam->m_SteamID.ConvertToUint64(), (ESteamAuthSessionResponse)pParam->m_eAuthSessionResponse, pParam->m_OwnerSteamID.ConvertToUint64());
}
