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

#ifndef ACTIVITYTSENTRY_H
#define ACTIVITYTSENTRY_H

#include <QVariantHash>

class CFbsBitmap;

class ActivityTsEntry
{

public:
    ActivityTsEntry(const QVariantHash &activityData);
    ~ActivityTsEntry();

    void setThumbnail(const CFbsBitmap *bitmap);
    QVariantHash data() const;
    
private:
    QVariantHash mData;
    mutable const CFbsBitmap *mBitmap;
    
};

#endif // ACTIVITYTSENTRY_H
