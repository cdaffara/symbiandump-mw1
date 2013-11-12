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
* Description: This file contains testclass implementation.
*
*/

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "xmppsettings_api.h"
#include <xmppservicesettingsapi.h>
#include<centralrepository.h>

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;
const TUid  KXmppSettingsApiCenRep         ={0x1020e334};
_LIT( KParamName, "XMPP-PARAM-NUMBER-0x%08d");
_LIT( KParamValue16, "param value - 0x%08d");
// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/
void Cxmppsettings_api::ResetRepository()
    {
    CRepository* rep = CRepository::NewL( KXmppSettingsApiCenRep );
    User::LeaveIfError( rep->Reset() );
    delete rep;
    }
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Cxmppsettings_api::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Cxmppsettings_api::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Cxmppsettings_api::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Cxmppsettings_api::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Test_CreateSettingsRecordL", Cxmppsettings_api::Test_CreateSettingsRecordL ),
        ENTRY( "Test_DefaultSettingsRecord", Cxmppsettings_api::Test_DefaultSettingsRecord ),
        ENTRY( "Test_GetSettingsRecordIdArray", Cxmppsettings_api::Test_GetSettingsRecordIdArray ),
        ENTRY( "Test_SetParamInt", Cxmppsettings_api::Test_SetParamInt ),
        ENTRY( "Test_SetParamLDes", Cxmppsettings_api::Test_SetParamLDes ),
        ENTRY( "Test_GetParamInt", Cxmppsettings_api::Test_GetParamInt ),
        ENTRY( "Test_GetParamLDes", Cxmppsettings_api::Test_GetParamLDes ),
        ENTRY( "Test_RemoveSettingsRecord", Cxmppsettings_api::Test_RemoveSettingsRecord ),
        ENTRY( "Test_RemoveParam", Cxmppsettings_api::Test_RemoveParam ),
        ENTRY( "Test_GetRepositoryKeyL", Cxmppsettings_api::Test_GetRepositoryKeyL ),

        
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// Cxmppsettings_api::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cxmppsettings_api::Test_CreateSettingsRecordL( CStifItemParser& aItem )
    {
    
    ResetRepository();
     iApi->CreateSettingsRecordL( iSettingId );
     if(0==iSettingId)
         {
         return KErrGeneral;
         }
    
    RArray<TUint32> array;

    iApi->GetSettingsRecordIdArrayL( array );
    TInt arraycount = array.Count();
    if(0==arraycount)
        {
        return KErrGeneral;
        }
    if(iSettingId!=array[0])
        {
        return KErrGeneral;
        }
    array.Reset();
    array.Close();
    ResetRepository();
    TInt count = 5;
    for (TInt i=0; i< count; i++ )
        {
        iApi->CreateSettingsRecordL( iSettingId ) ;
        }
    iApi->GetSettingsRecordIdArrayL( array );
    arraycount = array.Count();
    if(count!=arraycount)
        {
        return KErrGeneral;
        }
    array.Reset();
    array.Close();
    ResetRepository();
    return KErrNone;
        }

TInt Cxmppsettings_api::Test_DefaultSettingsRecord( CStifItemParser& aItem )
    {
    ResetRepository();
    iApi->CreateSettingsRecordL( iSettingId );
    TUint32 settingId;
    iApi->DefaultSettingsRecordL( settingId );
    if(settingId!=iSettingId)
        {
        return KErrGeneral;
        }
    ResetRepository();
    return KErrNone;
    
    }

TInt Cxmppsettings_api::Test_GetSettingsRecordIdArray( CStifItemParser& aItem )
    {
    
    ResetRepository();
     iApi->CreateSettingsRecordL( iSettingId );
     if(0==iSettingId)
         {
         return KErrGeneral;
         }
    
    RArray<TUint32> array;

    iApi->GetSettingsRecordIdArrayL( array );
    TInt arraycount = array.Count();
    if(0==arraycount)
        {
        return KErrGeneral;
        }
    if(iSettingId!=array[0])
        {
        return KErrGeneral;
        }
    array.Reset();
    array.Close();
    ResetRepository();
    TInt count = 5;
    for (TInt i=0; i< count; i++ )
        {
        iApi->CreateSettingsRecordL( iSettingId ) ;
        }
    iApi->GetSettingsRecordIdArrayL( array );
    arraycount = array.Count();
    if(count!=arraycount)
        {
        return KErrGeneral;
        }
    array.Reset();
    array.Close();
    ResetRepository();
    return KErrNone;
        }
