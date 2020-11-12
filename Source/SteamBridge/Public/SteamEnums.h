// Copyright 2020 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "SteamEnums.generated.h"

UENUM(BlueprintType)
enum class ESteamGameOverlayTypes : uint8
{
	Friends,
	Community,
	Players,
	Settings,
	OfficialGameGroup,
	Stats,
	Achievements
};

UENUM(BlueprintType)
enum class ESteamGameUserOverlayTypes : uint8
{
	SteamID,
	Chat,
	JoinTrade,
	Stats,
	Achievements,
	FriendAdd,
	FriendRemove,
	FriendRequestAccept,
	FriendRequestIgnore
};

UENUM(BlueprintType)
enum class ESteamBeginAuthSessionResult : uint8
{
	OK = 0 UMETA(DisplayName = "OK"),
	InvalidTicket = 1 UMETA(DisplayName = "InvalidTicket"),
	DuplicateRequest = 2 UMETA(DisplayName = "DuplicateRequest"),
	InvalidVersion = 3 UMETA(DisplayName = "InvalidVersion"),
	GameMismatch = 4 UMETA(DisplayName = "GameMismatch"),
	ExpiredTicket = 5 UMETA(DisplayName = "ExpiredTicket")
};

UENUM(BlueprintType)
enum class ESteamAuthSessionResponse : uint8
{
	OK = 0 UMETA(DisplayName = "OK"),
	UserNotConnectedToSteam = 1 UMETA(DisplayName = "UserNotConnectedToSteam"),
	NoLicenseOrExpired = 2 UMETA(DisplayName = "NoLicenseOrExpired"),
	VACBanned = 3 UMETA(DisplayName = "VACBanned"),
	LoggedInElseWhere = 4 UMETA(DisplayName = "LoggedInElseWhere"),
	VACCheckTimedOut = 5 UMETA(DisplayName = "VACCheckTimedOut"),
	AuthTicketCanceled = 6 UMETA(DisplayName = "AuthTicketCanceled"),
	AuthTicketInvalidAlreadyUsed = 7 UMETA(DisplayName = "AuthTicketInvalidAlreadyUsed"),
	AuthTicketInvalid = 8 UMETA(DisplayName = "AuthTicketInvalid"),
	PublisherIssuedBan = 9 UMETA(DisplayName = "PublisherIssuedBan")
};

UENUM(BlueprintType)
enum class ESteamVoiceResult : uint8
{
	OK = 0 UMETA(DisplayName = "OK"),
	NotInitialized = 1 UMETA(DisplayName = "NotInitialized"),
	NotRecording = 2 UMETA(DisplayName = "NotRecording"),
	NoData = 3 UMETA(DisplayName = "NoData"),
	BufferTooSmall = 4 UMETA(DisplayName = "BufferTooSmall"),
	DataCorrupted = 5 UMETA(DisplayName = "DataCorrupted"),
	Restricted = 6 UMETA(DisplayName = "Restricted"),
	UnsupportedCodec = 7 UMETA(DisplayName = "UnsupportedCodec"),
	ReceiverOutOfDate = 8 UMETA(DisplayName = "ReceiverOutOfDate"),
	ReceiverDidNotAnswer = 9 UMETA(DisplayName = "ReceiverDidNotAnswer")
};

UENUM(BlueprintType)
enum class ESteamUserHasLicenseForAppResult : uint8
{
	HasLicense = 0 UMETA(DisplayName = "HasLicense"),
	DoesNotHaveLicense = 1 UMETA(DisplayName = "DoesNotHaveLicense"),
	NoAuth = 2 UMETA(DisplayName = "NoAuth")
};

UENUM(BlueprintType)
enum class ESteamFailureType : uint8
{
	FlushedCallbackQueue UMETA(DisplayName = "FlushedCallbackQueue"),
	PipeFail UMETA(DisplayName = "PipeFail")
};

