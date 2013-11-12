/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Camcorder controller plug-in custom commands
*
*/



#ifndef CAMCCONTROLLERCUSTOMCOMMANDS_H
#define CAMCCONTROLLERCUSTOMCOMMANDS_H

/**
* CamcorderMMFPlugin controller UID which can be used by the client to identify the
* controller. The client can proceed to use the custom commands defined in
* TCamCControllerCustomCommands if the UID matches to what is returned by
* CMMFControllerImplementationInformation::Uid() method.
*/ 
const TUid KCamCControllerImplementationUid = {0x101F8503};

/**
* TMMFEvent UID used for asynchronous stopping of video recording. This event
* indicates that no new video frames are captured and that audio recording
* has stopped.
*/
const TUid KCamCControllerCCVideoRecordStopped = {0x2000E546};

/**
* TMMFEvent UID used for asynchronous stopping of video recording. This event
* indicates that the video container files has been fully composed.
*/
const TUid KCamCControllerCCVideoFileComposed = {0x2000E547};


/**
* Custom command IDs specific to CamcorderMMFPlugin controller
*/
enum TCamCControllerCustomCommands
    {
    /**
    * Custom command for setting a new filename without closing and reopening
    * the controller. Expects the first parameter to be a TMMFFileConfig object
    * with the iPath member set to the new target file name. This custom command
    * should be used synchronously via
    * CVideoRecorderUtility::CustomCommandSync(). Prepare() should be called
    * after issuing the ECamCControllerCCNewFilename command.
    */
    ECamCControllerCCNewFilename = 0,

    /**
    * Custom command for initiating asynchronous stopping of video recording.
    * No parameters. This custom command should be used synchronously via
    * CVideoRecorderUtility::CustomCommandSync(). The command should only be
    * used then paused or recording and it triggers the
    * KCamCControllerCCVideoRecordStopped and KCamCControllerCCVideoFileComposed
    * events.
    */
    ECamCControllerCCVideoStopAsync
    };

#endif      // CAMCCONTROLLERCUSTOMCOMMANDS_H

// End of File
