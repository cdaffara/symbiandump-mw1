/*
* Copyright (c) 2009-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   CDisplayServiceUILayout class declaration.
*
*/


#ifndef DISPLAYSERVICEUILAYOUT_H
#define DISPLAYSERVICEUILAYOUT_H

// SYSTEM INCLUDE FILES
#include <w32std.h>

// USER INCLUDE FILES
#include "displayservicebase.h"
#include "displaysourcecontextdef.h"

// FORWARD DECLARATIONS
class CApaWindowGroupName;

// DATA TYPES

// CLASS DECLARATION

/**
 * Ws event observer.
 */
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
    virtual void HandleWsEventL( const TWsEvent &aEvent ) = 0;
    
    /**
     * Handle error occured in window server event.
     * 
     * @since S60 5.0
     * @param aEvent New window server event.
     * @return None.
     */
    virtual void HandleWsEventErrorL( TInt aError ) = 0;
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
    
    /**
     * Resolve current UI layout mode.
     * 
     * @since S60 5.0
     * @param aLayout Current UI layout mode.
     * @param aRotation Current UI rotation.
     * @return TDisplayUILayoutValues.
     */
    void ResolveUILayout( TDisplayUILayoutValues& aLayout,
        CFbsBitGc::TGraphicsOrientation& aRotation ) const;
    
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
    
    /** Screen device */
    CWsScreenDevice* iWsScreenDevice;
    };
        
/**
 * UI Layout service provides information about the current UI layout.
 * I.e. if the current UI layout for the application is portrait or landscape.
 *
 * @lib None.
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CDisplayServiceUILayout ): public CDisplayServiceBase,
public MWsEventObserver
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param None.
     * @return CDisplayServiceUILayout
     */
    static CDisplayServiceUILayout* NewL( MCFContextInterface& aCF );
    static CDisplayServiceUILayout* NewLC( MCFContextInterface& aCF );

    /**
     * C++ destructor.
     */
    virtual ~CDisplayServiceUILayout();

public: // From base classes
    
    // @see CDisplayServiceBase
    virtual void StartL();

    // @see CDisplayServiceBase
    virtual void Stop();
    
    // @see MWsEventObserver
    virtual void HandleWsEventL( const TWsEvent &aEvent );
    
    // @see MWsEventObserver
    virtual void HandleWsEventErrorL( TInt aError );
    
private:
    
    /**
     * Publish UI layout mode context.
     */
    void PublishContextL( TDisplayUILayoutValues aLayout,
        TBool aForce = EFalse );
    
    /**
     * Publish UI rotation context.
     */
    void PublishContextL( CFbsBitGc::TGraphicsOrientation aRotation,
        TBool aForce = EFalse  );

private:

    CDisplayServiceUILayout( MCFContextInterface& aCF );
    void ConstructL();

private: // Data
    
    /** Window server event handler */
    CWsEventHandler* iWsEventHandler;
    
    /** Current UI layout */
    TDisplayUILayoutValues iCurrentLayout;
    
    /** Current UI rotation */
    CFbsBitGc::TGraphicsOrientation iCurrentRotation;
    };

#endif // DISPLAYSERVICEUILAYOUT_H
