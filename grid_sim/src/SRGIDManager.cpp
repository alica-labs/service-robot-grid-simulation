#include "srgsim/SRGIDManager.h"

namespace srgsim
{
SRGIDManager::SRGIDManager()
        : objectCounter(0)
{
}
SRGIDManager::~SRGIDManager() {}

const essentials::ID* SRGIDManager::getIDFromBytes(const std::vector<uint8_t>& vectorID)
{
    if (vectorID.empty()) { // empty values result in none-id
        return nullptr;
    }
    // create tmpID for lookup the ID
    const essentials::ID* tmpID = new essentials::ID(vectorID.data(), vectorID.size());

    // make the manager thread-safe
    std::lock_guard<std::mutex> guard(mutex);

    // lookup the ID and insert it, if not available, yet
    auto entry = this->ids.insert(tmpID);
    if (!entry.second) { // delete tmpID if already present in agentIDs
        delete tmpID;
    }
    return *(entry.first);
}

const essentials::ID* SRGIDManager::generateID(int intID)
{
    // little-endian encoding
    std::vector<uint8_t> idByteVector;
    // TODO: replace with memcpy or std copy
    for (int i = 0; i < static_cast<int>(sizeof(intID)); i++) {
        idByteVector.push_back(*(((uint8_t*) &intID) + i));
    }
    return this->getIDFromBytes(idByteVector);
}

const essentials::ID* SRGIDManager::generateID()
{
    std::vector<uint8_t> idByteVector;
    for (int i = 0; i < static_cast<int>(sizeof(this->objectCounter)); i++) {
        idByteVector.push_back(*(((uint8_t*) &this->objectCounter) + i));
    }
    this->objectCounter++;
    return new essentials::ID(idByteVector.data(), idByteVector.size());
}

bool SRGIDManager::addID(const essentials::ID *id) {
    if(this->ids.find(id) == this->ids.end()) {
        this->ids.insert(id);
        return true;
    }
    return false;
}
} // namespace srgsim