/*************************************************************************
 *                                                                       *
 * Open Dynamics Engine, Copyright (C) 2001-2003 Russell L. Smith.       *
 * All rights reserved.  Email: russ@q12.org   Web: www.q12.org          *
 *                                                                       *
 * This library is free software; you can redistribute it and/or         *
 * modify it under the terms of EITHER:                                  *
 *   (1) The GNU Lesser General Public License as published by the Free  *
 *       Software Foundation; either version 2.1 of the License, or (at  *
 *       your option) any later version. The text of the GNU Lesser      *
 *       General Public License is included with this library in the     *
 *       file LICENSE.TXT.                                               *
 *   (2) The BSD-style license that is included with this library in     *
 *       the file LICENSE-BSD.TXT.                                       *
 *                                                                       *
 * This library is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files    *
 * LICENSE.TXT and LICENSE-BSD.TXT for more details.                     *
 *                                                                       *
 *************************************************************************/

#ifndef _ODE_COLLISION_H_
#define _ODE_COLLISION_H_

#include <ode/common.h>
#include <ode/collision_space.h>
#include <ode/contact.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup collide Collision Detection
 *
 * ODE has two main components: a dynamics simulation engine and a collision
 * detection engine. The collision engine is given information about the
 * shape of each body. At each time step it figures out which bodies touch
 * each other and passes the resulting contact point information to the user.
 * The user in turn creates contact joints between bodies.
 *
 * Using ODE's collision detection is optional - an alternative collision
 * detection system can be used as long as it can supply the right kinds of
 * contact information.
 */


/* ************************************************************************ */
/* general functions */

/**
 * @brief Destroy a geom, removing it from any space.
 *
 * Destroy a geom, removing it from any space it is in first. This one
 * function destroys a geom of any type, but to create a geom you must call
 * a creation function for that type.
 *
 * When a space is destroyed, if its cleanup mode is 1 (the default) then all
 * the geoms in that space are automatically destroyed as well.
 *
 * @param geom the geom to be destroyed.
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomDestroy (dGeomID geom);


/**
 * @brief Set the user-defined data pointer stored in the geom.
 *
 * @param geom the geom to hold the data
 * @param data the data pointer to be stored
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomSetData (dGeomID geom, void* data);


/**
 * @brief Get the user-defined data pointer stored in the geom.
 *
 * @param geom the geom containing the data
 * @ingroup collide
 */
ODE_API IMPORT_C void *dGeomGetData (dGeomID geom);


/**
 * @brief Set the body associated with a placeable geom.
 *
 * Setting a body on a geom automatically combines the position vector and
 * rotation matrix of the body and geom, so that setting the position or
 * orientation of one will set the value for both objects. Setting a body
 * ID of zero gives the geom its own position and rotation, independent
 * from any body. If the geom was previously connected to a body then its
 * new independent position/rotation is set to the current position/rotation
 * of the body.
 *
 * Calling these functions on a non-placeable geom results in a runtime
 * error in the debug build of ODE.
 *
 * @param geom the geom to connect
 * @param body the body to attach to the geom
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomSetBody (dGeomID geom, dBodyID body);


/**
 * @brief Get the body associated with a placeable geom.
 * @param geom the geom to query.
 * @sa dGeomSetBody
 * @ingroup collide
 */
ODE_API IMPORT_C dBodyID dGeomGetBody (dGeomID geom);


/**
 * @brief Set the position vector of a placeable geom.
 *
 * If the geom is attached to a body, the body's position will also be changed.
 * Calling this function on a non-placeable geom results in a runtime error in
 * the debug build of ODE.
 *
 * @param geom the geom to set.
 * @param x the new X coordinate.
 * @param y the new Y coordinate.
 * @param z the new Z coordinate.
 * @sa dBodySetPosition
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomSetPosition (dGeomID geom, dReal x, dReal y, dReal z);


/**
 * @brief Set the rotation matrix of a placeable geom.
 *
 * If the geom is attached to a body, the body's rotation will also be changed.
 * Calling this function on a non-placeable geom results in a runtime error in
 * the debug build of ODE.
 *
 * @param geom the geom to set.
 * @param R the new rotation matrix.
 * @sa dBodySetRotation
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomSetRotation (dGeomID geom, const dMatrix3 R);


/**
 * @brief Set the rotation of a placeable geom.
 *
 * If the geom is attached to a body, the body's rotation will also be changed.
 *
 * Calling this function on a non-placeable geom results in a runtime error in
 * the debug build of ODE.
 *
 * @param geom the geom to set.
 * @param Q the new rotation.
 * @sa dBodySetQuaternion
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomSetQuaternion (dGeomID geom, const dQuaternion Q);


/**
 * @brief Get the position vector of a placeable geom.
 *
 * If the geom is attached to a body, the body's position will be returned.
 *
 * Calling this function on a non-placeable geom results in a runtime error in
 * the debug build of ODE.
 *
 * @param geom the geom to query.
 * @returns A pointer to the geom's position vector.
 * @remarks The returned value is a pointer to the geom's internal
 *          data structure. It is valid until any changes are made
 *          to the geom.
 * @sa dBodyGetPosition
 * @ingroup collide
 */
ODE_API IMPORT_C const dReal * dGeomGetPosition (dGeomID geom);


