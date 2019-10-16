#define WINADVAPI __declspec(dllexport)
#define WIN32_NO_STATUS
#include <windows.h>
#undef WIN32_NO_STATUS
#include <ntstatus.h>
#include <SubAuth.h>

typedef struct _CRYPTO_BUFFER
{
    DWORD dwLength;
    DWORD dwMaximumLength;
    PBYTE lpBuffer;
} CRYPTO_BUFFER,
*PCRYPTO_BUFFER;
typedef CONST CRYPTO_BUFFER *PCCRYPTO_BUFFER;

typedef CONST UNICODE_STRING *PCUNICODE_STRING;

EXTERN_C_START

WINADVAPI BOOL WINAPI CheckSignatureInFile(
    LPCWSTR lpFileName)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptAcquireContextA(
    HCRYPTPROV *phProv,
    LPCSTR szContainer,
    LPCSTR szProvider,
    DWORD dwProvType,
    DWORD dwFlags)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptAcquireContextW(
    HCRYPTPROV *phProv,
    LPCWSTR szContainer,
    LPCWSTR szProvider,
    DWORD dwProvType,
    DWORD dwFlags)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptContextAddRef(
    HCRYPTPROV hProv,
    DWORD *pdwReserved,
    DWORD dwFlags)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptCreateHash(
    HCRYPTPROV hProv,
    ALG_ID Algid,
    HCRYPTKEY hKey,
    DWORD dwFlags,
    HCRYPTHASH *phHash)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptDecrypt(
    HCRYPTKEY hKey,
    HCRYPTHASH hHash,
    BOOL Final,
    DWORD dwFlags,
    BYTE *pbData,
    DWORD *pdwDataLen)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptDeriveKey(
    HCRYPTPROV hProv,
    ALG_ID Algid,
    HCRYPTHASH hBaseData,
    DWORD dwFlags,
    HCRYPTKEY *phKey)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptDestroyHash(
    HCRYPTHASH hHash)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptDestroyKey(
    HCRYPTKEY hKey)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptDuplicateHash(
    HCRYPTHASH hHash,
    DWORD *pdwReserved,
    DWORD dwFlags,
    HCRYPTHASH *phHash)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptDuplicateKey(
    HCRYPTKEY hKey,
    DWORD *pdwReserved,
    DWORD dwFlags,
    HCRYPTKEY *phKey)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptEncrypt(
    HCRYPTKEY hKey,
    HCRYPTHASH hHash,
    BOOL Final,
    DWORD dwFlags,
    BYTE *pbData,
    DWORD *pdwDataLen,
    DWORD dwBufLen)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptEnumProviderTypesA(
    DWORD dwIndex,
    DWORD *pdwReserved,
    DWORD dwFlags,
    DWORD *pdwProvType,
    LPSTR szTypeName,
    DWORD *pcbTypeName)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptEnumProviderTypesW(
    DWORD dwIndex,
    DWORD *pdwReserved,
    DWORD dwFlags,
    DWORD *pdwProvType,
    LPWSTR szTypeName,
    DWORD *pcbTypeName)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptEnumProvidersA(
    DWORD dwIndex,
    DWORD *pdwReserved,
    DWORD dwFlags,
    DWORD *pdwProvType,
    LPSTR szProvName,
    DWORD *pcbProvName)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptEnumProvidersW(
    DWORD dwIndex,
    DWORD *pdwReserved,
    DWORD dwFlags,
    DWORD *pdwProvType,
    LPWSTR szProvName,
    DWORD *pcbProvName)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptExportKey(
    HCRYPTKEY hKey,
    HCRYPTKEY hExpKey,
    DWORD dwBlobType,
    DWORD dwFlags,
    BYTE *pbData,
    DWORD *pdwDataLen)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptGenKey(
    HCRYPTPROV hProv,
    ALG_ID Algid,
    DWORD dwFlags,
    HCRYPTKEY *phKey)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptGenRandom(
    HCRYPTPROV hProv,
    DWORD dwLen,
    BYTE *pbBuffer)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptGetDefaultProviderA(
    DWORD dwProvType,
    DWORD *pdwReserved,
    DWORD dwFlags,
    LPSTR pszProvName,
    DWORD *pcbProvName)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptGetDefaultProviderW(
    DWORD dwProvType,
    DWORD *pdwReserved,
    DWORD dwFlags,
    LPWSTR pszProvName,
    DWORD *pcbProvName)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptGetHashParam(
    HCRYPTHASH hHash,
    DWORD dwParam,
    BYTE *pbData,
    DWORD *pdwDataLen,
    DWORD dwFlags)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptGetKeyParam(
    HCRYPTKEY hKey,
    DWORD dwParam,
    BYTE *pbData,
    DWORD *pdwDataLen,
    DWORD dwFlags)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptGetProvParam(
    HCRYPTPROV hProv,
    DWORD dwParam,
    BYTE *pbData,
    DWORD *pdwDataLen,
    DWORD dwFlags)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptGetUserKey(
    HCRYPTPROV hProv,
    DWORD dwKeySpec,
    HCRYPTKEY *phUserKey)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptHashData(
    HCRYPTHASH hHash,
    CONST BYTE *pbData,
    DWORD dwDataLen,
    DWORD dwFlags)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptHashSessionKey(
    HCRYPTHASH hHash,
    HCRYPTKEY hKey,
    DWORD dwFlags)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptImportKey(
    HCRYPTPROV hProv,
    CONST BYTE *pbData,
    DWORD dwDataLen,
    HCRYPTKEY hPubKey,
    DWORD dwFlags,
    HCRYPTKEY *phKey)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptReleaseContext(
    HCRYPTPROV hProv,
    DWORD dwFlags)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptSetHashParam(
    HCRYPTHASH hHash,
    DWORD dwParam,
    CONST BYTE *pbData,
    DWORD dwFlags)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptSetKeyParam(
    HCRYPTKEY hKey,
    DWORD dwParam,
    CONST BYTE *pbData,
    DWORD dwFlags)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptSetProvParam(
    HCRYPTPROV hProv,
    DWORD dwParam,
    CONST BYTE *pbData,
    DWORD dwFlags)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptSetProviderA(
    LPCSTR pszProvName,
    DWORD dwProvType)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptSetProviderExA(
    LPCSTR pszProvName,
    DWORD dwProvType,
    DWORD *pdwReserved,
    DWORD dwFlags)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptSetProviderExW(
    LPCWSTR pszProvName,
    DWORD dwProvType,
    DWORD *pdwReserved,
    DWORD dwFlags)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptSetProviderW(
    LPCWSTR pszProvName,
    DWORD dwProvType)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptSignHashA(
    HCRYPTHASH hHash,
    DWORD dwKeySpec,
    LPCSTR szDescription,
    DWORD dwFlags,
    BYTE *pbSignature,
    DWORD *pdwSigLen)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptSignHashW(
    HCRYPTHASH hHash,
    DWORD dwKeySpec,
    LPCWSTR szDescription,
    DWORD dwFlags,
    BYTE *pbSignature,
    DWORD *pdwSigLen)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptVerifySignatureA(
    HCRYPTHASH hHash,
    CONST BYTE *pbSignature,
    DWORD dwSigLen,
    HCRYPTKEY hPubKey,
    LPCSTR szDescription,
    DWORD dwFlags)
{
    return 0;
}

