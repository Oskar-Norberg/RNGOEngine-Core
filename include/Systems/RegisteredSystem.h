//
// Created by Oskar.Norberg on 2025-06-19.
//

#pragma once

#include <string>
#include <memory>
#include <vector>

class ISystem;

class RegisteredSystem
{
public:
    explicit RegisteredSystem(std::unique_ptr<ISystem> system, std::string name, std::vector<std::string> before = {}, std::vector<std::string> after = {})
        : system(std::move(system)), name(std::move(name)), before(std::move(before)), after(std::move(after))
    {
    }

    std::unique_ptr<ISystem> system;

    std::string name;
    std::vector<std::string> before;
    std::vector<std::string> after;
};
