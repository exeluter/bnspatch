#define _INC_MMSYSTEM
#include <Windows.h>
#define _WINMM_
#include <mmsyscom.h>
#undef WINMMAPI
#define WINMMAPI __declspec(dllexport)
#undef _INC_MMSYSTEM
#include <mmsystem.h>
#include <mmddk.h>
#undef PlaySound

#ifndef _WIN64
#define CALLBACK_ARGS_SIZE 16

typedef struct _CALLBACK_ARGS {
    DWORD       dwFlags;
    DWORD       dwFunctionAddr;
    WORD        wHandle;
    WORD        wMessage;
    DWORD       dwInstance;
    DWORD       dwParam1;
    DWORD       dwParam2;
} CALLBACK_ARGS;

typedef struct _CALLBACK_DATA {
    WORD wRecvCount;
    WORD wSendCount;
    CALLBACK_ARGS args[CALLBACK_ARGS_SIZE];
    WORD wIntsCount;
} CALLBACK_DATA;

typedef CALLBACK_DATA FAR *VPCALLBACK_DATA;
#endif

WINMMAPI BOOL WINAPI mciExecute(
	LPCSTR pszCommand)
{
    return 0;
}

EXTERN_C_START

WINMMAPI LRESULT WINAPI CloseDriver(
    HDRVR hDriver,
    LPARAM lParam1,
    LPARAM lParam2)
{
    return 0;
}

WINMMAPI LRESULT WINAPI DefDriverProc(
    DWORD_PTR dwDriverIdentifier,
    HDRVR hdrvr,
    UINT uMsg,
    LPARAM lParam1,
    LPARAM lParam2)
{
    return 0;
}

BOOL APIENTRY DriverCallback(
    DWORD_PTR dwCallback,
    DWORD dwFlags,
    HDRVR hDevice,
    DWORD dwMsg,
    DWORD_PTR dwUser,
    DWORD_PTR dwParam1,
    DWORD_PTR dwParam2)
{
    return 0;
}

WINMMAPI HMODULE WINAPI DrvGetModuleHandle(
    HDRVR hDriver)
{
    return 0;
}

WINMMAPI HMODULE WINAPI GetDriverModuleHandle(
    HDRVR hDriver)
{
    return 0;
}

#ifndef _WIN64
WINMMAPI DWORD WINAPI NotifyCallbackData(
    UINT uDevID,
    UINT uMsg,
    DWORD dwInstance,
    DWORD dwParam1,
    VPCALLBACK_DATA parg16)
{
	return 0;
}
#endif

WINMMAPI HDRVR WINAPI OpenDriver(
    LPCWSTR szDriverName,
    LPCWSTR szSectionName,
    LPARAM lParam2)
{
    return 0;
}

WINMMAPI BOOL WINAPI PlaySound(
    LPCSTR pszSound,
    HMODULE hmod,
    DWORD fdwSound)
{
    return 0;
}

WINMMAPI BOOL WINAPI PlaySoundA(
    LPCSTR pszSound,
    HMODULE hmod,
    DWORD fdwSound)
{
    return 0;
}

WINMMAPI BOOL WINAPI PlaySoundW(
    LPCWSTR pszSound,
    HMODULE hmod,
    DWORD fdwSound)
{
    return 0;
}

WINMMAPI LRESULT WINAPI SendDriverMessage(
    HDRVR hDriver,
    UINT message,
    LPARAM lParam1,
    LPARAM lParam2)
{
    return 0;
}

#ifndef _WIN64
WINMMAPI BOOL APIENTRY WOW32DriverCallback(
	DWORD dwCallback,
	DWORD dwFlags,
	WORD wID,
	WORD wMsg,
    DWORD dwUser,
    DWORD dw1,
    DWORD dw2)
{
	return 0;
}

WINMMAPI BOOL APIENTRY WOW32ResolveMultiMediaHandle(
    UINT uHandleType,
    UINT uMappingDirection,
    WORD wHandle16_In,
    LPWORD lpwHandle16_Out,
    DWORD dwHandle32_In,
    LPDWORD lpdwHandle32_Out)
{
	return 0;
}
#endif

