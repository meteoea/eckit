/*
 * (C) Copyright 1996-2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef SPMetadata_H
#define SPMetadata_H


namespace eckit {

template<class Traits>
struct SPMetadata {

    typedef typename Traits::Point   Point;

    Point  offset_;
    Point  scale_;
};


} // end namespace


#endif
