/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*     Defines RFS ECom interface.
*
*/


#ifndef RFSPLUGIN_H
#define RFSPLUGIN_H

//  INCLUDES
#include <e32base.h>
#include <ecom/ecom.h>
#include <rfsPluginDef.h>

// CONSTANTS

/**
*	An abstract class being representative of the
*	concrete class which the client wishes to use.
*	It acts as a base, for a real class to provide all the
*	functionality that a client requires.
*	It supplies instantiation & destruction by using
*	the ECom framework, and functional services
*	by using the methods of the actual class.
*/
class CRFSPlugin : public CBase
    {
public:

    /**
    * Two-phased constructor.
    */
    static CRFSPlugin* NewL( const TUid& aImplementationUid );

    /**
    * Destructor.
    */
    virtual ~CRFSPlugin();

    /** 
	* Request a list of all available implementations which satisfies this given interface.
    * @param aImplInfoArray Implementation instance array.
    * @return none.
	*/
    static void ListAllImplementationsL(RImplInfoPtrArray& aImplInfoArray);

    /** 
	* 
    * @param
    * @return
	*/
    virtual void RestoreFactorySettingsL( const TRfsReason aType ) = 0;

    /** 
	* 
    * @param
    * @return
	*/
    virtual void GetScriptL( const TRfsReason aType, TDes& aPath ) = 0;

    /** 
	* 
    * @param
    * @return
	*/
	virtual void ExecuteCustomCommandL( const TRfsReason aType, 
                                        TDesC& aCommand ) = 0;
protected:
    /**
    * C++ default Constructor
    */
    inline CRFSPlugin();

private:
    // Unique instance identifier key
    TUid iDtor_ID_Key;
    };

#include "rfsPlugin.inl"

#endif //RFSPLUGIN_H

