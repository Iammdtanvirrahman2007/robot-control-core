#pragma once

#include <string>

class Robot {
public:
    explicit Robot(std::string id);

    const std::string& id() const;

private:
    std::string id_;
};
