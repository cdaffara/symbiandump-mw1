
/*
 * Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 * Description :
 *
 */



// INCLUDES
#include "debug.h"
#include "atcmd.h"

// CONSTANTS

// ============================= LOCAL FUNCTIONS  =============================

// ============================= MEMBER FUNCTIONS =============================

CATCmdTranslation::CATCmdTranslation( HBufC8& aParam, const TInt aCode )
    : iParam( &aParam ),
      iCode( aCode )
    {
    }

CATCmdTranslation::~CATCmdTranslation()
    {
    delete iParam;
    }

TInt CATCmdTranslation::Code() const
    {
    return iCode;
    }

TPtrC8 CATCmdTranslation::Param() const
    {
    return iParam->Des();
    }

//  End of File  
