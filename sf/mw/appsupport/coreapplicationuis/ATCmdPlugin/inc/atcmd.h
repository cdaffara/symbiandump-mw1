
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


#ifndef __ATCMD_H
#define __ATCMD_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION
class TATCkpdCmd
  	{
    public:
        inline TATCkpdCmd() {}
    public:
        TInt iCode;
        TTimeIntervalMicroSeconds32 iStrokeDelay;
        TTimeIntervalMicroSeconds32 iStrokeTime;
    };

class CATCmdTranslation
    : public CBase
    {
    public:
        CATCmdTranslation( HBufC8& aParam, const TInt aCode );
        ~CATCmdTranslation();
    public:
        TInt Code() const;
        TPtrC8 Param() const;
    private:
        HBufC8* iParam;
        const TInt iCode;

    };



#endif // __ATCMD_H
            
// End of File
