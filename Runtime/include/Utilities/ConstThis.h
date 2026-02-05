//
// Created by Oskar.Norberg on 2026-02-05.
//

#pragma once

#define CONST_THIS const_cast<std::remove_const_t<std::remove_reference_t<decltype(*this)>>&>(*this)