WINMMAPI void WINAPI WOWAppExit(
	HANDLE hTask)
{
	(void)0;
}

#ifndef _WIN64
WINMMAPI DWORD WINAPI aux32Message(
    UINT uDeviceID,
    UINT uMessage,
    DWORD dwInstance,
    DWORD dwParam1,
    DWORD dwParam2)
{
	return 0;
}
#endif

WINMMAPI MMRESULT WINAPI auxGetDevCapsA(
    UINT_PTR uDeviceID,
    LPAUXCAPSA pac,
    UINT cbac)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI auxGetDevCapsW(
    UINT_PTR uDeviceID,
    LPAUXCAPSW pac,
    UINT cbac)
{
    return 0;
}

WINMMAPI UINT WINAPI auxGetNumDevs(void)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI auxGetVolume(
    UINT uDeviceID,
    LPDWORD pdwVolume)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI auxOutMessage(
    UINT uDeviceID,
    UINT uMsg,
    DWORD_PTR dw1,
    DWORD_PTR dw2)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI auxSetVolume(
    UINT uDeviceID,
    DWORD dwVolume)
{
    return 0;
}

#ifndef _WIN64
WINMMAPI DWORD WINAPI joy32Message(
    UINT uID,
    UINT uMessage,
    DWORD dwInstance,
    DWORD dwParam1,
    DWORD dwParam2)
{
	return 0;
}
#endif

WINMMAPI MMRESULT WINAPI joyConfigChanged(
    DWORD dwFlags)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI joyGetDevCapsA(
    UINT_PTR uJoyID,
    LPJOYCAPSA pjc,
    UINT cbjc)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI joyGetDevCapsW(
    UINT_PTR uJoyID,
    LPJOYCAPSW pjc,
    UINT cbjc)
{
    return 0;
}

WINMMAPI UINT WINAPI joyGetNumDevs(void)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI joyGetPos(
    UINT uJoyID,
    LPJOYINFO pji)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI joyGetPosEx(
    UINT uJoyID,
    LPJOYINFOEX pji)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI joyGetThreshold(
    UINT uJoyID,
    LPUINT puThreshold)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI joyReleaseCapture(
    UINT uJoyID)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI joySetCapture(
    HWND hwnd,
    UINT uJoyID,
    UINT uPeriod,
    BOOL fChanged)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI joySetThreshold(
    UINT uJoyID,
    UINT uThreshold)
{
    return 0;
}

#ifndef _WIN64
WINMMAPI DWORD WINAPI mci32Message(
    DWORD dwApi,
    DWORD dwF1,
    DWORD dwF2,
    DWORD dwF3,
    DWORD dwF4)
{
	return 0;
}
#endif

BOOL APIENTRY mciDriverNotify(
    HANDLE hwndCallback,
    MCIDEVICEID wDeviceID,
    UINT uStatus)
{
    return 0;
}

UINT APIENTRY mciDriverYield(
    MCIDEVICEID wDeviceID)
{
    return 0;
}

BOOL APIENTRY mciFreeCommandResource(
    UINT wTable)
{
    return 0;
}

WINMMAPI HTASK WINAPI mciGetCreatorTask(
    MCIDEVICEID mciId)
{
    return 0;
}

WINMMAPI MCIDEVICEID WINAPI mciGetDeviceIDA(
    LPCSTR pszDevice)
{
    return 0;
}

WINMMAPI MCIDEVICEID WINAPI mciGetDeviceIDFromElementIDA(
    DWORD dwElementID,
    LPCSTR lpstrType)
{
    return 0;
}

WINMMAPI MCIDEVICEID WINAPI mciGetDeviceIDFromElementIDW(
    DWORD dwElementID,
    LPCWSTR lpstrType)
{
    return 0;
}

WINMMAPI MCIDEVICEID WINAPI mciGetDeviceIDW(
    LPCWSTR pszDevice)
{
    return 0;
}

