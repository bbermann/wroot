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
                    {"htm",  "text/html"},
                    {"html", "text/html"},
                    {"gif",  "image/gif"},
                    {"jpg",  "image/jpeg"},
                    {"png",  "image/png"},
                    {"js", "text/javascript"},
                    {"css", "text/css"}
            };

    std::string extensionToType(const std::string &extension) {
        for (const Mapping &mapping : mappings) {
            if (mapping.extension == extension) {
                return mapping.mimeType;
            }
        }

        return "text/plain";
    }
}