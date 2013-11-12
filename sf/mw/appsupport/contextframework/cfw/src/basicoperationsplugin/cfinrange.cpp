/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFInRange class implementation.
*
*/



// INCLUDES
#include "cfinrange.h"
#include "cfcontextobject.h"
#include "cfcontextoperationutils.h"
#include "cfbasicoptrace.h"

#include <e32math.h>
#include <gmxmlnode.h>

// CONSTANTS
_LIT( KScriptInRangeName, "inRange" );

const TReal KEarthRadius = 6378.137;
const TReal KFlattening  = 1.000000 / 298.257223563; // Earth flattening (WGS84)
const TReal KEarthPS     = 0.000000000005;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCFInRange::CCFInRange
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCFInRange::CCFInRange( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    HBufC* aName,
    HBufC* aSource,
    const CCFContextOperation::TCmpType aType )
    :   CCFContextOperation( aServices, aParent, aName, aSource ),
        iType( aType )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFInRange::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCFInRange::ConstructL( const TDesC& aMinVal, const TDesC& aMaxVal )
    {
    FUNC_LOG;

    iMinVal = aMinVal.AllocL();
    iMaxVal = aMaxVal.AllocL();
    }

// -----------------------------------------------------------------------------
// CCFInRange::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFInRange* CCFInRange::NewL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    TDesC& aName,
    TDesC& aSource,
    const CCFContextOperation::TCmpType aType,
    const TDesC& aMinVal,
    const TDesC& aMaxVal )
    {
    FUNC_LOG;

    CCFInRange* self
        = NewLC( aServices, aParent, aName, aSource, aType, aMinVal, aMaxVal );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFInRange::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFInRange* CCFInRange::NewLC( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    TDesC& aName,
    TDesC& aSource,
    const CCFContextOperation::TCmpType aType,
    const TDesC& aMinVal,
    const TDesC& aMaxVal )
    {
    FUNC_LOG;

    HBufC* name = aName.AllocLC();
    HBufC* source = aSource.AllocLC();
    CCFInRange* self =
        new( ELeave ) CCFInRange( aServices, aParent, name, source, aType );
    CleanupStack::PushL( self );
    self->ConstructL( aMinVal, aMaxVal );

    // Cleanup
    CleanupStack::Pop( self );
    CleanupStack::Pop( source );
    CleanupStack::Pop( name );

    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFInRange::ParseL
// Construction with parsing from a DOM node.
// -----------------------------------------------------------------------------
//
CCFInRange* CCFInRange::ParseL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    CMDXMLNode& aNode )
    {
    FUNC_LOG;

    if ( aNode.NodeName().CompareF( KScriptInRangeName ) != 0 )
        {
        return NULL; // Cannot create in range operation from the given node.
        }

    TPtrC contextSource;
    TPtrC contextType;
    TPtrC contextValue;
    TCmpType cmpMinType( CCFContextOperation::EInvalidCmpType );
    TCmpType cmpMaxType( CCFContextOperation::EInvalidCmpType );
    TPtrC minValue;
    TPtrC maxValue;
    TInt contextLevelDelay( 0 );

    TBool contextRefOK( EFalse );
    TBool minArgOK( EFalse );
    TBool maxArgOK( EFalse );
    TBool argsOK( ETrue ); // Will be set to false if unknown nodes detected.
    CMDXMLNode* child = aNode.FirstChild();
    while ( child )
        {
        if ( child->NodeType() == CMDXMLNode::EElementNode )
            {
            // Mandatory EElementNode order: 1. contextRef, 2. minArg, 3. maxArg
            if ( !contextRefOK )
                {
                contextRefOK = CFContextOperationUtils::ParseContextRef(
                        *child,
                        contextSource,
                        contextType,
                        contextValue,
                        contextLevelDelay );
                if ( !contextRefOK )
                    {
                    argsOK = EFalse;
                    break;
                    }
                }
            else if ( !minArgOK )
                {
                minArgOK = CFContextOperationUtils::ParseComparisonTypeValue(
                        *child,
                        cmpMinType,
                        minValue );
                if ( !minArgOK )
                    {
                    argsOK = EFalse;
                    break;
                    }
                }
            else if ( !maxArgOK )
                {
                maxArgOK = CFContextOperationUtils::ParseComparisonTypeValue(
                        *child,
                        cmpMaxType,
                        maxValue );
                if ( !maxArgOK )
                    {
                    argsOK = EFalse;
                    break;
                    }
                }
            else
                {
                argsOK = EFalse; // Unsupported node encountered.
                break;
                }
            }
        else if ( child->NodeType() != CMDXMLNode::ECommentNode )
            {
            argsOK = EFalse; // Unsupported node encountered.
            break;
            }
        child = child->NextSibling();
        }

    TBool parsed( EFalse );
    if ( argsOK && contextRefOK && minArgOK && maxArgOK )
        {
        if ( cmpMinType == cmpMaxType && !contextValue.Length() )
            {
            parsed = ETrue;
            }
        }

    CCFInRange* self = NULL;
    if ( parsed )
        {
        self = NewL( aServices,
                aParent,
                contextType,
                contextSource,
                cmpMinType,
                minValue,
                maxValue );
        if ( contextLevelDelay )
            {
            self->iContextLevelDelay = contextLevelDelay;
            }
        }
    else
        {
        INFO( "CCFInRange::ParseL - Unsupported arguments" );
        }

    CREATE_DOM_INFO( self, aNode );

    return self;
    }


// Destructor
CCFInRange::~CCFInRange()
    {
    FUNC_LOG;

    delete iMinVal;
    delete iMaxVal;
    }

// -----------------------------------------------------------------------------
// CCFInRange::IsTrueL
// -----------------------------------------------------------------------------
//
TBool CCFInRange::IsTrueL( const CCFContextObject& aContextObject )
    {
    FUNC_LOG;

    TBool value( EFalse );
    switch ( iType )
        {
        case CCFContextOperation::EIntCmp:
            {
            TInt min = CFContextOperationUtils::StringToIntL( *iMinVal );
            TInt max = CFContextOperationUtils::StringToIntL( *iMaxVal );
            TInt cmp = CFContextOperationUtils::StringToIntL(
                    aContextObject.Value() );
            value = ( cmp >= min ) && ( cmp <= max );
            break;
            }
        case CCFContextOperation::ETimeCmp:
            {
            TTime min = CFContextOperationUtils::StringToTimeL( *iMinVal );
            TTime max = CFContextOperationUtils::StringToTimeL( *iMaxVal );
            TTime cmp = CFContextOperationUtils::StringToTimeL(
                    aContextObject.Value() );
            value = ( cmp >= min ) && ( cmp <= max );
            break;
            }
        case CCFContextOperation::EPositionCmp:
            {
            // local variable declaration & initialization
            TReal cmpLatitude( KErrNone ), cmpLongitude( KErrNone ),
                  objLatitude( KErrNone ), objLongitude( KErrNone );
            TReal calculatedDistance( KErrNone );
            TReal locationRadius( KErrNone );
            // get values contained by scripts
            CFContextOperationUtils::PositionToRealsL(
                *iMinVal,
                cmpLatitude,
                cmpLongitude );
            CFContextOperationUtils::PositionToRealsL(
                aContextObject.Value(),
                objLatitude,
                objLongitude );
            locationRadius = CFContextOperationUtils::StringToRealL(
                    *iMaxVal );
            calculatedDistance = EllipsoidDistanceBetweenTwoPositions(
                cmpLatitude,
                cmpLongitude,
                objLatitude,
                objLongitude );
            // error occurred; leave
            if ( calculatedDistance == KErrGeneral )
                {
                User::Leave( static_cast< TInt > ( calculatedDistance ) );
                }
            value = ( calculatedDistance <= locationRadius );
            break;
            }
        case CCFContextOperation::EFloatCmp:
            {
            TReal min = CFContextOperationUtils::StringToRealL( *iMinVal );
            TReal max = CFContextOperationUtils::StringToRealL( *iMaxVal );
            TReal cmp = CFContextOperationUtils::StringToRealL(
                    aContextObject.Value() );
            value = ( cmp >= min ) && ( cmp <= max );
            break;
            }
        case CCFContextOperation::EStringCmp:
        default:
            {
            value = ( aContextObject.Value() >= ( *iMinVal ) ) &&
                ( aContextObject.Value() <= ( *iMaxVal ) );
            break;
            }

        }

    return value;
    }

// -----------------------------------------------------------------------------
// CCFInRange::EllipsoidDistanceBetweenTwoPoints
// Method that takes two GPS positions as (latitude,longitude)-pairs
// (units==DEGREES) and calculates their ellipsoid distance.
// -----------------------------------------------------------------------------
//
TReal CCFInRange::EllipsoidDistanceBetweenTwoPositions( TReal aPos1Latitude,
    TReal aPos1Longitude,
    TReal aPos2Latitude,
    TReal aPos2Longitude )
    {
    FUNC_LOG;

    TInt err( KErrNone );
    TReal distance = 0.0;
    TReal faz, baz;
    TReal r = 1.0 - KFlattening;
    TReal tu1, tu2, cu1, su1, cu2, x, sx, cx, sy, cy, y, sa, c2a, cz, e, c, d;
    TReal cosy1, cosy2;

    // Make sure the coordinates differ.
    if ( ( aPos1Longitude == aPos2Longitude ) &&
         ( aPos1Latitude  == aPos2Latitude  ) )
        {
        return distance;
        }

    // Convert longitude and latitudes to radians.
    aPos1Longitude *= KDegToRad;
    aPos1Latitude  *= KDegToRad;
    aPos2Longitude *= KDegToRad;
    aPos2Latitude  *= KDegToRad;

    err = Math::Cos( cosy1, aPos1Latitude );
    if ( err != KErrNone )
        {
        return KErrGeneral;
        }
    err = Math::Cos( cosy2, aPos2Latitude );
    if ( err != KErrNone )
        {
        return KErrGeneral;
        }

    // Assure positive values since this is used as division denominator.
    if ( cosy1 == 0.0 )
        {
        cosy1 = 0.0000000001;
        }
    // Assure positive values since this is used as division denominator.
    if ( cosy2 == 0.0 )
        {
        cosy2 = 0.0000000001;
        }

    TReal temp1, temp2, temp3, temp4;
    err = Math::Sin( temp1, aPos1Latitude );
    if ( err != KErrNone )
        {
        return KErrGeneral;
        }
    err = Math::Sin( temp2, aPos2Latitude );
    if ( err != KErrNone )
        {
        return KErrGeneral;
        }

    tu1 = r * temp1 / cosy1;
    tu2 = r * temp2 / cosy2;
    err = Math::Sqrt( temp3, ( tu2 * tu2 + 1.0 ) );
    if ( err != KErrNone )
        {
        return KErrGeneral;
        }
    cu1 = 1.0 / temp3;
    su1 = cu1 * tu1;
    err = Math::Sqrt( temp4, ( tu2 * tu2 + 1.0 ) );
    if ( err != KErrNone )
        {
        return KErrGeneral;
        }
    cu2 = 1.0 / temp4;
    x = aPos2Longitude - aPos1Longitude;

    distance = cu1 * cu2;
    baz = distance * tu2;
    faz = baz * tu1;

    do  {
        err = Math::Sin( sx, x );
        if ( err != KErrNone )
            {
            return KErrGeneral;
            }
        err = Math::Cos( cx, x );
        if ( err != KErrNone )
            {
            return KErrGeneral;
            }
        tu1 = cu2 * sx;
        tu2 = baz - su1 * cu2 * cx;
        err = Math::Sqrt( sy, (tu1 * tu1 + tu2 * tu2) );
        if ( err != KErrNone )
            {
            return KErrGeneral;
            }
        cy = distance * cx + faz;
        err = Math::ATan( y, sy, cy );
        if ( err != KErrNone )
            {
            return KErrGeneral;
            }
        sa = distance * sx / sy;
        c2a = -sa * sa + 1.0;
        cz = faz + faz;
        if( c2a > 0.0 )
            {
            cz = -cz / c2a + cy;
            }
        e = cz * cz * 2. - 1.0;
        c = ( ( -3.0 * c2a + 4.0 ) * KFlattening + 4.0 )
            * c2a * KFlattening / 16.0;
        d = x;
        x = ( ( e * cy * c + cz ) * sy * c + y ) * sa;
        x = ( 1.0 - c ) * x * KFlattening + aPos2Longitude - aPos1Longitude;

        } while( ( Abs( d - x ) ) > KEarthPS );

    err = Math::Sqrt( x, ( ( 1.0 / r / r - 1.0 ) * c2a + 1.0 ) );
    if ( err != KErrNone )
        {
        return KErrGeneral;
        }
    x += 1.0;
    x = ( x - 2.0 ) / x;
    c = 1.0 - x;
    c = ( x * x / 4.0 + 1.0 ) / c;
    d = ( 0.375 * x * x - 1.0 ) * x;
    x = e * cy;
    distance = 1.0 - e - e;
    distance = ( ( ( ( sy * sy * 4.0 - 3.0 ) * distance * cz * d / 6.0 - x )
            * d / 4.0 + cz ) * sy * d + y ) * c * KEarthRadius * r;

    INFO_1( "Distance in meters: %d", distance * 1000.0 );

    return ( distance * 1000.0 ); // Return distance in meters.
    }
