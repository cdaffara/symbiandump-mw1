/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Context source plugin for sis installations.
*
*/


#ifndef CONTEXTSOURCE_V1_H
#define CONTEXTSOURCE_V1_H

// SYSTEM INCLUDE FILES
#include <e32base.h>
#include <CFContextSourcePlugIn.h>
#include <cfcontextsourceinterface.h>
#include <cfcontextsourceupgrade.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS
class TContextSourceInitParams;
class CCFContextObject;

// DATA TYPES

// CLASS DECLARATION

/**
 * Context source v1.
 *
 * @lib -
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CContextSource_V1 ):
    public CCFContextSourcePlugIn,
    public MCFContextSource,
    public MCFContextSourceUpgrade
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param None.
     * @return CContextSource_V1
     */
    static CContextSource_V1* NewL( TContextSourceInitParams* aParams );
    static CContextSource_V1* NewLC( TContextSourceInitParams* aParams );

    /**
     * C++ destructor.
     */
    virtual ~CContextSource_V1( );

private: // from CCFContextSourcePlugIn

    // @see CCFContextSourcePlugIn
    TAny* Extension( const TUid& aExtensionUid ) const;
    
    // @see CCFContextSourcePlugIn
    void HandleSettingL( CCFContextSourceSettingArray* aSettingList );
        
    // @see CCFContextSourcePlugIn
    void DefineContextsL();
    
    // @see CCFContextSourcePlugIn
    void InitializeL();

private: // from MCFContextSource

    // from MCFContextSource
    void Subscribers( const TDesC& aContextSource,
         const TDesC& aContextType );
    
    // from MCFContextSource
    void NoSubscribers( const TDesC& aContextSource,
         const TDesC& aContextType );

    // from MCFContextSource
    void HandleCommand( const CCFContextSourceCommand& aCommand );

private: // From MCFContextSourceUpgrade

    // @see MCFContextSourceUpgrade
    TInt CheckValidity( RThread& aClientThread,
            const CCFContextSourceSettingArray& aSettingList );
    
    // @see MCFContextSourceUpgrade
    void UpdateSettingsL( CCFContextSourceSettingArray* aSettingList );
    
private:

    // Modify context object which can leave
    void UpdateContextObjectL( const TDesC& aSource, const TDesC& aType, const TDesC& aValue );
    
private:

    CContextSource_V1( TContextSourceInitParams* aParams );
    void ConstructL( );

private: // Data

    /** Context object */
    CCFContextObject* iContext;
    };

#endif // CONTEXTSOURCE_V1_H

