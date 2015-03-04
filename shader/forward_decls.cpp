//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 12 April 2008 4:49:45 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "forward_decls.h"


// we use forward declarations of intrusive pointer hook functions 
// so we instantiate an intrusive pointer without seeing its full definition *and*
// we then make the future definitions inline so the optimiser can get at them 
// without whole program analysis we have to make sure all of them get referenced
// somewhere otherwise we get unresolved externals


//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{


} // end namespace
} // end namespace
