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


inline CRfsCompleter* CRfsSession::EpocCompleter()
    {
    return iEpocCompleter;
    }

inline TRequestStatus* CRfsSession::EpocCompleterRequestStatus()
    {
    return &( iEpocCompleter->iStatus );
    }

inline CRfsCompleter* CRfsSession::DosCompleter()
    {
    return iDosCompleter;
    }

inline TRequestStatus* CRfsSession::DosCompleterRequestStatus()
    {
    return &( iDosCompleter->iStatus );
    }

inline CRfsServer* CRfsSession::Server()
    {
    return (CRfsServer*)( CSession2::Server() );
    }

inline TBool CRfsSession::RequestHandled()
    {
    return ( iEpocCompleter->iFlags & KRfsCompletionHandled ) &&
           ( iDosCompleter->iFlags & KRfsCompletionHandled );
    }

inline void CRfsSession::CompleteMessage()
    {
    // Both Symbian OS and DOS side return values contain information of how many
    // RFS operations failed. Those values are summed up in the client
    // message's return value.

    Message().Complete( iEpocCompleter->iReturn + iDosCompleter->iReturn );
    }
