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
* Description:  UndoTask related helpers.
*
*/

#ifndef XIMPUNDOTASK_H
#define XIMPUNDOTASK_H

#include <e32base.h>


/**
 * Abstract UndoTask object interface.
 * UndoTask object encapsulates data and logic
 * needed to perform the undo operation.
 *
 * Concrete UndoTasks perform the desired undo operation
 * on its desctructor. So the concrete UndoTask object
 * can be pushed on the cleanupstack to have the undo
 * performed automaticly in case of leave.
 *
 * If undo operation should not be performed on destruction,
 * it should be canceled with CancelUndo(). However,
 * the undo object needs still to be deleted or
 * PopAndDestroy():ed.
 *
 * @since S60 v3.2
 */
class MXIMPUndoTask
    {

public:

    /**
     * Cancels the undo operation.
     *
     * When called, undo isn't performed on
     * destcruction.
     */
    virtual void CancelUndo() = 0;


    /**
     * Public destructor.
     * Object instancies can be deleted via this interface.
     *
     * Destruction performs the desired undo operation,
     * unless it has been previously canceled.
     */
    virtual ~MXIMPUndoTask() {}
    };





/**
 * Templated UndoTask to perform entry removal from
 * pointer array.
 *
 * @since S60 v3.2
 */
template< class T >
class CXIMPUndoPtrArrayEntryAdd : public CBase,
                                  public MXIMPUndoTask
    {
public:

    /**
     * Constructor to allocate new undo
     * task object.
     */
    inline static CXIMPUndoPtrArrayEntryAdd* NewLC();


    /**
     * Destructor.
     *
     * Destruction performs the undo operation,
     * unless it has been previously canceled.
     */
    inline ~CXIMPUndoPtrArrayEntryAdd();


private:
    inline CXIMPUndoPtrArrayEntryAdd();


public:

    /**
     * Initializes the undo operation to remove and destroy
     * given object from given object array.
     *
     * @param aObj Templated object to remove.
     * @param aObjArray Templated object array from which
     *        to remove.
     */
    inline void InitToRemoveAndDestroyOnUndo( T& aObj,
                                              RPointerArray< T >& aObjArray );

    /**
     * Cancels the initialized undo operation.
     */
    inline void CancelUndo();


private: // data

    /**
     * Pointer to RPointerArray from where to remove the entry.
     * Not own.
     */
    RPointerArray< T >* iObjArray;


    /**
     * Pointer to entry which to remove from the array.
     *
     * Object is removed from the array and deleted
     * when this undo object is destroyed.
     */
    T*  iObj;
    };






/**
 * Templated UndoTask to perform entry replace on
 * pointer array.
 *
 * @since S60 v4.0
 */
template< class T >
class CXIMPUndoPtrArrayEntryReplace : public CBase,
                                      public MXIMPUndoTask
    {
public:

    /**
     * Constructor to allocate new undo
     * task object.
     */
    inline static CXIMPUndoPtrArrayEntryReplace* NewLC();


    /**
     * Destructor.
     *
     * Destruction performs the undo operation,
     * unless it has been previously canceled.
     */
    inline ~CXIMPUndoPtrArrayEntryReplace();


private:
    inline CXIMPUndoPtrArrayEntryReplace();


public:

    /**
     * Initializes the undo operation to remove and destroy
     * given object from given object array.
     *
     * @param aTarget Templated object which to replace.
     * @param aObjArray Templated object array on which to replace.
     * @param aSource Templated object which to insert.
     */
    inline void InitToReplaceOnUndo( T& aTarget,
                                     RPointerArray< T >& aObjArray,
                                     T& aSource );

    /**
     * Cancels the initialized undo operation.
     */
    inline void CancelUndo();


private: // data

    /**
     * Pointer to RPointerArray from where to replace the entry.
     * Not own.
     */
    RPointerArray< T >* iObjArray;


    /**
     * Pointer to entry which to replace on the the array.
     * Object is deleted and replaced with source object
     * when this undo object is destroyed.
     */
    T*  iTarget;


    /**
     * Pointer to entry which to insert on the the array.
     *
     * Object is deleted when undo is canceled.
     */
    T*  iSource;

    };


#include "ximpundotask.inl"

#endif // XIMPUNDOTASK_H


