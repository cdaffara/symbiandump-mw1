/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApCenRepController class definition. 
*
*/


#ifndef SYSAPCENREPCONTROLLER_H
#define SYSAPCENREPCONTROLLER_H

// INCLUDES
#include <e32base.h>
#include <centralrepository.h>
#include "SysAp.hrh"

// CLASS DECLARATION

/**
*  CSysApCenRepController
*
*  @lib   sysap
*  @since 3.0
*/

class CSysApCenRepController : public CBase
    {
    public: // Constructors and destructor
        /**
        * Default constructor
        */
        static CSysApCenRepController* NewL();

        /**
        * Destructor.
        */
        virtual ~CSysApCenRepController();

    private:

        /**
        * EPOC default constructor.
        * @param None
        * @return void
        */
        void ConstructL();
        
        /**
        * C++ default constructor.
        * @param None
        * @return void
        */
        CSysApCenRepController();
        
    public:

        /**
        * Returns the value of a CenRep integer key
        * @param TUid aRepository
        * @param TUint32 aKey
        * @param [out] aErrorReturn if ETrue, the return value contains an error value due to
        *                           CenRep read failure
        * @return void
        */         
        TInt GetInt( const TUid aRepository, const TUint32 aKey, TBool* aErrorReturn = NULL );

        /**
        * Updates a CenRep integer key
        * @param TUid aRepository
        * @param TUint32 aKey
        * @param TInt aNewValue
        * @return void
        */         
        void SetInt( const TUid aRepository, const TUint32 aKey, const TInt aNewValue );

        /**
        * Returns the profile id for the selected accessory type.
        * @param aAccessoryType accessory type
        * @return TInt profile id
        */         
        TInt ProfileIdForSelectedAccessory( TSysApAccessoryType aAccessoryType );
        
    private:
        // By default, prohibit copy constructor
        CSysApCenRepController( const CSysApCenRepController& );
    
        // Prohibit assigment operator
        CSysApCenRepController& operator= ( const CSysApCenRepController& );
    
    private:
        CRepository*          iSysApRepository;
        CRepository*          iProfileSettingsRepository;
    };

#endif      // SYSAPCENREPCONTROLLER_H
            
// End of File
