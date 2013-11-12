/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  API event object base implementation.
*
*/

#ifndef CXIMPAPIEVENTBASE_H
#define CXIMPAPIEVENTBASE_H

#include "ximpapiobjbase.h"

class RWriteStream;
class RReadStream;


/**
 * API event object base.
 *
 * Concrete event types must implement static
 * NewFromStreamLC() constructor and event types
 * must be registered to event automation system.
 *
 * For details, see XIMPEventCodec implementation.
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
class CXIMPApiEventBase: public CXIMPApiObjBase
    {
public:

    enum { KClassId = XIMPIMP_CLSID_CXIMPAPIEVENTBASE };
    
public:
    IMPORT_C CXIMPApiEventBase();

public:
    IMPORT_C virtual ~CXIMPApiEventBase();

public:

    /**
     * Compare the content of this event with another event of the same type.
     * Volatile things (e.g. timestamps) are not compared, but things like user
     * ids and such are.
     * @return ETrue if the content matches.
     */
    virtual TBool EqualsContent( const CXIMPApiEventBase& aOtherInstance ) const = 0;


    /**
     * Virtual extenalize method for concrete event
     * objects to externalize their data.
     */
    virtual void ExternalizeL( RWriteStream& aStream ) const = 0;
    };



/**
 * Declares MXIMPEventBase and CXIMPApiEventBase methods for
 * concrete event implementation class header (.h) file.
 */
#define XIMPIMP_DECLARE_EVENT_BASE_METHODS \
    TInt32 EventInterfaceId() const; \
    void ExternalizeL( RWriteStream& aStream ) const; \
    TBool EqualsContent( const CXIMPApiEventBase& aOtherInstance ) const; \


/**
 * Implements EventInterfaceId() for MXIMPEventBase-derived
 * implementation class (.cpp) file.
 */
#define XIMPIMP_IMPLEMENT_EVENT_BASE_METHODS( TheClass, TheImplementedIf ) \
    TInt32 TheClass::EventInterfaceId() const \
    { \
    return TheImplementedIf::KInterfaceId; \
    } \


#endif // CXIMPREQUESTCOMPLETEEVENTIMP_H
