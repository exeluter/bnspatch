# bnspatch [![Build status](https://ci.appveyor.com/api/projects/status/altu43hhdd5akowp?svg=true)](https://ci.appveyor.com/project/zeffy/bnspatch)
Personal project I decided to (partially) release to the public.

### Disclaimer
This project is for educational purposes only. There is no support;
attempts to contact me for support will be ignored. No binaries are distributed.
Pull requests that I think add value to the project will be accepted.
Using it on a live server is against the Blade & Soul
[Rules of Conduct][0] and [User Agreement][1]. I accept no responsibility
should you manage to get yourself banned while using it.

## Features
Check out the [**Projects**](https://github.com/zeffy/bnspatch/projects) tab for a
break-down of features by component.

## FAQ

#### Q: Does this work with [**d912pxy**][3]?
Yes! In fact, I highly recommend using it.

#### Q: Do I have to use all of the components for bnspatch to work?
No. Each component works individually and may be used together or separately.

#### Q: Isn't BnS Buddy more safe to use than this?
TL;DR: No.

Long version: For some reason, BnS Buddy's developer gets really insecure
when alternatives to his program show up and [really][12] [likes to][13] spread FUD
around to discourage people from using them, using scary buzzwords like "injection".
While bnspatch does make use of a `winmm.dll` proxy, BnS Buddy does
the **exact** same thing to enable multi-client (and previously did with the XIGNCODE3
bypass, when it was still used).

So bnspatch isn't any or more (or less) "inject-y" or unsafe than BnS Buddy is.
I don't expect the developer to understand any of this though, since they literally
stole both of those features from others, without giving any credit 
([dead link][14] ([mirror][15]), [link][16]).

## Acknowledgements
- [gabime/**spdlog**][4] (MIT license)
- [madler/**zlib**][5] (zlib license)
- [microsoft/**Detours**][6] (MIT license)
- [microsoft/**GSL**][7] (MIT license)
- [microsoft/**wil**][8] (MIT license)
- [MonoMod/**MonoMod**][9] (MIT license)
- [morelinq/**MoreLINQ**][10] (Apache-2.0 license)
- [processhacker/**phnt**][11] (CC-BY-4.0 license)

[0]: https://us.ncsoft.com/en/legal/user-agreements/blade-and-soul-rules-of-conduct.php
[1]: https://us.ncsoft.com/en/legal/user-agreements/blade-and-soul-user-agreement.php
[2]: https://docs.unrealengine.com/udk/Three/CommandLineArguments.html
[3]: https://github.com/megai2/d912pxy
[4]: https://github.com/gabime/spdlog
[5]: https://github.com/madler/zlib
[6]: https://github.com/microsoft/Detours
[7]: https://github.com/microsoft/GSL
[8]: https://github.com/microsoft/wil
[9]: https://github.com/MonoMod/MonoMod
[10]: https://github.com/morelinq/MoreLINQ
[11]: https://github.com/processhacker/phnt
[12]: https://archive.li/XFWdU
[13]: https://i.imgur.com/rzFEY0F.png
[14]: https://github.com/Evengard/bnsnogg
[15]: https://github.com/zeffy/bnsnogg-mirror
[16]: https://github.com/VirtualPuppet/XignCode3-bypass