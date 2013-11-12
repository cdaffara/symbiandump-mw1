/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/
#ifndef AFCMD_H
#define AFCMD_H

const TInt KRequestAppIdOffset(0);
const TInt KRequestActOffset(1);
const TInt KRequestData(2);
const TInt KRequestIdOffset(0);

const TInt KResponseDataSizeOffset(0);
const TInt KResponseIdOffset(3);
const TInt KResponseDataOffset(1);


enum ActivityCmd {
    SaveActivity =0,
    RemoveActivity,
    RemoveApplicationActivities,
    Activities,
    ApplicationActivities,
    ApplicationActivity,
    WaitActivity,
    LaunchActivity,
    GetThumbnail,
    GetData,
    NotifyChange,
    CancelWait,
    CancelNotify
};

#endif  //AFCMD_H

