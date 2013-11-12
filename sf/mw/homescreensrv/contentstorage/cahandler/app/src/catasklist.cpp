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

#include<apgwgnam.h>

#include"catasklist.h"

CCaTaskList* CCaTaskList::NewL( RWsSession& aWsSession )
    {
    CCaTaskList* self = NewLC( aWsSession );
    CleanupStack::Pop( self );
    return self;
    }

CCaTaskList* CCaTaskList::NewLC( RWsSession& aWsSession )
    {
    CCaTaskList* self = new(ELeave) CCaTaskList( aWsSession );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CCaTaskList::CCaTaskList( RWsSession& aWsSession )
: iWs(aWsSession)
    {
    }

void CCaTaskList::ConstructL()
    {
    UpdateListL();
    }

CCaTaskList::~CCaTaskList()
    {
    iWgs.Close();
    }

void CCaTaskList::UpdateListL()
    {
    User::LeaveIfError( iWs.WindowGroupList( &iWgs ) );
    }


TApaTask CCaTaskList::FindRootApp( TUid aAppUid ) const
    {
    TApaTask task( iWs );
    task.SetWgId(0);        // initialise task to non-existant task
    // wgId = 0 tells FindAppByUid to start looking for apps
    TInt wgId(0);
    FOREVER
        {
        CApaWindowGroupName::FindByAppUid( aAppUid, iWs, wgId );
        // KErrNotFound means that no more apps can be found
        if( wgId == KErrNotFound )
            break;
        if( IsRootWindowGroup( wgId ) )
            {
            // Found a root wg with the right app UID, return it.
            task.SetWgId( wgId );
            break;
            }
        }
    return task;
    }

TBool CCaTaskList::IsRootWindowGroup( TInt aWgId ) const
    {
    TInt count = iWgs.Count();
    for( TInt i=0; i<count; i++ )
        {
        const RWsSession::TWindowGroupChainInfo& info = iWgs[i];
        // find the window group id and check that it has no parent
        if ( info.iId == aWgId )
        	{
            return ( info.iParentId <= 0 );
        	}
        }
    return EFalse;
    }
