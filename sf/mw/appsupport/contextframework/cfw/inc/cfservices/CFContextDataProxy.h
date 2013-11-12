/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextDataProxy class declaration.
*
*/


#ifndef C_CFCONTEXTDATAPROXY_H
#define C_CFCONTEXTDATAPROXY_H

#include <cfcontextdataobject.h>

// CONSTANTS
const TUid KContextDataProxyUid = { 0x2000B4CF };

/**
* This class is intended to use to transmit data object send from
* a client process to all interested subscribers. The reason for this
* is that a context data sent from a client process need to be internalized
* in to object before it can be published. Since cfserver don't know and
* does not need to know the corret object CCFContextDataProxy is used to
* transmit the data which can be then processed and set in a context
* indication.
*
* Do not use this class for any other purposes!
*
* @internal
* @since S60 5.0
* @lib None
*/
NONSHARABLE_CLASS( CCFContextDataProxy ): public CCFContextDataObject
    {
    public:
    
        // Two phased constructors
        IMPORT_C static CCFContextDataProxy* NewL();
        IMPORT_C static CCFContextDataProxy* NewLC();
        
        // Destructor
        ~CCFContextDataProxy();
        
    public: // New methods
    
        /**
        * Set context data uid.
        *
        * @since S60 5.0
        * @param aUid The context data UID.
        * @return None.
        */
        IMPORT_C void SetContextDataUid( const TUid& aUid );
    
        /**
        * Set context data object.
        * 
        * @since S60 5.0
        * @param aData Streamed context data.
        * @return None.
        */
        IMPORT_C void SetContextData( HBufC8* aData );
    
    private: // From base class CCFContextDataObject
    
        // @see CCFContextDataObject
        const TUid& Uid() const;
        
        // @see CCFContextDataObject
        void ExternalizeL( RWriteStream& aStream );

        // @see CCFContextDataObject
        void InternalizeL( RReadStream& aStream );

        // @see CCFContextDataObject
        TInt Size() const;

    private:

        CCFContextDataProxy();
        void ConstructL();
        
    private: // Data
    
        /** Context data */
        HBufC8* iData;
        
        /** Context data uid */
        TUid iUid;
    };

#endif
