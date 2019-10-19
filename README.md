# bnspatch [![Build status](https://ci.appveyor.com/api/projects/status/altu43hhdd5akowp?svg=true)](https://ci.appveyor.com/project/zeffy/bnspatch)
Personal project I decided to (partially) release to the public.

### Understand the following
- For educational purposes only.
- There is no support. Attempts to contact me for support will be ignored.
- No binaries are distributed, compile it yourself.
- Pull requests that I think add value to the project will be accepted.
- Using it on a live server is against the Blade & Soul [Rules of Conduct][0] and [User Agreement][1].
- I accept no responsibility should you manage to get yourself banned while using it.

## Components
The client and launcher components can be used individually or in tandem.

### Client features (public)
Only tested on NA/EU client.

- [x] Enables multi-client.
- [x] Bypasses WinLicense "Detect File/Registry Monitors" protection.
- [x] Prevents `aegisty64.bin` from loading.

#### Planned features
- [ ] 32-bit client support.

### Launcher features (private, in-progress)
*Waiting on [**Harmony**][8] v2.x before making the source public.*

Compatible with NC Launcher 2 for NA/EU and KR.

- [x] Unnecessary localization files are no longer downloaded.
- [x] Controls on the login window no longer switch focus if a field isn't filled in.
- [x] Disables AppInsights telemetry.
- [x] Disables encryption on log files (only for debugging).
- [x] Can prevent the launcher from updating or repairing specific files.
- [x] [Command line arguments][2] that are set on the launcher shortcut are forwarded to the client
      when it is started.
- [x] Exposes a scriptable interface for certain events, such as the game starting and ending.
      
#### Planned features
- [ ] Apply patches to `xml64.dat` and `config64.dat` automatically after installing new updates.
- [ ] Only requests administrator priviledges when installing updates.

## FAQ

#### Q: Does this work with [**d912pxy**][3]?
Yes.

## Acknowledgements
- [processhacker/**phnt**][4] (CC-BY-4.0 license)
- [microsoft/**Detours**][5] (MIT license)
- [microsoft/**GSL**][6] (MIT license)
- [microsoft/**wil**][7] (MIT license)
- [MonoMod/**MonoMod**][8] (MIT license)
- [gabime/**spdlog**][9] (MIT license)
- [madler/**zlib**][10] (zlib license)

[0]: https://us.ncsoft.com/en/legal/user-agreements/blade-and-soul-rules-of-conduct.php
[1]: https://us.ncsoft.com/en/legal/user-agreements/blade-and-soul-user-agreement.php
[2]: https://docs.unrealengine.com/udk/Three/CommandLineArguments.html
[3]: https://github.com/megai2/d912pxy
[4]: https://github.com/processhacker/phnt
[5]: https://github.com/microsoft/Detours
[6]: https://github.com/microsoft/GSL
[7]: https://github.com/microsoft/wil
[8]: https://github.com/MonoMod/MonoMod
[9]: https://github.com/gabime/spdlog
[10]: https://github.com/madler/zlib