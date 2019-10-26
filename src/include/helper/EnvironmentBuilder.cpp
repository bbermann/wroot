#include "EnvironmentBuilder.hpp"

EnvironmentBuilder::EnvironmentBuilder() {
}

EnvironmentBuilder::~EnvironmentBuilder() {
}

void EnvironmentBuilder::setPair(String key, String value) {
    environment_[key] = value;
}

String EnvironmentBuilder::toString() {
    String envString;
    for (auto pair : environment_) {
        envString.append(pair.first + "=\"" + pair.second + "\" ");
    }
    return envString;
}