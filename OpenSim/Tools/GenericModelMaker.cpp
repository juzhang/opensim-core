// GenericModelMaker.cpp
// Author: Peter Loan
/*
 * Copyright (c) 2006, Stanford University. All rights reserved. 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including 
 * without limitation the rights to use, copy, modify, merge, publish, 
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//=============================================================================
// INCLUDES
//=============================================================================
#include "GenericModelMaker.h"
#include <OpenSim/Simulation/Model/AbstractModel.h>
#include <OpenSim/Simulation/Model/VisibleMarker.h>

//=============================================================================
// STATICS
//=============================================================================
using namespace std;
using namespace OpenSim;

//=============================================================================
// CONSTRUCTOR(S) AND DESTRUCTOR
//=============================================================================
//_____________________________________________________________________________
/**
 * Default constructor.
 */
GenericModelMaker::GenericModelMaker() :
   _fileName(_fileNameProp.getValueStr()),
	_markerSetProp(PropertyObj("", MarkerSet())),
	_markerSet((MarkerSet&)_markerSetProp.getValueObj())
{
	setNull();
	setupProperties();
}

//_____________________________________________________________________________
/**
 * Destructor.
 */
GenericModelMaker::~GenericModelMaker()
{
}

//_____________________________________________________________________________
/**
 * Copy constructor.
 *
 * @param aGenericModelMaker GenericModelMaker to be copied.
 */
GenericModelMaker::GenericModelMaker(const GenericModelMaker &aGenericModelMaker) :
   Object(aGenericModelMaker),
   _fileName(_fileNameProp.getValueStr()),
	_markerSetProp(PropertyObj("", MarkerSet())),
	_markerSet((MarkerSet&)_markerSetProp.getValueObj())
{
	setNull();
	setupProperties();
	copyData(aGenericModelMaker);
}

//_____________________________________________________________________________
/**
 * Copy this GenericModelMaker and return a pointer to the copy.
 * The copy constructor for this class is used.
 *
 * @return Pointer to a copy of this GenericModelMaker.
 */
Object* GenericModelMaker::copy() const
{
	GenericModelMaker *genericModelMaker = new GenericModelMaker(*this);
	return(genericModelMaker);
}

//=============================================================================
// CONSTRUCTION METHODS
//=============================================================================
//_____________________________________________________________________________
/**
 * Copy data members from one GenericModelMaker to another.
 *
 * @param aGenericModelMaker GenericModelMaker to be copied.
 */
void GenericModelMaker::copyData(const GenericModelMaker &aGenericModelMaker)
{
	_fileName = aGenericModelMaker._fileName;
	_markerSet = aGenericModelMaker._markerSet;
}

//_____________________________________________________________________________
/**
 * Set the data members of this GenericModelMaker to their null values.
 */
void GenericModelMaker::setNull()
{
	setType("GenericModelMaker");
}

//_____________________________________________________________________________
/**
 * Connect properties to local pointers.
 */
void GenericModelMaker::setupProperties()
{
	_fileNameProp.setComment("Model file (.osim) for the unscaled model."); 
	_fileNameProp.setName("model_file");
	_propertySet.append(&_fileNameProp);

	_markerSetProp.setComment("Set of model markers used to scale the model. "
		"Scaling is done based on distances between model markers compared to "
		"the same distances between the corresponding experimental markers.");
	_markerSetProp.setName("MarkerSet");
	_propertySet.append(&_markerSetProp);
}

//_____________________________________________________________________________
/**
 * Register the types used by this class.
 */
void GenericModelMaker::registerTypes()
{
	//Object::RegisterType(Marker());
}

//=============================================================================
// OPERATORS
//=============================================================================
//_____________________________________________________________________________
/**
 * Assignment operator.
 *
 * @return Reference to this object.
 */
GenericModelMaker& GenericModelMaker::operator=(const GenericModelMaker &aGenericModelMaker)
{
	// BASE CLASS
	Object::operator=(aGenericModelMaker);

	copyData(aGenericModelMaker);

	return(*this);
}

//=============================================================================
// UTILITY
//=============================================================================
//_____________________________________________________________________________
/**
 * Execute the model making process, which involves reading
 * an XML model file and possible updating its marker set.
 *
 * @return Pointer to the AbstractModel that is constructed.
 */
AbstractModel* GenericModelMaker::processModel(const string& aPathToSubject)
{
	AbstractModel* model = NULL;

	cout << endl << "Step 1: Loading generic model" << endl;

	try
	{
		_fileName = aPathToSubject + _fileName;

		model = new AbstractModel(_fileName);
		model->setup();

		if (model->builtOK())
			model->getDynamicsEngine().updateMarkerSet(_markerSet);
	}
	catch (Exception &x)
	{
		x.print(cout);
		return NULL;
	}

	return model;
}

void GenericModelMaker::peteTest() const
{
	cout << "   GenericModel: " << getName() << endl;
	cout << "      fileName: " << _fileName << endl;
	cout << "      markers:" << endl;

	for (int i = 0; i < _markerSet.getSize(); i++)
		_markerSet.get(i)->peteTest();
}

