// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "SteamGameServer.h"

#include "SteamBridgeUtils.h"

USteamGameServer::USteamGameServer()
{
	OnAssociateWithClanResultCallback.Register(this, &USteamGameServer::OnAssociateWithClanResult);
	OnComputeNewPlayerCompatibilityResultCallback.Register(this, &USteamGameServer::OnComputeNewPlayerCompatibilityResult);
	OnGSClientApproveCallback.Register(this, &USteamGameServer::OnGSClientApprove);
	OnGSClientDenyCallback.Register(this, &USteamGameServer::OnGSClientDeny);
	OnGSClientGroupStatusCallback.Register(this, &USteamGameServer::OnGSClientGroupStatus);
	OnGSClientKickCallback.Register(this, &USteamGameServer::OnGSClientKick);
	OnGSPolicyResponseCallback.Register(this, &USteamGameServer::OnGSPolicyResponse);
}

USteamGameServer::~USteamGameServer()
{
	OnAssociateWithClanResultCallback.Unregister();
	OnComputeNewPlayerCompatibilityResultCallback.Unregister();
	OnGSClientApproveCallback.Unregister();
	OnGSClientDenyCallback.Unregister();
	OnGSClientGroupStatusCallback.Unregister();
	OnGSClientKickCallback.Unregister();
	OnGSPolicyResponseCallback.Unregister();
}

ESteamBeginAuthSessionResult USteamGameServer::BeginAuthSession(TArray<uint8> AuthTicket, FSteamID SteamID)
{
	AuthTicket.SetNum(8192);
	return (ESteamBeginAuthSessionResult)SteamGameServer()->BeginAuthSession(AuthTicket.GetData(), 8192, SteamID.Value);
}

FHAuthTicket USteamGameServer::GetAuthSessionTicket(TArray<uint8> &AuthTicket)
{
	uint32 length = 0;
	AuthTicket.SetNum(8192);
	FHAuthTicket result = (FHAuthTicket)SteamGameServer()->GetAuthSessionTicket(AuthTicket.GetData(), 8192, &length);
	AuthTicket.SetNum(length);
	return result;
}

void USteamGameServer::OnAssociateWithClanResult(AssociateWithClanResult_t *pParam)
{
	m_OnAssociateWithClanResult.Broadcast((ESteamResult)pParam->m_eResult);
}

void USteamGameServer::OnComputeNewPlayerCompatibilityResult(ComputeNewPlayerCompatibilityResult_t *pParam)
{
	m_OnComputeNewPlayerCompatibilityResult.Broadcast((ESteamResult)pParam->m_eResult, pParam->m_cPlayersThatDontLikeCandidate, pParam->m_cPlayersThatCandidateDoesntLike,
		pParam->m_cClanPlayersThatDontLikeCandidate, pParam->m_SteamIDCandidate.ConvertToUint64());
}

void USteamGameServer::OnGSClientApprove(GSClientApprove_t *pParam)
{
	m_OnGSClientApprove.Broadcast(pParam->m_SteamID.ConvertToUint64(), pParam->m_OwnerSteamID.ConvertToUint64());
}

void USteamGameServer::OnGSClientDeny(GSClientDeny_t *pParam)
{
	m_OnGSClientDeny.Broadcast(pParam->m_SteamID.ConvertToUint64(), (ESteamDenyReason)pParam->m_eDenyReason, UTF8_TO_TCHAR(pParam->m_rgchOptionalText));
}

void USteamGameServer::OnGSClientGroupStatus(GSClientGroupStatus_t *pParam)
{
	m_OnGSClientGroupStatus.Broadcast(pParam->m_SteamIDUser.ConvertToUint64(), pParam->m_SteamIDGroup.ConvertToUint64(), pParam->m_bMember, pParam->m_bOfficer);
}

void USteamGameServer::OnGSClientKick(GSClientKick_t *pParam)
{
	m_OnGSClientKick.Broadcast(pParam->m_SteamID.ConvertToUint64(), (ESteamDenyReason)pParam->m_eDenyReason);
}

void USteamGameServer::OnGSPolicyResponse(GSPolicyResponse_t *pParam)
{
	m_OnGSPolicyResponse.Broadcast(pParam->m_bSecure == 1);
}
