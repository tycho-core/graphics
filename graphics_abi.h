//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2013 Martin Slater
// Created : Sunday, 10 November 2013 05:10:31 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef _TEMPLATE_ABI_H_5174B17E_ECA3_47F9_9BBA_E634B8353F65
#define _TEMPLATE_ABI_H_5174B17E_ECA3_47F9_9BBA_E634B8353F65

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "core/core_abi.h"

#if TYCHO_PC

// DLL interface
#ifdef TYCHO_GRAPHICS_EXPORTS
#define TYCHO_GRAPHICS_ABI __declspec(dllexport)
#else
#define TYCHO_GRAPHICS_ABI __declspec(dllimport)
#endif 

// disable a few warnings globally. should move these into specific cpp's to avoid polluting
// user header files
#pragma warning(disable : 4251) // class needs to have dll-interface to be used by clients of class ''
#pragma warning(disable : 4355) // 'this' : used in base member initializer list

#else // TYCHO_PC

#define TYCHO_GRAPHICS_ABI

#endif // TYCHO_GC

#ifdef __cplusplus
#include "core/memory/new.h"
#include "core/memory.h"
#include "core/debug/assert.h"
#endif 

#endif // _TEMPLATE_ABI_H_5174B17E_ECA3_47F9_9BBA_E634B8353F65
