/*************************************************************************
 *                                                                       *
 * Open Dynamics Engine, Copyright (C) 2001,2002 Russell L. Smith.       *
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

#ifndef _ODE_OBJECTS_H_
#define _ODE_OBJECTS_H_

#include <ode/common.h>
#include <ode/mass.h>
#include <ode/contact.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup world World
 *
 * The world object is a container for rigid bodies and joints. Objects in
 * different worlds can not interact, for example rigid bodies from two
 * different worlds can not collide.
 *
 * All the objects in a world exist at the same point in time, thus one
 * reason to use separate worlds is to simulate systems at different rates.
 * Most applications will only need one world.
 */


/**
 * @brief Create a new, empty world and return its ID number.
 * @return an identifier
 * @ingroup world
 */
ODE_API IMPORT_C dWorldID dWorldCreate(void);


/**
 * @brief Destroy a world and everything in it.
 *
 * This includes all bodies, and all joints that are not part of a joint
 * group. Joints that are part of a joint group will be deactivated, and
 * can be destroyed by calling, for example, dJointGroupEmpty().
 * @ingroup world
 * @param world the identifier for the world the be destroyed.
 */
ODE_API IMPORT_C void dWorldDestroy (dWorldID world);


/**
 * @brief Set the world's global gravity vector.
 *
 * The units are m/s^2, so Earth's gravity vector would be (0,0,-9.81),
 * assuming that +z is up. The default is no gravity, i.e. (0,0,0).
 *
 * @ingroup world
 */
ODE_API IMPORT_C void dWorldSetGravity (dWorldID, dReal x, dReal y, dReal z);


/**
 * @brief Get the gravity vector for a given world.
 * @ingroup world
 */
ODE_API IMPORT_C void dWorldGetGravity (dWorldID, dVector3 gravity);


/**
 * @brief Set the global ERP value, that controls how much error
 * correction is performed in each time step.
 * @ingroup world
 * @param dWorldID the identifier of the world.
 * @param erp Typical values are in the range 0.1--0.8. The default is 0.2.
 */
ODE_API IMPORT_C void dWorldSetERP (dWorldID, dReal erp);

/**
 * @brief Get the error reduction parameter.
 * @ingroup world
 * @return ERP value
 */
ODE_API IMPORT_C dReal dWorldGetERP (dWorldID);


/**
 * @brief Set the global CFM (constraint force mixing) value.
 * @ingroup world
 * @param cfm Typical values are in the range @m{10^{-9}} -- 1.
 * The default is 10^-5 if single precision is being used, or 10^-10
 * if double precision is being used.
 */
ODE_API IMPORT_C void dWorldSetCFM (dWorldID, dReal cfm);

/**
 * @brief Get the constraint force mixing value.
 * @ingroup world
 * @return CFM value
 */
ODE_API IMPORT_C dReal dWorldGetCFM (dWorldID);


/**
 * @brief Step the world.
 *
 * This uses a "big matrix" method that takes time on the order of m^3
 * and memory on the order of m^2, where m is the total number of constraint
 * rows. For large systems this will use a lot of memory and can be very slow,
 * but this is currently the most accurate method.
 * @ingroup world
 * @param stepsize The number of seconds that the simulation has to advance.
 */
ODE_API IMPORT_C void dWorldStep (dWorldID, dReal stepsize);


/**
 * @brief Converts an impulse to a force.
 * @ingroup world
 * @remarks
 * If you want to apply a linear or angular impulse to a rigid body,
 * instead of a force or a torque, then you can use this function to convert
 * the desired impulse into a force/torque vector before calling the
 * BodyAdd... function.
 * The current algorithm simply scales the impulse by 1/stepsize,
 * where stepsize is the step size for the next step that will be taken.
 * This function is given a dWorldID because, in the future, the force
 * computation may depend on integrator parameters that are set as
 * properties of the world.
 */
ODE_API IMPORT_C void dWorldImpulseToForce
(
  dWorldID, dReal stepsize,
  dReal ix, dReal iy, dReal iz, dVector3 force
);


/**
 * @brief Step the world.
 * @ingroup world
 * @remarks
 * This uses an iterative method that takes time on the order of m*N
 * and memory on the order of m, where m is the total number of constraint
 * rows N is the number of iterations.
 * For large systems this is a lot faster than dWorldStep(),
 * but it is less accurate.
 * @remarks
 * QuickStep is great for stacks of objects especially when the
 * auto-disable feature is used as well.
 * However, it has poor accuracy for near-singular systems.
 * Near-singular systems can occur when using high-friction contacts, motors,
 * or certain articulated structures. For example, a robot with multiple legs
 * sitting on the ground may be near-singular.
 * @remarks
 * There are ways to help overcome QuickStep's inaccuracy problems:
 * \li Increase CFM.
 * \li Reduce the number of contacts in your system (e.g. use the minimum
 *     number of contacts for the feet of a robot or creature).
 * \li Don't use excessive friction in the contacts.
 * \li Use contact slip if appropriate
 * \li Avoid kinematic loops (however, kinematic loops are inevitable in
 *     legged creatures).
 * \li Don't use excessive motor strength.
 * \liUse force-based motors instead of velocity-based motors.
 *
 * Increasing the number of QuickStep iterations may help a little bit, but
 * it is not going to help much if your system is really near singular.
 */
ODE_API IMPORT_C void dWorldQuickStep (dWorldID w, dReal stepsize);


/**
 * @brief Set the number of iterations that the QuickStep method performs per
 *        step.
 * @ingroup world
 * @remarks
 * More iterations will give a more accurate solution, but will take
 * longer to compute.
 * @param num The default is 20 iterations.
 */
ODE_API IMPORT_C void dWorldSetQuickStepNumIterations (dWorldID, int num);


/**
 * @brief Get the number of iterations that the QuickStep method performs per
 *        step.
 * @ingroup world
 * @return nr of iterations
 */
ODE_API IMPORT_C int dWorldGetQuickStepNumIterations (dWorldID);

/**
 * @brief Set the SOR over-relaxation parameter
 * @ingroup world
 * @param over_relaxation value to use by SOR
 */
ODE_API IMPORT_C void dWorldSetQuickStepW (dWorldID, dReal over_relaxation);

/**
 * @brief Get the SOR over-relaxation parameter
 * @ingroup world
 * @returns the over-relaxation setting
 */
ODE_API IMPORT_C dReal dWorldGetQuickStepW (dWorldID);

/* World contact parameter functions */

/**
 * @brief Set the maximum correcting velocity that contacts are allowed
 * to generate.
 * @ingroup world
 * @param vel The default value is infinity (i.e. no limit).
 * @remarks
 * Reducing this value can help prevent "popping" of deeply embedded objects.
 */
ODE_API IMPORT_C void dWorldSetContactMaxCorrectingVel (dWorldID, dReal vel);

/**
 * @brief Get the maximum correcting velocity that contacts are allowed
 * to generated.
 * @ingroup world
 */
ODE_API IMPORT_C dReal dWorldGetContactMaxCorrectingVel (dWorldID);

/**
 * @brief Set the depth of the surface layer around all geometry objects.
 * @ingroup world
 * @remarks
 * Contacts are allowed to sink into the surface layer up to the given
 * depth before coming to rest.
 * @param depth The default value is zero.
 * @remarks
 * Increasing this to some small value (e.g. 0.001) can help prevent
 * jittering problems due to contacts being repeatedly made and broken.
 */