UENUM(BlueprintType)
enum class ESteamDenyReason : uint8
{
	Invalid = 0 UMETA(DisplayName = "Invalid"),
	InvalidVersion = 1 UMETA(DisplayName = "InvalidVersion"),
	Generic = 2 UMETA(DisplayName = "Generic"),
	NotLoggedOn = 3 UMETA(DisplayName = "NotLoggedOn"),
	NoLicense = 4 UMETA(DisplayName = "NoLicense"),
	Cheater = 5 UMETA(DisplayName = "Cheater"),
	LoggedInElseWhere = 6 UMETA(DisplayName = "LoggedInElseWhere"),
	UnknownText = 7 UMETA(DisplayName = "UnknownText"),
	IncompatibleAnticheat = 8 UMETA(DisplayName = "IncompatibleAnticheat"),
	MemoryCorruption = 9 UMETA(DisplayName = "MemoryCorruption"),
	IncompatibleSoftware = 10 UMETA(DisplayName = "IncompatibleSoftware"),
	SteamConnectionLost = 11 UMETA(DisplayName = "SteamConnectionLost"),
	SteamConnectionError = 12 UMETA(DisplayName = "SteamConnectionError"),
	SteamResponseTimedOut = 13 UMETA(DisplayName = "SteamResponseTimedOut"),
	SteamValidationStalled = 14 UMETA(DisplayName = "SteamValidationStalled"),
	SteamOwnerLeftGuestUser = 15 UMETA(DisplayName = "SteamOwnerLeftGuestUser")
};

