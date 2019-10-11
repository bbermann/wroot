//
// Created by bbermann on 10/10/2019.
//

#include "MimeTypes.hpp"

namespace MimeTypes {
    struct Mapping {
        const char *extension;
        const char *mimeType;
    } mappings[] =
            {
                    {"gif",  "image/gif"},
                    {"htm",  "text/html"},
                    {"html", "text/html"},
                    {"jpg",  "image/jpeg"},
                    {"png",  "image/png"}
            };

    std::string extensionToType(const std::string &extension) {
        for (Mapping mapping : mappings) {
            if (mapping.extension == extension) {
                return mapping.mimeType;
            }
        }

        return "text/plain";
    }
}