ODE_API IMPORT_C void dWorldSetContactSurfaceLayer (dWorldID, dReal depth);

/**
 * @brief Get the depth of the surface layer around all geometry objects.
 * @ingroup world
 * @returns the depth
 */
ODE_API IMPORT_C dReal dWorldGetContactSurfaceLayer (dWorldID);

/* StepFast1 functions */

/**
 * @brief Step the world using the StepFast1 algorithm.
 * @param stepsize the nr of seconds to advance the simulation.
 * @param maxiterations The number of iterations to perform.
 * @ingroup world
 */
ODE_API IMPORT_C void dWorldStepFast1(dWorldID, dReal stepsize, int maxiterations);


/**
 * @defgroup disable Automatic Enabling and Disabling
 *
 * Every body can be enabled or disabled. Enabled bodies participate in the
 * simulation, while disabled bodies are turned off and do not get updated
 * during a simulation step. New bodies are always created in the enabled state.
 *
 * A disabled body that is connected through a joint to an enabled body will be
 * automatically re-enabled at the next simulation step.
 *
 * Disabled bodies do not consume CPU time, therefore to speed up the simulation
 * bodies should be disabled when they come to rest. This can be done automatically
 * with the auto-disable feature.
 *
 * If a body has its auto-disable flag turned on, it will automatically disable
 * itself when
 *   @li It has been idle for a given number of simulation steps.
 *   @li It has also been idle for a given amount of simulation time.
 *
 * A body is considered to be idle when the magnitudes of both its
 * linear average velocity and angular average velocity are below given thresholds.
 * The sample size for the average defaults to one and can be disabled by setting
 * to zero with 
 *
 * Thus, every body has six auto-disable parameters: an enabled flag, a idle step
 * count, an idle time, linear/angular average velocity thresholds, and the
 * average samples count.
 *
 * Newly created bodies get these parameters from world.
 */

/**
 * @brief Set the AutoEnableDepth parameter used by the StepFast1 algorithm.
 * @ingroup disable
 */
ODE_API IMPORT_C void dWorldSetAutoEnableDepthSF1(dWorldID, int autoEnableDepth);

/**
 * @brief Get the AutoEnableDepth parameter used by the StepFast1 algorithm.
 * @ingroup disable
 */
ODE_API IMPORT_C int dWorldGetAutoEnableDepthSF1(dWorldID);

/**
 * @brief Get auto disable linear threshold for newly created bodies.
 * @ingroup disable
 * @return the threshold
 */
ODE_API IMPORT_C dReal dWorldGetAutoDisableLinearThreshold (dWorldID);

/**
 * @brief Set auto disable linear threshold for newly created bodies.
 * @param linear_threshold default is 0.01
 * @ingroup disable
 */
ODE_API IMPORT_C void  dWorldSetAutoDisableLinearThreshold (dWorldID, dReal linear_threshold);

/**
 * @brief Get auto disable angular threshold for newly created bodies.
 * @ingroup disable
 * @return the threshold
 */
ODE_API IMPORT_C dReal dWorldGetAutoDisableAngularThreshold (dWorldID);

/**
 * @brief Set auto disable angular threshold for newly created bodies.
 * @param linear_threshold default is 0.01
 * @ingroup disable
 */
ODE_API IMPORT_C void dWorldSetAutoDisableAngularThreshold (dWorldID, dReal angular_threshold);

/**
 * @brief Get auto disable linear average threshold for newly created bodies.
 * @ingroup disable
 * @return the threshold
 */
ODE_API dReal dWorldGetAutoDisableLinearAverageThreshold (dWorldID);

/**
 * @brief Set auto disable linear average threshold for newly created bodies.
 * @param linear_average_threshold default is 0.01
 * @ingroup disable
 */
ODE_API void  dWorldSetAutoDisableLinearAverageThreshold (dWorldID, dReal linear_average_threshold);

/**
 * @brief Get auto disable angular average threshold for newly created bodies.
 * @ingroup disable
 * @return the threshold
 */
ODE_API dReal dWorldGetAutoDisableAngularAverageThreshold (dWorldID);

/**
 * @brief Set auto disable angular average threshold for newly created bodies.
 * @param linear_average_threshold default is 0.01
 * @ingroup disable
 */
ODE_API void dWorldSetAutoDisableAngularAverageThreshold (dWorldID, dReal angular_average_threshold);

/**
 * @brief Get auto disable sample count for newly created bodies.
 * @ingroup disable
 * @return number of samples used
 */
ODE_API IMPORT_C int dWorldGetAutoDisableAverageSamplesCount (dWorldID);

/**
 * @brief Set auto disable average sample count for newly created bodies.
 * @ingroup disable
 * @param average_samples_count Default is 1, meaning only instantaneous velocity is used.
 * Set to zero to disable sampling and thus prevent any body from auto-disabling.
 */
ODE_API IMPORT_C void dWorldSetAutoDisableAverageSamplesCount (dWorldID, unsigned int average_samples_count );

/**
 * @brief Get auto disable steps for newly created bodies.
 * @ingroup disable
 * @return nr of steps
 */
ODE_API IMPORT_C int dWorldGetAutoDisableSteps (dWorldID);

/**
 * @brief Set auto disable steps for newly created bodies.
 * @ingroup disable
 * @param steps default is 10
 */
ODE_API IMPORT_C void dWorldSetAutoDisableSteps (dWorldID, int steps);

/**
 * @brief Get auto disable time for newly created bodies.
 * @ingroup disable
 * @return nr of seconds
 */
ODE_API IMPORT_C dReal dWorldGetAutoDisableTime (dWorldID);

/**
 * @brief Set auto disable time for newly created bodies.
 * @ingroup disable
 * @param time default is 0 seconds
 */
ODE_API IMPORT_C void dWorldSetAutoDisableTime (dWorldID, dReal time);

/**
 * @brief Get auto disable flag for newly created bodies.
 * @ingroup disable
 * @return 0 or 1
 */
ODE_API IMPORT_C int dWorldGetAutoDisableFlag (dWorldID);

/**
 * @brief Set auto disable flag for newly created bodies.
 * @ingroup disable
 * @param do_auto_disable default is false.
 */
ODE_API IMPORT_C void dWorldSetAutoDisableFlag (dWorldID, int do_auto_disable);



/**
 * @defgroup bodies Rigid Bodies
 *
 * A rigid body has various properties from the point of view of the
 * simulation. Some properties change over time:
 *
 *  @li Position vector (x,y,z) of the body's point of reference.
 *      Currently the point of reference must correspond to the body's center of mass.
 *  @li Linear velocity of the point of reference, a vector (vx,vy,vz).
 *  @li Orientation of a body, represented by a quaternion (qs,qx,qy,qz) or
 *      a 3x3 rotation matrix.
 *  @li Angular velocity vector (wx,wy,wz) which describes how the orientation
 *      changes over time.
 *
 * Other body properties are usually constant over time:
 *
 *  @li Mass of the body.
 *  @li Position of the center of mass with respect to the point of reference.
 *      In the current implementation the center of mass and the point of
 *      reference must coincide.
 *  @li Inertia matrix. This is a 3x3 matrix that describes how the body's mass
 *      is distributed around the center of mass. Conceptually each body has an
 *      x-y-z coordinate frame embedded in it that moves and rotates with the body.
 *
 * The origin of this coordinate frame is the body's point of reference. Some values
 * in ODE (vectors, matrices etc) are relative to the body coordinate frame, and others
 * are relative to the global coordinate frame.
 *
 * Note that the shape of a rigid body is not a dynamical property (except insofar as
 * it influences the various mass properties). It is only collision detection that cares
 * about the detailed shape of the body.
 */