UENUM(BlueprintType)
enum class ESteamResult : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	OK = 1 UMETA(DisplayName = "OK"),
	Fail = 2 UMETA(DisplayName = "Fail"),
	NoConnection = 3 UMETA(DisplayName = "NoConnection"),
	InvalidPassword = 5 UMETA(DisplayName = "InvalidPassword"),
	LoggedInElsewhere = 6 UMETA(DisplayName = "LoggedInElsewhere"),
	InvalidProtocolVer = 7 UMETA(DisplayName = "InvalidProtocolVer"),
	InvalidParam = 8 UMETA(DisplayName = "InvalidParam"),
	FileNotFound = 9 UMETA(DisplayName = "FileNotFound"),
	Busy = 10 UMETA(DisplayName = "Busy"),
	InvalidState = 11 UMETA(DisplayName = "InvalidState"),
	InvalidName = 12 UMETA(DisplayName = "InvalidName"),
	InvalidEmail = 13 UMETA(DisplayName = "InvalidEmail"),
	DuplicateName = 14 UMETA(DisplayName = "DuplicateName"),
	AccessDenied = 15 UMETA(DisplayName = "AccessDenied"),
	Timeout = 16 UMETA(DisplayName = "Timeout"),
	Banned = 17 UMETA(DisplayName = "Banned"),
	AccountNotFound = 18 UMETA(DisplayName = "AccountNotFound"),
	InvalidSteamID = 19 UMETA(DisplayName = "InvalidSteamID"),
	ServiceUnavailable = 20 UMETA(DisplayName = "ServiceUnavailable"),
	NotLoggedOn = 21 UMETA(DisplayName = "NotLoggedOn"),
	Pending = 22 UMETA(DisplayName = "Pending"),
	EncryptionFailure = 23 UMETA(DisplayName = "EncryptionFailure"),
	InsufficientPrivilege = 24 UMETA(DisplayName = "InsufficientPrivilege"),
	LimitExceeded = 25 UMETA(DisplayName = "LimitExceeded"),
	Revoked = 26 UMETA(DisplayName = "Revoked"),
	Expired = 27 UMETA(DisplayName = "Expired"),
	AlreadyRedeemed = 28 UMETA(DisplayName = "AlreadyRedeemed"),
	DuplicateRequest = 29 UMETA(DisplayName = "DuplicateRequest"),
	AlreadyOwned = 30 UMETA(DisplayName = "AlreadyOwned"),
	IPNotFound = 31 UMETA(DisplayName = "IPNotFound"),
	PersistFailed = 32 UMETA(DisplayName = "PersistFailed"),
	LockingFailed = 33 UMETA(DisplayName = "LockingFailed"),
	LogonSessionReplaced = 34 UMETA(DisplayName = "LogonSessionReplaced"),
	ConnectFailed = 35 UMETA(DisplayName = "ConnectFailed"),
	HandshakeFailed = 36 UMETA(DisplayName = "HandshakeFailed"),
	IOFailure = 37 UMETA(DisplayName = "IOFailure"),
	RemoteDisconnect = 38 UMETA(DisplayName = "RemoteDisconnect"),
	ShoppingCartNotFound = 39 UMETA(DisplayName = "ShoppingCartNotFound"),
	Blocked = 40 UMETA(DisplayName = "Blocked"),
	Ignored = 41 UMETA(DisplayName = "Ignored"),
	NoMatch = 42 UMETA(DisplayName = "NoMatch"),
	AccountDisabled = 43 UMETA(DisplayName = "AccountDisabled"),
	ServiceReadOnly = 44 UMETA(DisplayName = "ServiceReadOnly"),
	AccountNotFeatured = 45 UMETA(DisplayName = "AccountNotFeatured"),
	AdministratorOK = 46 UMETA(DisplayName = "AdministratorOK"),
	ContentVersion = 47 UMETA(DisplayName = "ContentVersion"),
	TryAnotherCM = 48 UMETA(DisplayName = "TryAnotherCM"),
	PasswordRequiredToKickSession = 49 UMETA(DisplayName = "PasswordRequiredToKickSession"),
	AlreadyLoggedInElsewhere = 50 UMETA(DisplayName = "AlreadyLoggedInElsewhere"),
	Suspended = 51 UMETA(DisplayName = "Suspended"),
	Cancelled = 52 UMETA(DisplayName = "Cancelled"),
	DataCorruption = 53 UMETA(DisplayName = "DataCorruption"),
	DiskFull = 54 UMETA(DisplayName = "DiskFull"),
	RemoteCallFailed = 55 UMETA(DisplayName = "RemoteCallFailed"),
	PasswordUnset = 56 UMETA(DisplayName = "PasswordUnset"),
	ExternalAccountUnlinked = 57 UMETA(DisplayName = "ExternalAccountUnlinked"),
	PSNTicketInvalid = 58 UMETA(DisplayName = "PSNTicketInvalid"),
	ExternalAccountAlreadyLinked = 59 UMETA(DisplayName = "ExternalAccountAlreadyLinked"),
	RemoteFileConflict = 60 UMETA(DisplayName = "RemoteFileConflict"),
	IllegalPassword = 61 UMETA(DisplayName = "IllegalPassword"),
	SameAsPreviousValue = 62 UMETA(DisplayName = "SameAsPreviousValue"),
	AccountLogonDenied = 63 UMETA(DisplayName = "AccountLogonDenied"),
	CannotUseOldPassword = 64 UMETA(DisplayName = "CannotUseOldPassword"),
	InvalidLoginAuthCode = 65 UMETA(DisplayName = "InvalidLoginAuthCode"),
	AccountLogonDeniedNoMail = 66 UMETA(DisplayName = "AccountLogonDeniedNoMail"),
	HardwareNotCapableOfIPT = 67 UMETA(DisplayName = "HardwareNotCapableOfIPT"),
	IPTInitError = 68 UMETA(DisplayName = "IPTInitError"),
	ParentalControlRestricted = 69 UMETA(DisplayName = "ParentalControlRestricted"),
	FacebookQueryError = 70 UMETA(DisplayName = "FacebookQueryError"),
	ExpiredLoginAuthCode = 71 UMETA(DisplayName = "ExpiredLoginAuthCode"),
	IPLoginRestrictionFailed = 72 UMETA(DisplayName = "IPLoginRestrictionFailed"),
	AccountLockedDown = 73 UMETA(DisplayName = "AccountLockedDown"),
	AccountLogonDeniedVerifiedEmailRequired = 74 UMETA(DisplayName = "AccountLogonDeniedVerifiedEmailRequired"),
	NoMatchingURL = 75 UMETA(DisplayName = "NoMatchingURL"),
	BadResponse = 76 UMETA(DisplayName = "BadResponse"),
	RequirePasswordReEntry = 77 UMETA(DisplayName = "RequirePasswordReEntry"),
	ValueOutOfRange = 78 UMETA(DisplayName = "ValueOutOfRange"),
	UnexpectedError = 79 UMETA(DisplayName = "UnexpectedError"),
	Disabled = 80 UMETA(DisplayName = "Disabled"),
	InvalidCEGSubmission = 81 UMETA(DisplayName = "InvalidCEGSubmission"),
	RestrictedDevice = 82 UMETA(DisplayName = "RestrictedDevice"),
	RegionLocked = 83 UMETA(DisplayName = "RegionLocked"),
	RateLimitExceeded = 84 UMETA(DisplayName = "RateLimitExceeded"),
	AccountLoginDeniedNeedTwoFactor = 85 UMETA(DisplayName = "AccountLoginDeniedNeedTwoFactor"),
	ItemDeleted = 86 UMETA(DisplayName = "ItemDeleted"),
	AccountLoginDeniedThrottle = 87 UMETA(DisplayName = "AccountLoginDeniedThrottle"),
	TwoFactorCodeMismatch = 88 UMETA(DisplayName = "TwoFactorCodeMismatch"),
	TwoFactorActivationCodeMismatch = 89 UMETA(DisplayName = "TwoFactorActivationCodeMismatch"),
	AccountAssociatedToMultiplePartners = 90 UMETA(DisplayName = "AccountAssociatedToMultiplePartners"),
	NotModified = 91 UMETA(DisplayName = "NotModified"),
	NoMobileDevice = 92 UMETA(DisplayName = "NoMobileDevice"),
	TimeNotSynced = 93 UMETA(DisplayName = "TimeNotSynced"),
	SmsCodeFailed = 94 UMETA(DisplayName = "SmsCodeFailed"),
	AccountLimitExceeded = 95 UMETA(DisplayName = "AccountLimitExceeded"),
	AccountActivityLimitExceeded = 96 UMETA(DisplayName = "AccountActivityLimitExceeded"),
	PhoneActivityLimitExceeded = 97 UMETA(DisplayName = "PhoneActivityLimitExceeded"),
	RefundToWallet = 98 UMETA(DisplayName = "RefundToWallet"),
	EmailSendFailure = 99 UMETA(DisplayName = "EmailSendFailure"),
	NotSettled = 100 UMETA(DisplayName = "NotSettled"),
	NeedCaptcha = 101 UMETA(DisplayName = "NeedCaptcha"),
	GSLTDenied = 102 UMETA(DisplayName = "GSLTDenied"),
	GSOwnerDenied = 103 UMETA(DisplayName = "GSOwnerDenied"),
	InvalidItemType = 104 UMETA(DisplayName = "InvalidItemType"),
	IPBanned = 105 UMETA(DisplayName = "IPBanned"),
	GSLTExpired = 106 UMETA(DisplayName = "GSLTExpired"),
	InsufficientFunds = 107 UMETA(DisplayName = "InsufficientFunds"),
	TooManyPending = 108 UMETA(DisplayName = "TooManyPending")
};

