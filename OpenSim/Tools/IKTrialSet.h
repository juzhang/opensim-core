#ifndef __IKTrialSet_h__
#define __IKTrialSet_h__

// IKTrialSet.h
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

#include "osimToolsDLL.h"
#include "osimToolsDLL.h"
#include <OpenSim/Common/Set.h>
#include "IKTrial.h"

#ifdef SWIG
	#ifdef OSIMTOOLS_API
		#undef OSIMTOOLS_API
		#define OSIMTOOLS_API
	#endif
#endif

namespace OpenSim {

//=============================================================================
//=============================================================================
/**
 * A class for holding a set of SIMM coordinates.
 *
 * @authors Peter Loan
 * @version 1.0
 */

class  OSIMTOOLS_API IKTrialSet :	public Set<IKTrial>
{
private:
	void setNull();
public:
	IKTrialSet();
	IKTrialSet(const IKTrialSet& aSimmIKTrialSet);
	~IKTrialSet(void);
	//--------------------------------------------------------------------------
	// OPERATORS
	//--------------------------------------------------------------------------
#ifndef SWIG
	IKTrialSet& operator=(const IKTrialSet &aSimmIKTrialSet);
#endif
//=============================================================================
};	// END of class IKTrialSet
//=============================================================================
//=============================================================================

} // end of namespace OpenSim

#endif // __IKTrialSet_h__