/**
 * @brief Get auto disable linear average threshold.
 * @ingroup bodies
 * @return the threshold
 */
ODE_API IMPORT_C dReal dBodyGetAutoDisableLinearThreshold (dBodyID);

/**
 * @brief Set auto disable linear average threshold.
 * @ingroup bodies
 * @return the threshold
 */
ODE_API IMPORT_C void  dBodySetAutoDisableLinearThreshold (dBodyID, dReal linear_average_threshold);

/**
 * @brief Get auto disable angular average threshold.
 * @ingroup bodies
 * @return the threshold
 */
ODE_API IMPORT_C dReal dBodyGetAutoDisableAngularThreshold (dBodyID);

/**
 * @brief Set auto disable angular average threshold.
 * @ingroup bodies
 * @return the threshold
 */
ODE_API IMPORT_C void  dBodySetAutoDisableAngularThreshold (dBodyID, dReal angular_average_threshold);

/**
 * @brief Get auto disable average size (samples count).
 * @ingroup bodies
 * @return the nr of steps/size.
 */
ODE_API IMPORT_C int dBodyGetAutoDisableAverageSamplesCount (dBodyID);

/**
 * @brief Set auto disable average buffer size (average steps).
 * @ingroup bodies
 * @param average_samples_count the nr of samples to review.
 */
ODE_API IMPORT_C void dBodySetAutoDisableAverageSamplesCount (dBodyID, unsigned int average_samples_count);


/**
 * @brief Get auto steps a body must be thought of as idle to disable
 * @ingroup bodies
 * @return the nr of steps
 */
ODE_API IMPORT_C int dBodyGetAutoDisableSteps (dBodyID);

/**
 * @brief Set auto disable steps.
 * @ingroup bodies
 * @param steps the nr of steps.
 */
ODE_API IMPORT_C void dBodySetAutoDisableSteps (dBodyID, int steps);

/**
 * @brief Get auto disable time.
 * @ingroup bodies
 * @return nr of seconds
 */
ODE_API IMPORT_C dReal dBodyGetAutoDisableTime (dBodyID);

/**
 * @brief Set auto disable time.
 * @ingroup bodies
 * @param time nr of seconds.
 */
ODE_API IMPORT_C void  dBodySetAutoDisableTime (dBodyID, dReal time);

/**
 * @brief Get auto disable flag.
 * @ingroup bodies
 * @return 0 or 1
 */
ODE_API IMPORT_C int dBodyGetAutoDisableFlag (dBodyID);

/**
 * @brief Set auto disable flag.
 * @ingroup bodies
 * @param do_auto_disable 0 or 1
 */
ODE_API IMPORT_C void dBodySetAutoDisableFlag (dBodyID, int do_auto_disable);

/**
 * @brief Set auto disable defaults.
 * @remarks
 * Set the values for the body to those set as default for the world.
 * @ingroup bodies
 */
ODE_API IMPORT_C void  dBodySetAutoDisableDefaults (dBodyID);


/**
 * @brief Retrives the world attached to te given body.
 * @remarks
 * 
 * @ingroup bodies
 */
ODE_API IMPORT_C dWorldID dBodyGetWorld (dBodyID);

/**
 * @brief Create a body in given world.
 * @remarks
 * Default mass parameters are at position (0,0,0).
 * @ingroup bodies
 */
ODE_API IMPORT_C dBodyID dBodyCreate (dWorldID);

/**
 * @brief Destroy a body.
 * @remarks
 * All joints that are attached to this body will be put into limbo:
 * i.e. unattached and not affecting the simulation, but they will NOT be
 * deleted.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodyDestroy (dBodyID);

/**
 * @brief Set the body's user-data pointer.
 * @ingroup bodies
 * @param data arbitraty pointer
 */
ODE_API IMPORT_C void  dBodySetData (dBodyID, void *data);

/**
 * @brief Get the body's user-data pointer.
 * @ingroup bodies
 * @return a pointer to the user's data.
 */
ODE_API IMPORT_C void *dBodyGetData (dBodyID);

/**
 * @brief Set position of a body.
 * @remarks
 * After setting, the outcome of the simulation is undefined
 * if the new configuration is inconsistent with the joints/constraints
 * that are present.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodySetPosition   (dBodyID, dReal x, dReal y, dReal z);

/**
 * @brief Set the orientation of a body.
 * @ingroup bodies
 * @remarks
 * After setting, the outcome of the simulation is undefined
 * if the new configuration is inconsistent with the joints/constraints
 * that are present.
 */
ODE_API IMPORT_C void dBodySetRotation   (dBodyID, const dMatrix3 R);

/**
 * @brief Set the orientation of a body.
 * @ingroup bodies
 * @remarks
 * After setting, the outcome of the simulation is undefined
 * if the new configuration is inconsistent with the joints/constraints
 * that are present.
 */
ODE_API IMPORT_C void dBodySetQuaternion (dBodyID, const dQuaternion q);

/**
 * @brief Set the linear velocity of a body.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodySetLinearVel  (dBodyID, dReal x, dReal y, dReal z);

/**
 * @brief Set the angular velocity of a body.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodySetAngularVel (dBodyID, dReal x, dReal y, dReal z);

/**
 * @brief Get the position of a body.
 * @ingroup bodies
 * @remarks
 * When getting, the returned values are pointers to internal data structures,
 * so the vectors are valid until any changes are made to the rigid body
 * system structure.
 * @sa dBodyCopyPosition
 */
ODE_API IMPORT_C const dReal * dBodyGetPosition   (dBodyID);


/**
 * @brief Copy the position of a body into a vector.
 * @ingroup bodies
 * @param body  the body to query
 * @param pos   a copy of the body position
 * @sa dBodyGetPosition
 */
ODE_API IMPORT_C void dBodyCopyPosition (dBodyID body, dVector3 pos);


/**
 * @brief Get the rotation of a body.
 * @ingroup bodies
 * @return pointer to a 4x3 rotation matrix.
 */
ODE_API IMPORT_C const dReal * dBodyGetRotation   (dBodyID);


/**
 * @brief Copy the rotation of a body.
 * @ingroup bodies
 * @param body   the body to query
 * @param R      a copy of the rotation matrix
 * @sa dBodyGetRotation
 */
ODE_API IMPORT_C void dBodyCopyRotation (dBodyID, dMatrix3 R);


/**
 * @brief Get the rotation of a body.
 * @ingroup bodies
 * @return pointer to 4 scalars that represent the quaternion.
 */
ODE_API IMPORT_C const dReal * dBodyGetQuaternion (dBodyID);


/**
 * @brief Copy the orientation of a body into a quaternion.
 * @ingroup bodies
 * @param body  the body to query
 * @param quat  a copy of the orientation quaternion
 * @sa dBodyGetQuaternion
 */
