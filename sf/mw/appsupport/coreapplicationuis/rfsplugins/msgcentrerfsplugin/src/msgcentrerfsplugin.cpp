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
* Description:  Implementation of CMsgCentreRFSPlugin class
*
*/


// SYSTEM INCLUDE

// USER INCLUDE
#include <rfsClient.h>
#include <eikenv.h>
#include <eikappui.h>
#include <MsvFactorySettings.h>
#include "msgcentrerfsplugin.h"
#include "trace.h"

NONSHARABLE_CLASS(CMsgCentreRfsAppUi) : public CEikAppUi
     {
     public: // From CEikAppUi
         void ConstructL();
     };

// ---------------------------------------------------------------------------
// CMsgCentreRfsAppUi::ConstructL
// ---------------------------------------------------------------------------
// 
void CMsgCentreRfsAppUi::ConstructL()
    {
    FUNC_LOG;

    CEikAppUi::BaseConstructL( ENonStandardResourceFile | ENoScreenFurniture );
    }

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CMsgCentreRFSPlugin::NewL
// ---------------------------------------------------------------------------
//
CMsgCentreRFSPlugin* CMsgCentreRFSPlugin::NewL( TAny* /*aInitParams*/ )
    {
    FUNC_LOG;

    return new ( ELeave ) CMsgCentreRFSPlugin;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMsgCentreRFSPlugin::~CMsgCentreRFSPlugin()
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// CMsgCentreRFSPlugin::RestoreFactorySettingsL
// ---------------------------------------------------------------------------
//
void CMsgCentreRFSPlugin::RestoreFactorySettingsL( const TRfsReason aType )
    {
    FUNC_LOG;

    if ( aType == ERfsNormal || aType == ERfsDeep )
        {
        INFO_1( "Doing RFS (reason %d)", aType );

        // Messaging centre RFS requires UIKON environment, so we have to 
        // create a dummy environment before calling it.
        TInt err( KErrNoMemory );
        
        // Uninstall the current active scheduler.
        CActiveScheduler* scheduler = CActiveScheduler::Current();
        CActiveScheduler::Install( NULL );
    
        // Allocate UIKON env and app ui objects.
    
        // This installs a new cleanup stack
        CMsgCentreRfsAppUi* ui = NULL;
        CEikonEnv* env = new CEikonEnv();
        if ( env )
            {
            env->DisableExitChecks( ETrue ); // must do this
            ui = new CMsgCentreRfsAppUi();
            }
        if ( ui )
            {
            // UIKON env will install a new active scheduler.
            TRAP( err, { env->ConstructL(); ui->ConstructL(); } );
            ERROR( err, "Dummy UIKON construction failed");
            if ( err == KErrNone )
                {
                // Now that a dummy UIKON env has been created,
                // this does the actual RFS operation.
                TRAP( err, MsvFactorySettings::ResetL(
                    aType == ERfsNormal ? EMsvFactorySettingsLevelNormal :
                                          EMsvFactorySettingsLevelDeep ) );
                ERROR( err, "MsvFactorySettings::ResetL() failed");
                }
            else
                {
                // There is no proper cleanup available for this situation,
                // so we terminate the server.
                User::Exit( err );
                }
            }
        else
            {
            delete env;
            env = NULL;
            }
        if ( env )
            {
            // Get rid of the UIKON environment.
            env->PrepareToExit();
            env->DestroyEnvironment();
            }
    
        // CEikonEnv's cleanup stack and active scheduler are now uninstalled.
    
        // Put the RFS server's active scheduler back.
        CActiveScheduler::Install( scheduler );
    
        ERROR( err, "Message centre RFS failed");
        User::LeaveIfError( err );
        }
    }

// ---------------------------------------------------------------------------
// CMsgCentreRFSPlugin::GetScriptL
// ---------------------------------------------------------------------------
//
void CMsgCentreRFSPlugin::GetScriptL( const TRfsReason /*aType*/, TDes& /*aPath*/ )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// CMsgCentreRFSPlugin::ExecuteCustomCommandL
// ---------------------------------------------------------------------------
//
void CMsgCentreRFSPlugin::ExecuteCustomCommandL(
    const TRfsReason /*aType*/,
    TDesC& /*aCommand*/ )
    {
    FUNC_LOG;
    }
