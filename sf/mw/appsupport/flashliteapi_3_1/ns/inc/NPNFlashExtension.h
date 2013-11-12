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
* Description:  Flash Netscape Plugin command Definitions
*
*/

#ifndef NPNFLASH_EXTENSIONS_H
#define NPNFLASH_EXTENSIONS_H

#include "NPNExtensions.h" //screensaver command set defined by screensaver
#include "NPNFlashDefs.h" //parameter definitions

/* 
* Enumerations for setting the information to/from flash plugin
*/

enum
    {
    /* Defines Start/end animation event */
    NPNAnimationEvent=101,
    
    /* Defines variable to get config structure from caller */
    NPNGetconfigParam=110,
    
    /* Defines variable to set volume */
    NPNVolumeEvent,
    
    /* Defines variable to set zoom */
    NPNZoomEvent,
    
   
    };
    
/* 
* Enumerations for getting the information from format specific plugins 
*   
*/    
    
enum
    {

    /* Gets the last error encountered by the plugin. 
     * Int - System wide error code
     */
    NPPGetLastError=101,
    
    /* Checks whether the error is fatal 
     * Bool True if the error is fatal, false if not
     */
    NPPIsErrorFatal=102,
    
    /* Gets the status of plugin. 
     * Int - System wide error code
     */
    NPPGetPluginStatus=210,
    
    /* Defines variable to get current frame number */
    NPPGetCurFrame,
    
    /* Defines variable to get total frame number */
    NPPGetTotalFrames,
  
  	/* Defines variable to get total duration */
    NPPGetTotalDuration,
    };


/** 
 Enumeration for zoom operations.
*/
enum
{
	NPZoomIn,	///< Zoom In option selected.
	NPZoomOut,	///< Zoom Out option selected.
	NPShowAll	///< Show All option selected.
};
 
/**
*  Value sent by caller during newp instance creation.
*
*  @lib
*  @since Series 60 3rd Edition, Feature Pack 2
*/   
enum  
    {
    /* animation plug-in Mode , use flash as plug-in*/
    NPAnimPluginMode = 110
      
    };    
   
/**
*  Value sent by caller for Set_value.
*
*  @lib
*  @since Series 60 3rd Edition, Feature Pack 2
*/ 
enum NPFlashAnimationEvent
    {
    /* Sent to the plugin to pause */
    NPPPauseAnimation = 150,
    
    /* Sent to the plugin to pause */
    NPPResumeAnimation =151 ,
    
    /* Sent to the plugin to stop */
    NPPStopAnimation =152,
    
    /* Sent to the plugin to rewind */
    NPPRewindAnimation =153,
    
    /* Sent to the plugin to mute audio */
    NPPMuteAnimation =154,
    
       /* Sent to the plugin to Unmute audio */
    NPPUnmuteAnimation=155,

    /* Sent to the plugin to zoomin */
    NPPZoomInAnimation=156,

   /* Sent to the plugin to zoomout*/
    NPPZoomOutAnimation=157
        
    } ;
 
/**
*  Value sent by plug-in for player status.
*
*  @lib
*  @since Series 60 3rd Edition, Feature Pack 1
*/ 
enum NPFlashPlayerState //
            {
            NPContentError = -2,
            NPInstanceError = -1,
            NPEngineNotInitialized,
            NPEngineInitialized,
            NPContentClosed = 2,
            NPContentOpening,
            NPContentOpen,
            NPContentPreparing,
            NPContentReady, // player's stopped state
            NPContentPlaying,
            NPPlayerPaused,
            NPPlayerStopped,
            NPPlayComplete
            };

       
/* 
 * The NPInteractionMode needs to be sent to the Flash Plugin
 * in the newp call, The interaction mode attribute name is passed 
 * in argn and the attribute value is passed in argv parameter.
 * The mode parameter of newp is not used in Screensaver mode.
 */
 
// Flash animation plug-in structure definitions
/**
*  Plug-in Mode set by caller
*
*  @lib
*  @since Series 60 3rd Edition, Feature Pack 2
*/