/**
 * @brief Copy the position of a geom into a vector.
 * @ingroup collide
 * @param geom  the geom to query
 * @param pos   a copy of the geom position
 * @sa dGeomGetPosition
 */
ODE_API IMPORT_C void dGeomCopyPosition (dGeomID geom, dVector3 pos);


/**
 * @brief Get the rotation matrix of a placeable geom.
 *
 * If the geom is attached to a body, the body's rotation will be returned.
 *
 * Calling this function on a non-placeable geom results in a runtime error in
 * the debug build of ODE.
 *
 * @param geom the geom to query.
 * @returns A pointer to the geom's rotation matrix.
 * @remarks The returned value is a pointer to the geom's internal
 *          data structure. It is valid until any changes are made
 *          to the geom.
 * @sa dBodyGetRotation
 * @ingroup collide
 */
ODE_API IMPORT_C const dReal * dGeomGetRotation (dGeomID geom);


/**
 * @brief Get the rotation matrix of a placeable geom.
 *
 * If the geom is attached to a body, the body's rotation will be returned.
 *
 * Calling this function on a non-placeable geom results in a runtime error in
 * the debug build of ODE.
 *
 * @param geom   the geom to query.
 * @param R      a copy of the geom rotation
 * @sa dGeomGetRotation
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomCopyRotation(dGeomID geom, dMatrix3 R);


/**
 * @brief Get the rotation quaternion of a placeable geom.
 *
 * If the geom is attached to a body, the body's quaternion will be returned.
 *
 * Calling this function on a non-placeable geom results in a runtime error in
 * the debug build of ODE.
 *
 * @param geom the geom to query.
 * @param result a copy of the rotation quaternion.
 * @sa dBodyGetQuaternion
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomGetQuaternion (dGeomID geom, dQuaternion result);


/**
 * @brief Return the axis-aligned bounding box.
 *
 * Return in aabb an axis aligned bounding box that surrounds the given geom.
 * The aabb array has elements (minx, maxx, miny, maxy, minz, maxz). If the
 * geom is a space, a bounding box that surrounds all contained geoms is
 * returned.
 *
 * This function may return a pre-computed cached bounding box, if it can
 * determine that the geom has not moved since the last time the bounding
 * box was computed.
 *
 * @param geom the geom to query
 * @param aabb the returned bounding box
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomGetAABB (dGeomID geom, dReal aabb[6]);


/**
 * @brief Determing if a geom is a space.
 * @param geom the geom to query
 * @returns Non-zero if the geom is a space, zero otherwise.
 * @ingroup collide
 */
ODE_API IMPORT_C int dGeomIsSpace (dGeomID geom);


/**
 * @brief Query for the space containing a particular geom.
 * @param geom the geom to query
 * @returns The space that contains the geom, or NULL if the geom is
 *          not contained by a space.
 * @ingroup collide
 */
ODE_API IMPORT_C dSpaceID dGeomGetSpace (dGeomID);


/**
 * @brief Given a geom, this returns its class.
 *
 * The ODE classes are:
 *  @li dSphereClass
 *  @li dBoxClass
 *  @li dCylinderClass
 *  @li dPlaneClass
 *  @li dRayClass
 *  @li dConvexClass
 *  @li dGeomTransformClass
 *  @li dTriMeshClass
 *  @li dSimpleSpaceClass
 *  @li dQuadTreeSpaceClass
 *  @li dFirstUserClass
 *  @li dLastUserClass
 *
 * User-defined class will return their own number.
 *
 * @param geom the geom to query
 * @returns The geom class ID.
 * @ingroup collide
 */
ODE_API IMPORT_C int dGeomGetClass (dGeomID geom);


/**
 * @brief Set the "category" bitfield for the given geom.
 *
 * The category bitfield is used by spaces to govern which geoms will
 * interact with each other. The bitfield is guaranteed to be at least
 * 32 bits wide. The default category values for newly created geoms
 * have all bits set.
 *
 * @param geom the geom to set
 * @param bits the new bitfield value
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomSetCategoryBits (dGeomID geom, unsigned long bits);


/**
 * @brief Set the "collide" bitfield for the given geom.
 *
 * The collide bitfield is used by spaces to govern which geoms will
 * interact with each other. The bitfield is guaranteed to be at least
 * 32 bits wide. The default category values for newly created geoms
 * have all bits set.
 *
 * @param geom the geom to set
 * @param bits the new bitfield value
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomSetCollideBits (dGeomID geom, unsigned long bits);


/**
 * @brief Get the "category" bitfield for the given geom.
 *
 * @param geom the geom to set
 * @param bits the new bitfield value
 * @sa dGeomSetCategoryBits
 * @ingroup collide
 */
ODE_API IMPORT_C unsigned long dGeomGetCategoryBits (dGeomID);


/**
 * @brief Get the "collide" bitfield for the given geom.
 *
 * @param geom the geom to set
 * @param bits the new bitfield value
 * @sa dGeomSetCollideBits
 * @ingroup collide
 */
ODE_API IMPORT_C unsigned long dGeomGetCollideBits (dGeomID);


