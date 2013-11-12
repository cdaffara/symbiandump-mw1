/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 *
 * Description:
 * Source file for the main class of the ConsolePlayer test app.
 *
 */

#include <string.h>
#include <stdlib.h>
#include "openmaxplayerutility.h"

#define FILE_EXTN "file:///"
#define FILE_EXTN_LEN strlen("file:///")

XAboolean  bIsPlayDone = XA_BOOLEAN_FALSE;

void PlayCallbackFun (XAPlayItf /*caller*/, void* pContext, XAuint32 event)
{
    char callback_string[50];
    COpenMaxPlayerUtility* pSelf = (COpenMaxPlayerUtility*)pContext;
    if(event & XA_PLAYEVENT_HEADMOVING)
    {
        strcpy(callback_string, "XA_PLAYEVENT_HEADMOVING");
    }
    else if(event & XA_PLAYEVENT_HEADATEND)
    {
        strcpy(callback_string, "XA_PLAYEVENT_HEADATEND");
        bIsPlayDone = XA_BOOLEAN_TRUE;
        pSelf->Stop();
    }
    else if(event & XA_PLAYEVENT_HEADATMARKER)
    {
        strcpy(callback_string, "XA_PLAYEVENT_HEADATMARKER");
    }
    else if(event & XA_PLAYEVENT_HEADATNEWPOS)
    {
        strcpy(callback_string, "XA_PLAYEVENT_HEADATNEWPOS");
    }
    else if(event & XA_PLAYEVENT_HEADSTALLED)
    {
        strcpy(callback_string, "XA_PLAYEVENT_HEADSTALLED");
    }
    //LOGFILE (callback_string);
}

COpenMaxPlayerUtility::COpenMaxPlayerUtility()
{
    Init();
}

COpenMaxPlayerUtility::~COpenMaxPlayerUtility()
{
    /* Destroy the player */
    DestroyPlayer ();

    /* Destroy Output Mix object */
    (*m_outputMix)->Destroy(m_outputMix);

    /* Shutdown OpenMAX AL */
    (*m_engine)->Destroy(m_engine);
}

// Initializes the OpenMAX AL engine and start the playback of some
// music from a file and draw the graphical equalizer
//
void COpenMaxPlayerUtility::Init (void)
{
    XAresult res;
    int i;
    XAEngineOption EngineOption[] = {
        (XAuint32) XA_ENGINEOPTION_THREADSAFE,
        (XAuint32) XA_BOOLEAN_TRUE
    };

    /* Create OpenMAX AL */
   res = xaCreateEngine(&m_engine, 1, EngineOption, 0, NULL, NULL);
   CheckErr(res, "xaCreateEngine");

    /* Realizing the XA Engine in synchronous mode. */
   res = (*m_engine)->Realize(m_engine, XA_BOOLEAN_FALSE);
   CheckErr(res, "Realize(engine)");

    /* Get the XA Engine Interface which is implicit */
   res = (*m_engine)->GetInterface(m_engine, XA_IID_ENGINE, (void**) &m_EngineItf);
   CheckErr(res, "GetInterface(engine)");

    /* Initialize arrays required[] and iidArray[] */
    for (i = 0; i < MAX_NUMBER_INTERFACES; i++)
    {
        m_required[i] = XA_BOOLEAN_FALSE;
        m_iidArray[i] = XA_IID_NULL;
    }

    /* Set arrays required[] and iidArray[] for VOLUME and EQUALIZER interfaces */
    m_required[0] = XA_BOOLEAN_TRUE;
    m_iidArray[0] = XA_IID_VOLUME;
    m_required[1] = XA_BOOLEAN_TRUE;
    m_iidArray[1] = XA_IID_EQUALIZER;

    /* Create Output Mix object to be used by player */
    res = (*m_EngineItf)->CreateOutputMix(m_EngineItf, &m_outputMix, 2, m_iidArray, m_required);
    CheckErr(res, "CreateOutputMix");

    /* Realizing the Output Mix object in synchronous mode. */
    res = (*m_outputMix)->Realize(m_outputMix, XA_BOOLEAN_FALSE);
    CheckErr(res, "Realize(outputMix)");

    /* Get volume and equalizer interfaces */
    res = (*m_outputMix)->GetInterface(m_outputMix, XA_IID_VOLUME, (void**) &m_volumeItf);
    CheckErr(res, "GetInterface(volume)");
    res = (*m_outputMix)->GetInterface(m_outputMix, XA_IID_EQUALIZER, (void**) &m_equalizerItf);
    CheckErr(res, "GetInterface(equalizer)");

    // Setup the audio sink structure
    m_locator_outputmix.locatorType = XA_DATALOCATOR_OUTPUTMIX;
    m_locator_outputmix.outputMix = m_outputMix;
    m_audioSink.pLocator = (void*) &m_locator_outputmix;
    m_audioSink.pFormat = NULL;

    /* Set arrays required[] and iidArray[] for no interfaces (PlayItf is implicit) */
    m_required[0] = XA_BOOLEAN_TRUE;
    m_iidArray[0] = XA_IID_DYNAMICSOURCE;
    m_required[1] = XA_BOOLEAN_FALSE;
    m_iidArray[1] = XA_IID_NULL;

    // Setup the video sink structure
    // Set nativeWindowHandle and nativeDisplayHandle to platform specific values
    XANativeHandle nativeWindowHandle = NULL;
    XANativeHandle nativeDisplayHandle = NULL;

    m_locator_displayregion.locatorType = XA_DATALOCATOR_NATIVEDISPLAY;
    m_locator_displayregion.hDisplay = nativeDisplayHandle;
    m_locator_displayregion.hWindow = nativeWindowHandle;
    m_videoSink.pLocator = (void*) &m_locator_displayregion;
    m_videoSink.pFormat = NULL;

    /* Before we start set volume to -3dB (-300mB) and enable equalizer */
    XAmillibel minVol = m_nVolume = XA_MILLIBEL_MIN;
    XAmillibel maxVol = 0;
    res = (*m_volumeItf)->GetMaxVolumeLevel (m_volumeItf, &maxVol);
    CheckErr(res, "GetMaxVolumeLevel");

    // before Play, set volume
    res = (*m_volumeItf)->SetVolumeLevel(m_volumeItf, minVol);
    CheckErr(res, "SetVolumeLevel");
}

