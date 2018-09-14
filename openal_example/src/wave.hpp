#pragma once
#include <cstdint>
#include <string>
#include <fstream>

class WaveFormat
{
public:
    WaveFormat();

    bool load(const std::string& fname);

    std::size_t loadBuffer(void* buf, std::size_t sz);
    std::size_t waveSize() const;
    std::uint16_t numChannels() const;
    std::uint32_t sampleRate() const;
    std::uint16_t bitsPerSample() const;
private:

    struct
    {
        std::uint8_t chunkID[4];        //! "RIFF"
        std::uint32_t chunkSize;        //! Размер файла - 8 байт
        std::uint8_t format[4];         //! "WAVE"
    } header_ __attribute__((packed));

    struct
    {
        std::uint8_t subChunkID[4];     //! "fmt "
        std::uint32_t subChunkSize;     //!
        std::uint16_t audioFormat;      //! 1 is PCM
        std::uint16_t numChannels;      //! Количество каналов
        std::uint32_t sampleRate;
        std::uint32_t byteRate;
        std::uint16_t blockAlign;
        std::uint16_t bitsPerSample;
        std::uint16_t extSize;
    } fmt_ __attribute__((packed));

    struct
    {
        std::uint8_t subChunkID[4];
        std::uint32_t subChunkSize;
    } data_ __attribute__((packed));

    std::size_t dataPosition_;
    std::ifstream file_;
};
