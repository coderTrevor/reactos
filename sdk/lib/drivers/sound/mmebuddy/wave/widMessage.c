/*
 * PROJECT:     ReactOS Sound System "MME Buddy" Library
 * LICENSE:     GPL - See COPYING in the top level directory
 * FILE:        lib/sound/mmebuddy/wave/widMessage.c
 *
 * PURPOSE:     Provides the widMessage exported function, as required by
 *              the MME API, for wave input device support.
 *
 * PROGRAMMERS: Andrew Greenwood (silverblade@reactos.org)
*/

#include "precomp.h"

#include <debug.h>
/*
    Standard MME driver entry-point for messages relating to wave audio
    input.
*/
DWORD
APIENTRY
widMessage(
    UINT DeviceId,
    UINT Message,
    DWORD_PTR PrivateHandle,
    DWORD_PTR Parameter1,
    DWORD_PTR Parameter2)
{
    MMRESULT Result = MMSYSERR_NOTSUPPORTED;

    AcquireEntrypointMutex(WAVE_IN_DEVICE_TYPE);

    SND_TRACE(L"widMessage - Message type %d\n", Message);

    switch ( Message )
    {
        case WIDM_GETNUMDEVS :
        {
            Result = GetSoundDeviceCount(WAVE_IN_DEVICE_TYPE);
            break;
        }

        case WIDM_START :
        {
            Result = MmeSetState(PrivateHandle, TRUE);
            break;
        }

        case WIDM_STOP :
        {
            Result = MmeSetState(PrivateHandle, FALSE);
            break;
        }

        case WIDM_GETDEVCAPS :
        {

            Result = MmeGetSoundDeviceCapabilities(WAVE_IN_DEVICE_TYPE,
                                                   DeviceId,
                                                   (PVOID) Parameter1,
                                                   Parameter2);
            break;
        }
        case WIDM_OPEN :
        {            
            DPRINT1("widMessage(%ul, WIDM_OPEN, %p, %p, %p)\n", DeviceId, PrivateHandle, Parameter1, Parameter2);
            
            DPRINT1("wFormatTag: %u\n", ((LPWAVEOPENDESC)Parameter1)->lpFormat->wFormatTag);
            DPRINT1("nChannels: %u\n", ((LPWAVEOPENDESC)Parameter1)->lpFormat->nChannels);
            DPRINT1("nSamplesPerSec: %ul\n", ((LPWAVEOPENDESC)Parameter1)->lpFormat->nSamplesPerSec);
            DPRINT1("nAvgBytesPerSec: %ul\n", ((LPWAVEOPENDESC)Parameter1)->lpFormat->nAvgBytesPerSec);
            DPRINT1("nBlockAlign: %u\n", ((LPWAVEOPENDESC)Parameter1)->lpFormat->nBlockAlign);
            DPRINT1("wBitsPerSample: %u\n", ((LPWAVEOPENDESC)Parameter1)->lpFormat->wBitsPerSample);
            DPRINT1("cbSize: %u\n", ((LPWAVEOPENDESC)Parameter1)->lpFormat->cbSize);

            Result = MmeOpenDevice(WAVE_IN_DEVICE_TYPE,
                                       DeviceId,
                                       (LPWAVEOPENDESC) Parameter1,
                                       Parameter2,
                                       (DWORD_PTR*) PrivateHandle);
            break;
        }

        case WIDM_CLOSE :
        {
            Result = MmeCloseDevice(PrivateHandle);

            break;
        }

        case WIDM_PREPARE :
        {
            /* TODO: Do we need to pass 2nd parameter? */
            Result = MmePrepareWaveHeader(PrivateHandle, Parameter1);
            break;
        }

        case WIDM_UNPREPARE :
        {
            Result = MmeUnprepareWaveHeader(PrivateHandle, Parameter1);
            break;
        }

        case WIDM_RESET :
        {
            /* Stop playback, reset position to zero */
            Result = MmeResetWavePlayback(PrivateHandle);
            break;
        }

        case WIDM_ADDBUFFER :
        {
            Result = MmeWriteWaveHeader(PrivateHandle, Parameter1);
            break;
        }

        case DRV_QUERYDEVICEINTERFACESIZE :
        {
            Result = MmeGetDeviceInterfaceString(WAVE_IN_DEVICE_TYPE, DeviceId, NULL, 0, (DWORD*)Parameter1); //FIXME DWORD_PTR
            break;
        }

        case DRV_QUERYDEVICEINTERFACE :
        {
            Result = MmeGetDeviceInterfaceString(WAVE_IN_DEVICE_TYPE, DeviceId, (LPWSTR)Parameter1, Parameter2, NULL); //FIXME DWORD_PTR
            break;
        }


    }

    SND_TRACE(L"widMessage returning MMRESULT %d\n", Result);

    ReleaseEntrypointMutex(WAVE_IN_DEVICE_TYPE);

    return Result;
}
