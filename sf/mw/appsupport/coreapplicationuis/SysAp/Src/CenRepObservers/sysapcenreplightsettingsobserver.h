/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  A Central Repository key observer class for Light Settings API.
*
*/


#ifndef SYSAPCENREPLIGHTSETTINGSOBSERVER_H
#define SYSAPCENREPLIGHTSETTINGSOBSERVER_H

#include <e32base.h>
#include <cenrepnotifyhandler.h>

class CSysApAppUi;
class CRepository;
class CCenRepNotifyHandler;

/**
* A Central Repostitory observer for getting notifications from Light Settings API.
*
* System Application monitors backlight timeout changes with this observer.
*
* @lib None.
* @since S60 3.2
*/
class CSysApCenRepLightSettingsObserver : public CBase, public MCenRepNotifyHandlerCallback
    {
public:
    
    /**
    * Symbian two-phased constructor.
    *
    * @since S60 3.2
    * @param aSysApAppUi    reference to System Application application UI class
    * @return a new CSysApCenRepLightSettingsObserver instance
    */ 
    static CSysApCenRepLightSettingsObserver* NewL( CSysApAppUi& aSysApAppUi );

    /**
    * Destructor.
    */
    virtual ~CSysApCenRepLightSettingsObserver();

public: // Other functions
        
    /**
    * Returns the lights timeout in seconds.
    *
    * @since S60 3.2
    * @return TInt lights timeout in seconds
    */         
    TInt GetLightsTimeout() const;

protected: // from MCenRepNotifyHandlerCallback
    
    /** 
    * This callback method is used to notify the client about
    * changes for integer value keys, i.e. key type is EIntKey.
    *
    * @param aId Id of the key that has changed.
    * @param aNewValue The new value of the key.
    */
    void HandleNotifyInt( TUint32 aId, TInt aNewValue );
    
    /** 
    * This callback method is used to notify the client about errors
    * in the handler. Any error in handling causes the handler to stop
    * handling any more notifications. Handling can be restarted with
    * a call to aHandler->StartListeningL(), if the error is non-fatal.
    * However, be careful to trap any errors from this call if this is done.
    *
    * @param aId Id of the key this instance listens for or if notifications for
    *            whole repository are listened, could also be KInvalidNotificationId.
    * @param aError Error code.
    * @param aHandler Pointer to the handler instance. 
    *                 This pointer can be used to identify the handler or restart the listening.
    */
    void HandleNotifyError( TUint32 aId, TInt error, CCenRepNotifyHandler* aHandler );
    
protected:
    
    /**
    * Constructor.
    * 
    * @since S60 3.2
    * @param aSysApAppUi    reference to System Application application UI class
    */
    CSysApCenRepLightSettingsObserver( CSysApAppUi& aSysApAppUi );

private:

    /**
    * C++ default constructor.
    * 
    * Usage not permitted.
    */
    CSysApCenRepLightSettingsObserver();
    
    /**
    * Copy constructor.
    *
    * By default, prohibit copy constructor.
    */
    CSysApCenRepLightSettingsObserver( const CSysApCenRepLightSettingsObserver& );
    
    /**
    * Symbian 2nd-phase constuctor.
    */
    void ConstructL();
    
    /**
    * Copy constructor.
    *
    * By default, Prohibit assigment operator.
    */
    CSysApCenRepLightSettingsObserver& operator= ( const CSysApCenRepLightSettingsObserver& );

private: // data

    /**
    * Reference to System Application's UI class.
    */
    CSysApAppUi& iSysApAppUi;
    
    /**
    * Central repository session.
    * Own.
    */    
    CRepository* iSession;
    
    /**
    * Notification handler for lights timeout key.
    * Own.
    */ 
    CCenRepNotifyHandler* iDisplayLightsTimeoutHandler;
    };


#endif // SYSAPCENREPLIGHTSETTINGSOBSERVER_H

