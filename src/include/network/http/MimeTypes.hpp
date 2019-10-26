//
// Created by bbermann on 10/10/2019.
//

#pragma once

#include <string>

namespace MimeTypes {
    /// Convert a file extension into a MIME type.
    std::string extensionToType(const std::string &extension);
}