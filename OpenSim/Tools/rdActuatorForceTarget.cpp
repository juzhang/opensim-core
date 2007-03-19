// rdActuatorForceTarget.cpp
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Copyright (c) 2006 Stanford University and Realistic Dynamics, Inc.
// Contributors: Frank C. Anderson
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject
// to the following conditions:
// 
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS,
// CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
// THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// This software, originally developed by Realistic Dynamics, Inc., was
// transferred to Stanford University on November 1, 2006.
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//==============================================================================
// INCLUDES
//==============================================================================
#include <iostream>
#include <OpenSim/Common/Exception.h>
#include <OpenSim/Simulation/Model/DerivCallbackSet.h>
#include <OpenSim/SQP/rdFSQP.h>
#include "rdCMC.h"
#include "rdActuatorForceTarget.h"
#include "rdCMC_TaskSet.h"

using namespace std;
using namespace OpenSim;


//==============================================================================
// DESTRUCTOR & CONSTRUCTIOR(S)
//==============================================================================
//______________________________________________________________________________
/**
 * Destructor.
 */
rdActuatorForceTarget::~rdActuatorForceTarget()
{
}
//______________________________________________________________________________
/**
 * Constructor.
 *
 * @param aNX Number of controls.
 * @param aController Parent controller.
 */
rdActuatorForceTarget::rdActuatorForceTarget(int aNX,rdCMC *aController) :
	rdOptimizationTarget(aNX),
	_x(0.0), _y(0.0), _dydt(0.0), _dqdt(0.0), _dudt(0.0), _stressTermWeight(1.0)
{
	setNull();

	// CONTROLLER
	_controller = aController;

	// NUMBER OF CONTROLS
	_nx = aNX;
	if(_nx<=0) {
		throw(Exception("rdActuatorForceTarget: ERROR- no controls.\n"));
	}

	// ALLOCATE STATE ARRAYS
	AbstractModel *model = _controller->getModel();
	if(model==NULL) {
		throw(Exception("rdActuatorForceTarget: ERROR- no model.\n"));
	}
	int nx = model->getNumControls();
	int ny = model->getNumStates();
	int nq = model->getNumCoordinates();
	int nu = model->getNumSpeeds();
	_x.setSize(nx);
	_y.setSize(ny);
	_dydt.setSize(ny);
	_dqdt.setSize(nq);
	_dudt.setSize(nu);

	// NUMBERS OF CONSTRAINTS
	_nineqn = 0;
	_nineq = 0;
	_neqn = 0;
	_neq = 0;

	// DERIVATIVE PERTURBATION SIZES;
	setDX(1.0e-6);
}


//==============================================================================
// CONSTRUCTION AND DESTRUCTION
//==============================================================================
//______________________________________________________________________________
/**
 * Set all member variables to their NULL or default values.
 */
void rdActuatorForceTarget::
setNull()
{
	_controller = NULL;
}


//==============================================================================
// SET AND GET
//==============================================================================
//______________________________________________________________________________
/**
 * Set the weight of the actuator force stress term in the performance
 * criterion.
 *
 * @param aWeight Weight premultiplying the sum squared stresses.
 */
void rdActuatorForceTarget::
setStressTermWeight(double aWeight)
{
	_stressTermWeight = aWeight;
}


//==============================================================================
// PERFORMANCE AND CONSTRAINTS
//==============================================================================
//------------------------------------------------------------------------------
// PERFORMANCE AND CONSTRAINTS
//------------------------------------------------------------------------------
//______________________________________________________________________________
/**
 * Compute performance and the constraints given x.
 * Note - used by paramopt.
 *
 * @param x Array of controls.
 * @param p Value of the performance criterion.
 * @param c Array of constraint values.
 * @return Status (normal termination = 0, error < 0).
 */
int rdActuatorForceTarget::
compute(double *x,double *p,double *c)
{
	int status = 0;
	return(status);
}
//______________________________________________________________________________
/**
 * Compute the gradients of the performance and the constraints given x.
 * The array dx is an array of perturbation sizes which can be used to
 * compute the gradients numerically.
 *
 * Note- used by paramopt.
 *
 * @param dx Array of perturbations for numerical derivatives.
 * @param x Array of controls.
 * @param dpdx Derivative of the performance criterion with respect to
 * the controls.
 * @param dcdx Matrix of derivatives of the constraints with respect
 * to the controls.
 * @return Status (normal termination = 0, error < 0).
 */
int rdActuatorForceTarget::
computeGradients(double *dx,double *x,double *dpdx,double *dcdx)
{
	int status = 0;
	return(status);
}

