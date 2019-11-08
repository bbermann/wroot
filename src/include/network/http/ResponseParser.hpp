#pragma once

#ifndef WROOT_RESPONSEPARSER_HPP
#define WROOT_RESPONSEPARSER_HPP

#include <string>
#include <istream>
#include <include/network/http/Response.hpp>
#include <include/type/String.hpp>

class ResponseParser {
public:
    ResponseParser() = delete;

    ResponseParser(const ResponseParser &) = delete;

    ~ResponseParser() = delete;

    static Response fromStream(std::istream &input) {
        Response response;
        std::string row;

        enum State {
            Invalid = 0,
            FirstRow,
            HeaderRow,
            BodyRow,
        };

        State state = State::FirstRow;

        while (state != State::Invalid && std::getline(input, row)) {
            switch ((int) state) {
                // HTTP/1.0 404 Not Found
                case State::FirstRow: {
                    auto parts = String::explode(row, " ");

                    if (parts.size() < 3) {
                        state = State::Invalid;
                        break;
                    }

                    auto httpVersion = parseHttpVersion(parts.at(0));
                    response.httpVersionMajor = std::get<0>(httpVersion);
                    response.httpVersionMinor = std::get<1>(httpVersion);

                    response.status = static_cast<Response::StatusType>(std::stoi(parts.at(1)));

                    state = State::HeaderRow;
                    break;
                }

                    // Content-Type: text/html; charset=UTF-8
                case State::HeaderRow: {
                    if (row.empty() || row == "\r") {
                        std::cout << "indo p BodyRow" << std::endl;
                        state = State::BodyRow;
                        break;
                    }

                    std::cout << "headerrow" << std::endl;

                    auto parts = String::explode(row, ":");

                    if (parts.size() < 2) {
                        std::cout << "invalid: " << row << std::endl;
                        state = State::Invalid;
                        break;
                    }

                    // If the case below is broken in more than two parts, we should join the parts accidentally broken.
                    // [[Example]] Date: Thu, 07 Nov 2019 23:02:14 GMT
                    if (parts.size() > 2) {
                        for (auto i = 2; i < parts.size(); i++) {
                            parts.at(1).append(":").append(parts.at(i));
                        }
                    }

                    response.headers.insert({parts.at(0), parts.at(1)});

                    break;
                }

                    // <!DOCTYPE html>
                case State::BodyRow: {
                    response.content.append(row).append("\n");
                    break;
                }
            }
        }

        if (state == State::Invalid) {
            throw std::runtime_error("ResponseParser could not parse response from stream.");
        }

        std::cout << state << std::endl;

        return response;
    }

    static std::tuple<int, int> parseHttpVersion(const std::string &input) {
        auto split = String::explode(input, "/");

        if (split.size() != 2 || split.at(0) != "HTTP") {
            throw std::runtime_error("Invalid HTTP header.");
        }

        auto versionParts = String::explode(split.at(1), ".");

        if (versionParts.size() != 2) {
            throw std::runtime_error("Invalid HTTP version.");
        }

        return std::make_tuple<int, int>(
                std::stoi(versionParts.at(0)),
                std::stoi(versionParts.at(1))
        );
    }
};

#endif //WROOT_RESPONSEPARSER_HPP
