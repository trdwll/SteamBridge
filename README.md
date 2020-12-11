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
ISteamInventory | :heavy_check_mark: | [1] (5)
ISteamMatchmaking | :heavy_check_mark: | 
ISteamMatchmakingServers | :heavy_check_mark: | [1] (10)
ISteamMusic | :heavy_check_mark: | 
ISteamMusicRemote | :heavy_check_mark: | [1] (2)
ISteamNetworkingMessages | :x: | 
ISteamNetworkingSockets | :x: | 
ISteamNetworkingUtils | :x: | 
ISteamParties | :heavy_check_mark: | 
ISteamRemotePlay | :heavy_check_mark: | 
ISteamRemoteStorage | :x: | 
ISteamScreenshots | :x: | 
ISteamUGC | :x: | 
ISteamUser | :heavy_check_mark: | 
ISteamUserStats | :x: | 
ISteamUtils | :heavy_check_mark: | [1] (5)
ISteamVideo | :heavy_check_mark: | 
SteamWeb | :x: | [2] |


- [1] Some methods were left out temporarily and will be added eventually.
- [2] Steam Web API will come after the core interfaces are implemented and are stable. 

Another note for [1]. I'm skipping some methods that will take a while to implement. I'm doing this so I can add most of the functionality quickly. If you need a specific method that isn't in the plugin then post an [issue](https://github.com/trdwll/SteamBridge/issues) or add it yourself and submit a [Pull Request](https://github.com/trdwll/SteamBridge/pulls).


## Installation

Go to the [releases](https://github.com/trdwll/SteamBridge/releases) page and download a release, or use Git and clone the repo into your Plugins folder.


## Usage

Using SteamBridge is actually really easy.

- Edit your `DefaultEngine.ini` and make sure the Steam config exists like below.
- Open your project and start using SteamBridge.


## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Make sure you abide by our [Code Guidelines](https://github.com/trdwll/SteamBridge/blob/main/CODEGUIDELINES.md).

Please work out of __develop__ branch. __Keeping _main_ stable is a priority.__ 

## License
[MIT](https://choosealicense.com/licenses/mit/)
