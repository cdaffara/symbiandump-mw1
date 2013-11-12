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
* Description:  Resource reader for key event map.
*
*/


#ifndef KEFRESOURCE_H
#define KEFRESOURCE_H

// INCLUDES
#include <e32base.h>
#include <e32cons.h>
#include <barsread.h>

// CLASS DECLARATIONS

/**
* This class parses one KEF_KEY_EVENT from the resource file.
*
* @lib KeyEventFw.lib
* @since Series 60 3.2
*/
NONSHARABLE_CLASS( CKefResource )
    : public CBase 
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aReader resource reader.
        */
        static CKefResource* NewLC( TResourceReader& aReader );

        /**
        * Destructor.
        */
        ~CKefResource();
    
    public: // New functions

        /**
        * Returns the array containing scan codes for one KEF_KEY_EVENT.
        * @return reference to array of scan codes.
        */
        CArrayFixFlat<TUint16>& GetScanCodes();

        /**
        * Returns the array containing key codes for one KEF_KEY_EVENT.
        * @return  reference to array of key codes.
        */
        CArrayFixFlat<TUint16>& GetKeyCodes();

        /**
        * Returns the array containing device modes for one KEF_KEY_EVENT.
        * @return reference to array of device modes.
        */
        CArrayFixFlat<TUint16>& GetDeviceModes();

        /**
        * Returns the array containing event types for one KEF_KEY_EVENT.
        * @return reference to array of event types.
        */
        CArrayFixFlat<TUint8>& GetEventTypes();
        
        /**
        * Returns the feedback type for one KEF_KEY_EVENT.
        * @return feedback type value.
        * Only in use if RD_TACTILE_FEEDBACK is turned on.
        */
        TUint16 GetFeedbackType() const;


    private:
        
        /**
        * Symbian OS constructor.
        * @param aReader resource reader.
        */
        void ConstructL( TResourceReader& aReader );

        /**
        * Reads the scan code array for one KEF_KEY_EVENT in the resources.
        * @param aReader resource reader.
        */
        void AddScanCodesL( TResourceReader& aReader );

        /**
        * Reads the key code array for one KEF_KEY_EVENT in the resources.
        * @param aReader resource reader.
        */
        void AddKeyCodesL( TResourceReader& aReader );

        /**
        * Reads the device mode array for one KEF_KEY_EVENT in the resources.
        * @param aReader resource reader.
        */
        void AddDeviceModesL( TResourceReader& aReader );

        /**
        * Reads the event type array for one KEF_KEY_EVENT in the resources.
        * @param aReader resource reader.
        */
        void AddEventTypesL( TResourceReader& aReader );

        /**
        * Reads the feedback type for one KEF_KEY_EVENT in the resources.
        * @param aReader resource reader.
        * Only in use if RD_TACTILE_FEEDBACK is turned on.
        */
        void AddFeedbackTypeL( TResourceReader& aReader );

    private:
        
        // Owned array of scan codes.
        CArrayFixFlat<TUint16>* iScanCodes;
        
        // Owned array of key codes.
        CArrayFixFlat<TUint16>* iKeyCodes;

        // Owned array of device modes.
        CArrayFixFlat<TUint16>* iDeviceModes;

        // Owned array of event types.
        CArrayFixFlat<TUint8>* iEventTypes;

        // Feedback type. Only in use if RD_TACTILE_FEEDBACK is turned on.
        TUint16 iFeedbackType;

    };
    
/**
* Array of CKefResource instances.
*
* @lib KeyEventFw.lib
* @since Series 60 3.2
*/
NONSHARABLE_CLASS( CKefResourceArray )
    : public CBase
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CKefResourceArray* NewLC();
        
        /**
        * Destructor.
        */
        ~CKefResourceArray();
    
    public: // New functions

        /**
        * Returns a pointer array containing all the KEF_KEY_EVENTS from
        * KEF_KEY_EVENT_MAP of the resource file.  
        *
        * @param aReader resource reader.
        * @return Key table from resource file.
        */
        CArrayPtrFlat<CKefResource>* GetKeyEventsL(
                TResourceReader& aReader );

    private:

        /**
        * Symbian OS constructor.
        */
        void ConstructL();

    private:

        // Owned array of key resources.
        CArrayPtrFlat< CKefResource >* iKeyEvents;

    };

#endif // KEFRESOURCE_H

// End of File
