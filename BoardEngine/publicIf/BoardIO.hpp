#pragma once

#include <iosfwd>
struct Board;

std::ostream& operator<<(std::ostream&, const Board&) noexcept;