/**
 * @brief Enable a geom.
 *
 * Disabled geoms are completely ignored by dSpaceCollide and dSpaceCollide2,
 * although they can still be members of a space. New geoms are created in
 * the enabled state.
 *
 * @param geom   the geom to enable
 * @sa dGeomDisable
 * @sa dGeomIsEnabled
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomEnable (dGeomID geom);


/**
 * @brief Disable a geom.
 *
 * Disabled geoms are completely ignored by dSpaceCollide and dSpaceCollide2,
 * although they can still be members of a space. New geoms are created in
 * the enabled state.
 *
 * @param geom   the geom to disable
 * @sa dGeomDisable
 * @sa dGeomIsEnabled
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomDisable (dGeomID geom);


/**
 * @brief Check to see if a geom is enabled.
 *
 * Disabled geoms are completely ignored by dSpaceCollide and dSpaceCollide2,
 * although they can still be members of a space. New geoms are created in
 * the enabled state.
 *
 * @param geom   the geom to query
 * @returns Non-zero if the geom is enabled, zero otherwise.
 * @sa dGeomDisable
 * @sa dGeomIsEnabled
 * @ingroup collide
 */
ODE_API IMPORT_C int dGeomIsEnabled (dGeomID geom);

/* ************************************************************************ */
/* geom offset from body */

/**
 * @brief Set the local offset position of a geom from its body.
 *
 * Sets the geom's positional offset in local coordinates.
 * After this call, the geom will be at a new position determined from the
 * body's position and the offset.
 * The geom must be attached to a body.
 * If the geom did not have an offset, it is automatically created.
 *
 * @param geom the geom to set.
 * @param x the new X coordinate.
 * @param y the new Y coordinate.
 * @param z the new Z coordinate.
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomSetOffsetPosition (dGeomID geom, dReal x, dReal y, dReal z);


/**
 * @brief Set the local offset rotation matrix of a geom from its body.
 *
 * Sets the geom's rotational offset in local coordinates.
 * After this call, the geom will be at a new position determined from the
 * body's position and the offset.
 * The geom must be attached to a body.
 * If the geom did not have an offset, it is automatically created.
 *
 * @param geom the geom to set.
 * @param R the new rotation matrix.
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomSetOffsetRotation (dGeomID geom, const dMatrix3 R);


/**
 * @brief Set the local offset rotation of a geom from its body.
 *
 * Sets the geom's rotational offset in local coordinates.
 * After this call, the geom will be at a new position determined from the
 * body's position and the offset.
 * The geom must be attached to a body.
 * If the geom did not have an offset, it is automatically created.
 *
 * @param geom the geom to set.
 * @param Q the new rotation.
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomSetOffsetQuaternion (dGeomID geom, const dQuaternion Q);


/**
 * @brief Set the offset position of a geom from its body.
 *
 * Sets the geom's positional offset to move it to the new world
 * coordinates.
 * After this call, the geom will be at the world position passed in,
 * and the offset will be the difference from the current body position.
 * The geom must be attached to a body.
 * If the geom did not have an offset, it is automatically created.
 *
 * @param geom the geom to set.
 * @param x the new X coordinate.
 * @param y the new Y coordinate.
 * @param z the new Z coordinate.
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomSetOffsetWorldPosition (dGeomID geom, dReal x, dReal y, dReal z);


/**
 * @brief Set the offset rotation of a geom from its body.
 *
 * Sets the geom's rotational offset to orient it to the new world
 * rotation matrix.
 * After this call, the geom will be at the world orientation passed in,
 * and the offset will be the difference from the current body orientation.
 * The geom must be attached to a body.
 * If the geom did not have an offset, it is automatically created.
 *
 * @param geom the geom to set.
 * @param R the new rotation matrix.
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomSetOffsetWorldRotation (dGeomID geom, const dMatrix3 R);


/**
 * @brief Set the offset rotation of a geom from its body.
 *
 * Sets the geom's rotational offset to orient it to the new world
 * rotation matrix.
 * After this call, the geom will be at the world orientation passed in,
 * and the offset will be the difference from the current body orientation.
 * The geom must be attached to a body.
 * If the geom did not have an offset, it is automatically created.
 *
 * @param geom the geom to set.
 * @param Q the new rotation.
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomSetOffsetWorldQuaternion (dGeomID geom, const dQuaternion);


/**
 * @brief Clear any offset from the geom.
 *
 * If the geom has an offset, it is eliminated and the geom is
 * repositioned at the body's position.  If the geom has no offset,
 * this function does nothing.
 * This is more efficient than calling dGeomSetOffsetPosition(zero)
 * and dGeomSetOffsetRotation(identiy), because this function actually
 * eliminates the offset, rather than leaving it as the identity transform.
 *
 * @param geom the geom to have its offset destroyed.
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomClearOffset(dGeomID geom);


/**
 * @brief Check to see whether the geom has an offset.
 *
 * This function will return non-zero if the offset has been created.
 * Note that there is a difference between a geom with no offset,
 * and a geom with an offset that is the identity transform.
 * In the latter case, although the observed behaviour is identical,
 * there is a unnecessary computation involved because the geom will
 * be applying the transform whenever it needs to recalculate its world
 * position.
 *
 * @param geom the geom to query.
 * @returns Non-zero if the geom has an offset, zero otherwise.
 * @ingroup collide
 */
ODE_API IMPORT_C int dGeomIsOffset(dGeomID geom);


/**
 * @brief Get the offset position vector of a geom.
 *
 * Returns the positional offset of the geom in local coordinates.
 * If the geom has no offset, this function returns the zero vector.
 *
 * @param geom the geom to query.
 * @returns A pointer to the geom's offset vector.
 * @remarks The returned value is a pointer to the geom's internal
 *          data structure. It is valid until any changes are made
 *          to the geom.
 * @ingroup collide
 */
