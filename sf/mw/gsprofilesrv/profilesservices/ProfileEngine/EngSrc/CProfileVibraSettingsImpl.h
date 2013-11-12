/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Profile PTT settings implementation.
*
*/



#ifndef __CPROFILEVIBRASETTINGSIMPL_H
#define __CPROFILEVIBRASETTINGSIMPL_H

//  INCLUDES
#include <e32base.h>
#include <MProfileVibraSettings.h>
#include <MProfileSetVibraSettings.h>

// CLASS DECLARATION

class CRepository;

/**
*  Profile vibra settings implementation.
*
*  @lib ?library
*  @since 10.1
*/
NONSHARABLE_CLASS(CProfileVibraSettingsImpl) : public CBase,
                               public MProfileVibraSettings,
                               public MProfileSetVibraSettings
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CProfileVibraSettingsImpl* NewL();

        /**
        * Constructor.
        */
        CProfileVibraSettingsImpl();
        
        /**
        * Destructor.
        */
        virtual ~CProfileVibraSettingsImpl();

    public: // New functions

        /**
        * Internalize data from the central repository.
        * @since 3.0
        * @param aCenRep Central Repository object to read the settings
        * @param aProfileId the id of the profile to be internalized
        */
        void InternalizeL( CRepository& aCenRep, TInt aProfileId );

        /**
        * Externalize data to Central Repository.
        * @since 3.0
        * @param aCenRep Central Repository object to write the settings
        */
        void ExternalizeL( CRepository& aCenRep, TInt aProfileId ) const;

    public: // Functions from base classes

        /**
        * From MProfileVibraSettings
        */
        virtual TInt AlertVibra() const;
        
        /**
        * From MProfileVibraSettings
        */
        virtual TBool RingAlertVibra() const;
        
        /**
        * From MProfileVibraSettings
        */  
        virtual TBool MessageAlertVibra() const;
        
        /**
        * From MProfileVibraSettings
        */
        virtual TBool EmailAlertVibra() const;
        
        /**
        * From MProfileVibraSettings
        */
        virtual TBool ReminderAlarmVibra() const;
        
        /**
        * From MProfileVibraSettings
        */
        virtual TBool InformationVibra() const;
        
        
        /**
        * From MProfileSetVibraSettings
        */
        virtual void SetAlertVibra(TInt aAlertVibra);
        
        /**
        * From MProfileSetVibraSettings
        */
        virtual void SetRingAlertVibra(TBool aRingAlertVibra);
        
        /**
        * From MProfileSetVibraSettings
        */  
        virtual void SetMessageAlertVibra(TBool aMessageAlertVibra);
        
        /**
        * From MProfileSetVibraSettings
        */
        virtual void SetEmailAlertVibra(TBool aEmailAlertVibra);
        
        /**
        * From MProfileSetVibraSettings
        */
        virtual void SetReminderAlarmVibra(TBool aReminderAlarmVibra);
        
        /**
        * From MProfileSetVibraSettings
        */
        virtual void SetInformationVibra(TBool aInformationVibra);

    private:

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Data

        TInt iAlertVibra;

    };

#endif //__CPROFILEVIBRASETTINGSIMPL_H

//End of File
