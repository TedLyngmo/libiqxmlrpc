//  Libiqnet + Libiqxmlrpc - an object-oriented XML-RPC solution.
//  Copyright (C) 2004 Anton Dedov
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
//  
//  $Id: utf_conv.cc,v 1.2 2004-10-03 16:33:13 adedov Exp $

#include <errno.h>
#include "utf_conv.h"

using namespace iqxmlrpc;


Utf_conv* Utf_conv::from_utf( const std::string& to )
{
  return new Utf_conv( true, to );
}


Utf_conv* Utf_conv::to_utf( const std::string& from )
{
  return new Utf_conv( false, from );
}


Utf_conv::Utf_conv( bool from_utf, const std::string& enc )
{
  cd = iconv_open( from_utf ? enc.c_str() : "utf-8", 
                   from_utf ? "utf-8" : enc.c_str() );
  
  if( cd == (iconv_t)-1 )
    throw Unknown_charset_conversion( enc );
}


Utf_conv::~Utf_conv()
{
  iconv_close( cd );
}


namespace 
{
  class auto_iconv_arg {
    char* p1;
    char* p2;
    
  public:
    auto_iconv_arg( char* p_ ): p1(p_), p2(p_) {}
    ~auto_iconv_arg()
    {
      delete[] p1;
    }  
  
    char* ptr1()
    {
      return p1;
    }
    
    char** ptr2()
    {
      return &p2;
    }    
  };
};

std::string Utf_conv::convert( const std::string& s, unsigned max_sym_len )
{
  size_t ilen  = s.length();
  size_t olen  = ilen*max_sym_len;
  size_t ileft = ilen;
  size_t oleft = olen; 
  
  char* ibuf = const_cast<char*>(s.c_str());
  auto_iconv_arg obuf( new char[olen] );

  int code = iconv( cd, &ibuf, &ileft, obuf.ptr2(), &oleft );
  if( code == (size_t)-1 )
    throw Charset_conversion_failed();
    
  return std::string( obuf.ptr1(), olen - oleft );
}