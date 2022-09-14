#pragma once

#include <stdint.h>
#include <xhash>
#include <random>

namespace Oak
{
    static std::random_device s_RandomDevice;
    static std::mt19937_64 eng(s_RandomDevice());
    static std::uniform_int_distribution<uint64_t> s_UniformDistribution;
    
    class UUID
    {
    public:

        UUID()                            : m_UUID(s_UniformDistribution(eng)) {}
        UUID(uint64_t uuid)               : m_UUID(uuid) {}
        //UUID(const UUID&  other)          : m_UUID(other.m_UUID) {}
        //UUID(      UUID&& other) noexcept : m_UUID(std::move(other.m_UUID)) {}

        operator       uint64_t()       { return m_UUID; };
        operator const uint64_t() const { return m_UUID; }

        //UUID& operator=(UUID& other) { m_UUID = other.m_UUID; return *this; }
        //const UUID& operator=(const UUID& other) { m_UUID = other.m_UUID;  return *this; }

        //UUID& operator=(UUID&& other) noexcept
        //{
        //    if (this != &other)
        //    {
        //        m_UUID = std::move(other.m_UUID);
        //        other.m_UUID;
        //    }
        //    return *this;
        //}

    private:
        uint64_t m_UUID;

    };

}
 
namespace std 
{
    // this allows uuid to be hashed and for example used as a key in a map
    template<>
    struct hash<Oak::UUID>
    {
        std::size_t operator()(const Oak::UUID& uuid) const
        {
            // uuid is already a randomly generated 64 bit int, and is suitable for a hash
            return uuid;
            //return hash<uint64_t>()((uint64_t)uuid);
        }
    };
 }