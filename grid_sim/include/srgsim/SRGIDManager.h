#pragma once

#include <essentials/Identifier.h>

#include <mutex>
#include <unordered_set>

namespace srgsim
{
class SRGIDManager
{
public:
    SRGIDManager();
    virtual ~SRGIDManager();

    const essentials::Identifier* getIDFromBytes(const std::vector<uint8_t>& vectorID);

    const essentials::Identifier* generateID(int intID);
    const essentials::Identifier* generateID();
    bool addID(const essentials::Identifier* id);

private:
    std::unordered_set<const essentials::Identifier*, essentials::IdentifierHash, essentials::IdentifierEqualsComparator> ids;
    std::mutex mutex;
    int objectCounter;
};

} // namespace srgsim
