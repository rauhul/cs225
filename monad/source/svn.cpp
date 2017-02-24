#include <iostream>

#include "util.h"
#include "svn.h"

namespace svn {
    namespace internal {
        bool enabled = false;
    }
}

int svn::init(bool enabled) {
    svn::internal::enabled = enabled;
    if (!svn::internal::enabled) {
        return 0;
    }
    return 0;
}

int svn::checkout(std::string path) {
    if (!svn::internal::enabled) {
        std::cerr << "svn disabled" << std::endl;
        return 1;
    }
    return util::exec("svn", "co", path);
}

int svn::update(std::string path) {
    if (!svn::internal::enabled) {
        std::cerr << "svn disabled" << std::endl;
        return 1;
    }
    return util::exec("svn", "up", path);
}
