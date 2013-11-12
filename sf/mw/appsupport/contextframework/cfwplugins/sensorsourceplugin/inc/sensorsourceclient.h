/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSensorSourceClient class declaration.
 *
*/


#ifndef SENSORSOURCECLIENT_H
#define SENSORSOURCECLIENT_H

// SYSTEM INCLUDE FILES
#include <e32base.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS

// DATA TYPES

// CLASS DECLARATION

/**
 * This class wraps up the required information from the entity which
 * is using sensor source context information.
 *
 * @lib None.
 * @since S60 5.0
 */
class CSensorSourceClient : public CBase
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param None.
     * @return CSensorSourceClient
     */
    static CSensorSourceClient* NewL( const TSecureId& aSid,
        TInt aScriptId,
        const TDesC& aFeatureId );
    static CSensorSourceClient* NewLC( const TSecureId& aSid,
        TInt aScriptId,
        const TDesC& aFeatureId );

    /**
     * C++ destructor.
     */
    virtual ~CSensorSourceClient();

private:

    CSensorSourceClient( const TSecureId& aSid, TInt aScriptId );
    void ConstructL( const TDesC& aFeatureId );

public: // Data

    /** Secure ID of the client */
    TSecureId iSid;
    
    /** Script ID of the client (can be KErrNotFound)*/
    TInt iScriptId;
    
    /** Feature ID of the client (can be empty)*/
    TPtrC iFeatureId;

private: // Data

    /** Descriptor containing the featuer id */
    HBufC* iFeatureIdDesc;
    };
    
// Sensor source client list
typedef RPointerArray<CSensorSourceClient> RSensorSourceClientList;

#endif // SENSORSOURCECLIENT_H

