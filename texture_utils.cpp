//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 18 May 2008 5:01:37 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics/texture_utils.h"
#include "graphics/renderer.h"
#include "io/memory_stream.h"
#include "io/file_stream.h"
#include "io/interface.h"
#include "core/globals.h"
#include "image/format_png.h"
#include "image/image.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{

	TYCHO_GRAPHICS_ABI graphics::texture_ptr create_texture_from_memory_png(graphics::renderer_ptr renderer, void* data, int len)
	{
		io::memory_stream istr((char*)data, len);
		return renderer->create_texture(image::format_png::load(istr));	
	}

	TYCHO_GRAPHICS_ABI graphics::texture_ptr create_texture_from_file_png(io::interface* io, graphics::renderer_ptr renderer, const char* path)
	{
		io::stream_ptr istr = io->open_stream(path, io::open_flag_read);
		if(!istr)
			return graphics::texture_ptr();
		return renderer->create_texture(image::format_png::load(*istr.get()));	
	}

} // end namespace
} // end namespace
