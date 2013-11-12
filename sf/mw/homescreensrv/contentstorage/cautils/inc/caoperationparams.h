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

#ifndef __CAOPERATIONPARAMS_H__
#define __CAOPERATIONPARAMS_H__

#include <e32base.h>

/*
 * Describes operation type for Organize operation
 */
class TCaOperationParams
    {
public:
    enum OperationType
        {
        EInsert,
        EAppend,
        EPrepend,
        ERemove
        };

    /*
     * Operation type
     */
    OperationType iOperationType;

    /*
     * Group Id
     */
    TInt iGroupId;

    /*
     * Before Entry Id
     */
    TInt iBeforeEntryId;

    };

#endif //__CAOPERATIONPARAMS_H__
