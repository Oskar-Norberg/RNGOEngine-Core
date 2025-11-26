//
// Created by Oskar.Norberg on 2025-09-05.
//

#pragma once

// # Vendors
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <entt/entt.hpp>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <magic_enum/magic_enum.hpp>

#include <spdlog/spdlog.h>

#include <stb_image.h>

#include <yaml-cpp/yaml.h>

// TODO: Should tracy always be included and just disabled through a define?

// # Standard Library
#include <chrono>

#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <span>

#include <optional>
#include <variant>
#include <any>

#include <string>
#include <string_view>

#include <memory>

#include <filesystem>
#include <limits>
#include <concepts>
