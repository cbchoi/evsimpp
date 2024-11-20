#pragma once

#include <string>

#include "definition.hpp"
#include "object.hpp"

namespace evsim {

    class StringInfo:public CObject
    {
    public:
        StringInfo(std::string name);
        StringInfo(unsigned long hash, std::string name);
        virtual ~StringInfo();
    public:
        virtual operator unsigned long() const;
        virtual operator std::string() const;
        virtual bool operator==(const StringInfo& rhs) const;
        virtual void hash(const std::string& str);
    public:
        std::string m_name;
        unsigned long m_hash;

        static UNIQ OBJECT_ID;
    };
}