ODE_API IMPORT_C const dReal * dGeomGetOffsetPosition (dGeomID geom);


/**
 * @brief Copy the offset position vector of a geom.
 *
 * Returns the positional offset of the geom in local coordinates.
 * If the geom has no offset, this function returns the zero vector.
 *
 * @param geom   the geom to query.
 * @param pos    returns the offset position
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomCopyOffsetPosition (dGeomID geom, dVector3 pos);


/**
 * @brief Get the offset rotation matrix of a geom.
 *
 * Returns the rotational offset of the geom in local coordinates.
 * If the geom has no offset, this function returns the identity
 * matrix.
 *
 * @param geom the geom to query.
 * @returns A pointer to the geom's offset rotation matrix.
 * @remarks The returned value is a pointer to the geom's internal
 *          data structure. It is valid until any changes are made
 *          to the geom.
 * @ingroup collide
 */
ODE_API IMPORT_C const dReal * dGeomGetOffsetRotation (dGeomID geom);


/**
 * @brief Copy the offset rotation matrix of a geom.
 *
 * Returns the rotational offset of the geom in local coordinates.
 * If the geom has no offset, this function returns the identity
 * matrix.
 *
 * @param geom   the geom to query.
 * @param R      returns the rotation matrix.
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomCopyOffsetRotation (dGeomID geom, dMatrix3 R);


/**
 * @brief Get the offset rotation quaternion of a geom.
 *
 * Returns the rotation offset of the geom as a quaternion.
 * If the geom has no offset, the identity quaternion is returned.
 *
 * @param geom the geom to query.
 * @param result a copy of the rotation quaternion.
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomGetOffsetQuaternion (dGeomID geom, dQuaternion result);


/* ************************************************************************ */
/* collision detection */

/**
 *
 * @brief Given two geoms o1 and o2 that potentially intersect,
 * generate contact information for them.
 *
 * Internally, this just calls the correct class-specific collision
 * functions for o1 and o2.
 *
 * @param o1 The first geom to test.
 * @param o2 The second geom to test.
 *
 * @param flags The flags specify how contacts should be generated if
 * the geoms touch. The lower 16 bits of flags is an integer that
 * specifies the maximum number of contact points to generate. You must
 * ask for at least one contact. All other bits in flags must be zero.
 * In the future the other bits may be used to select from different
 * contact generation strategies.
 *
 * @param contact Points to an array of dContactGeom structures. The array
 * must be able to hold at least the maximum number of contacts. These
 * dContactGeom structures may be embedded within larger structures in the
 * array -- the skip parameter is the byte offset from one dContactGeom to
 * the next in the array. If skip is sizeof(dContactGeom) then contact
 * points to a normal (C-style) array. It is an error for skip to be smaller
 * than sizeof(dContactGeom).
 *
 * @returns If the geoms intersect, this function returns the number of contact
 * points generated (and updates the contact array), otherwise it returns 0
 * (and the contact array is not touched).
 *
 * @remarks If a space is passed as o1 or o2 then this function will collide
 * all objects contained in o1 with all objects contained in o2, and return
 * the resulting contact points. This method for colliding spaces with geoms
 * (or spaces with spaces) provides no user control over the individual
 * collisions. To get that control, use dSpaceCollide or dSpaceCollide2 instead.
 *
 * @remarks If o1 and o2 are the same geom then this function will do nothing
 * and return 0. Technically speaking an object intersects with itself, but it
 * is not useful to find contact points in this case.
 *
 * @remarks This function does not care if o1 and o2 are in the same space or not
 * (or indeed if they are in any space at all).
 *
 * @ingroup collide
 */
ODE_API IMPORT_C int dCollide (dGeomID o1, dGeomID o2, int flags, dContactGeom *contact,
	      int skip);

/**
 * @brief Determines which pairs of geoms in a space may potentially intersect,
 * and calls the callback function for each candidate pair.
 *
 * @param space The space to test.
 *
 * @param data Passed from dSpaceCollide directly to the callback
 * function. Its meaning is user defined. The o1 and o2 arguments are the
 * geoms that may be near each other.
 *
 * @param callback A callback function is of type @ref dNearCallback.
 *
 * @remarks Other spaces that are contained within the colliding space are
 * not treated specially, i.e. they are not recursed into. The callback
 * function may be passed these contained spaces as one or both geom
 * arguments.
 *
 * @remarks dSpaceCollide() is guaranteed to pass all intersecting geom
 * pairs to the callback function, but may also pass close but
 * non-intersecting pairs. The number of these calls depends on the
 * internal algorithms used by the space. Thus you should not expect
 * that dCollide will return contacts for every pair passed to the
 * callback.
 *
 * @sa dSpaceCollide2
 * @ingroup collide
 */
ODE_API IMPORT_C void dSpaceCollide (dSpaceID space, void *data, dNearCallback *callback);


