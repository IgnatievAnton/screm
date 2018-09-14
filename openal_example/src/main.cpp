#include <iostream>
#include <memory>

#include <AL/al.h>
#include <AL/alc.h>

#include "wave.hpp"

int main(int argc, char** argv)
{
    //! TODO: Добавить опции выбора wav файла
    if( argc != 2 )
    {
        std::cerr << "Usage: " << argv[0] << " <wav file>" << std::endl;
        return 1;
    }

    //! Открываем устройство воспроизведения
    ALCdevice* device = nullptr;
    device = alcOpenDevice(nullptr);
    if( !device )
    {
        std::cerr << "Failed open preffered device.\n";
        return 1;
    }

    std::cout << "Output device: " << alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;

    //! Получаем контекст и делаем его текущим
    ALCcontext* context = nullptr;
    context = alcCreateContext(device, nullptr);
    if( !alcMakeContextCurrent(context) )
    {
        std::cerr << "Failed make current context.\n";
        return 1;
    }
    //! Настраиваем ориентацию слушателя
    ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
    alListener3f( AL_POSITION, 0.0f, 0.0f, 1.0f );
    alListener3f( AL_VELOCITY, 0.0f, 0.0f, 0.0f );
    alListenerfv( AL_ORIENTATION, listenerOri );

    //! Формируем источник звука (на сцене)
    ALuint source;
    alGenSources( 1, &source );
    alSourcef( source, AL_PITCH, 1.0f );
    alSourcef( source, AL_GAIN, 1.0f );
    alSource3f( source, AL_POSITION, 0.0f, 0.0f, 0.0f );
    alSource3f( source, AL_VELOCITY, 0.0f, 0.0f, 0.0f );
    alSourcei( source, AL_LOOPING, AL_FALSE );

    //! Загружаем аудио в формате WAV
    WaveFormat wf;
    if( !wf.load( argv[1] ) )
    {
        std::cerr << "Failed load wave file: " << argv[1] << std::endl;
        return 1;
    }

    std::unique_ptr<char> data( new char[wf.waveSize()] );
    std::cout << "Wave size:  " << wf.waveSize() << " bytes.\n";
    std::cout << "Load bytes: " << wf.loadBuffer( data.get(), wf.waveSize() ) << std::endl;

    //! Отправляем загруженное аудио в буфер
    ALuint buffer;
    alGenBuffers( 1, &buffer );

    ALenum format = [&]()
    {
        switch (wf.bitsPerSample())
        {
        case 16:
            return wf.numChannels() > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
        case 8:
            return wf.numChannels() > 1 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
        }
        return AL_FORMAT_MONO8;
    } ();
    ALsizei freq = wf.sampleRate();
    std::cout << "Bits per sample: " << wf.bitsPerSample() << std::endl;
    std::cout << "Number channels: " << wf.numChannels() << std::endl;

    alBufferData( buffer, format, data.get(), wf.waveSize(), freq);

    alSourcei(source, AL_BUFFER, buffer);
    alSourcePlay(source);
    ALint source_state;
    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    while (source_state == AL_PLAYING) {
        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    }
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

    return 0;
}
