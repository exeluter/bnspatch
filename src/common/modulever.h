#pragma once

typedef struct tagLANGANDCODEPAGE
{
    WORD wLanguage;
    WORD wCodePage;
} LANGANDCODEPAGE;

EXTERN_C int GetModuleVersionInfo(HMODULE hModule, PCWSTR pwszSubBlock, LPCVOID *ppv);
