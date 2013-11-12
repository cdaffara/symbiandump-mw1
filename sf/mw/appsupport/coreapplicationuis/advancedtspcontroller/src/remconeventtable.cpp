/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implements Target Selector Plugin used by Symbian's Remote 
*				 Control Framework. TSP controls sending of operations between 
*				 Series 60 components and remote targets (e.g. headset).
*
*				 This version doesn't really self specify receiver for 
*				 a message but accepts all receivers suggested by RemCon server.
*
*
*/


// INCLUDE FILES
#include "RemConDebug.h"
#include "remconeventtable.h"
#include <remcon/remcontargetselectorpluginobserver.h>
#include <remconaddress.h>
#include <remcon/clientinfo.h>
#include <remcon/bearersecurity.h>
#include <RemConExtensionApi.h>
#include <ctsydomainpskeys.h>
#include <cfclient.h>
#include <cflistener.h>
#include <cfkeyvaluepair.h>
#include <cfactionindication.h>
#include <cfactionsubscription.h>
#include <cfcontextobject.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRemConTspController::CRemConTspController
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//

CRemConEventTable::CRemConEventTable()
	{
	}
    
// -----------------------------------------------------------------------------
// CRemConTspController::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRemConEventTable* CRemConEventTable::NewL( TInt aTableNum )
    {
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConEventTable::NewL()" );
    CRemConEventTable* self = new( ELeave ) CRemConEventTable();
    CleanupStack::PushL( self );
    self->ConstructL( aTableNum );
    CleanupStack::Pop( self );
    return self;
    }

void CRemConEventTable::ConstructL( TInt aTableNum )
    {
    iTableNum = aTableNum;
	}

// Destructor
CRemConEventTable::~CRemConEventTable()
	{
	COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConEventTable::~CRemConEventTable()" );
	  iRemConEventArray.Reset();
    iRemConExtCallEventArray.Reset();
    iRemConExtPocEventArray.Reset();
    iRemConExtSyncEventArray.Reset();
    iRulesForThisTable.Reset();
	  iDefaultApplicationsForThisTable.Reset();
	  
	  iRemConEventArray.Close();
    iRemConExtCallEventArray.Close();
    iRemConExtPocEventArray.Close();
    iRemConExtSyncEventArray.Close();
    iRulesForThisTable.Close();
    iDefaultApplicationsForThisTable.Close();
    iTransparentApplicationsForThisTable.Close();
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConEventTable::~CRemConEventTable() - return" );
	}

TInt CRemConEventTable::TableNum()
    {
    return iTableNum;
    }

void CRemConEventTable::ClearTable()
    {
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConEventTable::ClearTable()" );
    iRemConEventArray.Reset();
    iRemConExtCallEventArray.Reset();
    iRemConExtPocEventArray.Reset();
    iRemConExtSyncEventArray.Reset();
    iRulesForThisTable.Reset();
    iDefaultApplicationsForThisTable.Reset();
    iTransparentApplicationsForThisTable.Reset();
    COM_TRACE_( "[REMCONTSPCONTROLLER] CRemConEventTable::ClearTable() - return" );
    }

void CRemConEventTable::AddRemConEventToTable( TInt aEvent, RemConEventType aEventType )
    {
    if( aEventType == ERemConEvent )
        {
        iRemConEventArray.Append( aEvent );
        }
    if( aEventType == ERemConExtEventCall )
        {
        iRemConExtCallEventArray.Append( aEvent );
        }
    if( aEventType == ERemConExtEventPoc )
        {
        iRemConExtPocEventArray.Append( aEvent );
        }
    if( aEventType == ERemConExtEventSync )
        {
        iRemConExtSyncEventArray.Append( aEvent );
        }
    }
    
void CRemConEventTable::RemoveRemConEventFromTable( TInt aEvent, RemConEventType aEventType )
    {
    if( aEventType == ERemConEvent )
        {
        iRemConEventArray.Append( aEvent );
        }
    if( aEventType == ERemConExtEventCall )
        {
        iRemConExtCallEventArray.Append( aEvent );
        }
    if( aEventType == ERemConExtEventPoc )
        {
        iRemConExtPocEventArray.Append( aEvent );
        }
    if( aEventType == ERemConExtEventSync )
        {
        iRemConExtSyncEventArray.Append( aEvent );
        }
    }
    
TBool CRemConEventTable::IsRemConEventInTable( TInt aEvent, RemConEventType aEventType )
    {
    TBool retVal( EFalse );
    if( aEventType == ERemConEvent )
        {
        if( iRemConEventArray.Find( aEvent ) != KErrNotFound )
            {
            retVal = ETrue;
            }
        }
    if( aEventType == ERemConExtEventCall )
        {
        if( iRemConExtCallEventArray.Find( aEvent ) != KErrNotFound )
            {
            retVal = ETrue;
            }
        }
    if( aEventType == ERemConExtEventPoc )
        {
        if( iRemConExtPocEventArray.Find( aEvent ) != KErrNotFound )
            {
            retVal = ETrue;
            }
        }
    if( aEventType == ERemConExtEventSync )
        {
        if( iRemConExtSyncEventArray.Find( aEvent ) != KErrNotFound )
            {
            retVal = ETrue;
            }
        }
    return retVal;
    }
    
void CRemConEventTable::GetRulesForThisTable( RArray<TInt>& aArrayToRules )
    {
    TInt count = iRulesForThisTable.Count();
    for( TInt i = 0; i != count; i++ )
        {
        aArrayToRules.Append( iRulesForThisTable[ i ] );
        }
    }
    
void CRemConEventTable::AddRuleForThisTable( TInt aRule )
    {
    iRulesForThisTable.Append( aRule );
    }
    
void CRemConEventTable::RemoveRuleFromThisTable( TInt aRule )
    {
    iRulesForThisTable.Remove( aRule );
    }
    
void CRemConEventTable::AddDefaultApplicationForThisTable( TInt aRule )
    {
    iDefaultApplicationsForThisTable.Append( aRule );
    }
    
void CRemConEventTable::RemoveDefaultApplicationFromThisTable( TInt aRule )
    {
    iDefaultApplicationsForThisTable.Remove( aRule );
    }

void CRemConEventTable::GetDefaultApplicationsForTable( RArray<TInt>& aArrayToRules )
    {
    TInt count = iDefaultApplicationsForThisTable.Count();
    for( TInt i = 0; i != count; i++ )
        {
        aArrayToRules.Append( iDefaultApplicationsForThisTable[ i ] );
        }
    }

void CRemConEventTable::AddDefaultLaunchAppForThisTable( const TInt aUid )
    {
    iUid = TUid::Uid( aUid );
    }

void CRemConEventTable::GetDefaultLaunchAppForTable( TUid& aUid )
    {
    aUid = iUid;
    }    

void CRemConEventTable::AddTransparentApplicationForThisTable( TInt aRule )
    {
    iTransparentApplicationsForThisTable.Append( aRule );
    }

void CRemConEventTable::GetTransparentApplicationsForTable( RArray<TInt>& aArrayToRules )
    {
    TInt count = iTransparentApplicationsForThisTable.Count();
    for( TInt i = 0; i < count; i++ )
        {
        aArrayToRules.Append( iTransparentApplicationsForThisTable[ i ] );
        }
    }

// End of file
