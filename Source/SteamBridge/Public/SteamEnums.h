// Copyright 2020-2021 Russ 'trdwll' Treadwell <trdwll.com>. All Rights Reserved.

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
	None = 0,
	Unknown = 1 UMETA(DisplayName = "Unknown"),
	AnyChat = 2 UMETA(DisplayName = "AnyChat"),
	VoiceChat = 4 UMETA(DisplayName = "VoiceChat"),
	GroupChat = 8 UMETA(DisplayName = "GroupChat"),
	Rating = 16 UMETA(DisplayName = "Rating"),
	GameInvites = 32 UMETA(DisplayName = "GameInvites"),
	Trading = 64 UMETA(DisplayName = "Trading")
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

UENUM(BlueprintType)
enum class ESteamControllerSourceMode : uint8
{
	None = 0,
	Dpad = 1,
	Buttons = 2,
	FourButtons = 3,
	AbsoluteMouse = 4,
	RelativeMouse = 5,
	JoystickMove = 6,
	JoystickMouse = 7,
	JoystickCamera = 8,
	ScrollWheel = 9,
	Trigger = 10,
	TouchMenu = 11,
	MouseJoystick = 12,
	MouseRegion = 13,
	RadialMenu = 14,
	SingleButton = 15,
	Switches = 16
};