ODE_API IMPORT_C void dBodyCopyQuaternion(dBodyID body, dQuaternion quat);


/**
 * @brief Get the linear velocity of a body.
 * @ingroup bodies
 */
ODE_API IMPORT_C const dReal * dBodyGetLinearVel  (dBodyID);

/**
 * @brief Get the angular velocity of a body.
 * @ingroup bodies
 */
ODE_API IMPORT_C const dReal * dBodyGetAngularVel (dBodyID);

/**
 * @brief Set the mass of a body.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodySetMass (dBodyID, const dMass *mass);

/**
 * @brief Get the mass of a body.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodyGetMass (dBodyID, dMass *mass);

/**
 * @brief Add force at centre of mass of body in absolute coordinates.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodyAddForce            (dBodyID, dReal fx, dReal fy, dReal fz);

/**
 * @brief Add torque at centre of mass of body in absolute coordinates.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodyAddTorque           (dBodyID, dReal fx, dReal fy, dReal fz);

/**
 * @brief Add force at centre of mass of body in coordinates relative to body.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodyAddRelForce         (dBodyID, dReal fx, dReal fy, dReal fz);

/**
 * @brief Add torque at centre of mass of body in coordinates relative to body.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodyAddRelTorque        (dBodyID, dReal fx, dReal fy, dReal fz);

/**
 * @brief Add force at specified point in body in global coordinates.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodyAddForceAtPos       (dBodyID, dReal fx, dReal fy, dReal fz,
			                dReal px, dReal py, dReal pz);
/**
 * @brief Add force at specified point in body in local coordinates.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodyAddForceAtRelPos    (dBodyID, dReal fx, dReal fy, dReal fz,
			                dReal px, dReal py, dReal pz);
/**
 * @brief Add force at specified point in body in global coordinates.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodyAddRelForceAtPos    (dBodyID, dReal fx, dReal fy, dReal fz,
			                dReal px, dReal py, dReal pz);
/**
 * @brief Add force at specified point in body in local coordinates.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodyAddRelForceAtRelPos (dBodyID, dReal fx, dReal fy, dReal fz,
			                dReal px, dReal py, dReal pz);

/**
 * @brief Return the current accumulated force vector.
 * @return points to an array of 3 reals.
 * @remarks
 * The returned values are pointers to internal data structures, so
 * the vectors are only valid until any changes are made to the rigid
 * body system.
 * @ingroup bodies
 */
ODE_API IMPORT_C const dReal * dBodyGetForce   (dBodyID);

/**
 * @brief Return the current accumulated torque vector.
 * @return points to an array of 3 reals.
 * @remarks
 * The returned values are pointers to internal data structures, so
 * the vectors are only valid until any changes are made to the rigid
 * body system.
 * @ingroup bodies
 */
ODE_API IMPORT_C const dReal * dBodyGetTorque  (dBodyID);

/**
 * @brief Set the body force accumulation vector.
 * @remarks
 * This is mostly useful to zero the force and torque for deactivated bodies
 * before they are reactivated, in the case where the force-adding functions
 * were called on them while they were deactivated.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodySetForce  (dBodyID b, dReal x, dReal y, dReal z);

/**
 * @brief Set the body torque accumulation vector.
 * @remarks
 * This is mostly useful to zero the force and torque for deactivated bodies
 * before they are reactivated, in the case where the force-adding functions
 * were called on them while they were deactivated.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodySetTorque (dBodyID b, dReal x, dReal y, dReal z);

/**
 * @brief Get world position of a relative point on body.
 * @ingroup bodies
 * @param result will contain the result.
 */
ODE_API IMPORT_C void dBodyGetRelPointPos
(
  dBodyID, dReal px, dReal py, dReal pz,
  dVector3 result
);

/**
 * @brief Get velocity vector in global coords of a relative point on body.
 * @ingroup bodies
 * @param result will contain the result.
 */
ODE_API IMPORT_C void dBodyGetRelPointVel
(
  dBodyID, dReal px, dReal py, dReal pz,
  dVector3 result
);

/**
 * @brief Get velocity vector in global coords of a globally
 * specified point on a body.
 * @ingroup bodies
 * @param result will contain the result.
 */
ODE_API IMPORT_C void dBodyGetPointVel
(
  dBodyID, dReal px, dReal py, dReal pz,
  dVector3 result
);

/**
 * @brief takes a point in global coordinates and returns
 * the point's position in body-relative coordinates.
 * @remarks
 * This is the inverse of dBodyGetRelPointPos()
 * @ingroup bodies
 * @param result will contain the result.
 */
ODE_API IMPORT_C void dBodyGetPosRelPoint
(
  dBodyID, dReal px, dReal py, dReal pz,
  dVector3 result
);

/**
 * @brief Convert from local to world coordinates.
 * @ingroup bodies
 * @param result will contain the result.
 */
ODE_API IMPORT_C void dBodyVectorToWorld
(
  dBodyID, dReal px, dReal py, dReal pz,
  dVector3 result
);

/**
 * @brief Convert from world to local coordinates.
 * @ingroup bodies
 * @param result will contain the result.
 */
ODE_API IMPORT_C void dBodyVectorFromWorld
(
  dBodyID, dReal px, dReal py, dReal pz,
  dVector3 result
);

/**
 * @brief controls the way a body's orientation is updated at each timestep.
 * @ingroup bodies
 * @param mode can be 0 or 1:
 * \li 0: An ``infinitesimal'' orientation update is used.
 * This is fast to compute, but it can occasionally cause inaccuracies
 * for bodies that are rotating at high speed, especially when those
 * bodies are joined to other bodies.
 * This is the default for every new body that is created.
 * \li 1: A ``finite'' orientation update is used.
 * This is more costly to compute, but will be more accurate for high
 * speed rotations.
 * @remarks
 * Note however that high speed rotations can result in many types of
 * error in a simulation, and the finite mode will only fix one of those
 * sources of error.
 */
ODE_API IMPORT_C void dBodySetFiniteRotationMode (dBodyID, int mode);

/**
 * @brief sets the finite rotation axis for a body.
 * @ingroup bodies
 * @remarks
 * This is axis only has meaning when the finite rotation mode is set
 * If this axis is zero (0,0,0), full finite rotations are performed on
 * the body.
 * If this axis is nonzero, the body is rotated by performing a partial finite
 * rotation along the axis direction followed by an infinitesimal rotation
 * along an orthogonal direction.
 * @remarks
 * This can be useful to alleviate certain sources of error caused by quickly
 * spinning bodies. For example, if a car wheel is rotating at high speed
 * you can call this function with the wheel's hinge axis as the argument to
 * try and improve its behavior.
 */
ODE_API IMPORT_C void dBodySetFiniteRotationAxis (dBodyID, dReal x, dReal y, dReal z);

/**
 * @brief Get the way a body's orientation is updated each timestep.
 * @ingroup bodies
 * @return the mode 0 (infitesimal) or 1 (finite).
 */
ODE_API IMPORT_C int dBodyGetFiniteRotationMode (dBodyID);

/**
 * @brief Get the finite rotation axis.
 * @param result will contain the axis.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodyGetFiniteRotationAxis (dBodyID, dVector3 result);

/**
 * @brief Get the number of joints that are attached to this body.
 * @ingroup bodies
 * @return nr of joints
 */
