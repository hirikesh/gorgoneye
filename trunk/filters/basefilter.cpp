/* BaseFilter class
   Used to generate filters which are
   effectively functions with parameters
   that can be applied to an image.
   It may choose to permanently alter the image
   or store the result in another location
*/
#include "basefilter.h"

BaseFilter::BaseFilter(const std::string& nm, Store* st) :
        filterName(nm),
        imageStore(st)
{
}

std::vector<Param*> BaseFilter::params() const
{
        return filterParams;
}

const std::string BaseFilter::name() const
{
    return filterName;
}