UENUM(BlueprintType)
enum class ESteamInputActionOrigin : uint8
{
	None,
	A,
	B,
	X,
	Y,
	LeftBumper,
	RightBumper,
	LeftGrip,
	RightGrip,
	Start,
	Back,
	LeftPad_Touch,
	LeftPad_Swipe,
	LeftPad_Click,
	LeftPad_DPadNorth,
	LeftPad_DPadSouth,
	LeftPad_DPadWest,
	LeftPad_DPadEast,
	RightPad_Touch,
	RightPad_Swipe,
	RightPad_Click,
	RightPad_DPadNorth,
	RightPad_DPadSouth,
	RightPad_DPadWest,
	RightPad_DPadEast,
	LeftTrigger_Pull,
	LeftTrigger_Click,
	RightTrigger_Pull,
	RightTrigger_Click,
	LeftStick_Move,
	LeftStick_Click,
	LeftStick_DPadNorth,
	LeftStick_DPadSouth,
	LeftStick_DPadWest,
	LeftStick_DPadEast,
	Gyro_Move,
	Gyro_Pitch,
	Gyro_Yaw,
	Gyro_Roll,
	SteamController_Reserved0,
	SteamController_Reserved1,
	SteamController_Reserved2,
	SteamController_Reserved3,
	SteamController_Reserved4,
	SteamController_Reserved5,
	SteamController_Reserved6,
	SteamController_Reserved7,
	SteamController_Reserved8,
	SteamController_Reserved9,
	SteamController_Reserved10,
	PS4_X,
	PS4_Circle,
	PS4_Triangle,
	PS4_Square,
	PS4_LeftBumper,
	PS4_RightBumper,
	PS4_Options,
	PS4_Share,
	PS4_LeftPad_Touch,
	PS4_LeftPad_Swipe,
	PS4_LeftPad_Click,
	PS4_LeftPad_DPadNorth,
	PS4_LeftPad_DPadSouth,
	PS4_LeftPad_DPadWest,
	PS4_LeftPad_DPadEast,
	PS4_RightPad_Touch,
	PS4_RightPad_Swipe,
	PS4_RightPad_Click,
	PS4_RightPad_DPadNorth,
	PS4_RightPad_DPadSouth,
	PS4_RightPad_DPadWest,
	PS4_RightPad_DPadEast,
	PS4_CenterPad_Touch,
	PS4_CenterPad_Swipe,
	PS4_CenterPad_Click,
	PS4_CenterPad_DPadNorth,
	PS4_CenterPad_DPadSouth,
	PS4_CenterPad_DPadWest,
	PS4_CenterPad_DPadEast,
	PS4_LeftTrigger_Pull,
	PS4_LeftTrigger_Click,
	PS4_RightTrigger_Pull,
	PS4_RightTrigger_Click,
	PS4_LeftStick_Move,
	PS4_LeftStick_Click,
	PS4_LeftStick_DPadNorth,
	PS4_LeftStick_DPadSouth,
	PS4_LeftStick_DPadWest,
	PS4_LeftStick_DPadEast,
	PS4_RightStick_Move,
	PS4_RightStick_Click,
	PS4_RightStick_DPadNorth,
	PS4_RightStick_DPadSouth,
	PS4_RightStick_DPadWest,
	PS4_RightStick_DPadEast,
	PS4_DPad_North,
	PS4_DPad_South,
	PS4_DPad_West,
	PS4_DPad_East,
	PS4_Gyro_Move,
	PS4_Gyro_Pitch,
	PS4_Gyro_Yaw,
	PS4_Gyro_Roll,
	PS4_Reserved0,
	PS4_Reserved1,
	PS4_Reserved2,
	PS4_Reserved3,
	PS4_Reserved4,
	PS4_Reserved5,
	PS4_Reserved6,
	PS4_Reserved7,
	PS4_Reserved8,
	PS4_Reserved9,
	PS4_Reserved10,
	XBoxOne_A,
	XBoxOne_B,
	XBoxOne_X,
	XBoxOne_Y,
	XBoxOne_LeftBumper,
	XBoxOne_RightBumper,
	XBoxOne_Menu,
	XBoxOne_View,
	XBoxOne_LeftTrigger_Pull,
	XBoxOne_LeftTrigger_Click,
	XBoxOne_RightTrigger_Pull,
	XBoxOne_RightTrigger_Click,
	XBoxOne_LeftStick_Move,
	XBoxOne_LeftStick_Click,
	XBoxOne_LeftStick_DPadNorth,
	XBoxOne_LeftStick_DPadSouth,
	XBoxOne_LeftStick_DPadWest,
	XBoxOne_LeftStick_DPadEast,
	XBoxOne_RightStick_Move,
	XBoxOne_RightStick_Click,
	XBoxOne_RightStick_DPadNorth,
	XBoxOne_RightStick_DPadSouth,
	XBoxOne_RightStick_DPadWest,
	XBoxOne_RightStick_DPadEast,
	XBoxOne_DPad_North,
	XBoxOne_DPad_South,
	XBoxOne_DPad_West,
	XBoxOne_DPad_East,
	XBoxOne_Reserved0,
	XBoxOne_Reserved1,
	XBoxOne_Reserved2,
	XBoxOne_Reserved3,
	XBoxOne_Reserved4,
	XBoxOne_Reserved5,
	XBoxOne_Reserved6,
	XBoxOne_Reserved7,
	XBoxOne_Reserved8,
	XBoxOne_Reserved9,
	XBoxOne_Reserved10,
	XBox360_A,
	XBox360_B,
	XBox360_X,
	XBox360_Y,
	XBox360_LeftBumper,
	XBox360_RightBumper,
	XBox360_Start,
	XBox360_Back,
	XBox360_LeftTrigger_Pull,
	XBox360_LeftTrigger_Click,
	XBox360_RightTrigger_Pull,
	XBox360_RightTrigger_Click,
	XBox360_LeftStick_Move,
	XBox360_LeftStick_Click,
	XBox360_LeftStick_DPadNorth,
	XBox360_LeftStick_DPadSouth,
	XBox360_LeftStick_DPadWest,
	XBox360_LeftStick_DPadEast,
	XBox360_RightStick_Move,
	XBox360_RightStick_Click,
	XBox360_RightStick_DPadNorth,
	XBox360_RightStick_DPadSouth,
	XBox360_RightStick_DPadWest,
	XBox360_RightStick_DPadEast,
	XBox360_DPad_North,
	XBox360_DPad_South,
	XBox360_DPad_West,
	XBox360_DPad_East,
	XBox360_Reserved0,
	XBox360_Reserved1,
	XBox360_Reserved2,
	XBox360_Reserved3,
	XBox360_Reserved4,
	XBox360_Reserved5,
	XBox360_Reserved6,
	XBox360_Reserved7,
	XBox360_Reserved8,
	XBox360_Reserved9,
	XBox360_Reserved10,
	Switch_A,
	Switch_B,
	Switch_X,
	Switch_Y,
	Switch_LeftBumper,
	Switch_RightBumper,
	Switch_Plus,
	Switch_Minus,
	Switch_Capture,
	Switch_LeftTrigger_Pull,
	Switch_LeftTrigger_Click,
	Switch_RightTrigger_Pull,
	Switch_RightTrigger_Click,
	Switch_LeftStick_Move,
	Switch_LeftStick_Click,
	Switch_LeftStick_DPadNorth,
	Switch_LeftStick_DPadSouth,
	Switch_LeftStick_DPadWest,
	Switch_LeftStick_DPadEast,
	Switch_RightStick_Move,
	Switch_RightStick_Click,
	Switch_RightStick_DPadNorth,
	Switch_RightStick_DPadSouth,
	Switch_RightStick_DPadWest,
	Switch_RightStick_DPadEast,
	Switch_DPad_North,
	Switch_DPad_South,
	Switch_DPad_West,
	Switch_DPad_East,
	SwitchProGyro_Move,
	SwitchProGyro_Pitch,
	SwitchProGyro_Yaw,
	SwitchProGyro_Roll,
	Switch_Reserved0,
	Switch_Reserved1,
	Switch_Reserved2,
	Switch_Reserved3,
	Switch_Reserved4,
	Switch_Reserved5,
	Switch_Reserved6,
	Switch_Reserved7,
	Switch_Reserved8,
	Switch_Reserved9,
	Switch_Reserved10,
	PS5_X,
	PS5_Circle,
	PS5_Triangle,
	PS5_Square,
	PS5_LeftBumper,
	PS5_RightBumper,
	PS5_Option,	//Start
	PS5_Create,		//Back
	PS5_Mute,
	PS5_LeftPad_Touch,
	PS5_LeftPad_Swipe,
	PS5_LeftPad_Click,
	PS5_LeftPad_DPadNorth,
	PS5_LeftPad_DPadSouth,
	PS5_LeftPad_DPadWest,
	PS5_LeftPad_DPadEast,
	PS5_RightPad_Touch,
	PS5_RightPad_Swipe,
	PS5_RightPad_Click,
	PS5_RightPad_DPadNorth,
	PS5_RightPad_DPadSouth,
	PS5_RightPad_DPadWest,
	PS5_RightPad_DPadEast,
	PS5_CenterPad_Touch,
	PS5_CenterPad_Swipe,
	PS5_CenterPad_Click,
	PS5_CenterPad_DPadNorth,
	PS5_CenterPad_DPadSouth,
	PS5_CenterPad_DPadWest,
	PS5_CenterPad_DPadEast,
	PS5_LeftTrigger_Pull,
	PS5_LeftTrigger_Click,
	PS5_RightTrigger_Pull,
	PS5_RightTrigger_Click,
	PS5_LeftStick_Move,
	PS5_LeftStick_Click,
	PS5_LeftStick_DPadNorth,
	PS5_LeftStick_DPadSouth,
	PS5_LeftStick_DPadWest,
	PS5_LeftStick_DPadEast,
	PS5_RightStick_Move,
	PS5_RightStick_Click,
	PS5_RightStick_DPadNorth,
	PS5_RightStick_DPadSouth,
	PS5_RightStick_DPadWest,
	PS5_RightStick_DPadEast,
	PS5_DPad_North,
	PS5_DPad_South,
	PS5_DPad_West,
	PS5_DPad_East,
	PS5_Gyro_Move,
	PS5_Gyro_Pitch,
	PS5_Gyro_Yaw,
	PS5_Gyro_Roll,
	PS5_DPad_Move,
	PS5_Reserved1,
	PS5_Reserved2,
	PS5_Reserved3,
	PS5_Reserved4,
	PS5_Reserved5,
	PS5_Reserved6,
	PS5_Reserved7,
	PS5_Reserved8,
	PS5_Reserved9,
	PS5_Reserved10,
	PS5_Reserved11,
	PS5_Reserved12,
	PS5_Reserved13,
	PS5_Reserved14,
	PS5_Reserved15,
	PS5_Reserved16,
	PS5_Reserved17,
	PS5_Reserved18,
	PS5_Reserved19,
	PS5_Reserved20,
	Count
};

