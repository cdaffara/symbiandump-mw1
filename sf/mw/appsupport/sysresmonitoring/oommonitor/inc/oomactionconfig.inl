/*
* Copyright (c) 2006-2010  Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef OOMACTIONCONFIGINL_H_
#define OOMACTIONCONFIGINL_H_

// Set the priority of this action
// This priority will be applied if none of the attached rules can be applied
inline void COomActionConfig::SetDefaultPriority(TUint aPriority)
    {
    iDefaultPriority = aPriority;
    }


#endif /*OOMACTIONCONFIGINL_H_*/
