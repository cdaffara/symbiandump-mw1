/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:      Stack for XML SAX Parser.
*
*/






#ifndef _C_UPNPOBJECTSTACK_H_
#define _C_UPNPOBJECTSTACK_H_

// INCLUDES
#include <e32base.h>
#include "upnpobject.h"

/**
 * A Stack class, which is used to hold UPnP objects in order while parsing 
 * xml.
 *
 * @since Series 60 3.1
 * @lib upnpxmlparser.lib
 */
NONSHARABLE_CLASS( CUPnPObjectStack ) : public CBase
    {
public:
    /**
     * 2-phased constructor.
     */
    static inline CUPnPObjectStack* NewL();
    
    /**
     * Destructor.
     */
    inline virtual ~CUPnPObjectStack();

public:

    /**
     * Pops top object from the stack.
     * @return pointer to the popped object.
     */
    inline CUpnpObject* Pop();
    
    /**
     * Retuns the pointer of the top object.
     * @return pointer to the top object.
     */
    inline CUpnpObject* Top();
    
    /**
     * Pushes object to the stack.
     * @param aObject, an object, which is added to the stack as top object.
     */
    inline void PushL( CUpnpObject* aObject );
    
    /**
     * Returns the current count of the stack.
     * @return stack count.
     */
    inline TInt Count();
    
    /**
     * Clears the stack.
     */
    inline void Reset();
    
    /**
     * Clears the stack and deletes all the objects in it.
     */
    inline void ResetAndDestroy();

private:

    /**
     * Constructor.
     */
    inline CUPnPObjectStack();

private:

    /**
     * An array, which contains the objects in the stack.
     * Own.
     */
    RPointerArray<CUpnpObject>* iStack;
    };

#include "upnpobjectstack.inl"

#endif // _C_UPNPOBJECTSTACK_H_