UENUM(BlueprintType)
enum class ESteamPersonaChange : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	ChangeName = 1 UMETA(DisplayName = "ChangedName"),
	ChangeStatus = 2 UMETA(DisplayName = "ChangedStatus"),
	ChangeComeOnline = 3 UMETA(DisplayName = "ComeOnline"),
	ChangeGoneOffline = 4 UMETA(DisplayName = "GoneOffline"),
	ChangeGamePlayed = 5 UMETA(DisplayName = "ChangedGame"),
	ChangeGameServer = 6 UMETA(DisplayName = "ChangeServer"),
	ChangeAvatar = 7 UMETA(DisplayName = "ChangedAvatar"),
	ChangeJoinedSource = 8 UMETA(DisplayName = "ChangedSource"),
	ChangeLeftSource = 9 UMETA(DisplayName = "LeftSource"),
	ChangeRelationshipChanged = 10 UMETA(DisplayName = "ChangedRelationship"),
	ChangeNameFirstSet = 11 UMETA(DisplayName = "ChangedFirstName"),
	ChangeFacebookInfo = 12 UMETA(DisplayName = "ChangedFacebookInfo"),
	ChangeNickname = 13 UMETA(DisplayName = "ChangedNickname"),
	ChangeSteamLevel = 14 UMETA(DisplayName = "ChangeSteamLevel"),
	ChangeErr = 15 UMETA(DisplayName = "Error")
};

UENUM(BlueprintType)
enum class ESteamDurationControlNotification : uint8
{
	None,          // just informing you about progress, no notification to show
	OneHour,       // "you've been playing for an hour"
	ThreeHours,    // "you've been playing for 3 hours; take a break"
	HalfProgress,  // "your XP / progress is half normal"
	NoProgress     // "your XP / progress is zero"
};

UENUM(BlueprintType)
enum class ESteamDurationControlProgress : uint8
{
	Full = 0,  // Full progress
	Half = 1,  // deprecated - XP or persistent rewards should be halved
	None = 2,  // deprecated - XP or persistent rewards should be stopped

	ExitSoon_3h = 3,    // allowed 3h time since 5h gap/break has elapsed, game should exit - steam will terminate the game soon
	ExitSoon_5h = 4,    // allowed 5h time in calendar day has elapsed, game should exit - steam will terminate the game soon
	ExitSoon_Night = 5  // game running after day period, game should exit - steam will terminate the game soon
};

UENUM(BlueprintType)
enum class ESteamOverlayToStoreFlag : uint8
{
	None = 0,
	AddToCart = 1,
	AddToCartAndShow = 2
};

