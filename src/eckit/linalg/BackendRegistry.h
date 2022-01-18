/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation
 * nor does it submit to any jurisdiction.
 */


#include <cstdlib>
#include <map>

#include "eckit/exception/Exceptions.h"
#include "eckit/log/Log.h"
#include "eckit/thread/AutoLock.h"
#include "eckit/thread/Mutex.h"


namespace eckit {
namespace linalg {


template <typename LA>
class BackendRegistry {
public:
    BackendRegistry(const char* default_backend, const char* env_var) :
        default_(default_backend) {
        auto* envBackend = ::getenv(env_var);
        if (envBackend) {
            default_ = envBackend;
        }
    }

    void backend(const std::string& name) {
        AutoLock<Mutex> lock(mutex_);

        if (map_.find(name) == map_.end()) {
            throw BadParameter("Invalid backend " + name, Here());
        }
        default_ = name;
    }

    bool has(const std::string& name) const {
        AutoLock<Mutex> lock(mutex_);

        return map_.find(name) != map_.end();
    }


    const LA& find() const {
        return find(default_);
    }

    const LA& find(const std::string& name) const {
        AutoLock<Mutex> lock(mutex_);

        auto it = map_.find(name);
        if (it == map_.end()) {
            auto msg = "No backend named [" + name + "].";
            list(Log::error() << msg + " Backends are: ") << std::endl;
            throw BadParameter(msg, Here());
        }

        Log::info() << "Using backend " << it->first << std::endl;
        return *(it->second);
    }

    std::ostream& list(std::ostream& out) const {
        AutoLock<Mutex> lock(mutex_);

        const auto* sep = "";
        for (auto backend : map_) {
            out << sep << backend.first;
            sep = ", ";
        }

        return out;
    }

    void add(const std::string& name, LA* backend) {
        AutoLock<Mutex> lock(mutex_);

        ASSERT(map_.find(name) == map_.end());
        map_[name] = backend;
    }

private:  // members
    std::map<std::string, const LA*> map_;
    std::string default_;
    mutable Mutex mutex_;
};


}  // namespace linalg
}  // namespace eckit
