#pragma once

#include <essentials/ID.h>

#include <mutex>
#include <unordered_set>

namespace srgsim
{
class SRGIDManager
{
public:
    SRGIDManager();
    virtual ~SRGIDManager();

    const essentials::ID* getIDFromBytes(const std::vector<uint8_t>& vectorID);

    const essentials::ID* generateID(int intID);
    const essentials::ID* generateID();
    bool addID(const essentials::ID* id);

private:
    std::unordered_set<const essentials::ID*, essentials::IDHash, essentials::IDEqualsComparator> ids;
    std::mutex mutex;
    int objectCounter;
};

} // namespace srgsim
