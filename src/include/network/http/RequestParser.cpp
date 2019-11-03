//
// Created by bbermann on 10/10/2019.
//

#include "RequestParser.hpp"
#include "Request.hpp"

RequestParser::RequestParser(): state_(State::MethodStart) {
}

void RequestParser::reset() {
    this->state_ = State::MethodStart;
}

RequestParser::ResultType RequestParser::consume(Request &request, char input) {
    switch (state_) {
        case State::MethodStart:
            if (!isChar(input) || isCtl(input) || isTSpecial(input)) {
                return ResultType::Bad;
            } else {
                state_ = State::Method;
                request.method.push_back(input);
                return ResultType::Indeterminate;
            }
        case State::Method:
            if (input == ' ') {
                state_ = State::Uri;
                return ResultType::Indeterminate;
            } else if (!isChar(input) || isCtl(input) || isTSpecial(input)) {
                return ResultType::Bad;
            } else {
                request.method.push_back(input);
                return ResultType::Indeterminate;
            }
        case State::Uri:
            if (input == ' ') {
                state_ = State::HttpVersionH;
                return ResultType::Indeterminate;
            } else if (isCtl(input)) {
                return ResultType::Bad;
            } else {
                request.uri.push_back(input);
                return ResultType::Indeterminate;
            }
        case State::HttpVersionH:
            if (input == 'H') {
                state_ = State::HttpVersionT1;
                return ResultType::Indeterminate;
            } else {
                return ResultType::Bad;
            }
        case State::HttpVersionT1:
            if (input == 'T') {
                state_ = State::HttpVersionT2;
                return ResultType::Indeterminate;
            } else {
                return ResultType::Bad;
            }
        case State::HttpVersionT2:
            if (input == 'T') {
                state_ = State::HttpVersionP;
                return ResultType::Indeterminate;
            } else {
                return ResultType::Bad;
            }
        case State::HttpVersionP:
            if (input == 'P') {
                state_ = State::HttpVersionSlash;
                return ResultType::Indeterminate;
            } else {
                return ResultType::Bad;
            }
        case State::HttpVersionSlash:
            if (input == '/') {
                request.httpVersionMajor = 0;
                request.httpVersionMinor = 0;
                state_ = State::HttpVersionMajorStart;
                return ResultType::Indeterminate;
            } else {
                return ResultType::Bad;
            }
        case State::HttpVersionMajorStart:
            if (isDigit(input)) {
                request.httpVersionMajor = request.httpVersionMajor * 10 + input - '0';
                state_ = State::HttpVersionMajor;
                return ResultType::Indeterminate;
            } else {
                return ResultType::Bad;
            }
        case State::HttpVersionMajor:
            if (input == '.') {
                state_ = State::HttpVersionMinorStart;
                return ResultType::Indeterminate;
            } else if (isDigit(input)) {
                request.httpVersionMajor = request.httpVersionMajor * 10 + input - '0';
                return ResultType::Indeterminate;
            } else {
                return ResultType::Bad;
            }
        case State::HttpVersionMinorStart:
            if (isDigit(input)) {
                request.httpVersionMinor = request.httpVersionMinor * 10 + input - '0';
                state_ = State::HttpVersionMinor;
                return ResultType::Indeterminate;
            } else {
                return ResultType::Bad;
            }
        case State::HttpVersionMinor:
            if (input == '\r') {
                state_ = State::ExpectingNewline1;
                return ResultType::Indeterminate;
            } else if (isDigit(input)) {
                request.httpVersionMinor = request.httpVersionMinor * 10 + input - '0';
                return ResultType::Indeterminate;
            } else {
                return ResultType::Bad;
            }
        case State::ExpectingNewline1:
            if (input == '\n') {
                state_ = State::HeaderLineStart;
                return ResultType::Indeterminate;
            } else {
                return ResultType::Bad;
            }
        case State::HeaderLineStart:
            if (input == '\r') {
                state_ = State::ExpectingNewline3;
                return ResultType::Indeterminate;
            } else if (!request.headers.empty() && (input == ' ' || input == '\t')) {
                state_ = State::HeaderLws;
                return ResultType::Indeterminate;
            } else if (!isChar(input) || isCtl(input) || isTSpecial(input)) {
                return ResultType::Bad;
            } else {
                request.keyBuffer.push_back(input);
                state_ = State::HeaderName;
                return ResultType::Indeterminate;
            }
        case State::HeaderLws:
            if (input == '\r') {
                state_ = State::ExpectingNewline2;
                return ResultType::Indeterminate;
            } else if (input == ' ' || input == '\t') {
                return ResultType::Indeterminate;
            } else if (isCtl(input)) {
                return ResultType::Bad;
            } else {
                state_ = State::HeaderValue;
                request.valueBuffer.push_back(input);
                return ResultType::Indeterminate;
            }
        case State::HeaderName:
            if (input == ':') {
                state_ = State::SpaceBeforeHeaderValue;
                request.headers[request.keyBuffer] = "";
                return ResultType::Indeterminate;
            } else if (!isChar(input) || isCtl(input) || isTSpecial(input)) {
                return ResultType::Bad;
            } else {
                request.keyBuffer.push_back(input);
                return ResultType::Indeterminate;
            }
        case State::SpaceBeforeHeaderValue:
            if (input == ' ') {
                state_ = State::HeaderValue;
                return ResultType::Indeterminate;
            } else {
                return ResultType::Bad;
            }
        case State::HeaderValue:
            if (input == '\r') {
                state_ = State::ExpectingNewline2;
                // We clear this here since we may have or no another header
                request.keyBuffer.clear();
                request.valueBuffer.clear();
                return ResultType::Indeterminate;
            } else if (isCtl(input)) {
                return ResultType::Bad;
            } else {
                request.valueBuffer.push_back(input);
                return ResultType::Indeterminate;
            }
        case State::ExpectingNewline2:
            if (input == '\n') {
                state_ = State::HeaderLineStart;
                return ResultType::Indeterminate;
            } else {
                return ResultType::Bad;
            }
        case State::ExpectingNewline3:
            if (input == '\n') {
                return ResultType::Good;
            }

            return ResultType::Bad;
        default:
            return ResultType::Bad;
    }
}

bool RequestParser::isChar(int c) {
    return c >= 0 && c <= 127;
}

bool RequestParser::isCtl(int c) {
    return (c >= 0 && c <= 31) || (c == 127);
}

bool RequestParser::isTSpecial(int c) {
    switch (c) {
        case '(':
        case ')':
        case '<':
        case '>':
        case '@':
        case ',':
        case ';':
        case ':':
        case '\\':
        case '"':
        case '/':
        case '[':
        case ']':
        case '?':
        case '=':
        case '{':
        case '}':
        case ' ':
        case '\t':
            return true;
        default:
            return false;
    }
}

bool RequestParser::isDigit(int c) {
    return c >= '0' && c <= '9';
}