/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Initializer
*
*/


#ifndef SENSORDATACOMPENSATORINITIALIZER_H
#define SENSORDATACOMPENSATORINITIALIZER_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CSensorDataCompensatorPlg;
// CLASS DECLARATION

/**
*  Initializer state listener implementation.
*
*  sensordatacompensatorplugin.lib
*  @since S60 5.1
*/
NONSHARABLE_CLASS( CSensorDataCompensatorInitializer ) : public CActive
    {
    public: // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~CSensorDataCompensatorInitializer();

        /**
        * Two-phased constructor.
        */
        static CSensorDataCompensatorInitializer* NewL( CSensorDataCompensatorPlg& aPlg );

    protected: // New functions

        /**
        * C++ default constructor.
        */
        CSensorDataCompensatorInitializer( CSensorDataCompensatorPlg& aPlg );

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL();

        /**
        * Retry callback for ContextFramework initialisation
        *
        * @param aPtr: pointer to CSensorDataCompensatorInitializer
        * @return Error code
        */
        static TInt RetryCB( TAny* aPtr );

        /**
        * ContextFramework initialisation
        *
        * @param None
        * @return None
        */
        void InitCfw();

    protected:  // Functions from base classes

		//from CActive
		void DoCancel();

		void RunL();

	    TInt RunError(TInt aError);

    private:

        // not owned
        CSensorDataCompensatorPlg& iPlg;

        //owned
        RProperty iProperty;

        // owned
        CPeriodic* iPeriodic;

        // not owned
        TBool* iDeleteInd;
    };

#endif // SENSORDATACOMPENSATORINITIALIZER_H

// End of File
