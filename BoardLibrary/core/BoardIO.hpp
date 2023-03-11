#pragma once

#include <iosfwd>
#include "core/Board.hpp"

std::ostream& operator<<(std::ostream&, const Board&) noexcept;
