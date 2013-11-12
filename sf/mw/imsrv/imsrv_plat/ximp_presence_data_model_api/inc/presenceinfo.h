/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for presence info object.
*
*/

#ifndef MXIMPFWPRESENCEINFO_H
#define MXIMPFWPRESENCEINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>



/**
 * Name constants and similar definitions
 * for presence information components and
 * presence information fields.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
namespace NPresenceInfo
    {

    /**
     * Common type constants for MServicePresenceInfo
     * objects.
     *
     * @ingroup presencedatamodelapi
     * @since S60 v3.2
     */
    namespace NServiceType
        {
        _LIT8( KInstantMessaging,   "instant-messaging");
        _LIT8( KPushToTalk,         "push-to-talk");
        _LIT8( KVoip,               "voip");
        _LIT8( KCall,               "call");
        _LIT8( KSms,                "sms");
        _LIT8( KMms,                "mms");
        _LIT8( KEMail,              "e-mail");
        _LIT8( KGame,               "game");
        }


    /**
     * Common name constants MPresenceInfoField
     * objects.
     *
     * @ingroup presencedatamodelapi
     * @since S60 v3.2
     */
    namespace NFieldType
        {

        /**
         * Field type for "status message" field.
         *
         * Status message field contains user written
         * free text, describing for example his/hers current
         * presence status. Status message is in unicode format.
         *
         * Field storage format: MPresenceInfoFieldValueText
         */
        _LIT8( KStatusMessage,      "status-msg");



        /**
         * Field type for "avatar" field.
         *
         * Avatar field contains user selected image. Image may be
         * an avatar describing the user or it may be image showing
         * user current presence status.
         *
         * Image is in JPEG or similar format, for what there exists
         * image codes in the platform. Field contains image binary
         * content and MIME string describing the content format.
         *
         * Field storage format: MPresenceInfoFieldValueBinary
         */
        _LIT8( KAvatar,             "avatar");



        /**
         * Field type for "availability-text" field.
         *
         * Availability text field describes presentity's communication
         * availability as text. This is useful in case of branded
         * presence availability, since any text value is possible.
         *
         * Field storage format: MPresenceInfoFieldValueText
         */
        _LIT8( KAvailability,       "availability");
        


        /**
         * Field type for "availability" field enumeration value.
         *
         * Availability field describes presentitys communication
         * availability. Possible availability values are
         * listed in TAvailabilityValues enumeration.
         *
         * Field storage format: MPresenceInfoFieldValueEnum
         */
        _LIT8( KAvailabilityEnum,       "availability-enum");
        
        
        
        /**
         * This means that all field types are accepted.
         * Field types are status message, avatar and
         * availability.
         *
         */
        _LIT8( KAcceptAll,      "*");
        }



    /**
     * Common enumerated availability values for "availability"
     * presence information field.
     *
     * @ingroup presencedatamodelapi
     * @since S60 v3.2
     */
    enum TAvailabilityValues
        {

        /**
		* Presentity is not available for communication.
		*/
        ENotAvailable = 0,


        /**
		* Presentity is currently busy.
		* Presentity may or may not be available
		* for for communication.
		*/
        EBusy = 1,


        /**
		* Presentity is available for communication.
		*/
        EAvailable = 2,

		/**
		* Presentity appears Offline for communication.
		*/
        EHidden = 3,

		/**
		* Presentity is away.
		*/
        EAway = 4,

		/**
		* Presentity is extended away
		*/
        EExtAway = 5,
        
        /**
        * Presentity is on the Phone Call
        */
        EOnPhone = 6,
        
        /**
        * Presentity is in Do Not disturb status
        */
        EDoNotDisturb = 7,
                
        /**
        * Presentity of received contact request
        */        
        EPendingRequest = 8,
        
        /**
        * Presentity of received contact request when offline
        */
        EPendingRequestOffline = 9,
        
        /**
        * Presentity is Empty / Not matched
        */
        EEmptyStatus = 10,       
 
        /**
        * Presentity is offline
        */
        EOffline = 11,
   
        /**
        * Presentity is Local Pending
        */
        ELocalPending = 12,
        
        /**
        * Presentity is offline
        */
        ERemotePending = 13
        

        };


    const TInt KValueLengthUnlimited = -1;
    }



