/* BaseFilter class
   Used to generate filters which are
   effectively functions with parameters
   that can be applied to an image.
   It may choose to permanently alter the image
   or store the result in another location
*/
#include "basefilter.h"

BaseFilter::BaseFilter(Store* st, const std::string& nm) :
    imageStore(st),
    filterName(nm),
    enabled(false)
{
}

std::vector<Param*> BaseFilter::params() const
{
    return _params;
}

std::vector<Param*> BaseFilter::images() const
{
    return _images;
}

const std::string BaseFilter::name() const
{
    return filterName;
}

bool BaseFilter::isEnabled() const
{
    return enabled;
}

void BaseFilter::enable()
{
    enabled = true;
}

void BaseFilter::disable()
{
    enabled = false;
}
