
/*
 * Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 * Description :
 *
 */



// INCLUDES
#include <hwrmlightdomaincrkeys.h>
#include <coreapplicationuisdomainpskeys.h>
#include <hal.h>
#include <atcmdpluginresource.rsg>
#include <barsread.h>
#include <barsc.h>
#include <bautils.h>
#include <e32math.h>
#include <e32base.h>
#include <eikenv.h>
#include <apgtask.h>
#include "debug.h"
#include "atcmdplugin.h"

// CONSTANTS
_LIT8( KAtCkpd, "+CKPD" );
_LIT8( KAtCbklt, "+CBKLT" );
_LIT8( KCRLF, "\r\n" );
_LIT8( KAtCtsa, "+CTSA");

const TInt KCustomKeyCodeIdentifier = -1;
const TInt KGranularity = 10;
const TInt KTimeMultiplier = 100000;
const TInt KMicroSeconds = 1000000;

#define REPLY_BUFFER_SIZE 200

// These values are based on testing results.
// Values less than 2000 causes some times missing key events
// even all keys are simulated with long commands.
const TInt KDefaultStrokeDelay = 50000;
const TInt KDefaultStrokeTime = 10000;

const TInt KLightTarget = CHWRMLight::EPrimaryDisplayAndKeyboard;

_LIT8( KParameterSeparator, "," );
_LIT8( Kok, "\r\nOK\r\n");
_LIT8( KCmeErrorNotSupported, "\r\n+CME ERROR: 4\r\n");
_LIT8( KTestCommand, "=?" );
_LIT8( KReadCommand, "?" );
_LIT8( KCbkltTestCommandResp, ":(0,1,2)" );
_LIT8( KCkpdTestCommandResp,  ":(GSM)" );
_LIT8( KCtsaTestCommandResp, ":(0,1,2),(width),(height)" );
_LIT( KFileName, "\\resource\\apps\\atcmdpluginresource.rsc" );


// ============================= MEMBER FUNCTIONS =============================

