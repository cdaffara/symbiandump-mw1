/*
* ==============================================================================
*  Name        : ProfilesEngineExports.inc
*  Part of     : Profiles Engine
*  Interface   : ?Interface_category, ?Interface_name
*  Description : ?Description
*  Version     : ?Version
*
*  Copyright (c) 2002 Nokia Corporation.
*  This material, including documentation and any related
*  computer programs, is protected by copyright controlled by
*  Nokia Corporation. All rights are reserved. Copying,
*  including reproducing, storing, adapting or translating, any
*  or all of this material requires the prior written consent of
*  Nokia Corporation. This material also contains confidential
*  information which may not be disclosed to others without the
*  prior written consent of Nokia Corporation.
* ==============================================================================
*/
// To get the APP_LAYER_SYSTEMINCLUDE-definition
#include <platform_paths.hrh>


#ifndef PROFILESENGINEEXPORTS_INC
#define PROFILESENGINEEXPORTS_INC

    ../loc/Profile.loc    MW_LAYER_LOC_EXPORT_PATH(Profile.loc)

    // headers
    // for SDK
    //..\EngInc\Profile.hrh                       APP_LAYER_PUBLIC_EXPORT_PATH(Profile.hrh)
    // Domain API
/*
    ..\EngInc\CProfileChangeNotifyHandler.h     APP_LAYER_DOMAIN_EXPORT_PATH(CProfileChangeNotifyHandler.h)
    ..\EngInc\MProfile.h                        APP_LAYER_DOMAIN_EXPORT_PATH(MProfile.h)
    ..\EngInc\MProfileChangeObserver.h          APP_LAYER_DOMAIN_EXPORT_PATH(MProfileChangeObserver.h)
    ..\EngInc\MProfileEngine.h                  APP_LAYER_DOMAIN_EXPORT_PATH(MProfileEngine.h)
    ..\EngInc\MProfileExtraTones.h              APP_LAYER_DOMAIN_EXPORT_PATH(MProfileExtraTones.h)
    ..\EngInc\MProfileName.h                    APP_LAYER_DOMAIN_EXPORT_PATH(MProfileName.h)
    ..\EngInc\MProfileTones.h                   APP_LAYER_DOMAIN_EXPORT_PATH(MProfileTones.h)
    ..\EngInc\MProfilesNamesArray.h             APP_LAYER_DOMAIN_EXPORT_PATH(MProfilesNamesArray.h)
    ..\EngInc\TProfileToneSettings.h            APP_LAYER_DOMAIN_EXPORT_PATH(TProfileToneSettings.h)
    ..\EngInc\ProfileInternal.hrh               APP_LAYER_DOMAIN_EXPORT_PATH(ProfileInternal.hrh)
    ..\EngInc\MProfileExtraSettings.h           APP_LAYER_DOMAIN_EXPORT_PATH(MProfileExtraSettings.h)
    ..\EngInc\MProfilePttSettings.h             APP_LAYER_DOMAIN_EXPORT_PATH(MProfilePttSettings.h)
    ..\EngInc\ProfileEngineDomainConstants.h  APP_LAYER_DOMAIN_EXPORT_PATH(ProfileEngineDomainConstants.h)

    ..\EngInc\ProfileEngineSDKCRKeys.h          APP_LAYER_SDK_EXPORT_PATH(ProfileEngineSDKCRKeys.h)
    ..\EngInc\ProfileEngineDomainCRKeys.h       APP_LAYER_DOMAIN_EXPORT_PATH(ProfileEngineDomainCRKeys.h)
*/
    // Internal API headers: NOW IN DOMAIN API
/*
    ..\EngInc\MProfileEngineExtended.h          |..\..\..\Inc\MProfileEngineExtended.h
    ..\EngInc\MProfileExtended.h                |..\..\..\Inc\MProfileExtended.h
    ..\EngInc\MProfilePresence.h                |..\..\..\Inc\MProfilePresence.h
    ..\EngInc\MProfileSetPresence.h             |..\..\..\Inc\MProfileSetPresence.h
    ..\EngInc\ProfileEng.hrh                    |..\..\..\Inc\ProfileEng.hrh
    ..\EngInc\ProfileEngineConstants.h          |..\..\..\Inc\ProfileEngineConstants.h
    ..\EngInc\MProfileSetExtraTones.h           |..\..\..\Inc\MProfileSetExtraTones.h
    ..\EngInc\MProfileSetName.h                 |..\..\..\Inc\MProfileSetName.h
    ..\EngInc\ProfileEng.rh                     |..\..\..\Inc\ProfileEng.rh
    ..\EngInc\MProfilesLocalFeatures.h          |..\..\..\Inc\MProfilesLocalFeatures.h
    ..\EngInc\MProfileUtilitySingleton.h        |..\..\..\Inc\MProfileUtilitySingleton.h
    ..\EngInc\ProfileEngineInternalCRKeys.h     |..\..\..\Inc\ProfileEngineInternalCRKeys.h
    ..\EngInc\ProfileEngineInternalPSKeys.h     |..\..\..\Inc\ProfileEngineInternalPSKeys.h
*/
    // no_sound.wav
    // wins
    ../Data/No_Sound.wav    /epoc32/RELEASE/WINS/udeb/Z/System/Sounds/Digital/No_Sound.wav
    ../Data/No_Sound.wav    /epoc32/RELEASE/WINS/urel/Z/System/Sounds/Digital/No_Sound.wav
    ../Data/No_Sound.wav    /epoc32/RELEASE/WINSCW/udeb/Z/System/Sounds/Digital/No_Sound.wav
    ../Data/No_Sound.wav    /epoc32/RELEASE/WINSCW/urel/Z/System/Sounds/Digital/No_Sound.wav
    // thumb
    ../Data/No_Sound.wav    /epoc32/RELEASE/THUMB/udeb/Z/System/Sounds/Digital/No_Sound.wav
    ../Data/No_Sound.wav    /epoc32/RELEASE/THUMB/urel/Z/System/Sounds/Digital/No_Sound.wav
    // armv5
    ../Data/No_Sound.wav    /epoc32/release/armv5/udeb/Z/System/Sounds/Digital/No_Sound.wav
    ../Data/No_Sound.wav    /epoc32/release/armv5/urel/Z/System/Sounds/Digital/No_Sound.wav

    // these are needed for Profiles to work correctly in 3.0 emulator (resource directory
    // is hardcoded here because for some reason #defines seem not to work in here):
    // winscw
    ../Data/No_Sound.wav    /epoc32/RELEASE/WINSCW/udeb/Z/resource/No_Sound.wav
    ../Data/No_Sound.wav    /epoc32/RELEASE/WINSCW/urel/Z/resource/No_Sound.wav
    // thumb
    ../Data/No_Sound.wav    /epoc32/RELEASE/THUMB/udeb/Z/resource/No_Sound.wav
    ../Data/No_Sound.wav    /epoc32/RELEASE/THUMB/urel/Z/resource/No_Sound.wav
    // armv5
    ../Data/No_Sound.wav    /epoc32/release/armv5/udeb/Z/resource/No_Sound.wav
    ../Data/No_Sound.wav    /epoc32/release/armv5/urel/Z/resource/No_Sound.wav

    backup_registration.xml z:/private/100058F8/backup_registration.xml

#endif      // PROFILESENGINEEXPORTS_INC

// End of File