enum NPPluginMode
  {
  	/** 
		*  simple animation, this is default 
    */
    
  	NPAnimation = 4,
  	
  	/** 
		*  flash running in idle 
    */
    
  	NPIdle ,
  	
  	/** 
		*  flash running as video ring tone 
    */
    
  	NPRingTone,
  	
  	/** 
		*  flash running as call image 
    */
    
  	NPCallImage,
  	
  	/** 
		*  flash running as animated background in idle 
    */
    
  	NPBackground,
  	
  	/** 
		*  flash running as wall paper 
    */
    
  	NPWallpaper,
  	
  	/** 
		*  flash running as notification  for calendar apointments
    */
    
  	NPNotification,
  	
  	/** 
		*  flash running as startup screen
    */  	
  	NPStartupScreen,
  	
  	/** 
		*  flash running as shutdown screen
    */
  	NPShutdownScreen
  	
  };
  
/**
*  Preference flag set by caller NotifyL.
*
*  @lib
*  @since Series 60 3rd Edition, Feature Pack 2
*/
  
enum NPPluginPref
{
	/**
	*  Prefer controlling animation
	*  plug-in should not start playing animation,
	*  it should wait for start,stop, pause commands
	*/
	NPPrefControlAnim = 0x00001,

	/**
		*  Preference flag indicates caller expects error notification.
		*
	*/
	
	NPPrefErrorNotification = 0x00002,
	
	/**
		*  Preference flag indicates caller expects content status notification.
		*  ready,playing,paused, stopped etc.
	*/
	
	NPPrefContentStatusNotification = 0x00004,
	
	
	/**
		*  Preference flag indicates caller expects frame ready notification.
		*
	*/
	
	NPPrefFrameReadyNotification = 0x00008,
	
		
};

/**
*  notification type flag provided by flash plug-in in NotifyL.
*
*  @lib
*  @since Series 60 3rd Edition, Feature Pack 1
*/

enum NPFlashNotifyType
{
	/**
	*  Notification related to flash content error
	*
	*/
 NPNotifyContentError,
 /**
	*  Notification related to flash plug-in instance error
	*
	*/
 NPNotifyInstanceError,	

 /**
	*  Notification related to flash plug-in instance
	*
	*/	
 NPNotifyInstanceStatus
	
};

/**
*  Notification data (aParam)provided by flash plug-in in NotifyL.
*  Not mandatory to be returned for every NotifyType
*  @lib
*  @since Series 60 3rd Edition, Feature Pack 2
*/

typedef struct _NPNotifyParam
    {
    /* status code returned by the plugin */
    TInt  iStatusCode;
    
    /* additional parameter */
    TAny* iParam;
    
    } NPNotifyParam;
   
    
/**
*  Notification interface implemented by caller.
*  These methods are called by flash plug-in
*
*  @lib
*  @since Series 60 3rd Edition, Feature Pack 1
*/

class MFlashNotifier 
{
	public:
	 /**
        * Notifies the caller an event.Do not hold this method for longer
        * @since Series 60 3rd Edition, Feature Pack 1
        * @param aNotifyType The event type that is passed to the plug-in
        * @param aParam The parameter associated with the event
        * @return Not used
        */
    virtual TInt NotifyL (TInt aNotifyType, TAny *aParam)=0;

};

/**
*  Flash Configuration structure  passed by caller through get_value.
*  These structure has to passed if interaction mode is anim
*  Flash will get this structure through get_Value NPNGetconfigParam
*  @lib
*  @since Series 60 3rd Edition, Feature Pack 1
*/

typedef struct _NPFlashConfig{

    /**
    	Size of the structure
    **/
    
    TUint16 iSize;

    /**
    	version of the _NPFlashAdapter structure
    	
    	1 for S60 3.1
    **/
    
    TUint16 iVersion;

    /**
    	type of the mode idle, ring tone,call image, wallpaper
    	value defined in NPPluginMode
    **/
    TUint16 iModeType; 
    
    /** 
		* Etrue if MMI is supported. 
    */
    TBool iIsMMI;
	
   /** 
   * Name of the MMI ECOM plug-ins
   */
   const TDesC8* iExtensions;
   
    /** 
		* Etrue if noaudio , mute is valid. 
    */
    
    
    TBool iNoAudio;
    /** 
		* Etrue if iAudioPriority, iAudioPreference is valid. 
    */
    TBool iIsPriorityValid;
    
   /** 
    *  Priority for playing audio
   */
    TInt iAudioPriority; 
    
    /** 
     *  Preference for playing audio
     */
    TUint iAudioPreference;
    
    /**
    
    * Preference flag for Flash
    
    */
    
    TInt iPrefFlag;
    
    /**
    
    * Notification interface provided by caller
    
    **/
    
    MFlashNotifier* iNotifyCaller;
    
  
} NPFlashConfig;


#endif // NPNFLASH_EXTENSIONS_H
// End Of file


