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
 * Description :
 *
 */
#ifndef TSRUNNINGAPPLICATIONSTORAGE_H
#define TSRUNNINGAPPLICATIONSTORAGE_H

#include <e32base.h>
class MTsRunningApplication;

/**
 * Abstract interface of container with running applications desctiptors 
 */
class MTsRunningApplicationStorage
    {
public:
    /**
     * Provide access to running applications
     * @param aOffset - index of running application
     * @return running application entry
     */
    virtual const MTsRunningApplication& operator[] ( TInt aOffset ) const=0;
    
    /**
     * Provide information about number of running applications
     * @return number of running applications
     */
    virtual TInt Count() const =0;
    
    /**
     * @param aRunningApp embeded application entry
     * @return index of parent of embeded application
     */
    virtual TInt ParentIndex( const MTsRunningApplication& aRunningApp ) const =0;
    
    /**
     * Provide access to windows groups that were blocked( servers, applications without UI )
     * @return list of blocked window groups
     */
    virtual TArray<TInt> BlockedWindowGroups() const =0;

    };

#endif //TSRUNNINGAPPLICATIONSTORAGE_H