DWORD_PTR APIENTRY mciGetDriverData(
    MCIDEVICEID wDeviceID)
{
    return 0;
}

WINMMAPI BOOL WINAPI mciGetErrorStringA(
    MCIERROR mcierr,
    LPSTR pszText,
    UINT cchText)
{
    return 0;
}

WINMMAPI BOOL WINAPI mciGetErrorStringW(
    MCIERROR mcierr,
    LPWSTR pszText,
    UINT cchText)
{
    return 0;
}

WINMMAPI YIELDPROC WINAPI mciGetYieldProc(
    MCIDEVICEID mciId,
    LPDWORD pdwYieldData)
{
    return 0;
}

UINT APIENTRY mciLoadCommandResource(
    HANDLE hInstance,
    LPCWSTR lpResName,
    UINT wType)
{
    return 0;
}

WINMMAPI MCIERROR WINAPI mciSendCommandA(
    MCIDEVICEID mciId,
    UINT uMsg,
    DWORD_PTR dwParam1,
    DWORD_PTR dwParam2)
{
    return 0;
}

WINMMAPI MCIERROR WINAPI mciSendCommandW(
    MCIDEVICEID mciId,
    UINT uMsg,
    DWORD_PTR dwParam1,
    DWORD_PTR dwParam2)
{
    return 0;
}

WINMMAPI MCIERROR WINAPI mciSendStringA(
    LPCSTR lpstrCommand,
    LPSTR lpstrReturnString,
    UINT uReturnLength,
    HWND hwndCallback)
{
    return 0;
}

WINMMAPI MCIERROR WINAPI mciSendStringW(
    LPCWSTR lpstrCommand,
    LPWSTR lpstrReturnString,
    UINT uReturnLength,
    HWND hwndCallback)
{
    return 0;
}

BOOL APIENTRY mciSetDriverData(
    MCIDEVICEID wDeviceID,
    DWORD_PTR dwData)
{
    return 0;
}

WINMMAPI BOOL WINAPI mciSetYieldProc(
    MCIDEVICEID mciId,
    YIELDPROC fpYieldProc,
    DWORD dwYieldData)
{
    return 0;
}

#ifndef _WIN64
WINMMAPI DWORD WINAPI mid32Message(
    UINT uDeviceID,
    UINT uMessage,
    DWORD dwInstance,
    DWORD dwParam1,
    DWORD dwParam2)
{
	return 0;
}
#endif

WINMMAPI MMRESULT WINAPI midiConnect(
    HMIDI hmi,
    HMIDIOUT hmo,
    LPVOID pReserved)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiDisconnect(
    HMIDI hmi,
    HMIDIOUT hmo,
    LPVOID pReserved)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiInAddBuffer(
    HMIDIIN hmi,
    LPMIDIHDR pmh,
    UINT cbmh)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiInClose(
    HMIDIIN hmi)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiInGetDevCapsA(
    UINT_PTR uDeviceID,
    LPMIDIINCAPSA pmic,
    UINT cbmic)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiInGetDevCapsW(
    UINT_PTR uDeviceID,
    LPMIDIINCAPSW pmic,
    UINT cbmic)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiInGetErrorTextA(
    MMRESULT mmrError,
    LPSTR pszText,
    UINT cchText)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiInGetErrorTextW(
    MMRESULT mmrError,
    LPWSTR pszText,
    UINT cchText)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiInGetID(
    HMIDIIN hmi,
    LPUINT puDeviceID)
{
    return 0;
}

