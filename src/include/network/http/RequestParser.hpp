//
// Created by bbermann on 10/10/2019.
//

#pragma once

#include <tuple>

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

            if (result == ResultType::Good || result == ResultType::Bad) {
                return std::make_tuple(result, begin);
            }
        }

        return std::make_tuple(ResultType::Indeterminate, begin);
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
        ExpectingNewline3
    } state_;
};

