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
* Description:  ?Description
*
*/


#ifndef C_APPLICATIONSTATESOURCEPLUGIN_H
#define C_APPLICATIONSTATESOURCEPLUGIN_H

// INCLUDES
#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <viewcli.h>
#else
#include <viewcli.h>
#include <viewclipartner.h>
#endif
#include <cfcontextsourceplugin.h>
#include <cfcontextobject.h>
#include <cenrepnotifyhandler.h>
#include <w32std.h>
#include <apgwgnam.h>
#include "cfapplicationstatesettings.h"
#include "uidorientationpair.h"

// FORWARD DECLARATIONS
class CRepository;
class CCenRepNotifyHandler;
// FORWARD DECLARATIONS
class CWsEventHandler;

NONSHARABLE_CLASS( MWsEventObserver )
    {
public:

    /**
     * Handles window server event.
     * 
     * @since S60 5.0
     * @param aEvent New window server event.
     * @return None.
     */
    virtual void HandleWsEventL( RWsSession& aWsSession ) = 0;    
    };
// CLASS DECLARATION

/**
 * Application state source plugin.
 * Application state source plugin is responsible for listening view activations
 * from view server client session and mapping them to foreground application and
 * view contexts.
 * 
 * @lib -
 * @since S60 4.0
 */
NONSHARABLE_CLASS( CApplicationStateSourcePlugIn ): 
	public CCFContextSourcePlugIn,
	public MWsEventObserver,
	public MCenRepNotifyHandlerCallback
    {
public: // Constructors and destructor

    // Two phased constructors
    static CApplicationStateSourcePlugIn* NewL(
        TContextSourceInitParams* aParams );
    static CApplicationStateSourcePlugIn* NewLC(
        TContextSourceInitParams* aParams );

    // Destructor
    ~CApplicationStateSourcePlugIn();

public: // From CCFContextSourcePlugIn

    // @see CCFContextSourcePlugIn
    void HandleSettingL( CCFContextSourceSettingArray* aSettingList );

    // @see CCFContextSourcePlugIn
    void DefineContextsL();

    // @see CCFContextSourcePlugIn
    void InitializeL();



public: // From MCenRepNotifyHandlerCallback

    // @see MCenRepNotifyHandlerCallback
    void HandleNotifyGeneric( TUint32 aId );

private: // Constructors

    // C++ constrcutor
    CApplicationStateSourcePlugIn( TContextSourceInitParams* aParams );

    // Symbian second-phase constructor
    void ConstructL();

private: // New functions

    /**
     * Converts string to hex.
     *
     * @since S60 v4.0
     * @param aString Hexadecimal number as string having "0x" prefix.
     * @param aValue Storage for the unsigned integer.
     * @return KErrNone if successful, KErrNotFound otherwise.
     */
    TInt ConvertToHex( const TDesC& aString, TUint32& aValue );

    // Initialize application settings from XML
    void InitializeApplicationsFromXMLL();
    
    // Initialize the fg application context
    void InitializeFgApplicationL();
    void HandleWsEventL( RWsSession& aWsSession );
    // Handles the view server event
    void DoHandleViewEventL( const TVwsViewEvent& aEvent );
    
    // Resolve fg app orientation
    TPtrC ResolveFgAppOrientation( const TUid& aUid );
    
    // Find app from cache by uid
    TInt FindByUid( const TUid& aUid ) const;

private: // Data

    // Buffers for default settings.
    HBufC* iDefaultAppName;
    HBufC* iDefaultViewName;
    HBufC* iApplDefaultState;
    TInt iDefStateSetManyTimes;

    // View server client session for foreground application notifications.
    CVwsSessionWrapper* iVws;

    // Context instance for all the generated contexts.
    CCFContextObject* iContext;

    // The name of the previous foreground application used by background
    // application context publishing when the application was closed.
    // Additionally, used to control context publishing based on change.
    TPtrC iPreviousForegroundApplication;

    // The name of the previous foreground view used to control context
    // publishing based on change.
    TPtrC iPreviousForegroundView;

    // My settings.
    CCFContextSourceSettingArray* iSettings;

    // Foreground application setting list
    RApplicationStateSettingsPointerArray iApplicationSettings;
    CWsEventHandler* iWsEventHandler;
    
    // Previous foreground application orientation
    TPtrC iPreviousOrientation;
    
    // KCRUidDefaultAppOrientation cenrep
    CRepository* iCRAppOrientation;
    
    // Default application orientation
    TInt iDefaultFgAppOrientation;
    
    // Application uid and orientation cache
    RUidOrientationList iUidOrientationList;
    
    // KCRUidDefaultAppOrientation listener
    CCenRepNotifyHandler* iCRAppOrientationListener;
    };
/**
 * Listens events from window server and forwards them to observer.
 */
NONSHARABLE_CLASS( CWsEventHandler ): public CActive
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param None.
     * @return CDisplayServiceUILayout
     */
    static CWsEventHandler* NewL( MWsEventObserver& aObserver );
    static CWsEventHandler* NewLC( MWsEventObserver& aObserver );

    /**
     * C++ destructor.
     */
    virtual ~CWsEventHandler();
    
public:
    
    /**
     * Start event listening.
     * 
     * @since S60 5.0
     * @param None.
     * @return None.
     */
    void IssueRequest();
    
protected:
    
    // @see CActive
    virtual void RunL();
    
    // @see CActive
    virtual void DoCancel(); 
    
    // @see CActive
    virtual TInt RunError( TInt aError ); 
    
private:

    CWsEventHandler( MWsEventObserver& aObserver );
    void ConstructL();

private: // Data
    
    /** Observer */
    MWsEventObserver& iObserver;

    /** Window server session */
    RWsSession iWsSession;
    
    /** Window group for receiving window server events */
    RWindowGroup* iWindowGroup;
    
    /** Window group name to hide it from the task manager */
    CApaWindowGroupName* iWindowGroupName;
    };
#endif // C_APPLICATIONSTATESOURCEPLUGIN_H