/**
 * @brief Determines which geoms from one space may potentially intersect with 
 * geoms from another space, and calls the callback function for each candidate 
 * pair. 
 *
 * @param space1 The first space to test.
 *
 * @param space2 The second space to test.
 *
 * @param data Passed from dSpaceCollide directly to the callback
 * function. Its meaning is user defined. The o1 and o2 arguments are the
 * geoms that may be near each other.
 *
 * @param callback A callback function is of type @ref dNearCallback.
 *
 * @remarks This function can also test a single non-space geom against a 
 * space. This function is useful when there is a collision hierarchy, i.e. 
 * when there are spaces that contain other spaces.
 *
 * @remarks Other spaces that are contained within the colliding space are
 * not treated specially, i.e. they are not recursed into. The callback
 * function may be passed these contained spaces as one or both geom
 * arguments.
 *
 * @remarks dSpaceCollide2() is guaranteed to pass all intersecting geom
 * pairs to the callback function, but may also pass close but
 * non-intersecting pairs. The number of these calls depends on the
 * internal algorithms used by the space. Thus you should not expect
 * that dCollide will return contacts for every pair passed to the
 * callback.
 *
 * @sa dSpaceCollide
 * @ingroup collide
 */
ODE_API IMPORT_C void dSpaceCollide2 (dGeomID space1, dGeomID space2, void *data, dNearCallback *callback);


/* ************************************************************************ */
/* standard classes */

/* the maximum number of user classes that are supported */
enum {
  dMaxUserClasses = 4
};

/* class numbers - each geometry object needs a unique number */
enum {
  dSphereClass = 0,
  dBoxClass,
  dCapsuleClass,
  dCylinderClass,
  dPlaneClass,
  dRayClass,
  dConvexClass,
  dGeomTransformClass,
  dTriMeshClass,
  dHeightfieldClass,

  dFirstSpaceClass,
  dSimpleSpaceClass = dFirstSpaceClass,
  dQuadTreeSpaceClass,
  dLastSpaceClass = dQuadTreeSpaceClass,

  dFirstUserClass,
  dLastUserClass = dFirstUserClass + dMaxUserClasses - 1,
  dGeomNumClasses
};


/**
 * @defgroup collide_sphere Sphere Class
 * @ingroup collide
 */

/**
 * @brief Create a sphere geom of the given radius, and return its ID. 
 *
 * @param space   a space to contain the new geom. May be null.
 * @param radius  the radius of the sphere.
 *
 * @returns A new sphere geom.
 *
 * @remarks The point of reference for a sphere is its center.
 *
 * @sa dGeomDestroy
 * @sa dGeomSphereSetRadius
 * @ingroup collide_sphere
 */
ODE_API IMPORT_C dGeomID dCreateSphere (dSpaceID space, dReal radius);


/**
 * @brief Set the radius of a sphere geom.
 *
 * @param sphere  the sphere to set.
 * @param radius  the new radius.
 *
 * @sa dGeomSphereGetRadius
 * @ingroup collide_sphere
 */
ODE_API IMPORT_C void dGeomSphereSetRadius (dGeomID sphere, dReal radius);


/**
 * @brief Retrieves the radius of a sphere geom.
 *
 * @param sphere  the sphere to query.
 *
 * @sa dGeomSphereSetRadius
 * @ingroup collide_sphere
 */
ODE_API IMPORT_C dReal dGeomSphereGetRadius (dGeomID sphere);


/**
 * @brief Calculate the depth of the a given point within a sphere.
 *
 * @param sphere  the sphere to query.
 * @param x       the X coordinate of the point.
 * @param y       the Y coordinate of the point.
 * @param z       the Z coordinate of the point.
 *
 * @returns The depth of the point. Points inside the sphere will have a 
 * positive depth, points outside it will have a negative depth, and points
 * on the surface will have a depth of zero.
 *
 * @ingroup collide_sphere
 */
ODE_API IMPORT_C dReal dGeomSpherePointDepth (dGeomID sphere, dReal x, dReal y, dReal z);


//--> Convex Functions
ODE_API IMPORT_C dGeomID dCreateConvex (dSpaceID space,
			       dReal *_planes,
			       unsigned int _planecount,
			       dReal *_points,
			       unsigned int _pointcount,unsigned int *_polygons);

ODE_API IMPORT_C void dGeomSetConvex (dGeomID g,
			     dReal *_planes,
			     unsigned int _count,
			     dReal *_points,
			     unsigned int _pointcount,unsigned int *_polygons);
//<-- Convex Functions

/**
 * @defgroup collide_box Box Class
 * @ingroup collide
 */

/**
 * @brief Create a box geom with the provided side lengths.
 *
 * @param space   a space to contain the new geom. May be null.
 * @param lx      the length of the box along the X axis
 * @param ly      the length of the box along the Y axis
 * @param lz      the length of the box along the Z axis
 *
 * @returns A new box geom.
 *
 * @remarks The point of reference for a box is its center.
 *
 * @sa dGeomDestroy
 * @sa dGeomBoxSetLengths
 * @ingroup collide_box
 */
ODE_API IMPORT_C dGeomID dCreateBox (dSpaceID space, dReal lx, dReal ly, dReal lz);


/**
 * @brief Set the side lengths of the given box.
 *
 * @param box  the box to set
 * @param lx      the length of the box along the X axis
 * @param ly      the length of the box along the Y axis
 * @param lz      the length of the box along the Z axis
 *
 * @sa dGeomBoxGetLengths
 * @ingroup collide_box
 */
ODE_API IMPORT_C void dGeomBoxSetLengths (dGeomID box, dReal lx, dReal ly, dReal lz);


