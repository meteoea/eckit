/*
 * (C) Copyright 1996-2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/maths/Reduce.h"
#include "eckit/maths/List.h"

namespace eckit {
namespace maths {

//--------------------------------------------------------------------------------------------

Reduce::Reduce( ExpPtr f,  ExpPtr list ) : Function()
{
    push_back(f);
    push_back(list);
}

Reduce::Reduce( args_t& a ) : Function(a)
{
    ASSERT( a.size() == 2 );
}

std::string Reduce::returnSignature() const
{
    return Undef::sig(); /// @todo review this -- it could be the signature iterated over the list
}

ExpPtr Reduce::evaluate( Scope &ctx ) const
{
    ExpPtr f = args(0, ctx, false);

    const List::value_t& list = List::extract( ctx, args(1, ctx, true) );

    const size_t nlist = list.size();

    if(!nlist) // empty list
        return ExpPtr( new List() );

    ExpPtr e = list[0]->eval(ctx);

    for( size_t i = 1; i < nlist; ++i )
    {
//        DBGX(*e);
        e = f->eval( e, list[i]->eval(ctx) );
    }

    return e->eval(ctx);
}

void Reduce::asCode(std::ostream&o) const
{
    o << "maths::reduce("; printArgs(o); o <<")";
}

ExpPtr Reduce::cloneWith(args_t& a) const
{
    return ExpPtr( new Reduce(a) );
}
//--------------------------------------------------------------------------------------------

ExpPtr reduce( ExpPtr f,  ExpPtr list )
{
    return ExpPtr( new Reduce(f,list) );
}

//--------------------------------------------------------------------------------------------

} // namespace maths
} // namespace eckit
