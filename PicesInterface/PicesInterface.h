#ifndef  _PICESINTERFACE_
#define  _PICESINTERFACE_
#pragma once

using namespace System;

/*! 
 \namespace  PicesInterface
 \brief Classes that encapsulate Unmanaged code for use in the managed world of c# are located in this namepace.
 \details The c# Managed world can not make calls to unmanaged code.  So that unmanaged classes that are usefull 
  in BaseLibrary (KKU), SipperInstruments (SipperHardware), and PicesLibrary (MLL) can not be directly used.  A 
  separate set of classes need to be defined that will encapsulate the unmanaged classes providing a managed 
  interface need to be provided.  Ex:  'PicesFeatureVector' encapsulates an instance of the unmanaged class 
  'ImageFeatures'.
*/
namespace PicesInterface {

	public ref class Class1
	{
		// TODO: Add your methods for this class here.
	};
}

#endif