/**
 * @brief Get the side lengths of a box.
 *
 * @param box     the box to query
 * @param result  the returned side lengths
 *
 * @sa dGeomBoxSetLengths
 * @ingroup collide_box
 */
ODE_API IMPORT_C void dGeomBoxGetLengths (dGeomID box, dVector3 result);


/**
 * @brief Return the depth of a point in a box.
 * 
 * @param box  the box to query
 * @param x    the X coordinate of the point to test.
 * @param y    the Y coordinate of the point to test.
 * @param z    the Z coordinate of the point to test.
 *
 * @returns The depth of the point. Points inside the box will have a 
 * positive depth, points outside it will have a negative depth, and points
 * on the surface will have a depth of zero.
 */
ODE_API IMPORT_C dReal dGeomBoxPointDepth (dGeomID box, dReal x, dReal y, dReal z);


ODE_API IMPORT_C dGeomID dCreatePlane (dSpaceID space, dReal a, dReal b, dReal c, dReal d);
ODE_API IMPORT_C void dGeomPlaneSetParams (dGeomID plane, dReal a, dReal b, dReal c, dReal d);
ODE_API IMPORT_C void dGeomPlaneGetParams (dGeomID plane, dVector4 result);
ODE_API IMPORT_C dReal dGeomPlanePointDepth (dGeomID plane, dReal x, dReal y, dReal z);

ODE_API IMPORT_C dGeomID dCreateCapsule (dSpaceID space, dReal radius, dReal length);
ODE_API IMPORT_C void dGeomCapsuleSetParams (dGeomID ccylinder, dReal radius, dReal length);
ODE_API IMPORT_C void dGeomCapsuleGetParams (dGeomID ccylinder, dReal *radius, dReal *length);
ODE_API IMPORT_C dReal dGeomCapsulePointDepth (dGeomID ccylinder, dReal x, dReal y, dReal z);

// For now we want to have a backwards compatible C-API, note: C++ API is not.
#define dCreateCCylinder dCreateCapsule
#define dGeomCCylinderSetParams dGeomCapsuleSetParams
#define dGeomCCylinderGetParams dGeomCapsuleGetParams
#define dGeomCCylinderPointDepth dGeomCapsulePointDepth
#define dCCylinderClass dCapsuleClass

ODE_API IMPORT_C dGeomID dCreateCylinder (dSpaceID space, dReal radius, dReal length);
ODE_API IMPORT_C void dGeomCylinderSetParams (dGeomID cylinder, dReal radius, dReal length);
ODE_API IMPORT_C void dGeomCylinderGetParams (dGeomID cylinder, dReal *radius, dReal *length);

ODE_API IMPORT_C dGeomID dCreateRay (dSpaceID space, dReal length);
ODE_API IMPORT_C void dGeomRaySetLength (dGeomID ray, dReal length);
ODE_API IMPORT_C dReal dGeomRayGetLength (dGeomID ray);
ODE_API IMPORT_C void dGeomRaySet (dGeomID ray, dReal px, dReal py, dReal pz,
		  dReal dx, dReal dy, dReal dz);
ODE_API IMPORT_C void dGeomRayGet (dGeomID ray, dVector3 start, dVector3 dir);

/*
 * Set/get ray flags that influence ray collision detection.
 * These flags are currently only noticed by the trimesh collider, because
 * they can make a major differences there.
 */
ODE_API IMPORT_C void dGeomRaySetParams (dGeomID g, int FirstContact, int BackfaceCull);
ODE_API IMPORT_C void dGeomRayGetParams (dGeomID g, int *FirstContact, int *BackfaceCull);
ODE_API IMPORT_C void dGeomRaySetClosestHit (dGeomID g, int closestHit);
ODE_API IMPORT_C int dGeomRayGetClosestHit (dGeomID g);

ODE_API IMPORT_C dGeomID dCreateGeomTransform (dSpaceID space);
ODE_API IMPORT_C void dGeomTransformSetGeom (dGeomID g, dGeomID obj);
ODE_API IMPORT_C dGeomID dGeomTransformGetGeom (dGeomID g);
ODE_API IMPORT_C void dGeomTransformSetCleanup (dGeomID g, int mode);
ODE_API IMPORT_C int dGeomTransformGetCleanup (dGeomID g);
ODE_API IMPORT_C void dGeomTransformSetInfo (dGeomID g, int mode);
ODE_API IMPORT_C int dGeomTransformGetInfo (dGeomID g);


/* ************************************************************************ */
/* heightfield functions */


// Data storage for heightfield data.
struct dxHeightfieldData;
typedef struct dxHeightfieldData* dHeightfieldDataID;


/**
 * @brief Callback prototype
 *
 * Used by the callback heightfield data type to sample a height for a
 * given cell position.
 *
 * @param p_user_data User data specified when creating the dHeightfieldDataID
 * @param x The index of a sample in the local x axis. It is a value
 * in the range zero to ( nWidthSamples - 1 ).
 * @param x The index of a sample in the local z axis. It is a value
 * in the range zero to ( nDepthSamples - 1 ).
 *
 * @return The sample height which is then scaled and offset using the
 * values specified when the heightfield data was created.
 *
 * @ingroup collide
 */
typedef dReal dHeightfieldGetHeight( void* p_user_data, int x, int z );