ODE_API IMPORT_C int dBodyGetNumJoints (dBodyID b);

/**
 * @brief Return a joint attached to this body, given by index.
 * @ingroup bodies
 * @param index valid range is  0 to n-1 where n is the value returned by
 * dBodyGetNumJoints().
 */
ODE_API IMPORT_C dJointID dBodyGetJoint (dBodyID, int index);

/**
 * @brief Manually enable a body.
 * @param dBodyID identification of body.
 * @ingroup bodies
 */
ODE_API IMPORT_C void dBodyEnable (dBodyID);

/**
 * @brief Manually disable a body.
 * @ingroup bodies
 * @remarks
 * A disabled body that is connected through a joint to an enabled body will
 * be automatically re-enabled at the next simulation step.
 */
ODE_API IMPORT_C void dBodyDisable (dBodyID);

/**
 * @brief Check wether a body is enabled.
 * @ingroup bodies
 * @return 1 if a body is currently enabled or 0 if it is disabled.
 */
ODE_API IMPORT_C int dBodyIsEnabled (dBodyID);

/**
 * @brief Set whether the body is influenced by the world's gravity or not.
 * @ingroup bodies
 * @param mode when nonzero gravity affects this body.
 * @remarks
 * Newly created bodies are always influenced by the world's gravity.
 */
ODE_API IMPORT_C void dBodySetGravityMode (dBodyID b, int mode);

/**
 * @brief Get whether the body is influenced by the world's gravity or not.
 * @ingroup bodies
 * @return nonzero means gravity affects this body.
 */
ODE_API IMPORT_C int dBodyGetGravityMode (dBodyID b);



/**
 * @defgroup joints Joints
 *
 * In real life a joint is something like a hinge, that is used to connect two
 * objects.
 * In ODE a joint is very similar: It is a relationship that is enforced between
 * two bodies so that they can only have certain positions and orientations
 * relative to each other.
 * This relationship is called a constraint -- the words joint and
 * constraint are often used interchangeably.
 *
 * A joint has a set of parameters that can be set. These include:
 *
 *
 * \li  dParamLoStop Low stop angle or position. Setting this to
 *	-dInfinity (the default value) turns off the low stop.
 *	For rotational joints, this stop must be greater than -pi to be
 *	effective.
 * \li  dParamHiStop High stop angle or position. Setting this to
 *	dInfinity (the default value) turns off the high stop.
 *	For rotational joints, this stop must be less than pi to be
 *	effective.
 *	If the high stop is less than the low stop then both stops will
 *	be ineffective.
 * \li  dParamVel Desired motor velocity (this will be an angular or
 *	linear velocity).
 * \li  dParamFMax The maximum force or torque that the motor will use to
 *	achieve the desired velocity.
 *	This must always be greater than or equal to zero.
 *	Setting this to zero (the default value) turns off the motor.
 * \li  dParamFudgeFactor The current joint stop/motor implementation has
 *	a small problem:
 *	when the joint is at one stop and the motor is set to move it away
 *	from the stop, too much force may be applied for one time step,
 *	causing a ``jumping'' motion.
 *	This fudge factor is used to scale this excess force.
 *	It should have a value between zero and one (the default value).
 *	If the jumping motion is too visible in a joint, the value can be
 *	reduced.
 *	Making this value too small can prevent the motor from being able to
 *	move the joint away from a stop.
 * \li  dParamBounce The bouncyness of the stops.
 *	This is a restitution parameter in the range 0..1.
 *	0 means the stops are not bouncy at all, 1 means maximum bouncyness.
 * \li  dParamCFM The constraint force mixing (CFM) value used when not
 *	at a stop.
 * \li  dParamStopERP The error reduction parameter (ERP) used by the
 *	stops.
 * \li  dParamStopCFM The constraint force mixing (CFM) value used by the
 *	stops. Together with the ERP value this can be used to get spongy or
 *	soft stops.
 *	Note that this is intended for unpowered joints, it does not really
 *	work as expected when a powered joint reaches its limit.
 * \li  dParamSuspensionERP Suspension error reduction parameter (ERP).
 *	Currently this is only implemented on the hinge-2 joint.
 * \li  dParamSuspensionCFM Suspension constraint force mixing (CFM) value.
 *	Currently this is only implemented on the hinge-2 joint.
 *
 * If a particular parameter is not implemented by a given joint, setting it
 * will have no effect.
 * These parameter names can be optionally followed by a digit (2 or 3)
 * to indicate the second or third set of parameters, e.g. for the second axis
 * in a hinge-2 joint, or the third axis in an AMotor joint.
 */


/**
 * @brief Create a new joint of the ball type.
 * @ingroup joints
 * @remarks
 * The joint is initially in "limbo" (i.e. it has no effect on the simulation)
 * because it does not connect to any bodies.
 * @param dJointGroupID set to 0 to allocate the joint normally.
 * If it is nonzero the joint is allocated in the given joint group.
 */
ODE_API IMPORT_C dJointID dJointCreateBall (dWorldID, dJointGroupID);

/**
 * @brief Create a new joint of the hinge type.
 * @ingroup joints
 * @param dJointGroupID set to 0 to allocate the joint normally.
 * If it is nonzero the joint is allocated in the given joint group.
 */
ODE_API IMPORT_C dJointID dJointCreateHinge (dWorldID, dJointGroupID);

/**
 * @brief Create a new joint of the slider type.
 * @ingroup joints
 * @param dJointGroupID set to 0 to allocate the joint normally.
 * If it is nonzero the joint is allocated in the given joint group.
 */
ODE_API IMPORT_C dJointID dJointCreateSlider (dWorldID, dJointGroupID);

/**
 * @brief Create a new joint of the contact type.
 * @ingroup joints
 * @param dJointGroupID set to 0 to allocate the joint normally.
 * If it is nonzero the joint is allocated in the given joint group.
 */
ODE_API IMPORT_C dJointID dJointCreateContact (dWorldID, dJointGroupID, const dContact *);

/**
 * @brief Create a new joint of the hinge2 type.
 * @ingroup joints
 * @param dJointGroupID set to 0 to allocate the joint normally.
 * If it is nonzero the joint is allocated in the given joint group.
 */
ODE_API IMPORT_C dJointID dJointCreateHinge2 (dWorldID, dJointGroupID);

/**
 * @brief Create a new joint of the universal type.
 * @ingroup joints
 * @param dJointGroupID set to 0 to allocate the joint normally.
 * If it is nonzero the joint is allocated in the given joint group.
 */
ODE_API IMPORT_C dJointID dJointCreateUniversal (dWorldID, dJointGroupID);

/**
 * @brief Create a new joint of the PR (Prismatic and Rotoide) type.
 * @ingroup joints
 * @param dJointGroupID set to 0 to allocate the joint normally.
 * If it is nonzero the joint is allocated in the given joint group.
 */
ODE_API IMPORT_C dJointID dJointCreatePR (dWorldID, dJointGroupID);

/**
 * @brief Create a new joint of the fixed type.
 * @ingroup joints
 * @param dJointGroupID set to 0 to allocate the joint normally.
 * If it is nonzero the joint is allocated in the given joint group.
 */
ODE_API IMPORT_C dJointID dJointCreateFixed (dWorldID, dJointGroupID);

ODE_API IMPORT_C dJointID dJointCreateNull (dWorldID, dJointGroupID);

