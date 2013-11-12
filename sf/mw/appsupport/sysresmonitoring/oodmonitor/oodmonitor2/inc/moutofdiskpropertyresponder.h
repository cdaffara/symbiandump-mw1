/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __MOUTOFDISKPROPERTYRESPONDER_H__
#define __MOUTOFDISKPROPERTYRESPONDER_H__

class MOutOfDiskPropertyResponder
    {
public:
    virtual void HandlePropertyChangedL( const TUid& aCategory, TUint aKey ) = 0;
    };
#endif //__MOUTOFDISKPROPERTYRESPONDER_H__

// End of File
