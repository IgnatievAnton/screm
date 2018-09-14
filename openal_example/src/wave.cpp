#include "wave.hpp"
#include <iostream>
#include <cstring>

//! Описание формата WAV
//! https://sites.google.com/site/musicgapi/technical-documents/wav-file-format

WaveFormat::WaveFormat()
{
    std::cout << "Size header: " << sizeof (header_) << std::endl;
    std::cout << "Size format: " << sizeof (fmt_) << std::endl;
    std::cout << "Size data  : " << sizeof (data_) << std::endl;
}

bool WaveFormat::load(const std::string &fname)
{
    file_.open( fname, std::ios_base::in | std::ios_base::binary );
    if( !file_.is_open() )
    {
        std::cerr << "Failed open '" << fname << "' file.\n";
        return false;
    }
    file_.seekg(0, std::ios_base::end);
    std::size_t file__size = file_.tellg();
    file_.seekg(0, std::ios_base::beg);
    std::cout << "file size: " << file__size << " byte(s).\n";

    file_.read(reinterpret_cast<char*>(&header_), sizeof(header_));
    file_.read(reinterpret_cast<char*>(&fmt_), sizeof(fmt_));
    file_.seekg( sizeof(header_) + fmt_.subChunkSize + 8 );
    file_.read(reinterpret_cast<char*>(&data_), sizeof(data_));

    while( std::memcmp(data_.subChunkID, "data", 4) != 0 && file_ )
    {
        std::cerr << std::string((char*)data_.subChunkID, 4) << " is not 'data'.\n";
        std::cerr << "Seek " << data_.subChunkSize << " bytes.\n";
        file_.seekg( data_.subChunkSize, std::ios_base::cur );
        file_.read(reinterpret_cast<char*>(&data_), sizeof(data_));
    }

    std::cout << "Check 'RIFF': " << (std::memcmp(header_.chunkID, "RIFF", 4) ? "Fail" : "OK" ) << std::endl;
    std::cout << "Check 'WAVE': " << (std::memcmp(header_.format, "WAVE", 4) ? "Fail" : "OK" ) << std::endl;
    std::cout << "Check 'fmt ': " << (std::memcmp(fmt_.subChunkID, "fmt ", 4) ? "Fail" : "OK" ) << std::endl;
    std::cout << "Check 'data': " << (std::memcmp(data_.subChunkID, "data", 4) ? "Fail" : "OK" ) << std::endl;

    dataPosition_ = file_.tellg();
    std::cout << "Data position: " << dataPosition_ << " (size: " << data_.subChunkSize << " bytes)" << std::endl;
    std::cout << "Check data size: " << ((data_.subChunkSize + dataPosition_) == file__size ? "OK" : "Fail") << std::endl;

    return true;
}

std::size_t WaveFormat::loadBuffer(void *buf, std::size_t sz)
{
    std::size_t lsz = sz > data_.subChunkSize ? data_.subChunkSize : sz;
    file_.seekg( dataPosition_, std::ios_base::beg );
    file_.read( (char*)buf, lsz );
    lsz = file_.tellg();
    return lsz - dataPosition_;
}

std::size_t WaveFormat::waveSize() const
{
    return data_.subChunkSize;
}

uint16_t WaveFormat::numChannels() const
{
    return fmt_.numChannels;
}

uint32_t WaveFormat::sampleRate() const
{
    return fmt_.sampleRate;
}

uint16_t WaveFormat::bitsPerSample() const
{
    return fmt_.bitsPerSample;
}
