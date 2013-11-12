/*
* Copyright (c) 2003-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysapConfirmationQuery header file. The clas can be used for
*                showing global confirmation queries.
*
*/


#ifndef SYSAPCONFIRMATIONQUERY_H
#define SYSAPCONFIRMATIONQUERY_H

// INCLUDES
#include <e32base.h>
#include <AknGlobalConfirmationQuery.h>
#include "SysAp.hrh"

// CONSTANTS

// FORWARD DECLARATIONS

class CSysApAppUi;

// CLASS DECLARATION
/**
*  CSysApConfirmationQuery
*  
*  @lib   sysap
*  @since 1.2S 
*/

class CSysApConfirmationQuery: public CActive
{
public:

    /**
    * Constructor
    */         
    static CSysApConfirmationQuery* NewL( CSysApAppUi& aSysApAppUi );

    /**
    * Destructor.
    */
    virtual ~CSysApConfirmationQuery();

    /**
    * This method shows the note
    * @param TSysApConfirmationQueryIds
    * @param CCoeEnv*
    * @return void
    */         
    void ShowQueryL( const TSysApConfirmationQueryIds aQueryId, CCoeEnv* aLoaderEnv = NULL );

    /**
    * This method shows the note
    * @param aQueryId Query id defined by TSysApConfirmationQueryIds
    * @param aValue Query related value
    * @param aLoaderEnv Pointer to control environment
    */         
    void ShowQueryL( const TSysApConfirmationQueryIds aQueryId,
        const TDesC& aValue, CCoeEnv* aLoaderEnv = NULL );

    /**
    * From CActive. Cancels the active object from observing the list query
    * @param  None
    * @return void
    */
    void DoCancel();
    /**
    * From CActive. Gets called by active object when a selection is done.
    * @param  None
    * @return void
    */
    void RunL();

    /**
    * From CActive. Gets called by active object when leave happens inside RunL
    * @param None
    * @return void
    */
    TInt RunError( TInt aError );
    
    /**
    * Gets current query ID
    * @return Current query ID (TSysApConfirmationQueryIds)
    */
    TInt CurrentQuery();

private:

    /**
    * Constructors
    */         
    CSysApConfirmationQuery( CSysApAppUi& aSysApAppUi );
    CSysApConfirmationQuery();
    void ConstructL();

private:

    CSysApAppUi&                 iSysApAppUi;
    TSysApConfirmationQueryIds   iQueryId;
    CAknGlobalConfirmationQuery* iQuery;   
    TSysApConfirmationQueryIds   iPendingQuery;               
};

#endif // SYSAPCONFIRMATIONQUERYNOTE_H

// End of File

