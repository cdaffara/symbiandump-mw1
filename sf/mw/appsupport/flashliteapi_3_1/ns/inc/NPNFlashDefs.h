/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Flash Netscape Plugin Parameter Definitions
*
*/


#ifndef NPNFLASH_DEFS_H
#define NPNFLASH_DEFS_H

//These definitions are used for sending paramter to flash  plug-in through 
//web pages.

//Atribute to be set during newp call only
_LIT( KFlashInteractionMode , "interactionmode" );
_LIT(KFlashSwfSrc, "src");
_LIT(KFlashSwfMovie, "movie");
_LIT(KFlashBase, "base");
_LIT(KLoop, "loop");
_LIT(KQuality, "quality");
_LIT(KBackgroundcolor, "backgroundcolor");
_LIT(KBgcolor, "bgcolor");
_LIT(KWMode,"wmode");
_LIT(KWAlpha,"alpha");
_LIT(KFlashVars,"flashvars");
_LIT(KFlashName,"name");
_LIT(KFlashPlay,"play");
_LIT(KFlashAllowFullScreen,"allowFullScreen");
_LIT(KFlashAllowMobiFullScreen,"allowMobiFullScreen");
_LIT(KScale, "scale");
_LIT(KAlign,"align");
_LIT(KSAlign,"salign");

_LIT(KShowAll,"showall");
_LIT(KNoBorder,"noborder");
_LIT(KExactFit,"exactfit");
_LIT(KNoScale,"noscale");

_LIT(KLeft,"left");
_LIT(KRight,"right");
_LIT(KTop,"top");
_LIT(KBottom,"bottom");

_LIT(KTopLeft,"TL");
_LIT(KLeftTop,"LT");
_LIT(KTopRight,"TR");
_LIT(KRightTop,"RT");
_LIT(KBottomLeft,"BL");
_LIT(KLeftBottom,"LB");
_LIT(KBottomRight,"BR");
_LIT(KRightBottom,"RB");

//interaction mode Values
_LIT( KValueFlashAnimation, "anim" );    
_LIT( KValueScreensaverMode, "screensaver" );

//values for quality
_LIT(KHigh,"high");
_LIT(KMedium,"medium");
_LIT(KLow,"low");

//values for Loop
_LIT(KOn,"on");
_LIT(KOff,"off");
_LIT(KTrue,"true");
_LIT(KFalse,"false");
_LIT(KYes,"yes");	
_LIT(KNo,"no");	

//value for background transparency
_LIT(KWModeTransparent,"transparent");
_LIT(KWModeOpaque,"opaque");	
_LIT(KWModeWindow,"window");

_LIT(KSCriptAccess, "allowscriptaccess");

_LIT(KSCriptAccessAlways, "always");
_LIT(KSCriptAccessnever, "never");
_LIT(KSCriptAccessSameDomain, "SameDomain");

/**
*  Value sent by plug-in for player status.
*
*  @lib
*  @since Series 60 3rd Edition, Feature Pack 1
*/ 
/**
*  Value sent by plug-in for player status.
*
*  @lib
*  @since Series 60 3rd Edition, Feature Pack 1
*/ 
enum TFlashPlayerState //
            {
            EContentError = -2,
            EInstanceError = -1,
            EEngineNotInitialized,
            EEngineInitialized,
            EContentClosed = 2,
            EContentOpening,
            EContentOpen,
            EContentPreparing,
            EContentReady, // player's stopped state
            EContentPlaying,
            EPlayerPaused,
            EPlayerStopped,
            EPlayComplete
            };

#endif //NPNFLASH_DEFS_H
