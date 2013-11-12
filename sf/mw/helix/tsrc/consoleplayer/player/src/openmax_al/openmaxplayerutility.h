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

#ifndef OPENMAX_PLAYER_UTILITY_H
#define OPENMAX_PLAYER_UTILITY_H

#include <string.h>
#include <e32std.h>
#include <w32std.h>
#include <mmf\common\Mmfvideo.h> // for TVideoRotation and TAutoScaleType
#include "playerwindow.h"
#include "openmaxal.h"

#define MAX_NUMBER_INTERFACES 5

class COpenMaxPlayerUtility
{
public:
    COpenMaxPlayerUtility();
    ~COpenMaxPlayerUtility();

    void VideoFrameSize(TSize& aSize) const;
    TTimeIntervalMicroSeconds Position() const;
    TTimeIntervalMicroSeconds Duration() const;
    TInt Volume() const;
    TInt MaxVolume() const;
    TInt Balance()const;
    TVideoRotation Rotation() const;

    void SetVolume(TInt aVolume);
    void SetBalance(TInt aBalance);
    void SetPosition(const TTimeIntervalMicroSeconds& aPosition);
    void SetRotation(const RWindow& aWindow, TVideoRotation aRotation);
    void SetScaleFactor(const RWindow& aWindow, TReal32 aWidthPercentage, TReal32 aHeightPercentage);
    void SetAutoScale(const RWindow& aWindow, TAutoScaleType aScaleType);
    void StepFrame(TInt aStep);

    void OpenFile(const TDesC& aFileName,TUid aControllerUid = KNullUid);
    void OpenUrl(const TDesC& aUrl);
    void Prepare();
    void Close();
    void Play();
    TInt Stop();
    void Pause();

    void AddDisplayWindow(RWsSession& aWs, CWsScreenDevice& aScreenDevice, RWindow& aWindow);
    void SetWindowClipRect(const RWindow& aWindow, const TRect& aWindowClipRect);
    void SetVideoExtent(const RWindow& aWindow, const TRect& aVideoExtent);

    bool CheckErr (XAresult res, char* aMsg = "");

    void Init (void);
    void DestroyPlayer ();
    void DrawEQDisplay (void);
    void SetBandLevel(XAint16 band, XAboolean increase);
    void PlaySource(const XADataLocator_URI &source);
    void PlayPause(bool);
    void MuteChanged(bool);
    TInt SetPlayVelocity( TInt aVelocity );

protected:
    void StartBackLightTimer();
    void CancelBackLightTimer();

private:
    XAObjectItf m_engine; /* OpenMAX AL Engine */
    XAObjectItf player;
    XAObjectItf m_outputMix;

    XAEngineItf m_EngineItf;
    XAPlayItf m_playItf;
    XAEqualizerItf m_equalizerItf;
    XAVolumeItf m_volumeItf;

    XAInterfaceID m_iidArray[MAX_NUMBER_INTERFACES];
    XAboolean m_required[MAX_NUMBER_INTERFACES];

    XADataSource m_dataSource;
    XADataSink          m_audioSink;
    XADataSink          m_videoSink;
    XADataFormat_MIME m_mime;
    XADataLocator_OutputMix m_locator_outputmix;
    XADataLocator_NativeDisplay m_locator_displayregion;
    XADataLocator_URI uri;

    TRect   m_displayRect;
    int     m_nVolume;
    int     m_Velocity;
    XAchar  m_sourceName[1000];
};

#endif // OPENMAX_PLAYER_UTILITY_H
