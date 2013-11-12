/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MSysApBtController class definition. A virtual class.
*
*/


#ifndef SYSAPBTCONTROLLER_H
#define SYSAPBTCONTROLLER_H

// CLASS DECLARATION
/**
*  MSysApBtController
*  
*  @lib   sysap
*  @since 2.1 
*/

class CSysApAppUi;

class MSysApBtController
{
public:

    /**
    * Destructor.
    */
    virtual ~MSysApBtController() {};

    /**
    * Switches BT module on/off
    * @param None
    * @return void
    */	
    virtual TInt SetPowerState( TBool aBtState ) = 0;

};

MSysApBtController* CreateSysApBtControllerL( CSysApAppUi& aSysApAppUi );

#endif // SYSAPBTCONTROLLER_H

// End of File
