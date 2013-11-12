/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Factory and main interface into the KEF.
*
*/



#ifndef KEFFACTORY_H
#define KEFFACTORY_H

#include <e32base.h>

_LIT( KKefLibraryName, "KeyEventFw.dll" );
const TInt KKefMapFactoryFunctionOrdinal = 1;

class MKefProvider;

/**
 *  Interface to the KEF.
 *
 *  A window server plug-in should instanciate an object and 
 *  pass the received raw enents into this component. 
 *
 *  @lib KeyEventFw.lib
 *  @since S60 v3.2
 */
class CKefMap : public CBase
    {
public:

    /**
     * Sets the provider. Needs to be called before the 
     * OfferRawEvent function can be called.
     * 
     * @param aProvider API for generating key events.
     */
    virtual void SetProvider( MKefProvider& aProvider ) = 0;
    
    /**
     * Checks if key event map is enabled.
     * @return ETrue if enabled, EFalse otherwise.
     */
    virtual TBool IsKeyEventMapEnabled() const = 0;
    
    /**
     * Handles raw events.
     * @param aRawEvent the raw event from the kernel 
     * @param aSilentEvent If set, the event will not generate keysound.
     * @return ETrue if the event is consumed, EFalse otherwise.
     */
    virtual TBool OfferRawEvent( const TRawEvent& aRawEvent, TBool aSilentEvent ) = 0; 
    };


/**
 *  KEF Factory
 *
 *  Use this factory to create an instance of CKefMap.
 *
 *  This function can be used in two ways:
 *  1# static link
 *  -link into KeyEventFw.lib
 *  -and call KefMapFactory::CreateKefMapL()
 * 
 *  2# polymorhic DLL
 *  -not NOT link into KeyEventFw.lib
 *  -load library with RLibrary - use KKefLibraryName as library name
 *  -accesss the ordinal KKefMapFactoryFunctionOrdinal from the library
 *   to execute the function call
 *
 *  @lib KeyEventFw.lib
 *  @since S60 v3.2
 */
class KefMapFactory
    {
public:
    /**
     * Creates CKefMap instance
     * The entry ordinal 1 of the library points to this function.
     * 
     * @return New instance. Ownership transferred.
     */
    IMPORT_C static CKefMap* CreateKefMapL();
    };

#endif // KEFFACTORY_H
