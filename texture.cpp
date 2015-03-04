//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 6:58:28 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "texture.h"
#include "forward_decls.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_GRAPHICS_ABI, texture);

	/// constructor
	texture::texture(graphics::renderer* r, int width, int height, int num_mips) :
		m_width(width),
		m_height(height),
		m_num_mips(num_mips)
	{}


	/// \returns the underlying storage for this texture
	graphics::texture_storage_ptr texture::get_texture_storage()
	{ 
		return m_storage; 
	}
		
	/// set the underlying storage for this texture
	void texture::set_storage(graphics::texture_storage_ptr store) 
	{ 
		m_storage = store; 
	}
		
	/// \returns the width of the texture
	int texture::get_width() const
	{
		return m_width;
	}
	
	/// \returns the height of the texture
	int texture::get_height() const
	{
		return m_height;
	}
	
	/// \returns the number of mip levels in the texture
	int texture::get_num_mips() const
	{
		return m_num_mips;
	}


} // end namespace
} // end namespace
