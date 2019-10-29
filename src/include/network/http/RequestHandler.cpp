//
// Created by bbermann on 10/10/2019.
//

#include "RequestHandler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include "MimeTypes.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include <include/core/Core.hpp>
#include <include/library/CustomLibrary.hpp>
#include <include/library/FileLibrary.hpp>
#include <include/library/EvoLibrary.hpp>
#include <memory>

RequestHandler::RequestHandler() = default;

void RequestHandler::handle(const Request &request, Response &response) {
    std::shared_ptr<CustomLibrary> lib = std::make_shared<EvoLibrary>(request);
    lib->handle(response);

//    std::shared_ptr<CustomLibrary> lib = std::make_shared<FileLibrary>(request);
//    lib->handle(response);
}