WINMMAPI UINT WINAPI midiInGetNumDevs(void)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiInMessage(
    HMIDIIN hmi,
    UINT uMsg,
    DWORD_PTR dw1,
    DWORD_PTR dw2)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiInOpen(
    LPHMIDIIN phmi,
    UINT uDeviceID,
    DWORD_PTR dwCallback,
    DWORD_PTR dwInstance,
    DWORD fdwOpen)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiInPrepareHeader(
    HMIDIIN hmi,
    LPMIDIHDR pmh,
    UINT cbmh)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiInReset(
    HMIDIIN hmi)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiInStart(
    HMIDIIN hmi)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiInStop(
    HMIDIIN hmi)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiInUnprepareHeader(
    HMIDIIN hmi,
    LPMIDIHDR pmh,
    UINT cbmh)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiOutCacheDrumPatches(
    HMIDIOUT hmo,
    UINT uPatch,
    LPWORD pwkya,
    UINT fuCache)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiOutCachePatches(
    HMIDIOUT hmo,
    UINT uBank,
    LPWORD pwpa,
    UINT fuCache)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiOutClose(
    HMIDIOUT hmo)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiOutGetDevCapsA(
    UINT_PTR uDeviceID,
    LPMIDIOUTCAPSA pmoc,
    UINT cbmoc)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiOutGetDevCapsW(
    UINT_PTR uDeviceID,
    LPMIDIOUTCAPSW pmoc,
    UINT cbmoc)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiOutGetErrorTextA(
    MMRESULT mmrError,
    LPSTR pszText,
    UINT cchText)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiOutGetErrorTextW(
    MMRESULT mmrError,
    LPWSTR pszText,
    UINT cchText)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiOutGetID(
    HMIDIOUT hmo,
    LPUINT puDeviceID)
{
    return 0;
}

WINMMAPI UINT WINAPI midiOutGetNumDevs(void)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiOutGetVolume(
    HMIDIOUT hmo,
    LPDWORD pdwVolume)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiOutLongMsg(
    HMIDIOUT hmo,
    LPMIDIHDR pmh,
    UINT cbmh)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiOutMessage(
    HMIDIOUT hmo,
    UINT uMsg,
    DWORD_PTR dw1,
    DWORD_PTR dw2)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiOutOpen(
    LPHMIDIOUT phmo,
    UINT uDeviceID,
    DWORD_PTR dwCallback,
    DWORD_PTR dwInstance,
    DWORD fdwOpen)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiOutPrepareHeader(
    HMIDIOUT hmo,
    LPMIDIHDR pmh,
    UINT cbmh)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiOutReset(
    HMIDIOUT hmo)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiOutSetVolume(
    HMIDIOUT hmo,
    DWORD dwVolume)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiOutShortMsg(
    HMIDIOUT hmo,
    DWORD dwMsg)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiOutUnprepareHeader(
    HMIDIOUT hmo,
    LPMIDIHDR pmh,
    UINT cbmh)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiStreamClose(
    HMIDISTRM hms)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiStreamOpen(
    LPHMIDISTRM phms,
    LPUINT puDeviceID,
    DWORD cMidi,
    DWORD_PTR dwCallback,
    DWORD_PTR dwInstance,
    DWORD fdwOpen)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiStreamOut(
    HMIDISTRM hms,
    LPMIDIHDR pmh,
    UINT cbmh)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiStreamPause(
    HMIDISTRM hms)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiStreamPosition(
    HMIDISTRM hms,
    LPMMTIME lpmmt,
    UINT cbmmt)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiStreamProperty(
    HMIDISTRM hms,
    LPBYTE lppropdata,
    DWORD dwProperty)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiStreamRestart(
    HMIDISTRM hms)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI midiStreamStop(
    HMIDISTRM hms)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mixerClose(
    HMIXER hmx)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mixerGetControlDetailsA(
    HMIXEROBJ hmxobj,
    LPMIXERCONTROLDETAILS pmxcd,
    DWORD fdwDetails)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mixerGetControlDetailsW(
    HMIXEROBJ hmxobj,
    LPMIXERCONTROLDETAILS pmxcd,
    DWORD fdwDetails)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mixerGetDevCapsA(
    UINT_PTR uMxId,
    LPMIXERCAPSA pmxcaps,
    UINT cbmxcaps)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mixerGetDevCapsW(
    UINT_PTR uMxId,
    LPMIXERCAPSW pmxcaps,
    UINT cbmxcaps)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mixerGetID(
    HMIXEROBJ hmxobj,
    UINT  FAR *puMxId,
    DWORD fdwId)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mixerGetLineControlsA(
    HMIXEROBJ hmxobj,
    LPMIXERLINECONTROLSA pmxlc,
    DWORD fdwControls)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mixerGetLineControlsW(
    HMIXEROBJ hmxobj,
    LPMIXERLINECONTROLSW pmxlc,
    DWORD fdwControls)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mixerGetLineInfoA(
    HMIXEROBJ hmxobj,
    LPMIXERLINEA pmxl,
    DWORD fdwInfo)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mixerGetLineInfoW(
    HMIXEROBJ hmxobj,
    LPMIXERLINEW pmxl,
    DWORD fdwInfo)
{
    return 0;
}

