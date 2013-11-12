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
* Description:  CSysApCenRepMsgWaitingObserver class definition. 
*
*/


#ifndef SYSAPCENREPMSGWAITINGOBSERVER_H
#define SYSAPCENREPMSGWAITINGOBSERVER_H

#include <e32base.h>
#include <ctsydomaincrkeys.h>
#include <centralrepository.h>
#include <cenrepnotifyhandler.h>

class CSysApAppUi;

/**
*  CSysApCenRepMsgWaitingObserver class observes for changes in
*  message waiting display sstatus CenRep key.
*
*  @lib None.
*  @since S60 3.2
*/

class CSysApCenRepMsgWaitingObserver : public CBase, public MCenRepNotifyHandlerCallback
    {
    
public:
    
    /**
    * Values for voice mail waiting state.
    *
    * @since S60 3.2
    */
    enum TVoiceMailWaitingState
        {
        ENoVoiceMails, /// no voice mails
        EWaiting, /// voice mail waiting on primary line (ALS not supported)
        EWaitingOnLine1, /// voice mail waiting on primary line
        EWaitingOnLine2, /// voice mail waiting on secondary line
        EWaitingOnBothLines /// voice mail waiting on both primary and secondary line
        };

public:
    /**
    * Symbian two-phased constructor.
    *
    * @param aSysApAppUi reference to SysAp's application class
    */
    static CSysApCenRepMsgWaitingObserver* NewL( CSysApAppUi& aSysApAppUi );

    /**
    * Destructor.
    */
    virtual ~CSysApCenRepMsgWaitingObserver();

    /**
    * Updates message waiting indicators.
    *
    * @since S60 3.2
    */
    void UpdateMessageWaitingIndicatorsL();

public: // From MCenRepNotifyHandlerCallback
    void HandleNotifyInt( TUint32 aId, TInt aNewValue );
    void HandleNotifyError( TUint32 aId, TInt error, CCenRepNotifyHandler* aHandler );

private:

    /**
    * Symbian 2nd-phase constructor.
    */
    void ConstructL();

    /**
    * C++ default constructor.
    * Prohibited.
    */
    CSysApCenRepMsgWaitingObserver();
    
    /**
    * Constructor.
    *
    * @param aSysApAppUi reference to SysAp's application class
    */         
    CSysApCenRepMsgWaitingObserver( CSysApAppUi& aSysApAppUi );

    
    /** 
    * Copy constructor.
    * Prohibited.
    */
    CSysApCenRepMsgWaitingObserver( const CSysApCenRepMsgWaitingObserver& );

    /**
    * Assigment operator
    * Prohibited.
    */
    CSysApCenRepMsgWaitingObserver& operator= ( const CSysApCenRepMsgWaitingObserver& );
    
    /**
    * Updates message waiting indicators. Only indicators whose status have changed
    * are updated.
    *
    * @since S60 3.2
    */
    void UpdateMessageWaitingIndicatorsIfNeededL() const;
    
    /**
    * Updates voice mail waiting indicators.
    *
    * @since S60 3.2
    */
    void UpdateVoiceMailWaitingIndicatorsL() const;

    /**
    * Updates fax mail waiting indicator.
    *
    * @since S60 3.2
    */
    void UpdateFaxMessageWaitingIndicatorL() const;

    /**
    * Updates e-mail waiting indicator.
    *
    * @since S60 3.2
    */
    void UpdateEmailMessageWaitingIndicatorL() const;
    
    /**
    * Updates other message waiting indicator.
    *
    * @since S60 3.2
    */
    void UpdateOtherMessageWaitingIndicatorL() const;
    
    /**
    * Resolves current voice mail indicator state.
    *
    * @since S60 3.2
    * @return current voice mail state
    */
    TVoiceMailWaitingState ResolveVoiceMailState() const;

private:
    
    /**
    * SysAp's application class.
    */
    CSysApAppUi& iSysApAppUi;
    
    /**
    * CenRep session.
    * Own.
    */ 
    CRepository* iSession;
    
    /**
    * CenRep notification handler.
    * Own.
    */
    CCenRepNotifyHandler* iMessageWaitingHandler;
    
    /**
    * Bitmask containing MWIS information.
    */
    TInt iMessageWaitingDisplayStatus;
    
    /**
    * Bitmask containing MWIS information.
    */
    TInt iOldDisplayStatus;
    
    /**
    * Set to ETrue when indicators can be updated to UI.
    */
    TBool iUiReady;
    };

#endif      // SYSAPCENREPMSGWAITINGOBSERVER_H
            
// End of File
