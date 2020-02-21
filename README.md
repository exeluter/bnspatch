# <p align="center">bnspatch</p>

<p align="center">
  <a href="https://admin.gear.mycelium.com/gateways/3554/orders/new"><img src="https://img.shields.io/badge/donate-bitcoin-f7941a?logo=data%3Aimage%2Fsvg%2Bxml%3Bbase64%2CPHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCI%2BPHBhdGggZD0iTTIzLjYzOCAxNC45MDRjLTEuNjAyIDYuNDMtOC4xMTMgMTAuMzQtMTQuNTQyIDguNzM2QzIuNjcgMjIuMDUtMS4yNDQgMTUuNTI1LjM2MiA5LjEwNSAxLjk2MiAyLjY3IDguNDc1LTEuMjQzIDE0LjkuMzU4YTEyIDEyIDAgMCAxIDguNzM4IDE0LjU0OHYtLjAwMnptLTYuMzUtNC42MTNjLjI0LTEuNi0uOTc0LTIuNDUtMi42NC0zLjAzbC41NC0yLjE1My0xLjMxNS0uMzMtLjUyNSAyLjEwN2MtLjM0NS0uMDg3LS43MDUtLjE2Ny0xLjA2NC0uMjVsLjUyNi0yLjEyNy0xLjMyLS4zMy0uNTQgMi4xNjUtLjg0LS4yLTEuODE1LS40NS0uMzUgMS40MDcuOTU1LjIzNmMuNTM1LjEzNi42My40ODYuNjE1Ljc2NmwtMS40NzcgNS45MmMtLjA3NS4xNjYtLjI0LjQwNi0uNjE0LjMxNC4wMTUuMDItLjk2LS4yNC0uOTYtLjI0bC0uNjYgMS41IDEuNy40MjYuOTMuMjQyLS41NCAyLjIgMS4zMi4zMjcuNTQtMi4xN2MuMzYuMS43MDUuMiAxLjA1LjI3M2wtLjUgMi4xNTQgMS4zMi4zMy41NDUtMi4yYzIuMjQuNDI3IDMuOTMuMjU3IDQuNjQtMS43NzQuNTctMS42MzctLjAzLTIuNTgtMS4yMTctMy4xOTYuODU0LS4xOTMgMS41LS43NiAxLjY4LTEuOTNoLjAxem0tMyA0LjIyYy0uNDA0IDEuNjQtMy4xNTcuNzUtNC4wNS41M2wuNzItMi45Yy44OTYuMjMgMy43NTcuNjcgMy4zMyAyLjM3em0uNC00LjI0Yy0uMzcgMS41LTIuNjYyLjczNS0zLjQwNS41NWwuNjU0LTIuNjRjLjc0NC4xOCAzLjEzNy41MjQgMi43NSAyLjA4NHYuMDA2eiIgZmlsbD0iI2NjYyIvPjwvc3ZnPg%3D%3D&style=for-the-badge"/></a>
  <a href="https://ci.appveyor.com/project/zeffy/bnspatch"><img src="https://img.shields.io/appveyor/build/zeffy/bnspatch?logo=appveyor&logoColor=ccc&style=for-the-badge"/></a>
  <img src="https://img.shields.io/github/license/zeffy/bnspatch?style=for-the-badge"/>
</p>

<p align="center">Personal project I decided to (partially) release to the public.</p>

### Disclaimer
This project is for educational purposes only. There is no support; attempts to
contact me for support will be ignored. Using this on a live server is against the
Blade & Soul [Rules of Conduct][0] and [User Agreement][1]. I accept no
responsibility should you manage to get yourself banned while using it.

## Features
You can out the [**projects**](https://github.com/zeffy/bnspatch/projects) tab for a
road map of upcoming and already implemented features.

## Building
Everything should build as-is except for `Gaffeine.Data.XmlSerializers.dll`, which
may need a little manual configuration.

1. NC Launcher 2 must be installed.
2. With each new version of NC Launcher 2, `ParentAssemblyId` in `AssemblyInfo.cs` must
   be updated to match the MVID of `Gaffeine.Data.dll`, or nothing will work.
   You can find this value with dnSpy or CFF Explorer.
3. Updating the embedded dependencies is a little tricky. They are deflated with
   `zopfli --deflate --i1000`, but any deflater will do. Afterwards the length of
   the compressed data (32-bit unsigned) needs to be prepended to the beginning of the
   `.deflate` file.
4. If you update the `MonoMod.Utils.dll` dependency, you need to modify it so that
   its `System.Runtime.CompilerServices.IgnoresAccessChecksToAttribute` type is
   `internal` instead of `public`.

## FAQ

#### Q: Does this work with [**d912pxy**][3]?
Yes it does! In fact, I highly recommend using it.

#### Q: Do I have to use all of the components for this to work?
No. Each component works individually and may be used together or separately.

#### Q: Isn't this unsafe compared to BnS Buddy?
TL;DR: No.

Long version: BnS Buddy's developer, instead of using his influence to foster a
healthy development community for Blade & Soul, has a knee-jerk reaction whenever
alternatives to his program are mentioned and really likes to spread FUD around to
discourage people from using them, by using spooky terms like "injection"
[[link][10], [link][11]].

While bnspatch does make use of DLL hijacking to do its magic, this exact method
is also used by the multi-client feature in BnS Buddy (and the XIGNCODE3 bypass as
well, previously).

In other words, bnspatch isn't any or more (or less) "inject-y" or unsafe than
BnS Buddy is. I don't expect the developer to understand any of this though, since
they literally copy-pasted both of those features from other people, without giving
any credit [[dead link][12] ([mirror][13]), [link][14]].

## Acknowledgements
- [gabime/**spdlog**][3] (MIT license)
- [microsoft/**Detours**][4] (MIT license)
- [microsoft/**GSL**][5] (MIT license)
- [ocornut/**imgui**][6] (MIT license)
- [MonoMod/**MonoMod**][7] (MIT license)
- [processhacker/**phnt**][8] (CC-BY-4.0 license)
- [JustasMasiulis/**xorstr**][9] (Apache-2.0 license)

[0]: https://us.ncsoft.com/en/legal/user-agreements/blade-and-soul-rules-of-conduct.php
[1]: https://us.ncsoft.com/en/legal/user-agreements/blade-and-soul-user-agreement.php
[2]: https://docs.unrealengine.com/udk/Three/CommandLineArguments.html
[3]: https://github.com/megai2/d912pxy
[4]: https://github.com/microsoft/Detours
[5]: https://github.com/microsoft/GSL
[6]: https://github.com/ocornut/imgui
[7]: https://github.com/MonoMod/MonoMod
[8]: https://github.com/processhacker/phnt
[9]: https://github.com/JustasMasiulis/xorstr
[10]: https://archive.li/XFWdU
[11]: https://i.imgur.com/rzFEY0F.png
[12]: https://github.com/Evengard/bnsnogg
[13]: https://github.com/zeffy/bnsnogg-mirror
[14]: https://github.com/VirtualPuppet/XignCode3-bypass
