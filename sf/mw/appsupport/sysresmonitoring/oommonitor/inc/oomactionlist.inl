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
* Description:  Classes for executing OOM actions (e.g. closing applications and running plugins).
*
*/


#ifndef OOMACTIONLISTINL_H_
#define OOMACTIONLISTINL_H_

template <class T>
inline TInt COomPluginList<T>::Count()
    {
    return iPlugins.Count();
    }
    
template <class T>
inline T& COomPluginList<T>::Implementation(TInt aIndex)
    {
    return *(iPlugins[aIndex].iImpl);
    }

template <class T>
inline TInt32 COomPluginList<T>::Uid(TInt aIndex)
    {
    return iPlugins[aIndex].iUid;
    }
    
inline void COomActionList::SetCurrentTargets(TUint aCurrentRamTarget, TUint aCurrentSwapTarget)
    {
    iCurrentRamTarget = aCurrentRamTarget;
    iCurrentSwapTarget = aCurrentSwapTarget;
    }


#endif /*OOMACTIONLISTINL_H_*/
