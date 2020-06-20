# <p align="center">bnspatch</p>

<p align="center">
  <a href="https://patreon.com/pilao"><img src="https://img.shields.io/endpoint.svg?url=https%3A%2F%2Fshieldsio-patreon.herokuapp.com%2Fpilao%2Fpledges&style=for-the-badge"/></a>
  <a href="https://admin.gear.mycelium.com/gateways/3554/orders/new"><img src="https://img.shields.io/badge/donate-bitcoin-f7941a?logo=data%3Aimage%2Fsvg%2Bxml%3Bbase64%2CPHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCI%2BPHBhdGggZD0iTTIzLjYzOCAxNC45MDRjLTEuNjAyIDYuNDMtOC4xMTMgMTAuMzQtMTQuNTQyIDguNzM2QzIuNjcgMjIuMDUtMS4yNDQgMTUuNTI1LjM2MiA5LjEwNSAxLjk2MiAyLjY3IDguNDc1LTEuMjQzIDE0LjkuMzU4YTEyIDEyIDAgMCAxIDguNzM4IDE0LjU0OHYtLjAwMnptLTYuMzUtNC42MTNjLjI0LTEuNi0uOTc0LTIuNDUtMi42NC0zLjAzbC41NC0yLjE1My0xLjMxNS0uMzMtLjUyNSAyLjEwN2MtLjM0NS0uMDg3LS43MDUtLjE2Ny0xLjA2NC0uMjVsLjUyNi0yLjEyNy0xLjMyLS4zMy0uNTQgMi4xNjUtLjg0LS4yLTEuODE1LS40NS0uMzUgMS40MDcuOTU1LjIzNmMuNTM1LjEzNi42My40ODYuNjE1Ljc2NmwtMS40NzcgNS45MmMtLjA3NS4xNjYtLjI0LjQwNi0uNjE0LjMxNC4wMTUuMDItLjk2LS4yNC0uOTYtLjI0bC0uNjYgMS41IDEuNy40MjYuOTMuMjQyLS41NCAyLjIgMS4zMi4zMjcuNTQtMi4xN2MuMzYuMS43MDUuMiAxLjA1LjI3M2wtLjUgMi4xNTQgMS4zMi4zMy41NDUtMi4yYzIuMjQuNDI3IDMuOTMuMjU3IDQuNjQtMS43NzQuNTctMS42MzctLjAzLTIuNTgtMS4yMTctMy4xOTYuODU0LS4xOTMgMS41LS43NiAxLjY4LTEuOTNoLjAxem0tMyA0LjIyYy0uNDA0IDEuNjQtMy4xNTcuNzUtNC4wNS41M2wuNzItMi45Yy44OTYuMjMgMy43NTcuNjcgMy4zMyAyLjM3em0uNC00LjI0Yy0uMzcgMS41LTIuNjYyLjczNS0zLjQwNS41NWwuNjU0LTIuNjRjLjc0NC4xOCAzLjEzNy41MjQgMi43NSAyLjA4NHYuMDA2eiIgZmlsbD0iI2NjYyIvPjwvc3ZnPg%3D%3D&style=for-the-badge"/></a>
  <a href="https://ci.appveyor.com/project/zeffy/bnspatch"><img src="https://img.shields.io/appveyor/build/zeffy/bnspatch?logo=appveyor&logoColor=ccc&style=for-the-badge"/></a>
  <img src="https://img.shields.io/github/license/zeffy/bnspatch?style=for-the-badge"/>
</p>

<p align="center">
  Personal project I decided to (partially) release to the public.
</p>

### Disclaimer
This project is for educational purposes only. There is no support; attempts to
contact me for support will be ignored. Using this on a live server is against
the Blade & Soul [Rules of Conduct][0.0] and [User Agreement][0.1]. I accept no
responsibility should you manage to get yourself banned while using it.

[0.0]: https://us.ncsoft.com/en/legal/user-agreements/blade-and-soul-rules-of-conduct.php
[0.1]: https://us.ncsoft.com/en/legal/user-agreements/blade-and-soul-user-agreement.php

## Features
You can check out the [**projects**](https://github.com/zeffy/bnspatch/projects)
page for an overview of planned and already implemented features.

## FAQ

#### Q: Does this work with [**d912pxy**][1.0]?
Yes it does! In fact, I highly recommend using it.

#### Q: Do I have to use all of the components for this to work?
No. Each component works individually and may be used together or separately.

#### Q: Isn't this unsafe compared to BnS Buddy?
No. I've been using some variant of it since I started development in
2017, and have never been warned nor banned, and neither has anyone else I
shared it with.

#### Q: Is this compatible with BnS Buddy?
There is some overlapping functionality, but yes. Just understand that `patches.xml` will
always take priority over conflicting modifications made by BnS Buddy add-ons.

[1.0]: https://github.com/megai2/d912pxy

## Acknowledgements
- [microsoft/**Detours**][2.0] (MIT license)
- [microsoft/**GSL**][2.1] (MIT license)
- [ocornut/**imgui**][2.2] (MIT license)
- [zeux/**pugixml**][2.3] (MIT license)
- [processhacker/**phnt**][2.4] (CC-BY-4.0 license)
- [JustasMasiulis/**xorstr**][2.5] (Apache-2.0 license)

[2.0]: https://github.com/microsoft/Detours
[2.1]: https://github.com/microsoft/GSL
[2.2]: https://github.com/ocornut/imgui
[2.3]: https://github.com/zeux/pugixml
[2.4]: https://github.com/processhacker/phnt
[2.5]: https://github.com/JustasMasiulis/xorstr
