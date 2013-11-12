/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MSysApUsbIndicator class definition. A virtual class.
*
*/


#ifndef SYSAPUSBINDICATOR_H
#define SYSAPUSBINDICATOR_H

// CLASS DECLARATION
/**
*  MSysApUsbIndicator
*  
*  @lib   sysap
*  @since 2.1 
*/

class CSysApAppUi;

class MSysApUsbIndicator
{
public:

    /**
    * Destructor.
    */
    virtual ~MSysApUsbIndicator() {};
};

MSysApUsbIndicator* CreateSysApUsbIndicatorL( CSysApAppUi& aSysApAppUi );

#endif // SYSAPUSBINDICATOR_H

// End of File
