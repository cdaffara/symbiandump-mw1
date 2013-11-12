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
* Description:  Camcorder MMF API Test DLL
*
*/


#ifndef __CAMCTESTTOP_H
#define __CAMCTESTTOP_H


//  INCLUDES

#include <e32base.h>
#include "Camctest.h"
#include <videorecorder.h>


// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION


IMPORT_C  MTest* CreateTopFct ();
class CCamcTestTop : public CTestSuite
{
public:

	CCamcTestTop ();
    ~CCamcTestTop ();


	// A function to collect and return a suite of tests
	 static MTest* suiteL ();


protected:

private:

};




#endif


