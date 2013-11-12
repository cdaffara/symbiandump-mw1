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
* Description:  API data object base implementation.
*
*/

#ifndef CXIMPAPIDATAOBJBASE_H
#define CXIMPAPIDATAOBJBASE_H

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
class CXIMPApiDataObjBase : public CXIMPApiObjBase
    {
public:

    enum { KClassId = XIMPIMP_CLSID_CXIMPAPIDATAOBJBASE };
    
public:

    IMPORT_C CXIMPApiDataObjBase();

    IMPORT_C virtual ~CXIMPApiDataObjBase();

public:

    /**
     * Compare the content of this data object with another 
     * data object of the same type.
     * Volatile things (e.g. timestamps) are not compared, but things like user
     * ids and such are.
     * @return ETrue if the content matches.
     */
    virtual TBool EqualsContent( const CXIMPApiDataObjBase& aOtherInstance ) const = 0;

    /**
     * Virtual extenalize method for concrete data
     * objects to externalize their data.
     */
    virtual void ExternalizeL( RWriteStream& aStream ) const = 0;
    };



/**
 * Declares CXIMPApiDataObjBase methods for
 * concrete event implementation class header (.h) file.
 */
#define XIMPIMP_DECLARE_DATAOBJ_BASE_METHODS \
    void ExternalizeL( RWriteStream& aStream ) const; \
    IMPORT_C static CXIMPApiDataObjBase* NewFromStreamLC( RReadStream& aStream ); \
    virtual TBool EqualsContent( const CXIMPApiDataObjBase& aOtherInstance ) const; \

#define XIMPIMP_DECLARE_DATAOBJ_BASE_PRIV_METHODS   \
    // void ConstructL( RReadStream& aStream ); \


/**
  * Implements a NewFromStreamLC method
  */
#define XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( TheClass ) \
EXPORT_C CXIMPApiDataObjBase* TheClass::NewFromStreamLC( RReadStream& aStream ) \
    { \
    TheClass* self = new( ELeave ) TheClass; \
    CleanupStack::PushL( self ); \
    self->ConstructL(); \
    self->InternalizeL( aStream ); \
    return self; \
    } \

#endif // CXIMPAPIDATAOBJBASE_H
