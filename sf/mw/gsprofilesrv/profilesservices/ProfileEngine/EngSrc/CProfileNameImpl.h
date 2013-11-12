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
* Description:  Profile name implementation.
*
*/



#ifndef __CPROFILENAMEIMPL_H
#define __CPROFILENAMEIMPL_H

//  INCLUDES
#include <MProfileName.h>
#include <e32base.h>
#include <MProfileSetName.h>

// FORWARD DECLARATIONS
class RWriteStream;
class MProfilesNamesArray;
class CRepository;
// CLASS DECLARATION

/**
*  Profile name implementation.
*  This class implements MProfileName and MProfileSetName interfaces.
*
*  @lib ?library
*  @since 2.0
*/
NONSHARABLE_CLASS(CProfileNameImpl) : public CBase,
                         public MProfileName,
                         public MProfileSetName
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CProfileNameImpl* NewL();

        /**
        * Two-phased constructor.
        * @param aProfileId Profile ID
        * @param aProfileName Profile name
        */
        static CProfileNameImpl* NewLC(
            TInt aProfileId, const TDesC& aProfileName );

        /**
        * Two-phased constructor.
        * @param aProfileId Profile ID
        * @param aProfileName Profile name
        * @param aShortName Profile short name
        * @param aNaviName Profile short name
        */
        static CProfileNameImpl* NewLC(
            TInt aProfileId,
            const TDesC& aProfileName,
            const TDesC& aShortName,
            const TDesC& aNaviName,
            const TDesC& aTitleName );

        /**
        * Two-phased copy-constructor.
        * @param aProfileNameImpl profile name implementation to be copied.
        */
        static CProfileNameImpl* NewLC(
                const CProfileNameImpl& aProfileNameImpl );
            
        /**
        * Destructor.
        */
        virtual ~CProfileNameImpl();

    public: // New functions

        /**
        * Is profile names changed.
        * @since 2.0
        * @return ETrue if names is changed
        */
        TBool IsProfileNameChanged() const;

        /**
        * Set localised profile name.
        * @since 2.0
        * @param aNameArray Profiles names array
        */
        void SetLocalizedProfileNameL( const MProfilesNamesArray& aNameArray );

        /**
        * Set localised profile name.
        * @since 3.1
        * @param aNameImpl Profiles name impl from which the name and short name
        *        are copied into this Profile Name.
        * @param aUniquePart contains the part which is used to differentiate
        *        the automatically generated names from each other. This is
        *        merged in the name.
        */
        void SetLocalizedProfileNameL( const CProfileNameImpl& aNameImpl,
                                       const TDesC& aUniquePart );
    
        /**
        * Internalize data from the central repository.
        * @since 2.8
        * @param aCenRep Central Repository object to read the settings
        * @param aProfileId the id of the profile to be internalized
        */
        void InternalizeL( CRepository& aCenRep, TInt aProfileId );

        /**
        * Externalize data to the stream.
        * @since 2.0
        * @param aStream Write stream
        */
        void ExternalizeL( CRepository& aCenRep ) const;

    public: // Functions from base classes

        /**
        * From MProfileName.
        */
        virtual const TDesC& Name() const;

        /**
        * From MProfileName.
        */
        virtual TInt Id() const;

        /**
        * From MProfileSetName.
        */
        virtual void SetNameL( const TDesC& aName );

        /**
        * From MProfileName.
        */
        virtual const TDesC& ShortName() const;

        /**
        * From MProfileName.
        */
        virtual const TDesC& NaviName() const;

        /**
        * From MProfileName.
        */
        virtual const TDesC& TitleName() const;

    protected: // New functions

        /**
        * Set profile name, do not set iNameChanged.
        * @since 3.2
        * @param aName Profile name
        */
        void SetNameInternalL( HBufC*& aNameFieldRef, const TDesC& aName );

    private:

        /**
        * C++ default constructor.
        * @param aProfileId Profile ID
        */
        CProfileNameImpl( TInt aProfileId );

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aProfileName Profile name
        */
        void ConstructL( const TDesC& aProfileName );

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aProfileName Profile name
        * @param aShortName Profile short name
        * @param aNaviName Profile navi name
        */
        void ConstructL( const TDesC& aProfileName, const TDesC& aShortName,
                         const TDesC& aNaviName, const TDesC& aTitleName );

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aProfileNameImpl a profile name implementation to be copied.
        */
        void ConstructL( const CProfileNameImpl& aProfileNameImpl );
            
    private:    // Data

        // Own: Is name changed
        TBool iNameChanged;

        // Own: Profile ID
        TInt iId;

        // Own: Profile Name
        HBufC* iProfileName;

        // Own: Profile Name for navi pane with tabs
        HBufC* iShortName;

        // Own: Profile Name for navi pane
        HBufC* iNaviName;

        // Own: Profile Name for title pane
        HBufC* iTitleName;

    };

#endif      // __CPROFILENAMEIMPL_H

// End of File