void COpenMaxPlayerUtility::DestroyPlayer ()
    {
    XAresult res;
    XAuint32 playState;
    /* Stop the music */
    res = (*m_playItf)->GetPlayState(m_playItf, &playState);
    CheckErr(res, "GetPlayState");

    if ( playState != XA_PLAYSTATE_STOPPED )
        {
        res = (*m_playItf)->SetPlayState(m_playItf, XA_PLAYSTATE_STOPPED);
        CheckErr(res, "SetPlayState(stopped)");
        }

    // Destroy the player caused app to crash
//    (*player)->Destroy(player);
    }

/*
* Called when the display is repainted.
*/
void COpenMaxPlayerUtility::DrawEQDisplay (void)
{
    XAuint16 numBands;
    XAmillibel bandLevel;
    XAmillibel minLevel;
    XAmillibel maxLevel;
    XAmilliHertz minFreq;
    XAmilliHertz maxFreq;
    int band;
    XAresult res;
    res = (*m_equalizerItf)->GetNumberOfBands(m_equalizerItf, &numBands);
    CheckErr(res, "GetNumberOfBands(equalizerItf)");
    res = (*m_equalizerItf)->GetBandLevelRange(m_equalizerItf, &minLevel, &maxLevel);
    CheckErr(res, "GetBandLevelRange(equalizerItf)");
    for (band = 0; band<numBands; band++)
    {
        res = (*m_equalizerItf)->GetBandFreqRange(m_equalizerItf, (XAint16) band, &minFreq, &maxFreq);
        CheckErr(res, "GetBandFreqRange(equalizerItf)");
        res = (*m_equalizerItf)->GetBandLevel(m_equalizerItf, (XAint16)band, &bandLevel);
        CheckErr(res, "GetBandLevel(equalizerItf)");
 //       drawEQBand(minFreq, maxFreq, bandLevel);
    }
}

void COpenMaxPlayerUtility::VideoFrameSize(TSize& /*aSize*/) const
{
}

TTimeIntervalMicroSeconds COpenMaxPlayerUtility::Position() const
{
    return 0;
}

TTimeIntervalMicroSeconds COpenMaxPlayerUtility::Duration() const
{
    return 0;
}