/**
 * @brief Creates a heightfield geom.
 *
 * Uses the information in the given dHeightfieldDataID to construct
 * a geom representing a heightfield in a collision space.
 *
 * @param space The space to add the geom to.
 * @param data The dHeightfieldDataID created by dGeomHeightfieldDataCreate and
 * setup by dGeomHeightfieldDataBuildCallback, dGeomHeightfieldDataBuildByte,
 * dGeomHeightfieldDataBuildShort or dGeomHeightfieldDataBuildFloat.
 * @param bPlaceable If non-zero this geom can be transformed in the world using the
 * usual functions such as dGeomSetPosition and dGeomSetRotation. If the geom is
 * not set as placeable, then it uses a fixed orientation where the global y axis
 * represents the dynamic 'height' of the heightfield.
 *
 * @return A geom id to reference this geom in other calls.
 *
 * @ingroup collide
 */
ODE_API IMPORT_C dGeomID dCreateHeightfield( dSpaceID space,
					dHeightfieldDataID data, int bPlaceable );


/**
 * @brief Creates a new empty dHeightfieldDataID.
 *
 * Allocates a new dHeightfieldDataID and returns it. You must call
 * dGeomHeightfieldDataDestroy to destroy it after the geom has been removed.
 * The dHeightfieldDataID value is used when specifying a data format type.
 *
 * @return A dHeightfieldDataID for use with dGeomHeightfieldDataBuildCallback,
 * dGeomHeightfieldDataBuildByte, dGeomHeightfieldDataBuildShort or
 * dGeomHeightfieldDataBuildFloat.
 * @ingroup collide
 */
ODE_API IMPORT_C dHeightfieldDataID dGeomHeightfieldDataCreate();


/**
 * @brief Destroys a dHeightfieldDataID.
 *
 * Deallocates a given dHeightfieldDataID and all managed resources.
 *
 * @param d A dHeightfieldDataID created by dGeomHeightfieldDataCreate
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomHeightfieldDataDestroy( dHeightfieldDataID d );



/**
 * @brief Configures a dHeightfieldDataID to use a callback to
 * retrieve height data.
 *
 * Before a dHeightfieldDataID can be used by a geom it must be
 * configured to specify the format of the height data.
 * This call specifies that the heightfield data is computed by
 * the user and it should use the given callback when determining
 * the height of a given element of it's shape.
 *
 * @param d A new dHeightfieldDataID created by dGeomHeightfieldDataCreate
 *
 * @param width Specifies the total 'width' of the heightfield along
 * the geom's local x axis.
 * @param depth Specifies the total 'depth' of the heightfield along
 * the geom's local z axis.
 *
 * @param widthSamples Specifies the number of vertices to sample
 * along the width of the heightfield. Each vertex has a corresponding
 * height value which forms the overall shape.
 * Naturally this value must be at least two or more.
 * @param depthSamples Specifies the number of vertices to sample
 * along the depth of the heightfield.
 *
 * @param scale A uniform scale applied to all raw height data.
 * @param offset An offset applied to the scaled height data.
 *
 * @param thickness A value subtracted from the lowest height
 * value which in effect adds an additional cuboid to the base of the
 * heightfield. This is used to prevent geoms from looping under the
 * desired terrain and not registering as a collision. Note that the
 * thickness is not affected by the scale or offset parameters.
 *
 * @param bWrap If non-zero the heightfield will infinitely tile in both
 * directions along the local x and z axes. If zero the heightfield is
 * bounded from zero to width in the local x axis, and zero to depth in
 * the local z axis.
 *
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomHeightfieldDataBuildCallback( dHeightfieldDataID d,
				void* pUserData, dHeightfieldGetHeight* pCallback,
				dReal width, dReal depth, int widthSamples, int depthSamples,
				dReal scale, dReal offset, dReal thickness, int bWrap );

/**
 * @brief Configures a dHeightfieldDataID to use height data in byte format.
 *
 * Before a dHeightfieldDataID can be used by a geom it must be
 * configured to specify the format of the height data.
 * This call specifies that the heightfield data is stored as a rectangular
 * array of bytes (8 bit unsigned) representing the height at each sample point.
 *
 * @param d A new dHeightfieldDataID created by dGeomHeightfieldDataCreate
 *
 * @param pHeightData A pointer to the height data.
 * @param bCopyHeightData When non-zero the height data is copied to an
 * internal store. When zero the height data is accessed by reference and
 * so must persist throughout the lifetime of the heightfield.
 *
 * @param width Specifies the total 'width' of the heightfield along
 * the geom's local x axis.
 * @param depth Specifies the total 'depth' of the heightfield along
 * the geom's local z axis.
 *
 * @param widthSamples Specifies the number of vertices to sample
 * along the width of the heightfield. Each vertex has a corresponding
 * height value which forms the overall shape.
 * Naturally this value must be at least two or more.
 * @param depthSamples Specifies the number of vertices to sample
 * along the depth of the heightfield.
 *
 * @param scale A uniform scale applied to all raw height data.
 * @param offset An offset applied to the scaled height data.
 *
 * @param thickness A value subtracted from the lowest height
 * value which in effect adds an additional cuboid to the base of the
 * heightfield. This is used to prevent geoms from looping under the
 * desired terrain and not registering as a collision. Note that the
 * thickness is not affected by the scale or offset parameters.
 *
 * @param bWrap If non-zero the heightfield will infinitely tile in both
 * directions along the local x and z axes. If zero the heightfield is
 * bounded from zero to width in the local x axis, and zero to depth in
 * the local z axis.
 *
 * @ingroup collide
 */

