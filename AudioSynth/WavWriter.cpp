#include "WavWriter.h"

#include <fstream>

void writeUint16LE(std::ofstream& file, std::uint16_t value)
{
	const char bytes[2] = {
		static_cast<char>(value & 0xFF),
		static_cast<char>((value >> 8) & 0xFF)
	};

	file.write(bytes, 2);
}

void writeUint32LE(std::ofstream& file, std::uint32_t value)
{
	const char bytes[4] = {
		static_cast<char>(value & 0xFF),
		static_cast<char>((value >> 8) & 0xFF),
		static_cast<char>((value >> 16) & 0xFF),
		static_cast<char>((value >> 24) & 0xFF)
	};

	file.write(bytes, 4);
}
//
//
//--WAV writing function
//
//
bool writeWavFile(
	const std::string& filename,
	const std::vector<std::int16_t>& samples,
	std::uint32_t sampleRate)
{
	constexpr std::uint16_t channelCount = 1;
	constexpr std::uint16_t bitsPerSample = 16;

	const std::uint16_t blockAlign =
		channelCount * bitsPerSample / 8;

	const std::uint32_t byteRate =
		sampleRate * blockAlign;

	const std::uint32_t dataSize =
		static_cast<std::uint32_t>(
			samples.size() * sizeof(std::int16_t)
			);

	std::ofstream file(filename, std::ios::binary);

	if (!file)
	{
		return false;
	}

	file.write("RIFF", 4);
	writeUint32LE(file, 36 + dataSize);
	file.write("WAVE", 4);

	file.write("fmt ", 4);
	writeUint32LE(file, 16);
	writeUint16LE(file, 1);

	writeUint16LE(file, channelCount);
	writeUint32LE(file, sampleRate);
	writeUint32LE(file, byteRate);
	writeUint16LE(file, blockAlign);
	writeUint16LE(file, bitsPerSample);

	file.write("data", 4);
	writeUint32LE(file, dataSize);

	for (const std::int16_t sample : samples)
	{
		writeUint16LE(
			file,
			static_cast<std::uint16_t>(sample)
		);
	}

	file.close();
	return file.good();
}