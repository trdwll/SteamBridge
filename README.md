# SteamBridge

![GitHub All Releases](https://img.shields.io/github/downloads/trdwll/SteamBridge/total?label=total%20downloads&style=flat-square) ![Jenkins](https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins.trdwll.com%2Fjob%2FSteamBridge%2Fjob%2Fmain%2F&style=flat-square)

A UE4 implementation of the Steamworks API. 

Interface | Completed | Notes |
|---------------|---------------|---------------|
ISteamApps | :heavy_check_mark: | 
ISteamFriends | :heavy_check_mark: | [1] (1) |
ISteamGameServer | :heavy_check_mark: | [1] (4) |
ISteamGameServerStats | :heavy_check_mark: | [1] (1) |
ISteamHTMLSurface | :heavy_check_mark: | 
ISteamHTTP | :heavy_check_mark: | 
ISteamInput | :heavy_check_mark: | 
ISteamInventory | :x: | 
ISteamMatchmaking | :x: | 
ISteamMatchmakingServers | :x: | 
ISteamMusic | :x: | 
ISteamMusicRemote | :x: | 
ISteamNetworkingMessages | :x: | 
ISteamNetworkingSockets | :x: | 
ISteamNetworkingUtils | :x: | 
ISteamParties | :x: | 
ISteamRemotePlay | :x: | 
ISteamRemoteStorage | :x: | 
ISteamScreenshots | :x: | 
ISteamUGC | :x: | 
ISteamUser | :heavy_check_mark: | 
ISteamUserStats | :x: | 
ISteamUtils | :x: | 
ISteamVideo | :x: | 
SteamEncryptedAppTicket | :x: | 
SteamWeb | :x: | [2] |


- [1] Some methods were left out temporarily and will be added eventually.
- [2] Steam Web API will come after the core interfaces are implemented and are stable. 


## Installation

Go to the [releases](https://github.com/trdwll/SteamBridge/releases) page and download a release, or use Git and clone the repo into your Plugins folder.


## Usage

Using SteamBridge is actually really easy.

- Edit your `DefaultEngine.ini` and make sure the Steam config exists like below.
- Open your project and start using SteamBridge.


## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Make sure you abide by our [Code Guidelines](https://github.com/trdwll/SteamBridge/blob/main/CODEGUIDELINES.md).

## License
[MIT](https://choosealicense.com/licenses/mit/)
