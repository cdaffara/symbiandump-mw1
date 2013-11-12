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
* Description:  CSysApPowerKeyMenuObserver class definition. This class
*                implements the observer for getting the menu selection from
*                CAknGlobalListQuery.
*
*/


#ifndef SYSAPPOWERKEYMENUOBSERVER_H
#define SYSAPPOWERKEYMENUOBSERVER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CSysApAppUi;

/**
*  CSysApPowerKeyMenuObserver
*  
*  @lib   sysap
*  @since 1.0 
*/

class CSysApPowerKeyMenuObserver : public CActive
    {
public:
    /**
    * Constructor
    * @param CSysApAppUi& aSysApAppUi
    * @return void
    */         
    CSysApPowerKeyMenuObserver( CSysApAppUi& aSysApAppUi );

    /**
    * Destructor.
    */
    virtual ~CSysApPowerKeyMenuObserver();

    /**
    * This method activates the active object. The iStatus is given to
    * CAknGlobalListQuery with a call iGlobalListQuery->ShowListQueryL( 
    * profileNameArray, iSysApPowerKeyMenuObserver->iStatus, KProfileListInitialIndex ); 
    * from CSysApAppUi
    * @param note
    * @return void
    */         
    void Start();

private:
    /**
    * C++ default constructor.
    * @param None
    * @return void
    */
    CSysApPowerKeyMenuObserver();
    
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
    CSysApAppUi&    iSysApAppUi;
    };

#endif //SYSAPPOWERKEYMENUOBSERVER_H

// End of File