TInt COpenMaxPlayerUtility::Volume() const
    {
    return m_nVolume;
    }

TInt COpenMaxPlayerUtility::MaxVolume() const
    {
    return m_nVolume;
    }

TInt COpenMaxPlayerUtility::Balance()const
{
    return 0;
}

TVideoRotation COpenMaxPlayerUtility::Rotation() const
{
    return EVideoRotationNone;
}

void COpenMaxPlayerUtility::SetVolume(TInt aVolume)
    {
    m_nVolume = aVolume;
    XAresult res = (*m_volumeItf)->SetVolumeLevel(m_volumeItf, (XAmillibel)m_nVolume);
    CheckErr(res, "SetVolumeL");
    }

void COpenMaxPlayerUtility::SetBalance(TInt /*aBalance*/)
    {
    }

void COpenMaxPlayerUtility::SetPosition(const TTimeIntervalMicroSeconds& /*aPosition*/)
    {

    }

void COpenMaxPlayerUtility::SetRotation(const RWindow& /*aWindow*/, TVideoRotation /*aRotation*/)
    {

    }

void COpenMaxPlayerUtility::SetScaleFactor(const RWindow& /*aWindow*/, TReal32 /*aWidthPercentage*/,
        TReal32 /*aHeightPercentage*/)
    {

    }

void COpenMaxPlayerUtility::SetAutoScale(const RWindow& /*aWindow*/, TAutoScaleType /*aScaleType*/)
    {

    }

void COpenMaxPlayerUtility::StepFrame(TInt /*aStep*/)
    {

    }

void COpenMaxPlayerUtility::OpenFile(const TDesC& aFileName, TUid /*aControllerUid*/)
    {
    const TUint16 *pFile = aFileName.Ptr();
    TInt len = aFileName.Length();

    if (pFile)
        {
        strcpy((char*)m_sourceName, FILE_EXTN);
        len = wcstombs((char*)&m_sourceName[FILE_EXTN_LEN], (const wchar_t *)pFile, len);
        m_sourceName[FILE_EXTN_LEN + len] = '\0';
        uri.locatorType = XA_DATALOCATOR_URI;
        uri.URI = (XAchar*) m_sourceName;
        m_mime.formatType = XA_DATAFORMAT_MIME;
        m_mime.mimeType = (XAchar *) "audio/ra";

		//Do we need this here?
        PlaySource (uri);
        }
    }

void COpenMaxPlayerUtility::OpenUrl(const TDesC& /*aUrl*/)
    {

    }

void COpenMaxPlayerUtility::Prepare()
    {

    }

void COpenMaxPlayerUtility::Close()
    {

    }

void COpenMaxPlayerUtility::Pause()
    {
    XAresult res;
    res = (*m_playItf)->SetPlayState(m_playItf, XA_PLAYSTATE_PAUSED);
    CheckErr(res, "SetPlayState(paused)");
    }

void COpenMaxPlayerUtility::Play()
    {
    XAresult res;
    res = (*m_playItf)->SetPlayState(m_playItf, XA_PLAYSTATE_PLAYING);
    CheckErr(res, "SetPlayState(playing)");
    }

TInt COpenMaxPlayerUtility::Stop()
    {
    DestroyPlayer();
	  return 0;
    }

void COpenMaxPlayerUtility::AddDisplayWindow(RWsSession& aWs, CWsScreenDevice& /*aScreenDevice*/, RWindow& aWindow)
    {
    m_locator_displayregion.hDisplay = &aWs; //pointer to RWsSession
    m_locator_displayregion.hWindow = &aWindow; //pointer to RWindow
    }

void COpenMaxPlayerUtility::SetWindowClipRect(const RWindow& /*aWindow*/, const TRect& /*aWindowClipRect*/)
    {

    }

void COpenMaxPlayerUtility::SetVideoExtent(const RWindow& /*aWindow*/, const TRect& /*aVideoExtent*/)
    {

    }

/* Checks for error. If any errors exit the application! */
bool COpenMaxPlayerUtility::CheckErr (XAresult res, char* /*aMsg*/)
{
    return (res == XA_RESULT_SUCCESS);
}


