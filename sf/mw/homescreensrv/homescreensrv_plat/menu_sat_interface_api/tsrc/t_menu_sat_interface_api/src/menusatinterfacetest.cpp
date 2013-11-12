/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/
// INCLUDE FILES
#include <MenuSatInterface.h>
#include "menusatinterfacetest.h"


//  CONSTANTS
_LIT( KSatName, "Plus Mega" );

// ============================ MEMBER FUNCTIONS ===============================


void CMenuSatInterfaceTest::Test_SatUiVisibleL()
    {
    TInt result (KErrGeneral);
    
    TBool visible = ETrue;
	TRAP(result,
        CMenuSATInterface* interface = new(ELeave) CMenuSATInterface();
        CleanupStack::PushL( interface );    

	    interface->MakeSatUiVisible( visible, KSatName );
        CleanupStack::PopAndDestroy( interface );
        );
    TBuf<256> name; 
    TInt error = RProperty::Get( KCRUidCa, KCaSatUIName, name );
    
  	if( name == KSatName() 
  			&&  error == KErrNone )
  		{
  		result = KErrNone;
  		}
    QVERIFY(result == KErrNone);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CMenuSatInterfaceTest::Test_SatUiVisibleIconIdL()
    {
    TInt result (KErrGeneral);
    
    TBool visible = ETrue;
    TUint8 iconInfoId = 21;
	TRAP(result,
        CMenuSATInterface* interface = new(ELeave) CMenuSATInterface();
        CleanupStack::PushL( interface );    
        interface->MakeSatUiVisible( visible, KSatName, iconInfoId );
        CleanupStack::PopAndDestroy( interface );
    );
    TInt iconInfoIdTest = -1;
    RProperty::Get( KCRUidCa, KCaSatUIIconId, iconInfoIdTest );
    
  	if( iconInfoIdTest == iconInfoId )
  		{
  		result = KErrNone;
  		}
    QVERIFY(result == KErrNone);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CMenuSatInterfaceTest::Test_SatInterfaceConstructL()
    {
    TInt result (KErrGeneral);
    
    TRAP(result, 
	    CMenuSATInterface* interface = new(ELeave) CMenuSATInterface();
  	    if( interface )
  		    {
  		    result = KErrNone;
  		    }
    );
    QVERIFY(result == KErrNone);
    }

QTEST_MAIN(CMenuSatInterfaceTest)
//  End of File
