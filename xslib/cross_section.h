#ifndef CROSS_SECTION_H__
#define CROSS_SECTION_H__

#include <string>
#include <vector>

#include "../api.h"
#include "../util/parseable.h"

#include "types.h"


namespace xs
{
    class CrossSection : public Parseable
    {
    protected:
        xstype xsType;
    public:
        virtual double computeAreaForDepth(double depth) = 0;
        virtual xstype getType() { return this->xsType; }
        virtual bool setParameters(std::vector<std::string>::const_iterator firstPart, std::vector<std::string>::const_iterator end) = 0;
    };

    class Factory
    {
    public:
        static CrossSection* create(xstype xsType);
        static CrossSection* create(const std::string& type);
    };
}


#endif//CROSS_SECTION_H__