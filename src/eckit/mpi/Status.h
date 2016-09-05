/*
 * (C) Copyright 1996-2016 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef eckit_mpi_Status_h
#define eckit_mpi_Status_h

#include <iosfwd>

namespace eckit {
namespace mpi {

//----------------------------------------------------------------------------------------------------------------------

class StatusContent;

class Status {

public: // methods

    Status();

    ~Status();

    int source() const;
    int tag() const;
    int error() const;

private: // methods

    void print(std::ostream&) const;

    friend std::ostream& operator<<(std::ostream& s, const Status& o) {
        o.print(s); return s;
    }

private: // members

    StatusContent* pimpl_;
};

//----------------------------------------------------------------------------------------------------------------------

}  // namespace mpi
}  // namespace eckit

#endif