WINMMAPI UINT WINAPI mixerGetNumDevs(void)
{
    return 0;
}

WINMMAPI DWORD WINAPI mixerMessage(
    HMIXER hmx,
    UINT uMsg,
    DWORD_PTR dwParam1,
    DWORD_PTR dwParam2)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mixerOpen(
    LPHMIXER phmx,
    UINT uMxId,
    DWORD_PTR dwCallback,
    DWORD_PTR dwInstance,
    DWORD fdwOpen)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mixerSetControlDetails(
    HMIXEROBJ hmxobj,
    LPMIXERCONTROLDETAILS pmxcd,
    DWORD fdwDetails)
{
    return 0;
}

UINT APIENTRY mmDrvInstall(
    HDRVR hDriver,
    LPCWSTR wszDrvEntry,
    DRIVERMSGPROC drvMessage,
    UINT wFlags)
{
    return 0;
}

DWORD APIENTRY mmGetCurrentTask(VOID)
{
    return 0;
}

VOID APIENTRY mmTaskBlock(
    DWORD h)
{
	(void)0;
}

UINT APIENTRY mmTaskCreate(
	LPTASKCALLBACK lpfn,
	HANDLE FAR *lph,
	DWORD_PTR dwInst)
{
    return 0;
}

BOOL APIENTRY mmTaskSignal(
    DWORD h)
{
    return 0;
}

VOID APIENTRY mmTaskYield(VOID)
{
	(void)0;
}

WINMMAPI MMRESULT WINAPI mmioAdvance(
    HMMIO hmmio,
    LPMMIOINFO pmmioinfo,
    UINT fuAdvance)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mmioAscend(
    HMMIO hmmio,
    LPMMCKINFO pmmcki,
    UINT fuAscend)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mmioClose(
    HMMIO hmmio,
    UINT fuClose)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mmioCreateChunk(
    HMMIO hmmio,
    LPMMCKINFO pmmcki,
    UINT fuCreate)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mmioDescend(
    HMMIO hmmio,
    LPMMCKINFO pmmcki,
    const MMCKINFO  FAR *pmmckiParent,
    UINT fuDescend)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mmioFlush(
    HMMIO hmmio,
    UINT fuFlush)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mmioGetInfo(
    HMMIO hmmio,
    LPMMIOINFO pmmioinfo,
    UINT fuInfo)
{
    return 0;
}

WINMMAPI LPMMIOPROC WINAPI mmioInstallIOProcA(
    FOURCC fccIOProc,
    LPMMIOPROC pIOProc,
    DWORD dwFlags)
{
    return 0;
}

WINMMAPI LPMMIOPROC WINAPI mmioInstallIOProcW(
    FOURCC fccIOProc,
    LPMMIOPROC pIOProc,
    DWORD dwFlags)
{
    return 0;
}

WINMMAPI HMMIO WINAPI mmioOpenA(
    LPSTR pszFileName,
    LPMMIOINFO pmmioinfo,
    DWORD fdwOpen)
{
    return 0;
}

WINMMAPI HMMIO WINAPI mmioOpenW(
    LPWSTR pszFileName,
    LPMMIOINFO pmmioinfo,
    DWORD fdwOpen)
{
    return 0;
}

