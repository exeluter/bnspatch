# bnspatch [![Build status](https://ci.appveyor.com/api/projects/status/altu43hhdd5akowp?svg=true)](https://ci.appveyor.com/project/zeffy/bnspatch)
Personal project I decided to (partially) release to the public.

### Understand the following
- For educational purposes only.
- There is no support. Attempts to contact me for help will be ignored.
- Binaries are available from build artifacts and almost assuredly have severe bugs.
- Pull requests that I think add value to the project will be accepted.
- Using it on a live server is against the  Blade & Soul [Rules of Conduct][0] and [User Agreement][1].
- I accept no responsibility should you manage to get yourself banned while using it.

## Components
The client and launcher components of betterbns can be used individually or in tandem.

### Client features (public)
- [x] Enables multi-client.
- [x] Bypasses WinLicense "Detect File/Registry Monitors" protection.
- [x] Prevents `aegisty64.bin` from loading.

#### Planned features
- [ ] 32-bit client support.

### Launcher features (private, in-progress)
*Waiting on [**Harmony**][9] v2.x before making the source public.*

- Fixes miscellaneous bugs and annoying behavior in the official launcher, such as:
  * [x] Unnecessary localization files are no longer downloaded.
  * [x] Textboxes on the login window no longer unexpectedly change focus.
- [x] Disables AppInsights telemetry.
- [x] Disables encryption on log files (only for debugging).
- [x] Can Prevent the launcher from updating or repairing specific files.
- [x] [Command line arguments][2] that are set on the launcher shortcut are forwarded to the client
      when it is started.
      
#### Planned features
- [ ] Apply patches to `xml64.dat` and `config64.dat` automatically after installing new updates.
- [ ] Only requests administrator priviledges when installing updates.

## FAQ

#### Q: Why should I use this? What's wrong with BnS Buddy?
[Read this][3] and make your own decision.

#### Q: Does this work with [**d912pxy**][4]?
Yes.

## Acknowledgements
- [processhacker/**phnt**][5] (CC-BY-4.0 license)
- [microsoft/**Detours**][6] (MIT license)
- [microsoft/**GSL**][7] (MIT license)
- [microsoft/**wil**][8] (MIT license)
- [pardeike/**Harmony**][9] (MIT license)
- [gabime/**spdlog**][10] (MIT license)

[0]: https://us.ncsoft.com/en/legal/user-agreements/blade-and-soul-rules-of-conduct.php
[1]: https://us.ncsoft.com/en/legal/user-agreements/blade-and-soul-user-agreement.php
[2]: https://docs.unrealengine.com/udk/Three/CommandLineArguments.html
[3]: https://github.com/zeffy/bnspatch/blob/master/docs/BNSBUDDY_BAD.md
[4]: https://github.com/megai2/d912pxy
[5]: https://github.com/processhacker/phnt
[6]: https://github.com/microsoft/Detours
[7]: https://github.com/microsoft/GSL
[8]: https://github.com/microsoft/wil
[9]: https://github.com/pardeike/Harmony
[10]: https://github.com/gabime/spdlog
