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
* Description:  Classes for executing OOM actions (e.g. closing applications and running plugins).
*
*/


#ifndef OOMCLOSEAPPINL_H_
#define OOMCLOSEAPPINL_H_

inline TBool COomCloseApp::IsRunning()
    {
    return iAppCloserRunning;
    }

inline TUint COomCloseApp::WgId() const
    {
    return iWgId;
    }

#endif /*OOMCLOSEAPPINL_H_*/