// Added the _ since Steam actually has this enum already and we need it accessible in BP
UENUM(BlueprintType)
enum class ESteamInputType_ : uint8
{
	Unknown = 0,
	SteamController = 1,
	XBox360Controller = 2,
	XBoxOneController = 3,
	GenericXInput = 4,
	PS4Controller = 5,
	AppleMFiController = 6,
	AndroidController = 7,
	SwitchJoyConPair = 8,
	SwitchJoyConSingle = 9,
	SwitchProController = 10,
	MobileTouch = 11,
	PS3Controller = 12,
	PS5Controller = 13,
	Count = 14,
};

// Added the _ since Steam actually has this enum already and we need it accessible in BP
UENUM(BlueprintType)
enum class ESteamControllerLEDFlag_ : uint8
{
	SetColor = 0,
	RestoreUserDefault = 1
};

// Added the _ since Steam actually has this enum already and we need it accessible in BP
UENUM(BlueprintType)
enum class ESteamControllerPad_ : uint8
{
	Left = 0,
	Right = 1
};

UENUM(BlueprintType)
enum class ESteamXboxOrigin : uint8
{
	A,
	B,
	X,
	Y,
	LeftBumper,
	RightBumper,
	Menu,  //Start
	View,  //Back
	LeftTrigger_Pull,
	LeftTrigger_Click,
	RightTrigger_Pull,
	RightTrigger_Click,
	LeftStick_Move,
	LeftStick_Click,
	LeftStick_DPadNorth,
	LeftStick_DPadSouth,
	LeftStick_DPadWest,
	LeftStick_DPadEast,
	RightStick_Move,
	RightStick_Click,
	RightStick_DPadNorth,
	RightStick_DPadSouth,
	RightStick_DPadWest,
	RightStick_DPadEast,
	DPad_North,
	DPad_South,
	DPad_West,
	DPad_East,
	Count,
};