//------------------------------------------------------------------------------
// PERFORMANCE
//------------------------------------------------------------------------------
//______________________________________________________________________________
/**
 * Compute performance given x.
 *
 * @param aF Array of controls.
 * @param rP Value of the performance criterion.
 * @return Status (normal termination = 0, error < 0).
 */
int rdActuatorForceTarget::
computePerformance(double *aF,double *rP)
{
	int i;

	AbstractModel *model = _controller->getModel();
	rdCMC_TaskSet *taskSet = _controller->getTaskSet();

	// TIME STUFF
	//double timeNorm = model->getTimeNormConstant();
	double t = model->getTime();
	//double tReal = t * timeNorm;

	// SET
	model->getStates(&_y[0]);
	model->setStates(&_y[0]);
	model->getDerivCallbackSet()->set(t,&_x[0],&_y[0]);

	// ACTUATION
	model->getActuatorSet()->computeActuation();
	//int nf = model->getNumActuators();
	model->getDerivCallbackSet()->computeActuation(t,&_x[0],&_y[0]);
	ActuatorSet *actuatorSet = model->getActuatorSet();
	int nf = actuatorSet->getSize();
	AbstractActuator *actuator=NULL;
	for(i=0;i<nf;i++) {
		actuator = actuatorSet->get(i);
		actuator->setForce(aF[i]);
	}
	model->getActuatorSet()->apply();
	model->getDerivCallbackSet()->applyActuation(t,&_x[0],&_y[0]);

	// CONTACT
	model->getContactSet()->computeContact();
	model->getDerivCallbackSet()->computeContact(t,&_x[0],&_y[0]);
	model->getContactSet()->apply();
	model->getDerivCallbackSet()->applyContact(t,&_x[0],&_y[0]);

	// ACCELERATIONS
	model->getDynamicsEngine().computeDerivatives(&_dqdt[0],&_dudt[0]);
	model->getDerivCallbackSet()->computeDerivatives(t,&_x[0],&_y[0],&_dydt[0]);
	taskSet->computeAccelerations();
	Array<double> &w = taskSet->getWeights();
	Array<double> &aDes = taskSet->getDesiredAccelerations();
	Array<double> &a = taskSet->getAccelerations();

	// PERFORMANCE
	int nacc = aDes.getSize();
	double stress,pf,pacc;
	for(pf=0.0,i=0;i<nf;i++) {
		actuator = actuatorSet->get(i);
		stress = actuator->getStress();
		pf += stress * stress;
	}
	for(pacc=0.0,i=0;i<nacc;i++) {
		//cout<<w[i]<<" "<<aDes[i]<<" "<<a[i]<<endl;
		pacc += w[i]*(aDes[i]-a[i])*(aDes[i]-a[i]);
	}
	//cout<<"pf="<<pf<<"   "<<"pacc="<<pacc<<endl;
	//cout<<"pa = "<<pa<<endl;
	*rP = _stressTermWeight * pf + pacc;

	return(0);
}
//______________________________________________________________________________
/**
 * Compute the gradient of performance given x.
 *
 * @param x Array of controls.
 * @param dpdx Derivatives of performance with respect to the controls.
 * @return Status (normal termination = 0, error < 0).
 */
int rdActuatorForceTarget::
computePerformanceGradient(double *x,double *dpdx)
{
	int status = rdFSQP::CentralDifferences(this,_dx,x,dpdx);
	return(status);
}

//------------------------------------------------------------------------------
// CONSTRAINT
//------------------------------------------------------------------------------
//______________________________________________________________________________
/**
 * Compute constraint ic given x.
 *
 * @param x Array of controls.
 * @param ic Index of the constraint (indexing starts at 1, not 0).
 * @param c Value of constraint ic.
 * @return Status (normal termination = 0, error < 0).
 */
int rdActuatorForceTarget::
computeConstraint(double *x,int ic,double *c)
{
	int status = 0;
	return(status);
}
//______________________________________________________________________________
/**
 * Compute the gradient of constraint i given x.
 *
 * @param x Array of controls.
 * @param ic Index of the constraint (indexing starts at 1, not 0).
 * @param dcdx Derivative of constraint ic with respect to the controls.
 * @return Status (normal termination = 0, error < 0).
 */
int rdActuatorForceTarget::
computeConstraintGradient(double *x,int ic,double *dcdx)
{
	// COMPUTE GRADIENT
	rdFSQP::CentralDifferencesConstraint(this,_dx,x,ic,dcdx);
	return(0);
}

