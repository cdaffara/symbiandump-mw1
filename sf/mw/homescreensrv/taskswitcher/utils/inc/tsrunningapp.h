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
#ifndef TSRUNNINGAPPLICATION_H
#define TSRUNNINGAPPLICATION_H

#include <e32base.h>

/**
 * Abstract interface of container with data to describe running application 
 */
class MTsRunningApplication
    {
public:
    /**
     * Provide access to application unique identifier
     * @return running application identyfier
     */
    virtual TUid UidL()const =0;
    
    /**
     * Provide access to application caption
     * @return running application caption 
     */
    virtual const TDesC& CaptionL() const =0;
    
    /**
     * Provide information if application is hidden from application library
     * @return EFalse if it's not hidden, other values it is.  
     */
    virtual TBool IsHiddenL() const =0;
    
    /**
     * Provide information if application is required by the system and should be closed
     * @return EFalse if it's not system, other values it is.  
     */
    virtual TBool IsSystemL() const =0;
    
    /**
     * Provide access to process window group identifier
     * @return process window group identyfier
     */
    virtual TInt WindowGroupId() const =0;
    
    /**
     * Provide access to "parent" application window group identifier if entry is embeded application
     * @return parent application window group id
     */
    virtual TInt ParentWindowGroupId() const =0;
    
    /**
     * Provide information if application is running embeded-mode
     * @return EFalse if application isn't embeded, other values it is 
     */
    virtual TBool IsEmbeded() const =0;

    };
#endif //TSRUNNINGAPPLICATION_H