// Added the _ since Steam actually has this enum already and we need it accessible in BP
UENUM(BlueprintType)
enum class ESteamItemFlags_ : uint8
{
	NoTrade = 0,
	ItemRemoved = 8,
	ItemConsumed = 9,
};

UENUM(BlueprintType)
enum class ESteamFavoriteFlags : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Favorite = 1 UMETA(DisplayName = "Favorite"),
	History = 2 UMETA(DisplayName = "History"),
};

UENUM(BlueprintType)
enum class ESteamLobbyDistanceFilter : uint8
{
	Close = 0,
	Default = 1,
	Far = 2,
	Worldwide = 3
};

UENUM(BlueprintType)
enum class ESteamLobbyComparison : uint8
{
	EqualToOrLessThan = 0,
	LessThan = 1,
	Equal = 2,
	GreaterThan = 3,
	EqualToOrGreaterThan = 4,
	NotEqual = 5
};

UENUM(BlueprintType)
enum class ESteamLobbyType : uint8
{
	Private = 0 UMETA(DisplayName = "Private"),
	FriendsOnly = 1 UMETA(DisplayName = "FriendsOnly"),
	Public = 2 UMETA(DisplayName = "Public"),
	Invisible = 3 UMETA(DisplayName = "Invisible"),
};

UENUM(BlueprintType)
enum class ESteamChatMemberStateChange : uint8
{
	None = 0,
	Entered = 1 UMETA(DisplayName = "Entered"),
	Left = 2 UMETA(DisplayName = "Left"),
	Disconnected = 4 UMETA(DisplayName = "Disconnected"),
	Kicked = 8 UMETA(DisplayName = "Kicked"),
	Banned = 16 UMETA(DisplayName = "Banned")
};

UENUM(BlueprintType)
enum class ESteamAudioPlaybackStatus : uint8
{
	Undefined = 0,
	Playing = 1,
	Paused = 2,
	Idle = 3
};

UENUM(BlueprintType)
enum class ESteamPartyBeaconLocation : uint8
{
	Invalid = 0,
	ChatGroup = 1,
	Max
};

// Added the _ since Steam actually has this enum already and we need it accessible in BP
UENUM(BlueprintType)
enum class ESteamPartyBeaconLocationData_ : uint8
{
	Invalid = 0,
	Name = 1,
	IconURLSmall = 2,
	IconURLMedium = 3,
	IconURLLarge = 4,
};

// Added the _ since Steam actually has this enum already and we need it accessible in BP
UENUM(BlueprintType)
enum class ESteamDeviceFormFactor_ : uint8
{
	Unknown = 0,
	Phone = 1,
	Tablet = 2,
	Computer = 3,
	TV = 4,
};

