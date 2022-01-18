/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation
 * nor does it submit to any jurisdiction.
 */


#include "eckit/linalg/LinearAlgebra.h"

#include <map>

#include "eckit/eckit.h"
#include "eckit/linalg/BackendRegistry.h"
#include "eckit/thread/AutoLock.h"
#include "eckit/thread/Mutex.h"


namespace eckit {
namespace linalg {


//-----------------------------------------------------------------------------


namespace {


#ifdef eckit_HAVE_EIGEN
static const auto* defaultBackend = "eigen";
#else
static const auto* defaultBackend = "generic";
#endif

static pthread_once_t once                      = PTHREAD_ONCE_INIT;
static BackendRegistry<LinearAlgebra>* backends = nullptr;


static void init() {
    backends = new BackendRegistry<LinearAlgebra>(defaultBackend, "ECKIT_LINEAR_ALGEBRA_BACKEND");
}


}  // namespace


//-----------------------------------------------------------------------------


const LinearAlgebra& LinearAlgebra::backend() {
    pthread_once(&once, init);
    return backends->find();
}


const LinearAlgebra& LinearAlgebra::getBackend(const std::string& name) {
    pthread_once(&once, init);
    return backends->find(name);
}


bool LinearAlgebra::hasBackend(const std::string& name) {
    pthread_once(&once, init);
    return backends->has(name);
}


void LinearAlgebra::backend(const std::string& name) {
    pthread_once(&once, init);
    backends->backend(name);
}


std::ostream& LinearAlgebra::list(std::ostream& out) {
    pthread_once(&once, init);
    return backends->list(out);
}


LinearAlgebra::LinearAlgebra(const std::string& name) :
    name_(name) {
    pthread_once(&once, init);
    backends->add(name, this);
}


//-----------------------------------------------------------------------------


}  // namespace linalg
}  // namespace eckit
