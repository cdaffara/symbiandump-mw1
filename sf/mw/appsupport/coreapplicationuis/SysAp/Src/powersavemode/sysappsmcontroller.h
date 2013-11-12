/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SysAp power save mode controller class declaration.
*
*/


#ifndef SYSAPPSMCONTROLLER_H
#define SYSAPPSMCONTROLLER_H


#include <e32base.h>
#include <psmclientobserver.h>

class CPsmClient;

/**
* A callback interface for CSysApPsmController client.
*
*  @lib None.
*  @since S60 3.2
*/
class MSysApPsmControllerNotifyCallback
    {
    public:
        enum TSysApPsmStatus
            {
            EPsmActivationComplete, // power save mode activation completed
            EPsmActivationFailed, // failure in power save mode activation
            EPsmDeactivationComplete, // power save mode deactivation completed
            EPsmDeactivationFailed, // failure in power save mode deactivation
            };
        
        /**
        * Notifies client about operation completion. 
        *
        * @since S60 3.2
        * @param aStatus status of operation
        */
        virtual void NotifyPowerSaveModeL( TSysApPsmStatus aStatus ) = 0;
    
    };

/**
*  SysAp power save mode controller.
*
*  @lib None.
*  @since S60 3.2
*/
class CSysApPsmController : public CBase,
                            public MPsmClientObserver
    {

public:
    
    /**
    * Symbian two-phased constructor.
    *
    * @param aCallback notification callback
    */
    static CSysApPsmController* NewL( MSysApPsmControllerNotifyCallback& aCallback );

    /**
    * Destructor.
    */
    virtual ~CSysApPsmController();

    /**
    * Returns whether full power save mode is currently activated.
    *
    * @since S60 5.0
    * @return Returns ETrue if full power save mode is enabled, otherwise EFalse.
    */
    TBool FullPsmEnabled() const;
    
    /**
    * Returns whether partial power save mode is currently activated.
    *
    * @since S60 5.0
    * @return Returns ETrue if partial power save mode is enabled, otherwise EFalse.
    */
    TBool PartialPsmEnabled() const;
    
    /**
    * Getter for UI item ID.
    *
    * @since S60 3.2
    * @return UI item ID.
    */
    TInt UiItemId() const;

    /**
    * Setter for object specific UI item. Used for storing information related to
    * the UI item this object is tied to.
    *
    * @since S60 3.2
    * @param aUiItemId UI item ID
    */
    void SetUiItemId( TInt aUiItemId );
    
    
    /**
    * Activates or deactivates the partial power save mode.
    *
    * @since S60 5.0
    * @param aEnable If ETrue, power saving should be enabled, otherwise disabled.
    */
    void DoEnablePartialPsm( TBool aEnable );
    
    /**
    * Sets partial PSM to force stated, i.e. when full PSM is deactivated partial mode 
    * is activated instead of normal mode.
    *
    * @since S60 5.0
    * @param aBatteryIsLow ETrue if battery is low, EFalse if not low.
    */
    void BatteryLow( TBool aBatteryIsLow );
    
    /**
    * Releases partial PSM to force stated and resets the internal counter that is 
    * used to determining whether to display PSM activation query on battery low event.
    *
    * @since S60 5.0
    * @param aConnect ETrue if connected, EFalse if disconnected.
    */
    void ConnectCharger( TBool aConnect );
    
    /**
    * Returns whether charger has been already connected.
    *
    * @since S60 5.0
    */
    TBool ChargerConnected() const;
    
    /**
    * Full power save mode is activated by the user.
    *
    * @since S60 5.0
    * @param aEnable If ETrue, power saving should be enabled, otherwise disabled.
    */    
    void DoEnableFullPsm( TBool aEnable );
    
    /**
    * Stores the next operation that will be performed due to
    * user interaction.
    *
    * @since S60 3.2
    * @param aIsActivate ETrue next UI operation is activate power save
    *                    EFalse next UI operation is deactive power save
    */
    void SetNextUiOperation( TBool aIsActivate );
    
    /**
    * Returns the next UI operation
    *
    * @since S60 3.2
    */
    TBool NextUiOperation() const;
    
    /**
    * Returns whether activate PSM query should be shown.
    *
    * @since S60 5.0
    */
    TBool ShowActivateQuery();
    
    /**
    * Returns whether deactivate PSM query should be shown.
    *
    * @since S60 5.0
    */
    TBool ShowDeactivateQuery() const;
    
private:
    /**
    * C++ default constructor.
    * Not used.
    */
    CSysApPsmController();
    
    /**
    * Constructor
    */
    CSysApPsmController( MSysApPsmControllerNotifyCallback& aCallback );

    /**
    * Symbian 2nd-phase constructor.
    */
    void ConstructL();
    
    /**
    * Changes the power save mode. 
    *
    * @since S60 3.2
    * @param aNewMode the new PSM mode.
    */
    void SetPowerSaveMode( TInt aNewMode );

    // from MPsmClientObserver
    virtual void PowerSaveModeChanged( const TPsmsrvMode aMode );

    virtual void PowerSaveModeChangeError( const TInt aError );
    
    /**
    * Returns whether activation/deactivation query is enabled in device settings.
    */
    TBool QueriesEnabled() const;

private: // data
    /**
    * Client callback.
    */
    MSysApPsmControllerNotifyCallback& iCallback;
    
    /**
    * PSM Server client.
    * Own.
    */
    CPsmClient* iPsmClient;
    
    /**
    * Current activation status of power save mode.
    */
    TInt iActivePsm;

    /**
    * If ETrue, partial mode is activated always when user tries to restore normal mode.
    */ 
    TBool iForcedPartialPsm;

    /**
    * Storage for arbitrary information used by the client of this class. 
    * This variable is not used internally by this class.
    */
    TInt iUiItemId;
    
    /**
    * Storage variable for client's use.
    */
    TBool iNextUiOperation;
    
    /**
    * Whether note dialogs should be shown.
    * This set to when partial PSM transitions are initiated.
    */
    TBool iDiscardUiNotes;
    
    /**
    * Counter for low battery events.
    * Used for determining whether PSM activation query should be shown.
    */
    TInt iBatteryLowCounter;
    
    /**
    * Whether charger has been connected.
    */
    TBool iChargerConnected;
    };


#endif // SYSAPPSMCONTROLLER_H
