/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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

    
inline CRfsServer::TSignal::TSignal() {}

inline CRfsServer::TSignal::TSignal(TRequestStatus& aStatus)
    : iStatus(&aStatus), iId(RThread().Id())
    {aStatus=KRequestPending; }

inline TPtrC CRfsServer::TSignal::Get() const
    {return TPtrC((const TText*)this,sizeof(*this)/sizeof(TText));}

inline TInt CRfsServer::TSignal::Set(const TDesC& aData)
    {
    return aData.Size()!=sizeof(*this) ? 
           KErrGeneral :
           (Mem::Copy(this,aData.Ptr(),sizeof(*this)),KErrNone);
    }
