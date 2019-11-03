//
// Created by bbermann on 10/10/2019.
//

#pragma once

#include <tuple>
#include <include/core/Core.hpp>
#include <include/type/String.hpp>
#include "KeyValuePair.hpp"
#include "Request.hpp"

struct Request;

class RequestParser {
public:
    /// Construct ready to parse the request method.
    RequestParser();

    /// Reset to initial parser state.
    void reset();

    /// Result of parse.
    enum ResultType {
        Good, Bad, Indeterminate
    };

    /// Parse some data. The enum return value is good when a complete request has
    /// been parsed, bad if the data is invalid, indeterminate when more data is
    /// required. The InputIterator return value indicates how much of the input
    /// has been consumed.
    template<typename InputIterator>
    std::tuple<ResultType, InputIterator> parse(Request &request, InputIterator begin, InputIterator end) {
        while (begin != end) {
            ResultType result = consume(request, *begin++);

            if (result == ResultType::Bad) {
                return std::make_tuple(result, begin);
            }

            if (result == ResultType::Good) {
                if (String::contains(request.uri, "?") && !String::endsWith(request.uri, "?")) {
                    // Parse request.uri arguments into request.body
                    StringList exploded = String::explode(request.uri, "?");
                    String args = exploded.back();

                    parseArguments(request, args);
                }

                if (begin != end) {
                    // Parse the request body into request.body
                    parseArguments(request, std::string(begin, end));
                }

                return std::make_tuple(result, begin);
            }
        }

        return std::make_tuple(ResultType::Indeterminate, begin);
    }

    void parseArguments(Request &request, const std::string &argumentsString) const {
        try {
            for (const auto &pair : String::explode(argumentsString, "&")) {
                StringList keyValue = String::explode(pair, "=");

                // If the value is empty, the key is returned by end() as the value
                request.body[keyValue.front()] = keyValue.back();
            }
        } catch (const std::exception &exception) {
            Core::warning(String("Failed parsing request body data: ") + exception.what());
        }
    }

private:
    /// Handle the next character of input.
    ResultType consume(Request &request, char input);

    /// Check if a byte is an HTTP character.
    static inline bool isChar(int c);

    /// Check if a byte is an HTTP control character.
    static inline bool isCtl(int c);

    /// Check if a byte is defined as an HTTP tspecial character.
    static inline bool isTSpecial(int c);

    /// Check if a byte is a digit.
    static inline bool isDigit(int c);

    /// The current state of the parser.
    enum State {
        MethodStart,
        Method,
        Uri,
        HttpVersionH,
        HttpVersionT1,
        HttpVersionT2,
        HttpVersionP,
        HttpVersionSlash,
        HttpVersionMajorStart,
        HttpVersionMajor,
        HttpVersionMinorStart,
        HttpVersionMinor,
        ExpectingNewline1,
        HeaderLineStart,
        HeaderLws,
        HeaderName,
        SpaceBeforeHeaderValue,
        HeaderValue,
        ExpectingNewline2,
        ExpectingNewline3,
        OptionalBodyData
    } state_;
};

