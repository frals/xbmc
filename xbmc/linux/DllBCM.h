#pragma once
/*
 *      Copyright (C) 2005-2011 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#if defined(HAVE_LIBBCM_HOST)

#if (defined HAVE_CONFIG_H) && (!defined WIN32)
  #include "config.h"
#endif
#ifndef __GNUC__
#pragma warning(push)
#pragma warning(disable:4244)
#endif

extern "C" {
#include <bcm_host.h>
}

#include "DynamicDll.h"
#include "utils/log.h"

////////////////////////////////////////////////////////////////////////////////////////////

class DllBcmHostDisplayInterface
{
public:
  virtual ~DllBcmHostDisplayInterface() {}

  virtual DISPMANX_DISPLAY_HANDLE_T vc_dispmanx_display_open( uint32_t device ) = 0;
  virtual DISPMANX_UPDATE_HANDLE_T vc_dispmanx_update_start( int32_t priority ) = 0;
  virtual DISPMANX_ELEMENT_HANDLE_T vc_dispmanx_element_add ( DISPMANX_UPDATE_HANDLE_T update, DISPMANX_DISPLAY_HANDLE_T display,
                                                                              int32_t layer, const VC_RECT_T *dest_rect, DISPMANX_RESOURCE_HANDLE_T src,
                                                                              const VC_RECT_T *src_rect, DISPMANX_PROTECTION_T protection,
                                                                              VC_DISPMANX_ALPHA_T *alpha,
                                                                              DISPMANX_CLAMP_T *clamp, DISPMANX_TRANSFORM_T transform ) = 0;
  virtual int vc_dispmanx_update_submit_sync( DISPMANX_UPDATE_HANDLE_T update ) = 0;
  virtual int vc_dispmanx_element_remove( DISPMANX_UPDATE_HANDLE_T update, DISPMANX_ELEMENT_HANDLE_T element ) = 0;
  virtual int vc_dispmanx_display_close( DISPMANX_DISPLAY_HANDLE_T display ) = 0;
  virtual int vc_dispmanx_display_get_info( DISPMANX_DISPLAY_HANDLE_T display, DISPMANX_MODEINFO_T * pinfo ) = 0;
};

#if (defined USE_EXTERNAL_LIBBCM_HOST)
class DllBcmHostDisplay : public DllDynamic, DllBcmHostDisplayInterface
{
public:
  virtual DISPMANX_DISPLAY_HANDLE_T vc_dispmanx_display_open( uint32_t device )
     { return ::vc_dispmanx_display_open(device); };
  virtual DISPMANX_UPDATE_HANDLE_T vc_dispmanx_update_start( int32_t priority )
    { return ::vc_dispmanx_update_start(priority); };
  virtual DISPMANX_ELEMENT_HANDLE_T vc_dispmanx_element_add ( DISPMANX_UPDATE_HANDLE_T update, DISPMANX_DISPLAY_HANDLE_T display,
                                                                              int32_t layer, const VC_RECT_T *dest_rect, DISPMANX_RESOURCE_HANDLE_T src,
                                                                              const VC_RECT_T *src_rect, DISPMANX_PROTECTION_T protection,
                                                                              VC_DISPMANX_ALPHA_T *alpha,
                                                                              DISPMANX_CLAMP_T *clamp, DISPMANX_TRANSFORM_T transform )
    { return ::vc_dispmanx_element_add(update, display, layer, dest_rect, src, src_rect, protection, alpha, clamp, transform); };
  virtual int vc_dispmanx_update_submit_sync( DISPMANX_UPDATE_HANDLE_T update )
    { return ::vc_dispmanx_update_submit_sync(update); };
  virtual int vc_dispmanx_element_remove( DISPMANX_UPDATE_HANDLE_T update, DISPMANX_ELEMENT_HANDLE_T element )
    { return ::vc_dispmanx_element_remove(update, element); };
  virtual int vc_dispmanx_display_close( DISPMANX_DISPLAY_HANDLE_T display )
    { return ::vc_dispmanx_display_close(display); };
  virtual int vc_dispmanx_display_get_info( DISPMANX_DISPLAY_HANDLE_T display, DISPMANX_MODEINFO_T *pinfo )
    { return ::vc_dispmanx_display_get_info(display, pinfo); };
  virtual bool ResolveExports() 
    { return true; }
  virtual bool Load() 
  {
    CLog::Log(LOGDEBUG, "DllBcm: Using omx system library");
    return true;
  }
  virtual void Unload() {}
};
#else
class DllBcmHostDisplay : public DllDynamic, DllBcmHostDisplayInterface
{
  //DECLARE_DLL_WRAPPER(DllBcmHostDisplay, "/opt/vc/lib/libopenmaxil.so")
  DECLARE_DLL_WRAPPER(DllBcmHostDisplay, "/opt/vc/lib/libEGL.so")

  DEFINE_METHOD1(DISPMANX_DISPLAY_HANDLE_T, vc_dispmanx_display_open, (uint32_t p1 ))
  DEFINE_METHOD1(DISPMANX_UPDATE_HANDLE_T,  vc_dispmanx_update_start, (int32_t p1 ))
  DEFINE_METHOD10(DISPMANX_ELEMENT_HANDLE_T, vc_dispmanx_element_add, (DISPMANX_UPDATE_HANDLE_T p1, DISPMANX_DISPLAY_HANDLE_T p2,
                                                                              int32_t p3, const VC_RECT_T *p4, DISPMANX_RESOURCE_HANDLE_T p5,
                                                                              const VC_RECT_T *p6, DISPMANX_PROTECTION_T p7,
                                                                              VC_DISPMANX_ALPHA_T *p8,
                                                                              DISPMANX_CLAMP_T *p9, DISPMANX_TRANSFORM_T p10 ))
  DEFINE_METHOD1(int, vc_dispmanx_update_submit_sync, (DISPMANX_UPDATE_HANDLE_T p1))
  DEFINE_METHOD2(int, vc_dispmanx_element_remove, (DISPMANX_UPDATE_HANDLE_T p1, DISPMANX_ELEMENT_HANDLE_T p2))
  DEFINE_METHOD1(int, vc_dispmanx_display_close, (DISPMANX_DISPLAY_HANDLE_T p1))
  DEFINE_METHOD2(int, vc_dispmanx_display_get_info, (DISPMANX_DISPLAY_HANDLE_T p1, DISPMANX_MODEINFO_T *p2))
  BEGIN_METHOD_RESOLVE()
    RESOLVE_METHOD(vc_dispmanx_display_open)
    RESOLVE_METHOD(vc_dispmanx_update_start)
    RESOLVE_METHOD(vc_dispmanx_element_add)
    RESOLVE_METHOD(vc_dispmanx_update_submit_sync)
    RESOLVE_METHOD(vc_dispmanx_element_remove)
    RESOLVE_METHOD(vc_dispmanx_display_close)
    RESOLVE_METHOD(vc_dispmanx_display_get_info)
  END_METHOD_RESOLVE()

public:
  virtual bool Load()
  {
    return DllDynamic::Load();
  }
};
#endif

class DllBcmHostInterface
{
public:
  virtual ~DllBcmHostInterface() {}

  virtual void bcm_host_init() = 0;
  virtual void bcm_host_deinit() = 0;
  virtual int32_t graphics_get_display_size( const uint16_t display_number, uint32_t *width, uint32_t *height) = 0;
};

#if (defined USE_EXTERNAL_LIBBCM_HOST)
class DllBcmHost : public DllDynamic, DllBcmHostInterface
{
public:
  virtual void bcm_host_init()
    { return ::bcm_host_init(); };
  virtual void bcm_host_deinit()
    { return ::bcm_host_deinit(); };
  virtual int32_t graphics_get_display_size( const uint16_t display_number, uint32_t *width, uint32_t *height)
    { return ::graphics_get_display_size(display_number, width, height); };
  virtual bool ResolveExports() 
    { return true; }
  virtual bool Load() 
  {
    CLog::Log(LOGDEBUG, "DllBcm: Using omx system library");
    return true;
  }
  virtual void Unload() {}
};
#else
class DllBcmHost : public DllDynamic, DllBcmHostInterface
{
  DECLARE_DLL_WRAPPER(DllBcmHost, "/opt/vc/lib/libbcm_host.so")

  DEFINE_METHOD0(void,    bcm_host_init)
  DEFINE_METHOD0(void,    bcm_host_deinit)
  DEFINE_METHOD3(int32_t, graphics_get_display_size, (const uint16_t p1, uint32_t *p2, uint32_t *p3))
  BEGIN_METHOD_RESOLVE()
    RESOLVE_METHOD(bcm_host_init)
    RESOLVE_METHOD(bcm_host_deinit)
    RESOLVE_METHOD(graphics_get_display_size)
  END_METHOD_RESOLVE()

public:
  virtual bool Load()
  {
    return DllDynamic::Load();
  }
};
#endif

#endif