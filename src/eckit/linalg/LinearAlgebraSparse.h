/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation
 * nor does it submit to any jurisdiction.
 */


#pragma once

#include <iosfwd>
#include <string>

#include "eckit/linalg/types.h"


namespace eckit {
namespace linalg {


//-----------------------------------------------------------------------------


/// @deprecated Use LinearAlgebraDense/LinearAlgebraSparse instead
class LinearAlgebraSparse {
public:
    // - Constructors

    LinearAlgebraSparse() = default;

    // - Static methods

    /// Get the currently selected sparse backend
    static const LinearAlgebraSparse& backend();

    /// Select the given sparse backend as the default
    static void backend(const std::string& name);

    /// List all available backends
    static std::ostream& list(std::ostream&);

    /// Get a backend by name
    static const LinearAlgebraSparse& getBackend(const std::string& name);

    /// Check if a backend is available
    static bool hasBackend(const std::string& name);

    // - Methods

    /// Return active backend name
    const std::string& name() const {
        return name_;
    }

    /// Compute the product of a sparse matrix A and vector x
    /// @note y must be allocated and sized correctly
    virtual void spmv(const SparseMatrix& A, const Vector& x, Vector& y) const = 0;

    /// Compute the product of sparse matrix A and dense matrix X
    /// @note Y must be allocated and sized correctly
    virtual void spmm(const SparseMatrix& A, const Matrix& X, Matrix& Y) const = 0;

    /// Compute the product x A' y with x and y diagonal matrices stored as
    /// vectors and A a sparse matrix
    /// @note B does NOT need to be allocated/sized correctly
    virtual void dsptd(const Vector& x, const SparseMatrix& A, const Vector& y, SparseMatrix& B) const = 0;

protected:
    LinearAlgebraSparse(const std::string& name);

    virtual ~LinearAlgebraSparse() = default;

private:
    std::string name_;

    virtual void print(std::ostream&) const = 0;

    friend std::ostream& operator<<(std::ostream& s, const LinearAlgebraSparse& p) {
        p.print(s);
        return s;
    }
};


//-----------------------------------------------------------------------------


}  // namespace linalg
}  // namespace eckit