WINMMAPI LONG WINAPI mmioRead(
    HMMIO hmmio,
    HPSTR pch,
    LONG cch)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mmioRenameA(
    LPCSTR pszFileName,
    LPCSTR pszNewFileName,
    LPCMMIOINFO pmmioinfo,
    DWORD fdwRename)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mmioRenameW(
    LPCWSTR pszFileName,
    LPCWSTR pszNewFileName,
    LPCMMIOINFO pmmioinfo,
    DWORD fdwRename)
{
    return 0;
}

WINMMAPI LONG WINAPI mmioSeek(
    HMMIO hmmio,
    LONG lOffset,
    int iOrigin)
{
    return 0;
}

WINMMAPI LRESULT WINAPI mmioSendMessage(
    HMMIO hmmio,
    UINT uMsg,
    LPARAM lParam1,
    LPARAM lParam2)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mmioSetBuffer(
    HMMIO hmmio,
    LPSTR pchBuffer,
    LONG cchBuffer,
    UINT fuBuffer)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI mmioSetInfo(
    HMMIO hmmio,
    LPCMMIOINFO pmmioinfo,
    UINT fuInfo)
{
    return 0;
}

WINMMAPI FOURCC WINAPI mmioStringToFOURCCA(
    LPCSTR sz,
    UINT uFlags)
{
    return 0;
}

WINMMAPI FOURCC WINAPI mmioStringToFOURCCW(
    LPCWSTR sz,
    UINT uFlags)
{
    return 0;
}

WINMMAPI LONG WINAPI mmioWrite(
    HMMIO hmmio,
    const char *pch,
    LONG cch)
{
    return 0;
}

WINMMAPI UINT WINAPI mmsystemGetVersion(void)
{
    return 0;
}

#ifndef _WIN64
WINMMAPI DWORD WINAPI mod32Message(
    UINT uDeviceID,
    UINT uMessage,
    DWORD dwInstance,
    DWORD dwParam1,
    DWORD dwParam2)
{
	return 0;
}

WINMMAPI DWORD WINAPI mxd32Message(
    UINT uDeviceID,
    UINT uMessage,
    DWORD dwInstance,
    DWORD dwParam1,
    DWORD dwParam2)
{
	return 0;
}
#endif

WINMMAPI BOOL WINAPI sndPlaySoundA(
    LPCSTR pszSound,
    UINT fuSound)
{
    return 0;
}

WINMMAPI BOOL WINAPI sndPlaySoundW(
    LPCWSTR pszSound,
    UINT fuSound)
{
    return 0;
}

#ifndef _WIN64
WINMMAPI DWORD WINAPI tid32Message(
    UINT uDevId,
    UINT uMessage,
    DWORD dwInstance,
    DWORD dwParam1,
    DWORD dwParam2)
{
	return 0;
}
#endif

WINMMAPI MMRESULT WINAPI timeBeginPeriod(
    UINT uPeriod)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI timeEndPeriod(
    UINT uPeriod)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI timeGetDevCaps(
    LPTIMECAPS ptc,
    UINT cbtc)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI timeGetSystemTime(
    LPMMTIME pmmt,
    UINT cbmmt)
{
    return 0;
}

WINMMAPI DWORD WINAPI timeGetTime(void)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI timeKillEvent(
    UINT uTimerID)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI timeSetEvent(
    UINT uDelay,
    UINT uResolution,
    LPTIMECALLBACK fptc,
    DWORD_PTR dwUser,
    UINT fuEvent)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveInAddBuffer(
    HWAVEIN hwi,
    LPWAVEHDR pwh,
    UINT cbwh)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveInClose(
    HWAVEIN hwi)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveInGetDevCapsA(
    UINT_PTR uDeviceID,
    LPWAVEINCAPSA pwic,
    UINT cbwic)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveInGetDevCapsW(
    UINT_PTR uDeviceID,
    LPWAVEINCAPSW pwic,
    UINT cbwic)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveInGetErrorTextA(
    MMRESULT mmrError,
    LPSTR pszText,
    UINT cchText)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveInGetErrorTextW(
    MMRESULT mmrError,
    LPWSTR pszText,
    UINT cchText)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveInGetID(
    HWAVEIN hwi,
    LPUINT puDeviceID)
{
    return 0;
}

