/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApMsgSimMemLowQuery class definition. This class is
*                responsible for showing a query when SIM card's SMS memory
*                is full and there is a SIM specific SMS coming.
*
*/


#ifndef SYSAPMSGSIMMEMLOWQUERY_H
#define SYSAPMSGSIMMEMLOWQUERY_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CAknGlobalConfirmationQuery;

// CLASS DECLARATION
/**
*  CSysApMsgSimMemLowQuery
*  
*  @lib   sysap
*  @since 1.0* 
*/

class CSysApMsgSimMemLowQuery : public CActive
    {
public:
    /**
    * Constructor
    * @param ETrue if cover display is supported
    * @return void
    */         
    CSysApMsgSimMemLowQuery(TBool aCoverDisplaySupported);

    /**
    * Destructor.
    */
    virtual ~CSysApMsgSimMemLowQuery();

    /**
    * This method activates the active object. The iStatus is given to
    * CAknGlobalListQuery with a call iGlobalListQuery->ShowListQueryL( 
    * profileNameArray, iSysApPowerKeyMenuObserver->iStatus, KProfileListInitialIndex ); 
    * from CSysApAppUi
    * @param note
    * @return void
    */         
    void StartL( const TDesC& aPrompt );

private:
    /**
    * From CActive. Cancels the active object from observing the list query
    * @param None
    * @return void
    */
    void DoCancel();
    /**
    * From CActive. Gets called by active object when a selection is done.
    * @param None
    * @return void
    */
    void RunL();

    /**
    * From CActive. Gets called by active object when leave happens inside RunL
    * @param None
    * @return void
    */
    TInt RunError( TInt aError );

private:
    CAknGlobalConfirmationQuery* iSimMemoryLowQuery;
    TBool iCoverDisplaySupported;
    };

#endif //SYSAPPOWERKEYMENUOBSERVER_H

// End of File
