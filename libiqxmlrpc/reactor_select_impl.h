//  Libiqxmlrpc - an object-oriented XML-RPC solution.
//  Copyright (C) 2004-2006 Anton Dedov
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
//  $Id: reactor_select_impl.h,v 1.4 2006-09-07 04:45:21 adedov Exp $

#ifndef _iqxmlrpc_reactor_select_impl_h_
#define _iqxmlrpc_reactor_select_impl_h_

#ifndef HAVE_POLL
#include "reactor.h"

#include <boost/utility.hpp>

namespace iqnet
{

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#endif

//! Reactor implementation helper based on select() system call.
class LIBIQXMLRPC_API Reactor_select_impl: boost::noncopyable {
  Socket::Handler max_fd;
  fd_set read_set, write_set, err_set;
  Reactor_base::HandlerStateList hs;

public:
  Reactor_select_impl();
  virtual ~Reactor_select_impl();

  void reset(const Reactor_base::HandlerStateList&);
  bool poll(Reactor_base::HandlerStateList& out, Reactor_base::Timeout);
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

} // namespace iqnet

#endif // not defined HAVE_POLL
#endif
