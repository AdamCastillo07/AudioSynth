#pragma once

#include <cstdint>
#include <string>
#include <vector>

bool writeWavFile(
    const std::string& filename,
    const std::vector<std::int16_t>& samples,
    std::uint32_t sampleRate
);