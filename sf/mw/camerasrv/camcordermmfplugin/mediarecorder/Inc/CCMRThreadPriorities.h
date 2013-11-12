/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Video parameters optimized for reference HW
*                when porting to another HW these may need to be changed
*
*/



#ifndef CCMRTHREADPRIORITIES_H
#define CCMRTHREADPRIORITIES_H

// Audio thread priority. In some (at least low performance) HWs this should be EPriorityNormal 
// to have audio and video running smoothly, whereas in some HWs it should be EPriorityMore, 
// e.g. if fast response time to DevSound/HW buffer input is required
// Please check video thread priorities too when changing these.
// In udeb EPriorityMore seems to make audio thread the only active thread which makes the system 
// unresponsive, hence the priority is then always normal, but the audio recording may not work properly
// in all HWs (some frames may be missing)
#ifdef _DEBUG
const TThreadPriority KCMRAudioThreadPrioritySWCodec = EPriorityNormal;
const TThreadPriority KCMRAudioThreadPriorityHWAccelarated = EPriorityNormal;
#else
const TThreadPriority KCMRAudioThreadPrioritySWCodec = EPriorityMore;
const TThreadPriority KCMRAudioThreadPriorityHWAccelarated = EPriorityMore;
#endif

// Video thread priority. In some configurations high performance video requires higher thread 
// priority to be able to move buffers fast enough between camera and video encoder.
// However, it may have negative impact to audio operation. 
// Hence please check audio thread priorities too when changing these.
// In udeb it is better to have them as normal.
#ifdef _DEBUG
const TThreadPriority KCMRVideoThreadPriorityNormal = EPriorityNormal;
const TThreadPriority KCMRVideoThreadPriorityHighPerf = EPriorityNormal;
#else
const TThreadPriority KCMRVideoThreadPriorityNormal = EPriorityMore;
const TThreadPriority KCMRVideoThreadPriorityHighPerf = EPriorityMore;
#endif

#endif // CCMRVIDEOSETTINGS_H

// End of file
