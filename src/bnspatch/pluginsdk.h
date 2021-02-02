#pragma once

struct PluginInfo
{
  void *Reserved1;
  void *Reserved2;
  void *Reserved3;
  void(__cdecl *Init)(void);
};

typedef void(__cdecl *GetPluginInfoFn)(PluginInfo *);
