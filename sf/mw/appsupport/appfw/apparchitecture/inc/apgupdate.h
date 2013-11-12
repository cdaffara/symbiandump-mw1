// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// apgupdate.h
//

#ifndef __APGUPDATE_H__
#define __APGUPDATE_H__

#include <e32std.h>
#include <f32file.h>
#include <s32strm.h>


/*
 * Contains application uid and corresponding action performed by installer.
 * @publishedAll
 */
class TApaAppUpdateInfo
    {
public:    
    /*
     * Defines actions performed by installers on an application.
     * @publishedAll
     */
    enum TApaAppAction
        {
        //Application is installed or upgraded.
        EAppPresent,
        //Application is uninstalled.
        EAppNotPresent,
        //Application information is changed.
        EAppInfoChanged
        };
    
public:    
    IMPORT_C void InternalizeL(RReadStream& aReadStream);
    IMPORT_C void ExternalizeL(RWriteStream& aWriteStream) const; 
    IMPORT_C TApaAppUpdateInfo(TUid aAppUid, TApaAppUpdateInfo::TApaAppAction aAction);
    IMPORT_C TApaAppUpdateInfo();    
public:
    TUid iAppUid;
    TApaAppAction iAction;
    };
#endif //__APGUPDATE_H__