TInt Cxmppsettings_api::Test_SetParamInt( CStifItemParser& aItem )
    {
    ResetRepository();
    iApi->CreateSettingsRecordL( iSettingId );

    _LIT( KParamName, "XMPP-PARAM-NUMBER-0x%08d");

    TInt count = 128; // 0xFF
    TInt err;
   for (TInt i=0; i < count; i++ )
    {
    HBufC* paramName = HBufC::NewL( KParamName().Length()+20 );
    paramName->Des().AppendFormat( KParamName(), i );

    TInt paramValue(i);

    TRAP(err,iApi->SetParamL( iSettingId, *paramName, paramValue));
    if(err!=KErrNone)
        {
        ResetRepository();
        return KErrGeneral;
        }

    delete paramName;
    }
    ResetRepository();
    return KErrNone;
    }

TInt Cxmppsettings_api::Test_SetParamLDes( CStifItemParser& aItem )
    {
    TInt err;
    ResetRepository();
    iApi->CreateSettingsRecordL( iSettingId );

    TInt count = 128; // 0xFF

    for (TInt i=0; i < count; i++ )
        {
        HBufC* paramName = HBufC::NewL( KParamName().Length()+20 );
        paramName->Des().AppendFormat( KParamName(), i );

        HBufC* paramValue = HBufC::NewL( KParamValue16().Length()+20 );
        paramValue->Des().AppendFormat( KParamValue16(), i );

        TRAP(err,iApi->SetParamL( iSettingId, *paramName, *paramValue));
        if(err!=KErrNone)
           {
            ResetRepository();
            delete paramName;
            delete paramValue;
            return KErrGeneral;
                }

        delete paramName;
        delete paramValue;
        }
    ResetRepository();
    return KErrNone;
    
    }

TInt Cxmppsettings_api::Test_GetParamInt( CStifItemParser& aItem )
    {
    ResetRepository();
    iApi->CreateSettingsRecordL( iSettingId );

    _LIT( KParamName, "XMPP-PARAM-NUMBER-0x%08d");

    TInt count = 128; // 0xFF
    TInt err;
   for (TInt i=0; i < count; i++ )
    {
    HBufC* paramName = HBufC::NewL( KParamName().Length()+20 );
    paramName->Des().AppendFormat( KParamName(), i );

    TInt paramValue(i);

    TRAP(err,iApi->SetParamL( iSettingId, *paramName, paramValue));
    if(err!=KErrNone)
        {
        ResetRepository();
        return KErrGeneral;
        }

    delete paramName;
    }
   for ( TInt i=0; i < count; i++ )
           {
           HBufC* paramName = HBufC::NewL( KParamName().Length()+20 );
           paramName->Des().AppendFormat( KParamName(), i );

           TInt paramValue(0);

           iApi->GetParamL( iSettingId,*paramName,paramValue);

           if( i!= paramValue)
               {
               return KErrGeneral;
               }

           delete paramName;
           }
    ResetRepository();
    return KErrNone;
    }
    
TInt Cxmppsettings_api::Test_GetParamLDes( CStifItemParser& aItem )
    {
    ResetRepository();
    iApi->CreateSettingsRecordL( iSettingId );

    _LIT( KParamName, "XMPP-PARAM-NUMBER-0x%08d");

    TInt count = 128; // 0xFF
    TInt err;
   for (TInt i=0; i < count; i++ )
    {
    HBufC* paramName = HBufC::NewL( KParamName().Length()+20 );
    paramName->Des().AppendFormat( KParamName(), i );

    HBufC* paramValue = HBufC::NewL( KParamValue16().Length()+20 );
    paramValue->Des().AppendFormat( KParamValue16(), i );

    TRAP(err,iApi->SetParamL( iSettingId, *paramName, *paramValue));
    if(err!=KErrNone)
       {
        ResetRepository();
        delete paramName;
        delete paramValue;
        return KErrGeneral;
            }

    delete paramName;
    delete paramValue;
    }
   for ( TInt i=0; i < count; i++ )
           {
           HBufC* paramName = HBufC::NewL( KParamName().Length()+20 );
           paramName->Des().AppendFormat( KParamName(), i );

           HBufC* paramValue = HBufC::NewL( KParamValue16().Length()+20 );
           paramValue->Des().AppendFormat( KParamValue16(), i );

           RBuf buf;
           buf.CreateL( 255 );

           TRAP(err,iApi->GetParamL( iSettingId,*paramName, buf) );
           
           if(err!=KErrNone)
               {
               return KErrGeneral;
               }
           //*paramValue, buf, ;

           delete paramName;
           delete paramValue;

           buf.Close();
           }
    ResetRepository();
    return KErrNone;
    }

