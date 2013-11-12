/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef CATASKLIST_H
#define CATASKLIST_H

// INCLUDES

#include<w32std.h>
#include<apgtask.h>

// CLASS DECLARATION

/**
*  Class for finding out about running applications.
*
*  This is a modification of the original CAknTaskList class, which
*  differs in that it does not discriminate applications based on their
*  window group priority.
*/
class CCaTaskList : public CBase
    {
public:

	/**
    * Factory function
    * @param aWsSession an open session to the window server, often from CEikonEnv::WsSession()
    * @return a new fully constructed instance of CCaTaskList
    */
    static CCaTaskList* NewL( RWsSession& aWsSession );

    /**
    * Factory function
    * @param aWsSession an open session to the window server, often from CEikonEnv::WsSession()
    * @return a new fully constructed instance of CCaTaskList, which is on the cleanup stack
    */

    static CCaTaskList* NewLC( RWsSession& aWsSession );
    /**
    * Destructor.
    */
    ~CCaTaskList();

    /**
    * Refresh the window group array
    */
    void UpdateListL();

    /**
    * Accessor for the window group array
    * @return an array containing the window groups of running applications.
    */
    const RArray<RWsSession::TWindowGroupChainInfo>& WgArray() const;

    /**
    * Find an application with the requested UID 3, which is running as a root application
    * @param aAppUid the UID 3 of the target application.
    * @return a TApaTask which refers to the running instance of the application.
    * if the application is not running, the TApaTask's Exists() function will return EFalse.
    */
    TApaTask FindRootApp( TUid aAppUid ) const;

    /**
    * Query whether an application's window group is running as a root application.
    * @param aWgId the window group identifier of the target application.
    * @return ETrue if this window group is running as a root window group.
    */
    TBool IsRootWindowGroup( TInt aWgId ) const;

private:

    CCaTaskList( RWsSession& aWsSession );

    void ConstructL();

private:

    RWsSession& iWs;

    RArray<RWsSession::TWindowGroupChainInfo> iWgs;

    };

#endif