/**
 * @brief Create a new joint of the A-motor type.
 * @ingroup joints
 * @param dJointGroupID set to 0 to allocate the joint normally.
 * If it is nonzero the joint is allocated in the given joint group.
 */
ODE_API IMPORT_C dJointID dJointCreateAMotor (dWorldID, dJointGroupID);

/**
 * @brief Create a new joint of the L-motor type.
 * @ingroup joints
 * @param dJointGroupID set to 0 to allocate the joint normally.
 * If it is nonzero the joint is allocated in the given joint group.
 */
ODE_API IMPORT_C dJointID dJointCreateLMotor (dWorldID, dJointGroupID);

/**
 * @brief Create a new joint of the plane-2d type.
 * @ingroup joints
 * @param dJointGroupID set to 0 to allocate the joint normally.
 * If it is nonzero the joint is allocated in the given joint group.
 */
ODE_API IMPORT_C dJointID dJointCreatePlane2D (dWorldID, dJointGroupID);

/**
 * @brief Destroy a joint.
 * @ingroup joints
 *
 * disconnects it from its attached bodies and removing it from the world.
 * However, if the joint is a member of a group then this function has no
 * effect - to destroy that joint the group must be emptied or destroyed.
 */
ODE_API IMPORT_C void dJointDestroy (dJointID);


/**
 * @brief Create a joint group
 * @ingroup joints
 * @param max_size deprecated. Set to 0.
 */
ODE_API IMPORT_C dJointGroupID dJointGroupCreate (int max_size);

/**
 * @brief Destroy a joint group.
 * @ingroup joints
 *
 * All joints in the joint group will be destroyed.
 */
ODE_API IMPORT_C void dJointGroupDestroy (dJointGroupID);

/**
 * @brief Empty a joint group.
 * @ingroup joints
 *
 * All joints in the joint group will be destroyed,
 * but the joint group itself will not be destroyed.
 */
ODE_API IMPORT_C void dJointGroupEmpty (dJointGroupID);

/**
 * @brief Attach the joint to some new bodies.
 * @ingroup joints
 *
 * If the joint is already attached, it will be detached from the old bodies
 * first.
 * To attach this joint to only one body, set body1 or body2 to zero - a zero
 * body refers to the static environment.
 * Setting both bodies to zero puts the joint into "limbo", i.e. it will
 * have no effect on the simulation.
 * @remarks
 * Some joints, like hinge-2 need to be attached to two bodies to work.
 */
ODE_API IMPORT_C void dJointAttach (dJointID, dBodyID body1, dBodyID body2);

/**
 * @brief Set the user-data pointer
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetData (dJointID, void *data);

/**
 * @brief Get the user-data pointer
 * @ingroup joints
 */
ODE_API IMPORT_C void *dJointGetData (dJointID);

/**
 * @brief Get the type of the joint
 * @ingroup joints
 * @return the type, being one of these:
 * \li JointTypeBall
 * \li JointTypeHinge
 * \li JointTypeSlider
 * \li JointTypeContact
 * \li JointTypeUniversal
 * \li JointTypeHinge2
 * \li JointTypeFixed
 * \li JointTypeAMotor
 * \li JointTypeLMotor
 */
ODE_API IMPORT_C int dJointGetType (dJointID);

/**
 * @brief Return the bodies that this joint connects.
 * @ingroup joints
 * @param index return the first (0) or second (1) body.
 * @remarks
 * If one of these returned body IDs is zero, the joint connects the other body
 * to the static environment.
 * If both body IDs are zero, the joint is in ``limbo'' and has no effect on
 * the simulation.
 */
ODE_API IMPORT_C dBodyID dJointGetBody (dJointID, int index);

/**
 * @brief Sets the datastructure that is to receive the feedback.
 *
 * The feedback can be used by the user, so that it is known how
 * much force an individual joint exerts.
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetFeedback (dJointID, dJointFeedback *);

/**
 * @brief Gets the datastructure that is to receive the feedback.
 * @ingroup joints
 */
ODE_API IMPORT_C dJointFeedback *dJointGetFeedback (dJointID);

/**
 * @brief Set the joint anchor point.
 * @ingroup joints
 *
 * The joint will try to keep this point on each body
 * together. The input is specified in world coordinates.
 */
ODE_API IMPORT_C void dJointSetBallAnchor (dJointID, dReal x, dReal y, dReal z);

/**
 * @brief Set the joint anchor point.
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetBallAnchor2 (dJointID, dReal x, dReal y, dReal z);

/**
 * @brief Set hinge anchor parameter.
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetHingeAnchor (dJointID, dReal x, dReal y, dReal z);

ODE_API IMPORT_C void dJointSetHingeAnchorDelta (dJointID, dReal x, dReal y, dReal z, dReal ax, dReal ay, dReal az);

/**
 * @brief Set hinge axis.
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetHingeAxis (dJointID, dReal x, dReal y, dReal z);

/**
 * @brief set joint parameter
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetHingeParam (dJointID, int parameter, dReal value);

/**
 * @brief Applies the torque about the hinge axis.
 *
 * That is, it applies a torque with specified magnitude in the direction
 * of the hinge axis, to body 1, and with the same magnitude but in opposite
 * direction to body 2. This function is just a wrapper for dBodyAddTorque()}
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointAddHingeTorque(dJointID joint, dReal torque);

/**
 * @brief set the joint axis
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetSliderAxis (dJointID, dReal x, dReal y, dReal z);

/**
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetSliderAxisDelta (dJointID, dReal x, dReal y, dReal z, dReal ax, dReal ay, dReal az);

/**
 * @brief set joint parameter
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetSliderParam (dJointID, int parameter, dReal value);

/**
 * @brief Applies the given force in the slider's direction.
 *
 * That is, it applies a force with specified magnitude, in the direction of
 * slider's axis, to body1, and with the same magnitude but opposite
 * direction to body2.  This function is just a wrapper for dBodyAddForce().
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointAddSliderForce(dJointID joint, dReal force);

/**
 * @brief set anchor
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetHinge2Anchor (dJointID, dReal x, dReal y, dReal z);

/**
 * @brief set axis
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetHinge2Axis1 (dJointID, dReal x, dReal y, dReal z);

/**
 * @brief set axis
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetHinge2Axis2 (dJointID, dReal x, dReal y, dReal z);

/**
 * @brief set joint parameter
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetHinge2Param (dJointID, int parameter, dReal value);

/**
 * @brief Applies torque1 about the hinge2's axis 1, torque2 about the
 * hinge2's axis 2.
 * @remarks  This function is just a wrapper for dBodyAddTorque().
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointAddHinge2Torques(dJointID joint, dReal torque1, dReal torque2);

/**
 * @brief set anchor
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetUniversalAnchor (dJointID, dReal x, dReal y, dReal z);

/**
 * @brief set axis
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetUniversalAxis1 (dJointID, dReal x, dReal y, dReal z);

/**
 * @brief set axis
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetUniversalAxis2 (dJointID, dReal x, dReal y, dReal z);

/**
 * @brief set joint parameter
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetUniversalParam (dJointID, int parameter, dReal value);

/**
 * @brief Applies torque1 about the universal's axis 1, torque2 about the
 * universal's axis 2.
 * @remarks This function is just a wrapper for dBodyAddTorque().
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointAddUniversalTorques(dJointID joint, dReal torque1, dReal torque2);


/**
 * @brief set anchor
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetPRAnchor (dJointID, dReal x, dReal y, dReal z);

/**
 * @brief set the axis for the prismatic articulation
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetPRAxis1 (dJointID, dReal x, dReal y, dReal z);

/**
 * @brief set the axis for the rotoide articulation
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetPRAxis2 (dJointID, dReal x, dReal y, dReal z);

/**
 * @brief set joint parameter
 * @ingroup joints
 *
 * @note parameterX where X equal 2 refer to parameter for the rotoide articulation
 */