UENUM(BlueprintType)
enum class ESteamAPICallFailure_ : uint8
{
	None = 0,                // no failure
	SteamGone = 1,           // the local Steam process has gone away
	NetworkFailure = 2,      // the network connection to Steam has been broken, or was already broken
	InvalidHandle = 3,       // the SteamAPICall_t handle passed in no longer exists
	MismatchedCallback = 4,  // GetAPICallResult() was called with the wrong callback type for this API call
};

UENUM(BlueprintType)
enum class ESteamUniverse : uint8
{
	Invalid = 0,
	Public = 1,
	Beta = 2,
	Internal = 3,
	Dev = 4,
	Max
};

UENUM(BlueprintType)
enum class ESteamTextFilteringContext : uint8
{
	Unknown = 0,
	GameContent = 1,
	Chat = 2,
	Name = 3
};

UENUM(BlueprintType)
enum class ESteamNotificationPosition : uint8
{
	TopLeft = 0,
	TopRight = 1,
	BottomLeft = 2,
	BottomRight = 3,
};

UENUM(BlueprintType)
enum class ESteamGamepadTextInputMode : uint8
{
	Normal = 0,
	Password = 1
};

UENUM(BlueprintType)
enum class ESteamGamepadTextInputLineMode : uint8
{
	SingleLine = 0,
	MultipleLines = 1
};

UENUM(BlueprintType)
enum class ESteamLeaderboardDataRequest : uint8
{
	Global = 0,
	GlobalAroundUser = 1,
	Friends = 2,
	Users = 3
};

// the sort order of a leaderboard
UENUM(BlueprintType)
enum class ESteamLeaderboardSortMethod : uint8
{
	None = 0,
	Ascending = 1,   // top-score is lowest number
	Descending = 2,  // top-score is highest number
};

// the display type (used by the Steam Community web site) for a leaderboard
UENUM(BlueprintType)
enum class ESteamLeaderboardDisplayType : uint8
{
	None = 0,
	Numeric = 1,           // simple numerical score
	TimeSeconds = 2,       // the score represents a time, in seconds
	TimeMilliSeconds = 3,  // the score represents a time, in milliseconds
};

UENUM(BlueprintType)
enum class ESteamLeaderboardUploadScoreMethod : uint8
{
	None = 0,
	KeepBest = 1,     // Leaderboard will keep user's best score
	ForceUpdate = 2,  // Leaderboard will always replace score with specified
};

UENUM(BlueprintType)
enum class ESteamRemoteStoragePlatform : uint8
{
	None = 0,
	Windows = (1 << 0) UMETA(DisplayName = "Windows"),
	OSX = (1 << 1) UMETA(DisplayName = "OSX"),
	PS3 = (1 << 2) UMETA(DisplayName = "PS3"),
	Linux = (1 << 3) UMETA(DisplayName = "Linux"),
	Reserved = (1 << 4) UMETA(DisplayName = "Reserved"),
	All = 254 UMETA(DisplayName = "All")
};

UENUM(BlueprintType)
enum class ESteamVRScreenshotType : uint8
{
	None = 0,
	Mono = 1,
	Stereo = 2,
	MonoCubemap = 3,
	MonoPanorama = 4,
	StereoPanorama = 5
};

UENUM(BlueprintType)
enum class ESteamItemPreviewType : uint8
{
	Image = 0,	// standard image file expected (e.g. jpg, png, gif, etc.)
	YouTubeVideo = 1,	// video id is stored
	Sketchfab = 2,	// model id is stored
	EnvironmentMap_HorizontalCross = 3,	// standard image file expected - cube map in the layout
	EnvironmentMap_LatLong = 4,	// standard image file expected
	ReservedMax = 255,	// you can specify your own types above this value
};

UENUM(BlueprintType)
enum class ESteamWorkshopFileType : uint8
{
	First = 0,

