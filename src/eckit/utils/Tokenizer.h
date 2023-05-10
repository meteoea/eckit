/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @author Baudouin Raoult
/// @author Manuel Fuentes
/// @author Tiago Quintino

#ifndef eckit_Tokenizer_h
#define eckit_Tokenizer_h

#include <set>
#include <string>
#include <vector>

#include "eckit/memory/NonCopyable.h"

namespace eckit {

//---------------------------------------------------------------------------------------------------------------------

class Tokenizer : private NonCopyable {

public:  // methods
    Tokenizer(char, bool keepEmpty = false);
    Tokenizer(const std::string&, bool keepEmpty = false);

    ~Tokenizer();

    void operator()(const std::string&, std::vector<std::string>&) const;
    void operator()(std::istream&, std::vector<std::string>&) const;

    void operator()(const std::string&, std::set<std::string>&) const;
    void operator()(std::istream&, std::set<std::string>&) const;

    std::vector<std::string> tokenize(const std::string&) const;
    std::vector<std::string> tokenize(std::istream&) const;

private:
    std::set<char, std::less<char> > separator_;  // To make searching faster
    bool keepEmpty_;

private:
    void print(std::ostream&) const;

    friend std::ostream& operator<<(std::ostream& s, const Tokenizer& p) {
        p.print(s);
        return s;
    }
};

//---------------------------------------------------------------------------------------------------------------------

inline std::vector<std::string> Tokenizer::tokenize(const std::string& s) const {
    std::vector<std::string> r;
    this->operator()(s, r);
    return r;
}

inline std::vector<std::string> Tokenizer::tokenize(std::istream& s) const {
    std::vector<std::string> r;
    this->operator()(s, r);
    return r;
}

//---------------------------------------------------------------------------------------------------------------------

}  // namespace eckit

#endif