ODE_API IMPORT_C void dJointSetPRParam (dJointID, int parameter, dReal value);

/**
 * @brief Applies the torque about the rotoide axis of the PR joint
 *
 * That is, it applies a torque with specified magnitude in the direction 
 * of the rotoide axis, to body 1, and with the same magnitude but in opposite
 * direction to body 2. This function is just a wrapper for dBodyAddTorque()}
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointAddPRTorque (dJointID j, dReal torque);


/**
 * @brief Call this on the fixed joint after it has been attached to
 * remember the current desired relative offset and desired relative
 * rotation between the bodies.
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetFixed (dJointID);

/**
 * @brief set the nr of axes
 * @param num 0..3
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetAMotorNumAxes (dJointID, int num);

/**
 * @brief set axis
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetAMotorAxis (dJointID, int anum, int rel,
			  dReal x, dReal y, dReal z);

/**
 * @brief Tell the AMotor what the current angle is along axis anum.
 *
 * This function should only be called in dAMotorUser mode, because in this
 * mode the AMotor has no other way of knowing the joint angles.
 * The angle information is needed if stops have been set along the axis,
 * but it is not needed for axis motors.
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetAMotorAngle (dJointID, int anum, dReal angle);

/**
 * @brief set joint parameter
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetAMotorParam (dJointID, int parameter, dReal value);

/**
 * @brief set mode
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetAMotorMode (dJointID, int mode);

/**
 * @brief Applies torque0 about the AMotor's axis 0, torque1 about the
 * AMotor's axis 1, and torque2 about the AMotor's axis 2.
 * @remarks
 * If the motor has fewer than three axes, the higher torques are ignored.
 * This function is just a wrapper for dBodyAddTorque().
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointAddAMotorTorques (dJointID, dReal torque1, dReal torque2, dReal torque3);

/**
 * @brief Set the number of axes that will be controlled by the LMotor.
 * @param num can range from 0 (which effectively deactivates the joint) to 3.
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetLMotorNumAxes (dJointID, int num);

/**
 * @brief Set the AMotor axes.
 * @param anum selects the axis to change (0,1 or 2).
 * @param rel Each axis can have one of three ``relative orientation'' modes
 * \li 0: The axis is anchored to the global frame.
 * \li 1: The axis is anchored to the first body.
 * \li 2: The axis is anchored to the second body.
 * @remarks The axis vector is always specified in global coordinates
 * regardless of the setting of rel.
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetLMotorAxis (dJointID, int anum, int rel, dReal x, dReal y, dReal z);

/**
 * @brief set joint parameter
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetLMotorParam (dJointID, int parameter, dReal value);

/**
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetPlane2DXParam (dJointID, int parameter, dReal value);

/**
 * @ingroup joints
 */

ODE_API IMPORT_C void dJointSetPlane2DYParam (dJointID, int parameter, dReal value);

/**
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointSetPlane2DAngleParam (dJointID, int parameter, dReal value);

/**
 * @brief Get the joint anchor point, in world coordinates.
 *
 * This returns the point on body 1. If the joint is perfectly satisfied,
 * this will be the same as the point on body 2.
 */
ODE_API IMPORT_C void dJointGetBallAnchor (dJointID, dVector3 result);

/**
 * @brief Get the joint anchor point, in world coordinates.
 *
 * This returns the point on body 2. You can think of a ball and socket
 * joint as trying to keep the result of dJointGetBallAnchor() and
 * dJointGetBallAnchor2() the same.  If the joint is perfectly satisfied,
 * this function will return the same value as dJointGetBallAnchor() to
 * within roundoff errors. dJointGetBallAnchor2() can be used, along with
 * dJointGetBallAnchor(), to see how far the joint has come apart.
 */
ODE_API IMPORT_C void dJointGetBallAnchor2 (dJointID, dVector3 result);

/**
 * @brief Get the hinge anchor point, in world coordinates.
 *
 * This returns the point on body 1. If the joint is perfectly satisfied,
 * this will be the same as the point on body 2.
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointGetHingeAnchor (dJointID, dVector3 result);

/**
 * @brief Get the joint anchor point, in world coordinates.
 * @return The point on body 2. If the joint is perfectly satisfied,
 * this will return the same value as dJointGetHingeAnchor().
 * If not, this value will be slightly different.
 * This can be used, for example, to see how far the joint has come apart.
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointGetHingeAnchor2 (dJointID, dVector3 result);

/**
 * @brief get axis
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointGetHingeAxis (dJointID, dVector3 result);

/**
 * @brief get joint parameter
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetHingeParam (dJointID, int parameter);

/**
 * @brief Get the hinge angle.
 *
 * The angle is measured between the two bodies, or between the body and
 * the static environment.
 * The angle will be between -pi..pi.
 * When the hinge anchor or axis is set, the current position of the attached
 * bodies is examined and that position will be the zero angle.
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetHingeAngle (dJointID);

/**
 * @brief Get the hinge angle time derivative.
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetHingeAngleRate (dJointID);

/**
 * @brief Get the slider linear position (i.e. the slider's extension)
 *
 * When the axis is set, the current position of the attached bodies is
 * examined and that position will be the zero position.
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetSliderPosition (dJointID);

/**
 * @brief Get the slider linear position's time derivative.
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetSliderPositionRate (dJointID);

/**
 * @brief Get the slider axis
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointGetSliderAxis (dJointID, dVector3 result);

/**
 * @brief get joint parameter
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetSliderParam (dJointID, int parameter);

/**
 * @brief Get the joint anchor point, in world coordinates.
 * @return the point on body 1.  If the joint is perfectly satisfied,
 * this will be the same as the point on body 2.
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointGetHinge2Anchor (dJointID, dVector3 result);

/**
 * @brief Get the joint anchor point, in world coordinates.
 * This returns the point on body 2. If the joint is perfectly satisfied,
 * this will return the same value as dJointGetHinge2Anchor.
 * If not, this value will be slightly different.
 * This can be used, for example, to see how far the joint has come apart.
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointGetHinge2Anchor2 (dJointID, dVector3 result);

/**
 * @brief Get joint axis
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointGetHinge2Axis1 (dJointID, dVector3 result);

/**
 * @brief Get joint axis
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointGetHinge2Axis2 (dJointID, dVector3 result);

/**
 * @brief get joint parameter
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetHinge2Param (dJointID, int parameter);

/**
 * @brief Get angle
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetHinge2Angle1 (dJointID);

/**
 * @brief Get time derivative of angle
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetHinge2Angle1Rate (dJointID);

/**
 * @brief Get time derivative of angle
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetHinge2Angle2Rate (dJointID);

/**
 * @brief Get the joint anchor point, in world coordinates.
 * @return the point on body 1. If the joint is perfectly satisfied,
 * this will be the same as the point on body 2.
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointGetUniversalAnchor (dJointID, dVector3 result);

/**
 * @brief Get the joint anchor point, in world coordinates.
 * @return This returns the point on body 2.
 * @remarks
 * You can think of the ball and socket part of a universal joint as
 * trying to keep the result of dJointGetBallAnchor() and
 * dJointGetBallAnchor2() the same. If the joint is
 * perfectly satisfied, this function will return the same value
 * as dJointGetUniversalAnchor() to within roundoff errors.
 * dJointGetUniversalAnchor2() can be used, along with
 * dJointGetUniversalAnchor(), to see how far the joint has come apart.
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointGetUniversalAnchor2 (dJointID, dVector3 result);

/**
 * @brief Get axis
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointGetUniversalAxis1 (dJointID, dVector3 result);

/**
 * @brief Get axis
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointGetUniversalAxis2 (dJointID, dVector3 result);


/**
 * @brief get joint parameter
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetUniversalParam (dJointID, int parameter);

/**
 * @brief Get both angles at the same time.
 * @ingroup joints
 *
 * @param joint   The universal joint for which we want to calculate the angles
 * @param angle1  The angle between the body1 and the axis 1
 * @param angle2  The angle between the body2 and the axis 2
 *
 * @note This function combine getUniversalAngle1 and getUniversalAngle2 together
 *       and try to avoid redundant calculation
 */
