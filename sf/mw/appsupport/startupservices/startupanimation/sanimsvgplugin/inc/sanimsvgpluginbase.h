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
* Description:  Declaration of CSAnimSvgPluginBase class
*
*/


#ifndef SANIMSVGPLUGINBASE_H
#define SANIMSVGPLUGINBASE_H

#include "sanimimageplugin.h"
#include <SVGRequestObserver.h>

class CSvgEngineInterfaceImpl;
class MSvgError;

/**
*  Base class for different Start-up Image plug-ins that use SVGT engine.
*
*  @lib None
*  @since S60 3.2
*/
NONSHARABLE_CLASS( CSAnimSvgPluginBase )
  : public CSAnimImagePlugin,
    public MSvgRequestObserver
    {

public:

    /**
    * First phase constructor.
    *
    * @since S60 3.2
    *
    * @param aConstructionParameters Construction parameters from ECom.
    * May NOT be NULL.
    */
    CSAnimSvgPluginBase( TAny* aConstructionParameters );

    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CSAnimSvgPluginBase();

private:

    /**
    * From MSvgRequestObserver
    *
    * @since S60 3.2
    */
    virtual TBool ScriptCall(
        const TDesC& aScript,
        CSvgElementImpl* aCallerElement );

    /**
    * From MSvgRequestObserver
    *
    * @since S60 3.2
    */
    virtual TInt FetchImage(
        const TDesC& aUri,
        RFs& aSession,
        RFile& aFileHandle );

    /**
    * From MSvgRequestObserver
    *
    * @since S60 3.2
    */
	virtual TInt FetchFont(
	    const TDesC& aUri,
	    RFs& aSession,
	    RFile& aFileHandle );

    /**
    * From MSvgRequestObserver
    *
    * @since S60 3.2
    */
    virtual void GetSmilFitValue( TDes& aSmilValue );

    /**
    * From MSvgRequestObserver
    *
    * @since S60 3.2
    */
    virtual void UpdatePresentation( const TInt32& aNoOfAnimation );

protected:

    /**
    * Convert SVG engine error code to one of the Symbian system-wide error codes.
    *
    * @since S60 3.2
    *
    * @return One of the Symbian system-wide error codes or KErrNone.
    */
    TInt SvgToSymbianErr( MSvgError* aError );

protected: // data

    /** Default font spec to give to the engine. */
    TFontSpec iFontSpec;

    /** SVGT engine which generates the image / animation frames. Owned. May be NULL. */
    CSvgEngineInterfaceImpl* iEngine;

    };

#endif // SANIMSVGPLUGINBASE_H
