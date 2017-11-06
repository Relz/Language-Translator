
#ifndef LANGUAGETRANSLATOR_UNORDEREDMAPHASH_H
#define LANGUAGETRANSLATOR_UNORDEREDMAPHASH_H

#include <unordered_set>
#include <boost/functional/hash.hpp>

struct UnorderedSetHash {
    size_t operator()(const std::unordered_set<wchar_t>& unorderedSet) const {
	    return boost::hash_range(unorderedSet.begin(), unorderedSet.end());
    }
};

#endif //LANGUAGETRANSLATOR_UNORDEREDMAPHASH_H