TInt Cxmppsettings_api::Test_RemoveSettingsRecord( CStifItemParser& aItem )
    {
    ResetRepository();
    iApi->CreateSettingsRecordL( iSettingId );
    iApi->RemoveSettingsRecordL( iSettingId );

    RArray<TUint32> array;

    iApi->GetSettingsRecordIdArrayL( array );
    TInt arraycount = array.Count();
    if(1==arraycount)
        {
        return KErrGeneral;
        }

    array.Reset();

    iSettingId = 0;

    ResetRepository();

    TInt count = 128;

    for (TInt i=1; i< count; i++ )
        {
        iApi->CreateSettingsRecordL( iSettingId);
        }

    // because cen rep is reseted after every test, indexes should start
    // from zero and go up to count. So its safe to assume that these
    // indexes exist :)

    iApi->RemoveSettingsRecordL( 1 );
    iApi->RemoveSettingsRecordL( 55 );
    iApi->RemoveSettingsRecordL( 123 );
    iApi->RemoveSettingsRecordL( 96 ) ;
    iApi->RemoveSettingsRecordL( 2 );

    iApi->GetSettingsRecordIdArrayL( array );
    arraycount = array.Count();
    if((count-6)!=arraycount)
        {
        return KErrGeneral;
        }

    for( TInt i=0; i < array.Count(); i++ )
        {
         iApi->RemoveSettingsRecordL( array[i] );
        }

    array.Reset();
    iApi->GetSettingsRecordIdArrayL( array );
    if( 0!=(array.Count()))
        {
        return KErrGeneral;
        }
    array.Reset();
    array.Close();
    ResetRepository();
    return KErrNone;
      
    }
TInt Cxmppsettings_api::Test_RemoveParam( CStifItemParser& aItem )
    {


    ResetRepository();
    iApi->CreateSettingsRecordL( iSettingId );

    _LIT( KParamName, "XMPP-PARAM-NUMBER-0x%08d");

    TInt count = 128; // 0xFF
    TInt err;
   for (TInt i=0; i < count; i++ )
    {
    HBufC* paramName = HBufC::NewL( KParamName().Length()+20 );
    paramName->Des().AppendFormat( KParamName(), i );

    TInt paramValue(i);

    TRAP(err,iApi->SetParamL( iSettingId, *paramName, paramValue));
    if(err!=KErrNone)
        {
        ResetRepository();
        return KErrGeneral;
        }

    delete paramName;
    }
   
    count = 128; // 0xFF

    TInt param = 56;

        HBufC* paramName = HBufC::NewL( KParamName().Length()+20 );
        paramName->Des().AppendFormat( KParamName(), param );

        iApi->RemoveParamL( iSettingId,*paramName);
        TInt paramvalue;
        
        TRAP(err,iApi->GetParamL( iSettingId,*paramName,paramvalue));
        if(err!= -1)
            {
            return KErrGeneral;
            }
        iApi->SetParamL( iSettingId,*paramName,12345);
        delete paramName;

    for (TInt i=0; i < count; i++ )
        {
        HBufC* paramName = HBufC::NewL( KParamName().Length() + 20 );
        paramName->Des().AppendFormat( KParamName(), i );

        iApi->RemoveParamL( iSettingId,*paramName);

        delete paramName;
        }


    ResetRepository();
    return KErrNone;
    
    
    }
TInt Cxmppsettings_api::Test_GetRepositoryKeyL( CStifItemParser& aItem )
    {
    ResetRepository();
    iApi->CreateSettingsRecordL( iSettingId );

    _LIT( KParamName, "XMPP-PARAM-NUMBER-0x%08d");

    TInt count = 128; // 0xFF
    TInt err;
   for (TInt i=0; i < count; i++ )
    {
    HBufC* paramName = HBufC::NewL( KParamName().Length()+20 );
    paramName->Des().AppendFormat( KParamName(), i );

    HBufC* paramValue = HBufC::NewL( KParamValue16().Length()+20 );
    paramValue->Des().AppendFormat( KParamValue16(), i );

    TRAP(err,iApi->SetParamL( iSettingId, *paramName, *paramValue));
    if(err!=KErrNone)
       {
        ResetRepository();
        delete paramName;
        delete paramValue;
        return KErrGeneral;
            }
    TBool CreateColumnIfNotExist = EFalse ;
    TUint32 TheKey;
    TRAP(err,iApi->GetRepositoryKeyL( iSettingId,
                                   *paramName,
                                   CreateColumnIfNotExist,
                            TheKey ));
    delete paramName;
    delete paramValue;
    }
   for ( TInt i=0; i < count; i++ )
           {
           HBufC* paramName = HBufC::NewL( KParamName().Length()+20 );
           paramName->Des().AppendFormat( KParamName(), i );

           HBufC* paramValue = HBufC::NewL( KParamValue16().Length()+20 );
           paramValue->Des().AppendFormat( KParamValue16(), i );

           RBuf buf;
           buf.CreateL( 255 );

           TRAP(err,iApi->GetParamL( iSettingId,*paramName, buf) );
           
           if(err!=KErrNone)
               {
               return KErrGeneral;
               }
           //*paramValue, buf, ;

           delete paramName;
           delete paramValue;

           buf.Close();
           }
    ResetRepository();
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// Cxmppsettings_api::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt Cxmppsettings_api::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