class MPersonPresenceInfo;
class MServicePresenceInfo;
class MDevicePresenceInfo;


/**
 * Interface for presence info object.
 *
 *
 * @todo Interface might be extended with new methods.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresenceInfo : public MXIMPBase
    {
public:

    /** Interface ID for the MPresenceInfo. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_INFO };



public:

    /**
     * Public destructor. MPresenceInfo object
     * can be deleted through this interface.
     */
    virtual ~MPresenceInfo() {}


public: // Access to person presence


    /**
     * Returns read-only person presence information object.
     *
     * Returns the read-only person presence information
     * object or NULL if person presence information
     * not available.
     *
     * @return Person presence information object or NULL.
     */
    virtual const MPersonPresenceInfo* PersonPresence() const = 0;
    
    /**
     * Returns person presence information object.
     *
     * Returns the person presence information
     * object or NULL if person presence information
     * not available.
     *
     * @return Person presence information object or NULL.
     */
    virtual MPersonPresenceInfo* PersonPresence() = 0;


    /**
     * Sets person presence information object.
     *
     * @param [in] aPersonPresence
     *        New person presence value.
     *        Ownership of the argument is transfered to
     *        callee, if method succeeds without a leave.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     */
    virtual void SetPersonPresenceL(
                MPersonPresenceInfo* aPersonPresence ) = 0;



public: // Access to service presence


    /**
     * Returns count of service presence information objects.
     *
     * @return Count of service presence information objects.
     */
    virtual TInt ServicePresenceCount() const = 0;


    /**
     * Returns a read-only service presence information object.
     *
     * @param [in] aIndex
     *        Zero-based index of the service presence
     *        information object to return.
     *
     * @return Reference to a service presence information
     *        object at aIndex.
     */
    virtual const MServicePresenceInfo& ServicePresenceAt(
                TInt aIndex ) const = 0;


    /**
     * Looks up a read-only service presence information object,
     * having matching service type.
     *
     * @param [in,out] aServicePresence
     *        On entry, contains the object from where to start
     *        the lookup or NULL if lookup should be started from
     *        the beginning of the set.
     *        If object given in entry is not found from set,
     *        lookup is started from the beginning of the set.
     *        On return, if an matching object is found, this is
     *        set to point to the found object, else NULL.
     *        Found object ownership is not returned to caller.
     *
     * @param [in] aMatch
     *        The service type match pattern.
     *        Match pattern may contain question mark and
     *        asterisk characters ("?", "*") resepectively to
     *        match single or multiple characters. Service type
     *        are handled as folded in pattern matching.
     */
    virtual void LookupServicePresenceByServiceType(
                MServicePresenceInfo*& aServicePresence,
                const TDesC8& aMatch ) const = 0;


    /**
     * Adds service presence information object.
     *
     * @param [in] aServicePresence
     *        New service presence value.
     *        Ownership of the argument is transfered to
     *        callee, if method succeeds without a leave.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     */
    virtual void AddServicePresenceL(
                MServicePresenceInfo* aServicePresence ) = 0;




public: // Access to device presence


    /**
     * Returns count of device presence information objects.
     *
     * @return Count of device presence information objects.
     */
    virtual TInt DevicePresenceCount() const = 0;


    /**
     * Returns a read-only device presence information object.
     *
     * @param [in] aIndex
     *        Zero-based index of the device presence
     *        information object to return.
     *
     * @return Reference to a device presence information
     *         object at aIndex.
     */
    virtual const MDevicePresenceInfo& DevicePresenceAt(
                TInt aIndex ) const = 0;


    /**
     * Adds device presence information object.
     *
     * @param [in] aDevicePresence
     *        New device presence value.
     *        Ownership of the argument is transfered to
     *        callee, if method succeeds without a leave.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     */
    virtual void AddDevicePresenceL(
                MDevicePresenceInfo* aDevicePresence ) = 0;

    };



#endif //MXIMPFWPRESENCEINFO_H