UENUM(BlueprintType)
enum class ESteamChatEntryType : uint8
{
	Invalid = 0 UMETA(DisplayName = "Invalid"),
	ChatMessage = 1 UMETA(DisplayName = "ChatMessage"),
	Typing = 2 UMETA(DisplayName = "Typing"),
	InviteGame = 3 UMETA(DisplayName = "InviteGame"),
	Emote = 4 UMETA(DisplayName = "Emote"),
	LeftConversation = 6 UMETA(DisplayName = "LeftConversation"),
	Entered = 7 UMETA(DisplayName = "Entered"),
	WasKicked = 8 UMETA(DisplayName = "WasKicked"),
	WasBanned = 9 UMETA(DisplayName = "WasBanned"),
	Disconnected = 10 UMETA(DisplayName = "Disconnected"),
	HistoricalChat = 11 UMETA(DisplayName = "HistoricalChat"),
	LinkBlocked = 14 UMETA(DisplayName = "LinkBlocked")
};

UENUM(BlueprintType)
enum class ESteamFriendFlags : uint8
{
	Blocked = 0 UMETA(DisplayName = "Blocked"),
	FriendshipRequested = 1 UMETA(DisplayName = "FriendshipRequested"),
	Immediate = 2 UMETA(DisplayName = "Immediate"),
	ClanMember = 3 UMETA(DisplayName = "ClanMember"),
	OnGameServer = 4 UMETA(DisplayName = "OnGameServer"),
	HasPlayedWith = 5 UMETA(DisplayName = "HasPlayedWith"),
	FriendOfFriend = 6 UMETA(DisplayName = "FriendOfFriend"),
	RequestingFriendship = 7 UMETA(DisplayName = "RequestingFriendship"),
	RequestingInfo = 8 UMETA(DisplayName = "RequestingInfo"),
	Ignored = 9 UMETA(DisplayName = "Ignored"),
	IgnoredFriend = 10 UMETA(DisplayName = "IgnoredFriend"),
	Suggested = 11 UMETA(DisplayName = "Suggested"),
	ChatMember = 12 UMETA(DisplayName = "ChatMember"),
	All = 13 UMETA(DisplayName = "All")
};

