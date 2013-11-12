/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Configuration representation classes for Out of Memory Monitor.
*
*/


#ifndef OOMGLOBALCONFIGINL_H_
#define OOMGLOBALCONFIGINL_H_

void COomGlobalConfig::AddForceCheckPriorityL(TUint aPriority)
    {
    iForceCheckPriorities.InsertL(aPriority);
    }

// Return ETrue if a force check has been added for this priority, return EFalse otherwise
TBool COomGlobalConfig::ForceCheckAtPriority(TUint aPriority) const
    {
    return (iForceCheckPriorities.Find(aPriority) != NULL);
    }

#endif /*OOMGLOBALCONFIGINL_H_*/
