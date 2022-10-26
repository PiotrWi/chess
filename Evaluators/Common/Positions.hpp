#pragma once

namespace matherial
{
constexpr unsigned int NFeatures = 5u;
}

namespace movecount
{
constexpr unsigned int NFeatures = 1u;
}

namespace dual_bishop
{
constexpr unsigned int NFeatures = 1u;
}

constexpr unsigned int NAllCoefficients = matherial::NFeatures + movecount::NFeatures + dual_bishop::NFeatures;

constexpr unsigned int NMaterialOffset = 0u;
constexpr unsigned int MoveCountOffset = NMaterialOffset + matherial::NFeatures;
constexpr unsigned int DualBishopOffset = MoveCountOffset + movecount::NFeatures;