ODE_API IMPORT_C void dJointGetUniversalAngles (dJointID, dReal *angle1, dReal *angle2);

/**
 * @brief Get angle
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetUniversalAngle1 (dJointID);

/**
 * @brief Get angle
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetUniversalAngle2 (dJointID);

/**
 * @brief Get time derivative of angle
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetUniversalAngle1Rate (dJointID);

/**
 * @brief Get time derivative of angle
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetUniversalAngle2Rate (dJointID);



/**
 * @brief Get the joint anchor point, in world coordinates.
 * @return the point on body 1. If the joint is perfectly satisfied, 
 * this will be the same as the point on body 2.
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointGetPRAnchor (dJointID, dVector3 result);

/**
 * @brief Get the PR linear position (i.e. the prismatic's extension)
 *
 * When the axis is set, the current position of the attached bodies is
 * examined and that position will be the zero position.
 *
 * The position is the "oriented" length between the
 * position = (Prismatic axis) dot_product [(body1 + offset) - (body2 + anchor2)]
 *
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetPRPosition (dJointID);

/**
 * @brief Get the PR linear position's time derivative
 *
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetPRPositionRate (dJointID);


/**
 * @brief Get the prismatic axis
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointGetPRAxis1 (dJointID, dVector3 result);

/**
 * @brief Get the Rotoide axis
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointGetPRAxis2 (dJointID, dVector3 result);

/**
 * @brief get joint parameter
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetPRParam (dJointID, int parameter);



/**
 * @brief Get the number of angular axes that will be controlled by the
 * AMotor.
 * @param num can range from 0 (which effectively deactivates the
 * joint) to 3.
 * This is automatically set to 3 in dAMotorEuler mode.
 * @ingroup joints
 */
ODE_API IMPORT_C int dJointGetAMotorNumAxes (dJointID);

/**
 * @brief Get the AMotor axes.
 * @param anum selects the axis to change (0,1 or 2).
 * @param rel Each axis can have one of three ``relative orientation'' modes.
 * \li 0: The axis is anchored to the global frame.
 * \li 1: The axis is anchored to the first body.
 * \li 2: The axis is anchored to the second body.
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointGetAMotorAxis (dJointID, int anum, dVector3 result);

/**
 * @brief Get axis
 * @remarks
 * The axis vector is always specified in global coordinates regardless
 * of the setting of rel.
 * There are two GetAMotorAxis functions, one to return the axis and one to
 * return the relative mode.
 *
 * For dAMotorEuler mode:
 * \li	Only axes 0 and 2 need to be set. Axis 1 will be determined
	automatically at each time step.
 * \li	Axes 0 and 2 must be perpendicular to each other.
 * \li	Axis 0 must be anchored to the first body, axis 2 must be anchored
	to the second body.
 * @ingroup joints
 */
ODE_API IMPORT_C int dJointGetAMotorAxisRel (dJointID, int anum);

/**
 * @brief Get the current angle for axis.
 * @remarks
 * In dAMotorUser mode this is simply the value that was set with
 * dJointSetAMotorAngle().
 * In dAMotorEuler mode this is the corresponding euler angle.
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetAMotorAngle (dJointID, int anum);

/**
 * @brief Get the current angle rate for axis anum.
 * @remarks
 * In dAMotorUser mode this is always zero, as not enough information is
 * available.
 * In dAMotorEuler mode this is the corresponding euler angle rate.
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetAMotorAngleRate (dJointID, int anum);

/**
 * @brief get joint parameter
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetAMotorParam (dJointID, int parameter);

/**
 * @brief Get the angular motor mode.
 * @param mode must be one of the following constants:
 * \li dAMotorUser The AMotor axes and joint angle settings are entirely
 * controlled by the user.  This is the default mode.
 * \li dAMotorEuler Euler angles are automatically computed.
 * The axis a1 is also automatically computed.
 * The AMotor axes must be set correctly when in this mode,
 * as described below.
 * When this mode is initially set the current relative orientations
 * of the bodies will correspond to all euler angles at zero.
 * @ingroup joints
 */
ODE_API IMPORT_C int dJointGetAMotorMode (dJointID);

/**
 * @brief Get nr of axes.
 * @ingroup joints
 */
ODE_API IMPORT_C int dJointGetLMotorNumAxes (dJointID);

/**
 * @brief Get axis.
 * @ingroup joints
 */
ODE_API IMPORT_C void dJointGetLMotorAxis (dJointID, int anum, dVector3 result);

/**
 * @brief get joint parameter
 * @ingroup joints
 */
ODE_API IMPORT_C dReal dJointGetLMotorParam (dJointID, int parameter);


/**
 * @ingroup joints
 */
ODE_API IMPORT_C dJointID dConnectingJoint (dBodyID, dBodyID);

/**
 * @ingroup joints
 */
ODE_API IMPORT_C int dConnectingJointList (dBodyID, dBodyID, dJointID*);

/**
 * @brief Utility function
 * @return 1 if the two bodies are connected together by
 * a joint, otherwise return 0.
 * @ingroup joints
 */
ODE_API IMPORT_C int dAreConnected (dBodyID, dBodyID);

/**
 * @brief Utility function
 * @return 1 if the two bodies are connected together by
 * a joint that does not have type @arg{joint_type}, otherwise return 0.
 * @param body1 A body to check.
 * @param body2 A body to check.
 * @param joint_type is a dJointTypeXXX constant.
 * This is useful for deciding whether to add contact joints between two bodies:
 * if they are already connected by non-contact joints then it may not be
 * appropriate to add contacts, however it is okay to add more contact between-
 * bodies that already have contacts.
 * @ingroup joints
 */
ODE_API IMPORT_C int dAreConnectedExcluding (dBodyID body1, dBodyID body2, int joint_type);


#ifdef __cplusplus
}
#endif

#endif