WINMMAPI UINT WINAPI waveInGetNumDevs(void)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveInGetPosition(
    HWAVEIN hwi,
    LPMMTIME pmmt,
    UINT cbmmt)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveInMessage(
    HWAVEIN hwi,
    UINT uMsg,
    DWORD_PTR dw1,
    DWORD_PTR dw2)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveInOpen(
    LPHWAVEIN phwi,
    UINT uDeviceID,
    LPCWAVEFORMATEX pwfx,
    DWORD_PTR dwCallback,
    DWORD_PTR dwInstance,
    DWORD fdwOpen)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveInPrepareHeader(
    HWAVEIN hwi,
    LPWAVEHDR pwh,
    UINT cbwh)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveInReset(
    HWAVEIN hwi)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveInStart(
    HWAVEIN hwi)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveInStop(
    HWAVEIN hwi)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveInUnprepareHeader(
    HWAVEIN hwi,
    LPWAVEHDR pwh,
    UINT cbwh)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutBreakLoop(
    HWAVEOUT hwo)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutClose(
    HWAVEOUT hwo)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutGetDevCapsA(
    UINT_PTR uDeviceID,
    LPWAVEOUTCAPSA pwoc,
    UINT cbwoc)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutGetDevCapsW(
    UINT_PTR uDeviceID,
    LPWAVEOUTCAPSW pwoc,
    UINT cbwoc)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutGetErrorTextA(
    MMRESULT mmrError,
    LPSTR pszText,
    UINT cchText)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutGetErrorTextW(
    MMRESULT mmrError,
    LPWSTR pszText,
    UINT cchText)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutGetID(
    HWAVEOUT hwo,
    LPUINT puDeviceID)
{
    return 0;
}

WINMMAPI UINT WINAPI waveOutGetNumDevs(void)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutGetPitch(
    HWAVEOUT hwo,
    LPDWORD pdwPitch)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutGetPlaybackRate(
    HWAVEOUT hwo,
    LPDWORD pdwRate)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutGetPosition(
    HWAVEOUT hwo,
    LPMMTIME pmmt,
    UINT cbmmt)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutGetVolume(
    HWAVEOUT hwo,
    LPDWORD pdwVolume)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutMessage(
    HWAVEOUT hwo,
    UINT uMsg,
    DWORD_PTR dw1,
    DWORD_PTR dw2)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutOpen(
    LPHWAVEOUT phwo,
    UINT uDeviceID,
    LPCWAVEFORMATEX pwfx,
    DWORD_PTR dwCallback,
    DWORD_PTR dwInstance,
    DWORD fdwOpen)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutPause(
    HWAVEOUT hwo)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutPrepareHeader(
    HWAVEOUT hwo,
    LPWAVEHDR pwh,
    UINT cbwh)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutReset(
    HWAVEOUT hwo)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutRestart(
    HWAVEOUT hwo)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutSetPitch(
    HWAVEOUT hwo,
    DWORD dwPitch)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutSetPlaybackRate(
    HWAVEOUT hwo,
    DWORD dwRate)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutSetVolume(
    HWAVEOUT hwo,
    DWORD dwVolume)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutUnprepareHeader(
    HWAVEOUT hwo,
    LPWAVEHDR pwh,
    UINT cbwh)
{
    return 0;
}

WINMMAPI MMRESULT WINAPI waveOutWrite(
    HWAVEOUT hwo,
    LPWAVEHDR pwh,
    UINT cbwh)
{
    return 0;
}

#ifndef _WIN64
WINMMAPI MMRESULT WINAPI wid32Message(
	UINT_PTR uDeviceID,
	UINT uMsg,
	HWAVEIN hwi,
	DWORD_PTR dw1,
	DWORD_PTR fdwOpen)
{
	return 0;
}

WINMMAPI DWORD WINAPI wod32Message(
    UINT uDeviceID,
    UINT uMessage,
    DWORD dwInstance,
    DWORD dwParam1,
    DWORD dwParam2)
{
	return 0;
}
#endif

EXTERN_C_END