/*
* Called by UI when user increases or decreases a band level.
*/
void COpenMaxPlayerUtility::SetBandLevel(XAint16 band, XAboolean increase)
{
    XAuint16 numBands;
    XAmillibel bandLevel;
    XAmillibel minLevel;
    XAmillibel maxLevel;
    XAresult res;

    res = (*m_equalizerItf)->GetNumberOfBands(m_equalizerItf, &numBands);
    CheckErr(res, "GetNumberOfBands");

    res = (*m_equalizerItf)->GetBandLevelRange(m_equalizerItf, &minLevel, &maxLevel);
    CheckErr(res, "GetBandLevelRange");
    if (band >= numBands)
    {
        /* Error. Insert debug print here. */
        exit(0);
    }

    res = (*m_equalizerItf)->GetBandLevel(m_equalizerItf, band, &bandLevel);
    CheckErr(res, "GetBandLevel");

    if (increase==XA_BOOLEAN_TRUE)
    {
        /* increase the level by 1 dB (100mB) if the max supported level
        * is not exceeded */
        bandLevel = bandLevel + 100;
        if(bandLevel < maxLevel)
        {
            res = (*m_equalizerItf)->SetBandLevel(m_equalizerItf, band, bandLevel);
            CheckErr(res);
            DrawEQDisplay();
        }
    }
    else /* increase==false */
    {
        /* decrease the level by 1 dB (100mB) if the min supported level
        * is not crossed */
        bandLevel = bandLevel - 100;
        if( bandLevel > minLevel )
        {
            res = (*m_equalizerItf)->SetBandLevel(m_equalizerItf, band, bandLevel);
            CheckErr(res);
            DrawEQDisplay();
        }
    }
}

void COpenMaxPlayerUtility::PlaySource(const XADataLocator_URI &source)
    {
    m_dataSource.pLocator = (void*) &source;
    m_dataSource.pFormat = (void*) &m_mime;

    // Create the music player //
    XAresult res;
    res = (*m_EngineItf)->CreateMediaPlayer ( m_EngineItf, &player, &m_dataSource, NULL, &m_audioSink,
            &m_videoSink, NULL, NULL, 1, m_iidArray, m_required);
    CheckErr(res, "CreateMediaPlayer");

    // Realizing the player in synchronous mode.
    res = (*player)->Realize(player, XA_BOOLEAN_FALSE);
    CheckErr(res, "Realize(player)");

    // Get the play interface
    res = (*player)->GetInterface(player, XA_IID_PLAY, (void**) &m_playItf);
    CheckErr(res, "GetInterface(player)");

/*    XADynamicSourceItf sourceItf;
    res = (*player)->GetInterface(player, XA_IID_DYNAMICSOURCE, (void**) &sourceItf);
    CheckErr(res, "GetDynamicSource");

    res = (*sourceItf)->SetSource (sourceItf, &m_dataSource);
    CheckErr(res, "SetSource");
*/
    // before Play, set volume
    res = (*m_volumeItf)->SetVolumeLevel(m_volumeItf, m_nVolume);
    CheckErr(res, "SetVolumeLevel");

    XAmillisecond playDuration = 0;
    res = (*m_playItf)->GetDuration(m_playItf, &playDuration);
    bool b = CheckErr(res, "GetDuration");
    //if ( b && (playDuration != XA_DURATION_UNKNOWN) )
        //SetDurationL ((playDuration/1000));

    res = (*m_playItf)->SetCallbackEventsMask(m_playItf, XA_PLAYEVENT_HEADATEND | XA_PLAYEVENT_HEADMOVING);
    CheckErr(res, "SetCallbackEventsMask");

    res = (*m_playItf)->RegisterCallback(m_playItf, PlayCallbackFun, this);
    CheckErr(res, "RegisterCallback");
	//Do we need this here?
    Play();
    }

void COpenMaxPlayerUtility::PlayPause(bool bChecked)
    {
    if (bChecked)
        {
        Pause();
        }
    else
        {
        Play();
        }
    }

void COpenMaxPlayerUtility::MuteChanged(bool bChecked)
    {
    XAresult res = (*m_volumeItf)->SetMute(m_volumeItf, (XAboolean)bChecked);
    CheckErr(res, "MuteChanged");
    }

TInt COpenMaxPlayerUtility::SetPlayVelocity( TInt aVelocity )
    {
    m_Velocity = aVelocity;
    return 0;
    }
