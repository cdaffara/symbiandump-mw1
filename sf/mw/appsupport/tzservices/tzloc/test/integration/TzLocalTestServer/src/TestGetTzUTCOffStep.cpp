// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include <test/testexecutelog.h>
#include "TestGetTzUTCOffStep.h"

using namespace tzconstants;

CTestGetTzUTCOffStep::~CTestGetTzUTCOffStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestGetTzUTCOffStep::CTestGetTzUTCOffStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestGetTzUTCOffStep); 
	}


TVerdict CTestGetTzUTCOffStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
    {  
    if(!GetIntFromConfig(ConfigSection(),KUtcOffsetTag,iUtcOffset))
        {
        _LIT(KNoUtcOffset,"TestTzUTCOffset : No UTC Offset in ini");
    	ERR_PRINTF1(KNoUtcOffset);
        }       
        
    return TestStepResult(); 	 
    }
    
   
TVerdict CTestGetTzUTCOffStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
    {            
 	SetTestStepResult(EPass);
    TRAPD(err, iLocalizer = CTzLocalizer::NewL());
    __UHEAP_MARK;
    
    if (err == KErrNone)
        {
        CTzLocalizedTimeZoneArray* tz = NULL;
        TRAPD(res,tz = iLocalizer->GetTimeZonesWithUTCOffsetL(iUtcOffset,CTzLocalizer::ETzAlphaStandardNameAscending));
        CleanupStack::PushL(tz);
   
        if (res == KErrNone)
            {
            TBuf<KTagLength>tzTag;
        	TInt numTz = tz->Count();
        	for(TInt n=0; n<numTz; n++)
        	    {
        		tzTag = KStandardNameTag;
        		tzTag.AppendNum(n+1);
        		TPtrC tzFromIni;
        		if (!GetStringFromConfig(ConfigSection(),tzTag,tzFromIni))//i.e.(tz->At(n).StandardName() != tzFromIni)
        		    {
        		    SetTestStepResult(EFail);
        			ERR_PRINTF1(tzTag );
        			_LIT(KNoMatch,"CTestGetTzUTCOffStep: Tz Name Did not Match, expected: %S from inifile");
             		_LIT(KSX1,"CTestGetTzUTCOffStep: Tz StandardName was %S !");
					TPtrC temp(tz->At(n).StandardName() );
				
					if ( (&temp)->Length()!=0  )
 						{
						INFO_PRINTF2(KSX1, &temp );
  						if ( (&tzFromIni)->Length()!=0 )
							{
	                		ERR_PRINTF2(KNoMatch, &tzFromIni);		
							}
						}
					else if ( (&tzFromIni)->Length()!=0 )
							{
	                		ERR_PRINTF2(KNoMatch, &tzFromIni);		
							}
        		    }       		
        	    }
            }
        else
            {
            SetTestStepResult(EFail);
        	_LIT(KGetTzLeft,"TestGetTzUTCOffStep: GetTimeZoneWithUTCOffsetL Left");
		    ERR_PRINTF1(KGetTzLeft);
            }
            
        CleanupStack::PopAndDestroy(tz);
               
        }
    else
	    {
	    SetTestStepResult(EFail);
	    _LIT(KNewLLeft,"TestGetTzUTCOffStep: Problem with NewL");
		ERR_PRINTF1(KNewLLeft);
	    }
	
	__UHEAP_MARKEND;    
	return TestStepResult();	
    }

TVerdict CTestGetTzUTCOffStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}