UENUM(BlueprintType)
enum class ESteamPersonaState : uint8
{
	Offline = 0 UMETA(DisplayName = "Offline"),
	Online = 1 UMETA(DisplayName = "Online"),
	Busy = 2 UMETA(DisplayName = "Busy"),
	Away = 3 UMETA(DisplayName = "Away"),
	Snooze = 4 UMETA(DisplayName = "Snooze"),
	LookingToTrade = 5 UMETA(DisplayName = "LookingToTrade"),
	LookingToPlay = 6 UMETA(DisplayName = "LookingToPlay"),
	Max UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class ESteamFriendRelationship : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Blocked = 1 UMETA(DisplayName = "Blocked"),
	RequestRecipient = 2 UMETA(DisplayName = "RequestRecipient"),
	Friend = 3 UMETA(DisplayName = "Friend"),
	RequestInitiator = 4 UMETA(DisplayName = "RequestInitiator"),
	Ignored = 5 UMETA(DisplayName = "Ignored"),
	IgnoredFriend = 6 UMETA(DisplayName = "IgnoredFriend"),
	Max = 8 UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class ESteamAvatarSize : uint8
{
	Small UMETA(DisplayName = "Small (32*32)"),
	Medium UMETA(DisplayName = "Medium (64*64)"),
	Large UMETA(DisplayName = "Large (128*128)")
};

UENUM(BlueprintType)
enum class ESteamUserRestrictions : uint8
{
	Unknown = 0 UMETA(DisplayName = "Unknown"),
	AnyChat = 1 UMETA(DisplayName = "AnyChat"),
	VoiceChat = 2 UMETA(DisplayName = "VoiceChat"),
	GroupChat = 3 UMETA(DisplayName = "GroupChat"),
	Rating = 4 UMETA(DisplayName = "Rating"),
	GameInvites = 5 UMETA(DisplayName = "GameInvites"),
	Trading = 6 UMETA(DisplayName = "Trading")
};

UENUM(BlueprintType)
enum class ESteamChatRoomEnterResponse : uint8
{
	Unknown = 0 UMETA(DisplayName = "Unknown"),
	Success = 1 UMETA(DisplayName = "Success"),
	DoesntExist = 2 UMETA(DisplayName = "DoesntExist"),
	NotAllowed = 3 UMETA(DisplayName = "NotAllowed"),
	Full = 4 UMETA(DisplayName = "Full"),
	Error = 5 UMETA(DisplayName = "Error"),
	Banned = 6 UMETA(DisplayName = "Banned"),
	Limited = 7 UMETA(DisplayName = "Limited"),
	ClanDisabled = 8 UMETA(DisplayName = "ClanDisabled"),
	CommunityBan = 9 UMETA(DisplayName = "CommunityBan"),
	MemberBlockedYou = 10 UMETA(DisplayName = "MemberBlockedYou"),
	YouBlockedMember = 11 UMETA(DisplayName = "YouBlockedMember")
};

UENUM(BlueprintType)
enum class ESteamHTMLKeyModifiers : uint8
{
	None = 0,
	AltDown = 1 << 0,
	CtrlDown = 1 << 1,
	ShiftDown = 1 << 2,
};

UENUM(BlueprintType)
enum class ESteamHTMLMouseButton : uint8
{
	Left = 0,
	Right = 1,
	Middle = 2
};

UENUM(BlueprintType)
enum class ESteamMouseCursor : uint8
{
	dc_user = 0,
	dc_none = 1,
	dc_arrow = 2,
	dc_ibeam = 3,
	dc_hourglass = 4,
	dc_waitarrow = 5,
	dc_crosshair = 6,
	dc_up = 7,
	dc_sizenw = 8,
	dc_sizese = 9,
	dc_sizene = 10,
	dc_sizesw = 11,
	dc_sizew = 12,
	dc_sizee = 13,
	dc_sizen = 14,
	dc_sizes = 15,
	dc_sizewe = 16,
	dc_sizens = 17,
	dc_sizeall = 18,
	dc_no = 19,
	dc_hand = 20,
	dc_blank = 21,
	dc_middle_pan = 22,
	dc_north_pan = 23,
	dc_north_east_pan = 24,
	dc_east_pan = 25,
	dc_south_east_pan = 26,
	dc_south_pan = 27,
	dc_south_west_pan = 28,
	dc_west_pan = 29,
	dc_north_west_pan = 30,
	dc_alias = 31,
	dc_cell = 32,
	dc_colresize = 33,
	dc_copycur = 34,
	dc_verticaltext = 35,
	dc_rowresize = 36,
	dc_zoomin = 37,
	dc_zoomout = 38,
	dc_help = 39,
	dc_custom = 40,
	dc_last = 41
};

UENUM(BlueprintType)
enum class ESteamHTTPMethod : uint8
{
	Invalid = 0,
	GET = 1,
	HEAD = 2,
	POST = 3,
	PUT = 4,
	DELETE = 5,
	OPTIONS = 6,
	PATCH = 7
};

UENUM(BlueprintType)
namespace ESteamHTTPStatus
{
enum Type
{
	e_Invalid = 0,
	e_100Continue = 100,
	e_101SwitchingProtocols = 101,
	e_200OK = 200,
	e_201Created = 201,
	e_202Accepted = 202,
	e_203NonAuthoritative = 203,
	e_204NoContent = 204,
	e_205ResetContent = 205,
	e_206PartialContent = 206,
	e_300MultipleChoices = 300,
	e_301MovedPermanently = 301,
	e_302Found = 302,
	e_303SeeOther = 303,
	e_304NotModified = 304,
	e_305UseProxy = 305,
	e_307TemporaryRedirect = 307,
	e_400BadRequest = 400,
	e_401Unauthorized = 401,
	e_402PaymentRequired = 402,
	e_403Forbidden = 403,
	e_404NotFound = 404,
	e_405MethodNotAllowed = 405,
	e_406NotAcceptable = 406,
	e_407ProxyAuthRequired = 407,
	e_408RequestTimeout = 408,
	e_409Conflict = 409,
	e_410Gone = 410,
	e_411LengthRequired = 411,
	e_412PreconditionFailed = 412,
	e_413RequestEntityTooLarge = 413,
	e_414RequestURITooLong = 414,
	e_415UnsupportedMediaType = 415,
	e_416RequestedRangeNotSatisfiable = 416,
	e_417ExpectationFailed = 417,
	e_4xxUnknown = 418,
	e_429TooManyRequests = 429,
	e_500InternalServerError = 500,
	e_501NotImplemented = 501,
	e_502BadGateway = 502,
	e_503ServiceUnavailable = 503,
	e_504GatewayTimeout = 504,
	e_505HTTPVersionNotSupported = 505,
	e_5xxUnknown = 599
};
}