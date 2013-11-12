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
#include "tsdatatask.h"
#include <s32mem.h>
// -----------------------------------------------------------------------------
void TsDataTask::ExecuteL(const TArray<MTsDataStorage*>& dataStorages, const RMessage2& msg)
    {
    TInt responseCode(KErrNotSupported);
    for( TInt iter(0); iter < dataStorages.Count() && KErrNone != responseCode; ++iter )
        {
        if(dataStorages[iter]->IsSupported( msg.Function() ) ) 
            {
            HBufC8 *data = HBufC8::NewLC(msg.GetDesLength(0));
            TPtr8 des(data->Des());
            msg.ReadL(0, des);
            RDesReadStream dataStream(*data);
            CleanupClosePushL(dataStream);
            dataStorages[iter]->HandleDataL(msg.Function(), dataStream);
            CleanupStack::PopAndDestroy(&dataStream);
            CleanupStack::PopAndDestroy(data);
            responseCode = KErrNone;
            }
        }
    User::LeaveIfError(responseCode);
    msg.Complete(responseCode);
    }
