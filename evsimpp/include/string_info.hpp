#pragma once

#include <string>

namespace evsim {

    class StringInfo
    {
    public:
        StringInfo(std::string name);;
        StringInfo(unsigned long hash, std::string name);

    public:
        virtual operator unsigned long() const;
        virtual operator std::string() const;
        virtual bool operator==(const StringInfo& rhs) const;
        virtual void hash(const std::string& str);
    public:
        std::string m_name;
        unsigned long m_hash;
    };
}
