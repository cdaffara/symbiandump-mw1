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
* Description:  Interface for Search key  info object.
*
*/

#ifndef MSEARCHKEYINFO_H
#define MSEARCHKEYINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <searchdatamodelifids.hrh>
#include <badesca.h> 
#include <searchelement.h>

 enum TKeyType
     {
     EKey, 
     ELabel
     };

/**  file supportedsearchkey.h
 *  Interface class for  seach key  info 
 *
 */
class MSearchKeyInfo : public MXIMPBase
    {
    
	    
	public:

	    /** Interface ID for the MSearchKeyInfo. */
	    enum { KInterfaceId = SEARCH_IF_ID_SEARCH_KEY_INFO };


	public:

	    /**
	     * Public destructor.
	     * Objects can be deleted through this interface.
	     */
	    virtual ~MSearchKeyInfo() {}


	public:
	    /**
         *  returns key type 
         *  cleint has to access Key() and Label() methods based on this return value.
         *  if return value if Ekey , then  Key()
         *  else f return value if ELabel ,then Label()
         * 
         *  @return the TKeyType , which is the key type.
         *  any one value in the range of TKeyType enums 
         * 
         */
         virtual  TKeyType  Type()const =0 ;
         
        /**
         *   Sets the key type, whether label or key 
         *
         *   @param aType :  
         *        aType to be set any value from the set of TKeyType enums
         *
         */
        virtual void SetType( TKeyType aType)  = 0 ;
	    
	 	/**
	     *  returns key value 
	     * 
	     *  @return the TSearchKey value , which is the key.
	     *  this value is '0' if not set properly. ( If label 
	     *  else any one value in the range of TSearchKey enums 
	     *  also see searchelement.h for TSearchKey enums value. 
	     */
	    virtual  TSearchKey Key()const =0 ;
	    
	    
      	/**
	     *   Sets the key value
	     *
	     *   @param aKey :  
	     *        akey to be set any value from the set of TSearchKey enums
	     *
	     */
	    virtual void SetKey( TSearchKey aKey)  = 0 ;
	    
	    /**
         *   returns the search label.
         * 
         *  @return TDesc16& value of the  label. 
         */
        virtual const TDesC16&  Label() const =0 ;
        
        
        /**
         *   Sets the label
         *
         *  @param aLabel   label to be set
         */
        virtual void  SetLabelL(const TDesC16& aLabel)  = 0 ;  
    };


#endif // MSEARCHKEYINFO_H