	Community = 0,		// normal Workshop item that can be subscribed to
	Microtransaction = 1,		// Workshop item that is meant to be voted on for the purpose of selling in-game
	Collection = 2,		// a collection of Workshop or Greenlight items
	Art = 3,		// artwork
	Video = 4,		// external video
	Screenshot = 5,		// screenshot
	Game = 6,		// Greenlight game entry
	Software = 7,		// Greenlight software entry
	Concept = 8,		// Greenlight concept
	WebGuide = 9,		// Steam web guide
	IntegratedGuide = 10,		// application integrated guide
	Merch = 11,		// Workshop merchandise meant to be voted on for the purpose of being sold
	ControllerBinding = 12,		// Steam Controller bindings
	SteamworksAccessInvite = 13,		// internal
	SteamVideo = 14,		// Steam video
	GameManagedItem = 15,		// managed completely by the game, not the user, and not shown on the web

	// Update k_EWorkshopFileTypeMax if you add values.
	Max = 16
};

UENUM(BlueprintType)
enum class ESteamUGCQuery : uint8
{
	RankedByVote = 0,
	RankedByPublicationDate = 1,
	AcceptedForGameRankedByAcceptanceDate = 2,
	RankedByTrend = 3,
	FavoritedByFriendsRankedByPublicationDate = 4,
	CreatedByFriendsRankedByPublicationDate = 5,
	RankedByNumTimesReported = 6,
	CreatedByFollowedUsersRankedByPublicationDate = 7,
	NotYetRated = 8,
	RankedByTotalVotesAsc = 9,
	RankedByVotesUp = 10,
	RankedByTextSearch = 11,
	RankedByTotalUniqueSubscriptions = 12,
	RankedByPlaytimeTrend = 13,
	RankedByTotalPlaytime = 14,
	RankedByAveragePlaytimeTrend = 15,
	RankedByLifetimeAveragePlaytime = 16,
	RankedByPlaytimeSessionsTrend = 17,
	RankedByLifetimePlaytimeSessions = 18,
};

UENUM(BlueprintType)
enum class ESteamUGCMatchingUGCType : uint8
{
	Items = 0,		// both mtx items and ready-to-use items
	Items_Mtx = 1,
	Items_ReadyToUse = 2,
	Collections = 3,
	Artwork = 4,
	Videos = 5,
	Screenshots = 6,
	AllGuides = 7,		// both web guides and integrated guides
	WebGuides = 8,
	IntegratedGuides = 9,
	UsableInGame = 10,		// ready-to-use items and integrated guides
	ControllerBindings = 11,
	GameManagedItems = 12,		// game managed items (not managed by users)
	All = 254,		// @note: will only be valid for CreateQueryUserUGCRequest requests
};

UENUM(BlueprintType)
enum class ESteamUserUGCList : uint8
{
	Published,
	VotedOn,
	VotedUp,
	VotedDown,
	WillVoteLater,
	Favorited,
	Subscribed,
	UsedOrPlayed,
	Followed,
};

UENUM(BlueprintType)
enum class ESteamUserUGCListSortOrder : uint8
{
	CreationOrderDesc,
	CreationOrderAsc,
	TitleAsc,
	LastUpdatedDesc,
	SubscriptionDateDesc,
	VoteScoreDesc,
	ForModeration,
};

UENUM(BlueprintType)
enum class ESteamItemUpdateStatus : uint8
{
	Invalid = 0, // The item update handle was invalid, job might be finished, listen too SubmitItemUpdateResult_t
	PreparingConfig = 1, // The item update is processing configuration data
	PreparingContent = 2, // The item update is reading and processing content files
	UploadingContent = 3, // The item update is uploading content changes to Steam
	UploadingPreviewFile = 4, // The item update is uploading new preview file image
	CommittingChanges = 5  // The item update is committing all changes
};

UENUM(BlueprintType)
enum class ESteamRemoteStoragePublishedFileVisibility : uint8
{
	Public = 0,
	FriendsOnly = 1,
	Private = 2,
};

UENUM(BlueprintType)
enum class ESteamItemStatistic : uint8
{
	NumSubscriptions = 0,
	NumFavorites = 1,
	NumFollowers = 2,
	NumUniqueSubscriptions = 3,
	NumUniqueFavorites = 4,
	NumUniqueFollowers = 5,
	NumUniqueWebsiteViews = 6,
	ReportScore = 7,
	NumSecondsPlayed = 8,
	NumPlaytimeSessions = 9,
	NumComments = 10,
	NumSecondsPlayedDuringTimePeriod = 11,
	NumPlaytimeSessionsDuringTimePeriod = 12,
};