// ----------------------------------------------------------------------------
// CATCmdPluginEcom::NewL
// 2-phased constructor
// (may leave)
// ----------------------------------------------------------------------------
//
CATCmdPluginEcom* CATCmdPluginEcom::NewL()
    {
    CATCmdPluginEcom* self = new (ELeave) CATCmdPluginEcom();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// ----------------------------------------------------------------------------
// CATCmdPluginEcom::CATCmdPluginEcom
// Constructor
// ----------------------------------------------------------------------------
//
CATCmdPluginEcom::CATCmdPluginEcom() 
    : CATExtPluginBase()
    {
    TRACE_FUNC
    }
     
// ----------------------------------------------------------------------------
// CATCmdPluginEcom::ConstructL
// Constructor
// (may leave)
// ----------------------------------------------------------------------------
//
void CATCmdPluginEcom::ConstructL()
    {
    TRACE_FUNC
	
    LEAVE_IF_ERROR( iWsSession.Connect() );
    iCkpdCmdArray = new (ELeave) CArrayFixFlat<TATCkpdCmd>( KGranularity );
    
    iKpdTimer = new ( ELeave ) CATCmdTimer( *this, ECmdCkpd );
	iKpdTimer->ConstructL();
	
    iBkltTimer = new ( ELeave ) CATCmdTimer( *this, ECmdCbklt );
    iBkltTimer->ConstructL();
    
    iKeepAliveTimer = new ( ELeave ) CATCmdTimer( *this, EKeepAlive );
    iKeepAliveTimer->ConstructL();

	
    LoadResourcesL();
   	}

// ----------------------------------------------------------------------------
// CATCmdPluginEcom::::LoadResourcesL
// Load resources
// (may leave)
// Status : Approved
// ----------------------------------------------------------------------------
//
void CATCmdPluginEcom::LoadResourcesL()
    {
    TRACE_FUNC_ENTRY

    RFs fs;
    CleanupClosePushL( fs );

    TFileName fileName;
    fileName.Copy( KFileName );
    User::LeaveIfError( fs.Connect() );
    BaflUtils::NearestLanguageFile( fs, fileName );

    RResourceFile resourceFile;
    CleanupClosePushL( resourceFile );
    resourceFile.OpenL( fs, fileName );
    resourceFile.ConfirmSignatureL();

    HBufC8* buf8 = resourceFile.AllocReadLC( R_ATCMDPLUGIN_TRANSLATIONTABLE );
    
    TResourceReader reader;
    reader.SetBuffer( buf8 );
    
    TInt count = reader.ReadInt16(); // array count
    iTranslations = new ( ELeave ) CArrayPtrFlat<CATCmdTranslation>( count );

    while ( count-- )
        {
        const TInt code = reader.ReadInt32(); 

        HBufC8* param = reader.ReadHBufC8L(); 
        CleanupStack::PushL( param );
        CATCmdTranslation* t = new (ELeave) CATCmdTranslation( *param, code );
        CleanupStack::Pop( param ); // transfer ownership

        CleanupStack::PushL( t );
        iTranslations->AppendL( t );
        CleanupStack::Pop( t ); // transfer ownership
        }

    CleanupStack::PopAndDestroy( 3 ); // resourceFile, fs, buf8

    TRACE_FUNC_EXIT
    }


// ----------------------------------------------------------------------------
// CATCmdPluginEcom::~CATCmdPluginEcom
// C++ Destructor
// ----------------------------------------------------------------------------
//
CATCmdPluginEcom::~CATCmdPluginEcom()
	{
	TRACE_FUNC
	
    if(iKpdTimer)
        {
        iKpdTimer->Cancel();
        delete iKpdTimer;
        }	    
    if(iBkltTimer)
        {
        iBkltTimer->Cancel();
        delete iBkltTimer;
        }
    if(iKeepAliveTimer)
        {
        iKeepAliveTimer->Cancel();
        delete iKeepAliveTimer;
        }
    if ( iTranslations)
        {
        iTranslations->ResetAndDestroy();
        delete iTranslations;
        }    
    if ( iCkpdCmdArray)
        {
        iCkpdCmdArray->Reset();
        delete iCkpdCmdArray;
        }
    iWsSession.Close();
	}


// ----------------------------------------------------------------------------
// CATCmdPluginEcom::HandleCommand
// Handle given command
// ----------------------------------------------------------------------------
//
void CATCmdPluginEcom::HandleCommand( const TDesC8& aCmd, RBuf8& aReply, TBool /* aReplyNeeded  */)
	{
	TRACE_FUNC_ENTRY
	TInt errValue = aReply.Create(REPLY_BUFFER_SIZE);
	if(errValue != KErrNone)
	    {
	    HandleCommandCompleted( errValue,EReplyTypeError );
	    }
	aReply.Zero();
	iReplyBuf = &aReply;
	
	if ( IsCommandSupported( aCmd ) )
	    {
	    	
		TRAPD( err, HandleCommandL( aCmd ) );

		if ( err )
			{
			iReplyBuf->Append( KCmeErrorNotSupported ); 
			}
		else
			{
			if(iReplyBuf->Length() == 0)
				{
				iReplyBuf->Append( Kok );
				} 
			}
		HandleCommandCompleted( KErrNone,EReplyTypeOk );      
		}
    TRACE_FUNC_EXIT
    }

// ----------------------------------------------------------------------------
// CATCmdPluginEcom::HandleCommandL
// Handle given command
// (may leave)
// ----------------------------------------------------------------------------
//
void CATCmdPluginEcom::HandleCommandL( const TDesC8& aCmd )
	{
    // Verify command
    TInt offset;
    HBufC8* cmd = aCmd.AllocLC();
//    CleanupStack:: PushL(cmd);
    TPtr8 ptr = cmd->Des();

    ptr.Trim();
    TPtrC8 cptr;

    if ( ( offset = ptr.FindF(KAtCkpd) ) >= 0 )
        {
        cptr.Set( ptr.Mid( offset + KAtCkpd().Length() ) );         
        HandleCkpdCommandL( cptr );
        }
    else if ( ( offset = ptr.FindF(KAtCbklt) ) >= 0 ) 
        {
        cptr.Set( ptr.Mid( offset + KAtCbklt().Length() ) );
        HandleCbkltCommandL( cptr );
        }
    else if ( ( offset = ptr.FindF(KAtCtsa) ) >= 0 )
    	{
    	cptr.Set( ptr.Mid( offset + KAtCtsa().Length() ) );
    	HandleCtsaCommandL( cptr );
    	} 
    CleanupStack::PopAndDestroy( cmd );
    }    

// -------------------------
//  HandleCtsaCommandL
//
// -------------------------

void CATCmdPluginEcom::HandleCtsaCommandL( const TDesC8& aCmd )
    {
    
    if ( aCmd.Compare( KTestCommand ) == 0 )
        {
        // Test command just returns "OK"        
        iReplyBuf->Append( KCRLF );
        iReplyBuf->Append( KAtCtsa );
        iReplyBuf->Append( KCtsaTestCommandResp );
        iReplyBuf->Append( KCRLF );
        TRACE_FUNC_EXIT
        return;
        }  
    
    // Check boundary condition
	if ( ( aCmd.Length() < 6 ) || ( aCmd[2] !=  ',' ))
	    {
	    // Command Syntax Error
	    User::Leave( KErrNotSupported );           
	    } 	
 	TInt   pos=0;
 	TInt8  actionValue = aCmd[1] - '0';
 	TInt16 xCoordinateValue = 0;
 	TInt16 yCoordinateValue = 0;
 	TInt16 commandLen = aCmd.Length();  
 	 
 	
 	if( actionValue < 0 || actionValue > 4 )
 	    {
 	    // Command Syntax Error          
 	    User::Leave( KErrNotSupported );                  
 	    }
 	
 	pos=3;  // X Cordinate starts from 3d character
 	
 	CWsScreenDevice *scrnDevPointer = new(ELeave) CWsScreenDevice(iWsSession);
 	TSize coordinateSize;
 	
	scrnDevPointer->Construct();
 	coordinateSize = scrnDevPointer->SizeInPixels();  
 	delete scrnDevPointer;
 	
	while(pos < commandLen)
 	    {
 	    if((aCmd[pos] >=  '0') && (aCmd[pos] <=  '9'))
 	        {
 	        xCoordinateValue *= 10;
 	        xCoordinateValue += aCmd[pos] - '0';
 	        pos++;
 	        if(xCoordinateValue > coordinateSize.iWidth )
 	            {
 	            // Command Syntax Error            
 	            User::Leave( KErrNotSupported );           
 	            }
 	        }
 	    else if((aCmd[pos] ==  ',') && pos > 3 )
            {
            pos++;
            break;
            }
 	    else
 	        {
 	        // Command Syntax Error            
 	        User::Leave( KErrNotSupported );             
 	        } 	    
 	    }
	if(pos == commandLen)	
		{
		// Command of type =0,100        /* Command Syntax Error */
 	    User::Leave( KErrNotSupported );  
		}
 	while(pos < commandLen)
 	    {
 	    if((aCmd[pos] >=  '0') && (aCmd[pos] <=  '9'))
 	        {
 	        yCoordinateValue *= 10;
 	        yCoordinateValue += aCmd[pos] - '0';
 	        pos++;
			if(yCoordinateValue > coordinateSize.iHeight)
 	            {
 	            // Command Syntax Error            
 	            User::Leave( KErrNotSupported );           
 	            }
 	        }
 	    else
 	        {
 	        // Command Syntax Error            
 	        User::Leave( KErrNotSupported );             
 	        }       
 	    }
 	
 	TRawEvent rawEvent;
	switch(actionValue)
		{
		case 0:  // Release pointer event
		    {
			rawEvent.Set( TRawEvent::EButton1Up, xCoordinateValue, yCoordinateValue);
			iWsSession.SimulateRawEvent(rawEvent);
			iWsSession.Flush();
			break;
			}
		case 1:   // Depress pointer event
			{
			rawEvent.Set( TRawEvent::EButton1Down, xCoordinateValue, yCoordinateValue);
			iWsSession.SimulateRawEvent(rawEvent);
			iWsSession.Flush();
			break;
			}
		case 2:   // Single tap pointer event
			{
			rawEvent.Set( TRawEvent::EButton1Down, xCoordinateValue, yCoordinateValue);
			iWsSession.SimulateRawEvent(rawEvent);
			rawEvent.Set( TRawEvent::EButton1Up, xCoordinateValue+1, yCoordinateValue+1);
			iWsSession.SimulateRawEvent(rawEvent);
			iWsSession.Flush();
			break;
			}
		case 3:   // Double tap pointer event
			{
			rawEvent.Set( TRawEvent::EButton1Down, xCoordinateValue, yCoordinateValue);
			iWsSession.SimulateRawEvent(rawEvent);
			rawEvent.Set( TRawEvent::EButton1Up, xCoordinateValue+1, yCoordinateValue+1);
			iWsSession.SimulateRawEvent(rawEvent);
			rawEvent.Set( TRawEvent::EButton1Down, xCoordinateValue, yCoordinateValue);
			iWsSession.SimulateRawEvent(rawEvent);
			rawEvent.Set( TRawEvent::EButton1Up, xCoordinateValue+1, yCoordinateValue+1);
			iWsSession.SimulateRawEvent(rawEvent);
			iWsSession.Flush();
			break;
			}
		}
	}     
    

// ----------------------------------------------------------------------------
// CATCmdPluginEcom::HandleCkpdCommandL
// Handle CKPD command
// (may leave)
// ----------------------------------------------------------------------------
//
void CATCmdPluginEcom::HandleCkpdCommandL( const TDesC8& aCmd )
    {
    TRACE_FUNC_ENTRY
    TBool commandVerified(EFalse);
    TInt strokeTime = 0;
    TInt strokeDelay = 0;
    
    if ( aCmd.Compare( KTestCommand ) == 0 )
        {
        iReplyBuf->Append( KCRLF );
        iReplyBuf->Append( KAtCkpd );
        iReplyBuf->Append( KCkpdTestCommandResp );
        iReplyBuf->Append( KCRLF );
        TRACE_FUNC_EXIT
        return;
        }

    CArrayFix<TATCkpdCmd>* ckpdCmds = 
        new (ELeave) CArrayFixFlat<TATCkpdCmd>( KGranularity );
    CleanupStack::PushL( ckpdCmds );
    
    // Check that keys are defined
    if (  aCmd.Length() <= 1  )
        {
        // Command Syntax Error          
        User::Leave( KErrNotSupported );           
        }

    // Go through keys
    TBuf8<2> key;
    for (TInt i = 1 ; i < aCmd.Length(); i++ )
        {
        key.Copy(&aCmd[i],1);

        // Go through translation table for every character
        for ( TInt j = 0; j < iTranslations->Count(); j++ )
            {
            CATCmdTranslation& translation = *iTranslations->At( j );
            if ( key.CompareF( translation.Param() ) == 0 )
                {
                // Check if key is escape character for manufacturer specific keys
                if ( translation.Code() == KCustomKeyCodeIdentifier )
                    {
                    // Read next key to form key combination
                    i++;
                    
                    // Check that there are key after escape character
                    if ( i >= aCmd.Length() )
                        {
                        TRACE_INFO(( _L( "INFO: invalid manufacturer specific key" ) ))            
                        // Not handled
                        User::Leave( KErrNotSupported );           
                        }

                    if ( key.Length() < key.MaxLength() )
                        {
                        key.Append(aCmd[i]);
                        }

                    // Start looping again for looking keycombination
                    // from translation table
                    j = 0;
                    }
                else
                    {
                    // Command verified
                    commandVerified = ETrue;

                    if ( translation.Code() != EKeyNull )
                        {
                        TATCkpdCmd cmd;
                        cmd.iCode = translation.Code();
                        ckpdCmds->AppendL( cmd );
                        }

                    break;
                    }
                }
            }

        if (!commandVerified)
            {
            // Check if character is parameter separator
            if ( key.Compare( KParameterSeparator ) == 0 )
                {
                i++;

                if ( i > aCmd.Length() )
                    {
                    TRACE_ERROR(( _L( "ERROR: invalid stroke time parameter" ) ))

                    // Not handled
                    User::Leave( KErrNotSupported );
                    }

                ParseParametersL(
                        aCmd.Right(aCmd.Length()-i),
                        strokeTime,
                        strokeDelay);

                strokeTime *= KTimeMultiplier;
                strokeDelay *= KTimeMultiplier;
                
                break;
                }
            else
                {
                TRACE_ERROR(( _L8( "ERROR: Command %S not supported" ), &key ))

                // Not handled
                User::Leave( KErrNotSupported );
                }
            }

        commandVerified = EFalse;
        }

    // Set default stroke time and delay if those are not set with parameters or
    // parameter value have been zero. If values are not greater than ~2ms there
    // is possibility to loose key events with long commands even all keys are
    // simulated by atcmdplugin
    if ( !strokeTime )
        {
        strokeTime = KDefaultStrokeTime;
        }

    if ( !strokeDelay )
        {
        strokeDelay = KDefaultStrokeDelay;
        }

    // Append commands to command array
    for ( TInt count = 0; count < ckpdCmds->Count(); count++ )
        {
        TATCkpdCmd cmd = ckpdCmds->At(count);
        cmd.iStrokeTime = strokeTime;
        cmd.iStrokeDelay = strokeDelay;
        
        iCkpdCmdArray->AppendL( cmd );
        }

    CleanupStack::PopAndDestroy(ckpdCmds);

    SendKeyL();
    
    TRACE_FUNC_EXIT
    }

// ----------------------------------------------------------------------------
// CATCmdPluginEcom::HandleCbkltCommandL
// Handle CKPD command
// (may leave)
// ----------------------------------------------------------------------------
//
void CATCmdPluginEcom::HandleCbkltCommandL( const TDesC8& aCmd )
    {
    TRACE_FUNC_ENTRY

         
    if ( aCmd.Compare( KTestCommand ) == 0 )
        {
        iReplyBuf->Append( KCRLF );
        iReplyBuf->Append( KAtCbklt );
        iReplyBuf->Append( KCbkltTestCommandResp );
        iReplyBuf->Append( KCRLF );
        TRACE_FUNC_EXIT
        return;
        }
		
	if(iLightStatus != ELightOnWithDuration)
		{
		iLight = CHWRMLight::NewL( this );
	
		iLightStatus = 
			( iLight->LightStatus( KLightTarget ) == CHWRMLight::ELightOff )?
			ELightOff : ELightOn;   
		delete iLight;		
		}

    if ( aCmd.Compare( KReadCommand ) == 0 )
        {
        iReplyBuf->Append( KCRLF );
        iReplyBuf->Append( KAtCbklt );
        iReplyBuf->Append( ':' );
        iReplyBuf->Append( ' ' );
        iReplyBuf->AppendNum( iLightStatus );
        if ( iLightStatus == ELightOnWithDuration )
            {
            iReplyBuf->Append( ',' );
            iReplyBuf->AppendNum( iDuration );
            }
        iReplyBuf->Append( KCRLF );
        TRACE_FUNC_EXIT
        return;
        }

    if ( aCmd.Length() <= 1 )
        {
        // Command Syntax Error        
        User::Leave( KErrNotSupported );
        }

    switch ( aCmd[ 1 ] )
        {
        case '0':
            {
            if ( aCmd.Length() == 2 )
                {
                iKeepAliveTimer->Cancel();
                DisableBackLight();
                iLightStatus = ELightOff;
                }
            else
                {
                //if the command is too long it means that is incorrect
                User::Leave( KErrNotSupported );
                }
            break;
            }
        case '1':
            {
            if ( ( aCmd.Length() > 3 ) && ( aCmd[ 2 ] == ',' ) )
                {                
                TInt pos = 3;
                TInt commandLen = aCmd.Length();
                iDuration = 0;
                while(pos < commandLen)
                        {
                        if((aCmd[pos] >=  '0') && (aCmd[pos] <=  '9'))
                            {
                            iDuration *= 10;
                            iDuration += aCmd[pos] - '0';
                            pos++;
                            }
                        else
                            {
                            User::Leave( KErrNotSupported );             
                            }
                        }
                iBkltTimer->Cancel();
                iBkltTimer->After( iDuration * KMicroSeconds );
                EnableBackLight();
                if ( !iKeepAliveTimer->IsActive() )
                     {
                     iKeepAliveTimer->After( 2000000 );
                     }
                iLightStatus = ELightOnWithDuration;
                }
            else
                {
                //the command is wrong
                User::Leave( KErrNotSupported );
                }
            break;
            }
        case '2':
            {
            if ( aCmd.Length() == 2 )
                {
                iBkltTimer->Cancel();
                EnableBackLight();
                if ( !iKeepAliveTimer->IsActive() )
                    {
                    iKeepAliveTimer->After( 2000000 );
                    }
                iLightStatus = ELightOn;
                }
            else
                {
                //if the command is too long it means that is incorrect
                User::Leave( KErrNotSupported );
                }
            break;
            }
        default:
            {
            User::Leave( KErrNotSupported );
            }
        }
    TRACE_FUNC_EXIT
    }

// ----------------------------------------------------------------------------
// CATCmdPluginEcom::EnableBackLight()
// ----------------------------------------------------------------------------
//
void CATCmdPluginEcom::EnableBackLight()
	{
	TRACE_FUNC
	RProperty::Set( KPSUidCoreApplicationUIs, KLightsATCForcedLightsOn, EForcedLightsOn );
    }

// ----------------------------------------------------------------------------
// CATCmdPluginEcom::DisableBackLight()
// ----------------------------------------------------------------------------
//
void CATCmdPluginEcom::DisableBackLight()
	{
	TRACE_FUNC
    RProperty::Set( KPSUidCoreApplicationUIs, KLightsATCForcedLightsOn, EForcedLightsOff );
    }

// -----------------------------------------------------------------------------
// CATCmdPluginEcom::LightStatusChanged()
// -----------------------------------------------------------------------------
//
void CATCmdPluginEcom::LightStatusChanged( TInt /*aTarget*/, CHWRMLight::TLightStatus /*aStatus*/ )
	{
	//TRACE_STATE(( _L( "** LightStatusChanged %d, %d" ), aTarget, aStatus ))
  }

// ----------------------------------------------------------------------------
// CATCmdPluginEcom::IsCommandSupported
// -
// ----------------------------------------------------------------------------
//
TBool CATCmdPluginEcom::IsCommandSupported( const TDesC8& aCmd )
    {
    TRACE_FUNC
    TBool ret(EFalse);
    
    if ( aCmd != KNullDesC8 && ( aCmd.FindF(KAtCkpd) >= 0 || 
                                 aCmd.FindF(KAtCbklt) >= 0 || aCmd.FindF(KAtCtsa) >= 0 ))
        {
        ret = ETrue;
        }
    return ret;
    }

// CATCmdPluginEcom::ParseParameterL
// -
// (may leave)
// ----------------------------------------------------------------------------
//
void CATCmdPluginEcom::ParseParametersL( 
        const TDesC8& aCmd,
        TInt& aStrokeTime,
        TInt& aStrokeDelay )
    {
    TRACE_FUNC_ENTRY
    
    TInt i(0);
    
    if ( !aCmd.Length() )
        {
        TRACE_ERROR(( _L( "ERROR: invalid stroke time parameter" ) ))            
        // Not handled
        User::Leave( KErrNotSupported );           
        }


    // Parse stroke time parameter
    TBuf8<3> strokeTimeBuf;

    // Go through command until end or parameter separator is reached
    for ( ; i < aCmd.Length(); i++ )
        {
        if ( aCmd[i] >= '0' && aCmd[i] <= '9' )
            {
            strokeTimeBuf.Append(aCmd[i]);
            continue;
            }
        else if ( aCmd[i] == ',' || aCmd[i] == '\r' || aCmd[i] == '\n' )
            {
            i++;
            break;
            }
        else
            {
            TRACE_ERROR(( _L( "ERROR: invalid stroke time parameter" ) ))

            // Not handled
            User::Leave( KErrNotSupported );   
            }
        }

    TLex8 strokeTimeLex( strokeTimeBuf );          
    strokeTimeLex.Val( aStrokeTime );

    // Verify parsed parameter
    if ( aStrokeTime < 0 || aStrokeTime > 255 )
        {
        TRACE_ERROR(( _L( "ERROR: invalid stroke time parameter" ) ))

        // Not handled
        User::Leave( KErrNotSupported );
        }


    // Parse stroke delay parameter
    TBuf8<3> strokeDelayBuf;

    for ( ; i < aCmd.Length(); i++ )
        {
        if ( aCmd[i] >= '0' && aCmd[i] <= '9' )
            {
            strokeDelayBuf.Append(aCmd[i]);
            continue;
            }
        else if ( aCmd[i] == ',' || aCmd[i] == '\r' || aCmd[i] == '\n' )
            {
            break;
            }
        else
            {
            TRACE_ERROR(( _L( "ERROR: invalid stroke delay parameter" ) ))

            // Not handled
            User::Leave( KErrNotSupported );   
            }
        }

    TLex8 strokeDelayLex( strokeDelayBuf );          
    strokeDelayLex.Val( aStrokeDelay );

    if ( aStrokeDelay < 0 || aStrokeDelay > 255 )
        {
        TRACE_ERROR(( _L( "ERROR: invalid stroke delay parameter" ) ))

        // Not handled
        User::Leave( KErrNotSupported );
        }
    
    TRACE_FUNC_EXIT
    }

// ----------------------------------------------------------------------------
// CATCmdPluginEcom::SendKeyL
// -
// (may leave)
// ----------------------------------------------------------------------------
//
void CATCmdPluginEcom::SendKeyL()
	{
	TRACE_FUNC

	if ( !iKpdTimer->IsActive() )
	    {
	    const TInt count = iCkpdCmdArray->Count();
	    if ( count )
	        {
            TATCkpdCmd cmd = iCkpdCmdArray->At(0);
            TKeyEvent key;
            key.iScanCode = cmd.iCode;
            key.iCode = 0;
            key.iRepeats = 0;
            
            TTimeIntervalMicroSeconds32 strokeTime = cmd.iStrokeTime;
            TTimeIntervalMicroSeconds32 strokeDelay = cmd.iStrokeDelay;

            TRACE_INFO(( _L( "   Scancode:%d, stroke time:%d, stroke delay: %d" ),
                        key.iScanCode, strokeTime.Int(), strokeDelay.Int() ))

            if ( iLastEvent == EEventNull || iLastEvent == EEventKeyUp)
                {
                SimulateRawEventL( key, EEventKeyDown );                    
                
                if ( strokeTime.Int() > 0)
                    {
                    iKpdTimer->After( strokeTime );
                    return;
                    }
                }
            
            if ( iLastEvent == EEventKeyDown || iLastEvent == EEventKey )
                {
                SimulateRawEventL( key, EEventKeyUp );

                iCkpdCmdArray->Delete( 0 );
                
                if ( strokeDelay.Int() > 0 && iCkpdCmdArray->Count() > 0 )
                    {
                    iKpdTimer->After( strokeDelay );
                    return;
                    }
                else
                    {
                    SendNextKeyL();
                    }
                }
	        }
	    }
	}


// ----------------------------------------------------------------------------
// CATCmdPluginEcom::SendNextKeyL
// -
// (may leave)
// ----------------------------------------------------------------------------
//
void CATCmdPluginEcom::SendNextKeyL()
    {
    TRACE_FUNC

    const TInt count = iCkpdCmdArray->Count();
    if ( count )
        {
        SendKeyL();
        }
    }

// ----------------------------------------------------------------------------
// CATCmdPluginEcom::TimerExpiredL
// -
// (may leave)
// ----------------------------------------------------------------------------
//
void CATCmdPluginEcom::TimerExpiredL( TInt aRelatedCommand )
	{
	TRACE_FUNC
	
	if( aRelatedCommand == ECmdCkpd )
		{
        SendNextKeyL();
		}
	else if ( aRelatedCommand == ECmdCbklt )
	    {
	    DisableBackLight();
	    iLightStatus = ELightOff;
	    iKeepAliveTimer->Cancel();
	    }
	else
	    {
	    User::ResetInactivityTime();
	    iKeepAliveTimer->After( 2000000 );
	    }
	}		
    
// ----------------------------------------------------------------------------
// CATCmdPluginEcom::SimulateRawEventL
// -
// (may leave)
// ----------------------------------------------------------------------------
//
void CATCmdPluginEcom::SimulateRawEventL( TKeyEvent& aKey, TEventCode aEvent )   
    {
    TRACE_FUNC

    TRawEvent rawEvent;
    TRawEvent::TType rawType( TRawEvent::ENone );
    switch ( aEvent )
        {
        case EEventKeyUp:
            rawType = TRawEvent::EKeyUp;
            break;
        case EEventKeyDown:
            rawType = TRawEvent::EKeyDown;
            break;
        default:
            break;
        }

    if ( rawType != TRawEvent::ENone )
        {
        rawEvent.Set( rawType, aKey.iScanCode );
        iWsSession.SimulateRawEvent( rawEvent );
        iWsSession.Flush();
        }

    iLastEvent = aEvent;
    }

// ========================= OTHER EXPORTED FUNCTIONS =========================

// ----------------------------------------------------------------------------
// CATCmdPluginEcom::CATCmdTimer::CATCmdTimer
// C++Constructor
// ----------------------------------------------------------------------------
//
CATCmdPluginEcom::CATCmdTimer::CATCmdTimer( MATCmdTimer& aObserver, 
                                         TInt aRelatedCommand )
    : CTimer( EPriorityStandard ),
      iObserver( aObserver ),
      iRelatedCommand( aRelatedCommand )
    {
    TRACE_FUNC

    CActiveScheduler::Add( this );
    }

// ----------------------------------------------------------------------------
// CATCmdPluginEcom::CATCmdTimer::ConstructL
// 2nd phase constructor
// ----------------------------------------------------------------------------
//
void CATCmdPluginEcom::CATCmdTimer::ConstructL()
    {
    TRACE_FUNC
    CTimer::ConstructL();
    }

// ----------------------------------------------------------------------------
// CATCmdPluginEcom::CATCmdTimer::RunL
// Timer callback
// ----------------------------------------------------------------------------
//
void CATCmdPluginEcom::CATCmdTimer::RunL()
    {
    TRACE_FUNC

    const TInt err = iStatus.Int();
    if ( err != KErrCancel )
        {
        iObserver.TimerExpiredL( iRelatedCommand );
        }
    }


void CATCmdPluginEcom::ReportConnectionName( const TDesC8&  /*  aName  */)
    {
    
    }
void CATCmdPluginEcom::HandleCommandCancel()
    {
    
    }
void CATCmdPluginEcom::ReportExternalHandleCommandError()
    {
    
    }
void CATCmdPluginEcom::ReportNvramStatusChange( const TDesC8& /*  aNvram  */)
    {
    
    }
void CATCmdPluginEcom::ReceiveUnsolicitedResultCancel()
    {
    
    }
void CATCmdPluginEcom::ReceiveUnsolicitedResult()
    {
    
    }
TInt CATCmdPluginEcom::GetNextPartOfReply( RBuf8& /* aNextReply  */)
    {
    return KErrNone;
    }
TInt CATCmdPluginEcom::NextReplyPartLength()
    {
    return KErrNone;
    }


//  End of File
