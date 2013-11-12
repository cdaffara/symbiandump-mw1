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
* Description:
*
*/
#include "tsdataprovidertask.h"
#include "tstaskmonitorglobals.h"

// -----------------------------------------------------------------------------
/**
 * Task handle data reqest. Function analyse reqest parameters and generate valid response.
 * @param aProvider - serialized data provider
 * @param aMsg - request message
 */
void TsDataProviderTask::ExecuteL( const MTsDataProvider& aProvider, 
                                   const RMessage2& aMsg )
    {
    switch( aMsg.Function() ) 
        {
    case GetRunningAppInfo:
        aMsg.WriteL( KRequestDataSizeOffset, 
                     TPckgBuf<TInt>( aProvider.Data().Length() ) );
        aMsg.Complete( KErrNone );
        break;
    
    case FlushData:
        if ( aProvider.Data().Length() > aMsg.GetDesMaxLengthL(KRequestDataBufferOffset) )
            {
            User::Leave( KErrCorrupt );
            }
        aMsg.WriteL( KRequestDataBufferOffset, aProvider.Data() );
        aMsg.Complete( KErrNone );
        break;
        }
    }
