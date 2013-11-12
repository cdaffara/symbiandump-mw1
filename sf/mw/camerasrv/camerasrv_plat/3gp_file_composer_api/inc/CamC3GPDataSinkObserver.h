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
* Description:  3GP data sink observer class definition
*
*/


#ifndef     __CAMC3GPDATASINKOBSERVER_H__
#define     __CAMC3GPDATASINKOBSERVER_H__


//  INCLUDES

#include <e32base.h>


//  CLASS DEFINITIONS

class M3GPDataSinkObserver
    {
    public:

        /**
        * When the 3GP data sink notices that the disk is getting full, it
        * calls this method (implemented by the controller) to indicate that
        * the file composer has stopped saving.
        * @since 2.1
        * @param None
        * @return No return value
        */
        virtual void MfcoDiskFullL() = 0;

        /**
        * When the 3GP data sink notices that the file size limit has been
        * reached, it calls this method (implemented by the controller) to
        * indicate that the file composer has stopped saving.
        * Size limit of the file has been reached.
        * @since 2.1
        * @param None
        * @return No return value
        */
        virtual void MfcoSizeLimitReachedL() = 0;
    };


#endif      //  __CAMC3GPDATASINKOBSERVER_H__

// End of File