WINADVAPI BOOL WINAPI CryptVerifySignatureW(
    HCRYPTHASH hHash,
    CONST BYTE *pbSignature,
    DWORD dwSigLen,
    HCRYPTKEY hPubKey,
    LPCWSTR szDescription,
    DWORD dwFlags)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI RtlEncryptDES1block1key(
    LPCBYTE data,
    LPCBYTE key,
    LPBYTE output)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI RtlDecryptDES1block1key(
    LPCBYTE data,
    LPCBYTE key,
    LPBYTE output)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI RtlEncryptDESMagicBlock1key(
    LPCBYTE key,
    LPBYTE output)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI RtlEncryptDESblocksECB(
    PCCRYPTO_BUFFER data,
    PCCRYPTO_BUFFER key,
    PCRYPTO_BUFFER output)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI RtlDecryptDESblocksECB(
    PCCRYPTO_BUFFER data,
    PCCRYPTO_BUFFER key,
    PCRYPTO_BUFFER output)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI RtlDigestLM(
    LPCSTR data,
    LPBYTE output)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI RtlDigestNTLM(
    PCUNICODE_STRING data,
    LPBYTE output)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI RtlLMResponseToChallenge(
    LPCBYTE challenge,
    LPCBYTE hash,
    LPBYTE output)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI RtlDigestMD4only16Bytes(
    LPVOID unk0,
    LPCBYTE data,
    LPBYTE output)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI RtlEncryptDES2blocks2keys(
    LPCBYTE data,
    LPCBYTE key,
    LPBYTE output)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI RtlDecryptDES2blocks2keys(
    LPCBYTE data,
    LPCBYTE key,
    LPBYTE output)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI RtlEncryptDES2blocks1key(
    LPCBYTE data,
    LPCBYTE key,
    LPBYTE output)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI RtlDecryptDES2blocks1key(
    LPCBYTE data,
    LPCBYTE key,
    LPBYTE output)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI RtlEncryptDES2blocks1DWORD(
    LPCBYTE data,
    LPDWORD key,
    LPBYTE output)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI RtlDecryptDES2blocks1DWORD(
    LPCBYTE data,
    LPDWORD key,
    LPBYTE output)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI SystemFunction028(
    NDR_CCONTEXT CContext,
    LPBYTE output)
{
    return 0;
}

WINADVAPI RPC_STATUS RPC_ENTRY SystemFunction029(
    LPVOID unk0,
    LPBYTE output)
{
    return 0;
}

WINADVAPI BOOLEAN WINAPI RtlEqualMemory16Bytes(
    LPCBYTE data1,
    LPCBYTE data2)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI RtlEncryptDecryptRC4(
    PCRYPTO_BUFFER data,
    PCCRYPTO_BUFFER key)
{
    return 0;
}

WINADVAPI NTSTATUS NTAPI SystemFunction034(
    RPC_BINDING_HANDLE hRPC,
    HANDLE hUnk0,
    LPBYTE output)
{
    return 0;
}

WINADVAPI BOOL WINAPI RtlCheckSignatureInFile(
    LPCWSTR filename)
{
    return 0;
}

EXTERN_C_END