ODE_API IMPORT_C void dGeomHeightfieldDataBuildSingle( dHeightfieldDataID d,
				const float* pHeightData, int bCopyHeightData,
				dReal width, dReal depth, int widthSamples, int depthSamples,
				dReal scale, dReal offset, dReal thickness, int bWrap );

/**
 * @brief Configures a dHeightfieldDataID to use height data in 
 * double precision floating point format.
 *
 * Before a dHeightfieldDataID can be used by a geom it must be
 * configured to specify the format of the height data.
 * This call specifies that the heightfield data is stored as a rectangular
 * array of double precision floats representing the height at each
 * sample point.
 *
 * @param d A new dHeightfieldDataID created by dGeomHeightfieldDataCreate
 *
 * @param pHeightData A pointer to the height data.
 * @param bCopyHeightData When non-zero the height data is copied to an
 * internal store. When zero the height data is accessed by reference and
 * so must persist throughout the lifetime of the heightfield.
 *
 * @param width Specifies the total 'width' of the heightfield along
 * the geom's local x axis.
 * @param depth Specifies the total 'depth' of the heightfield along
 * the geom's local z axis.
 *
 * @param widthSamples Specifies the number of vertices to sample
 * along the width of the heightfield. Each vertex has a corresponding
 * height value which forms the overall shape.
 * Naturally this value must be at least two or more.
 * @param depthSamples Specifies the number of vertices to sample
 * along the depth of the heightfield.
 *
 * @param scale A uniform scale applied to all raw height data.
 * @param offset An offset applied to the scaled height data.
 *
 * @param thickness A value subtracted from the lowest height
 * value which in effect adds an additional cuboid to the base of the
 * heightfield. This is used to prevent geoms from looping under the
 * desired terrain and not registering as a collision. Note that the
 * thickness is not affected by the scale or offset parameters.
 *
 * @param bWrap If non-zero the heightfield will infinitely tile in both
 * directions along the local x and z axes. If zero the heightfield is
 * bounded from zero to width in the local x axis, and zero to depth in
 * the local z axis.
 *
 * @ingroup collide
 */

ODE_API IMPORT_C void dGeomHeightfieldDataSetBounds( dHeightfieldDataID d,
				dReal minHeight, dReal maxHeight );


/**
 * @brief Assigns a dHeightfieldDataID to a heightfield geom.
 *
 * Associates the given dHeightfieldDataID with a heightfield geom.
 * This is done without affecting the GEOM_PLACEABLE flag.
 *
 * @param g A geom created by dCreateHeightfield
 * @param d A dHeightfieldDataID created by dGeomHeightfieldDataCreate
 * @ingroup collide
 */
ODE_API IMPORT_C void dGeomHeightfieldSetHeightfieldData( dGeomID g, dHeightfieldDataID d );


/**
 * @brief Gets the dHeightfieldDataID bound to a heightfield geom.
 *
 * Returns the dHeightfieldDataID associated with a heightfield geom.
 *
 * @param g A geom created by dCreateHeightfield
 * @return The dHeightfieldDataID which may be NULL if none was assigned.
 * @ingroup collide
 */
ODE_API IMPORT_C dHeightfieldDataID dGeomHeightfieldGetHeightfieldData( dGeomID g );



/* ************************************************************************ */
/* utility functions */

ODE_API IMPORT_C void dClosestLineSegmentPoints (const dVector3 a1, const dVector3 a2,
				const dVector3 b1, const dVector3 b2,
				dVector3 cp1, dVector3 cp2);

ODE_API IMPORT_C int dBoxTouchesBox (const dVector3 _p1, const dMatrix3 R1,
		    const dVector3 side1, const dVector3 _p2,
		    const dMatrix3 R2, const dVector3 side2);

ODE_API IMPORT_C int dBoxBox (const dVector3 p1, const dMatrix3 R1,
	     const dVector3 side1, const dVector3 p2,
	     const dMatrix3 R2, const dVector3 side2,
	     dVector3 normal, dReal *depth, int *return_code,
	     int maxc, dContactGeom *contact, int skip);

ODE_API IMPORT_C void dInfiniteAABB (dGeomID geom, dReal aabb[6]);
ODE_API IMPORT_C void dInitODE(void);
ODE_API IMPORT_C void dCloseODE(void);

/* ************************************************************************ */
/* custom classes */

typedef void dGetAABBFn (dGeomID, dReal aabb[6]);
typedef int dColliderFn (dGeomID o1, dGeomID o2,
			 int flags, dContactGeom *contact, int skip);
typedef dColliderFn * dGetColliderFnFn (int num);
typedef void dGeomDtorFn (dGeomID o);
typedef int dAABBTestFn (dGeomID o1, dGeomID o2, dReal aabb[6]);

typedef struct dGeomClass {
  int bytes;
  dGetColliderFnFn *collider;
  dGetAABBFn *aabb;
  dAABBTestFn *aabb_test;
  dGeomDtorFn *dtor;
} dGeomClass;

ODE_API IMPORT_C int dCreateGeomClass (const dGeomClass *classptr);
ODE_API IMPORT_C void * dGeomGetClassData (dGeomID);
ODE_API IMPORT_C dGeomID dCreateGeom (int classnum);

/* ************************************************************************ */

#ifdef __cplusplus
}
#endif

#endif
