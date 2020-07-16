#pragma once
#include "plight/include/common/string.h"

#include "plight/include/sound/sound_buffer_io.h"

#include <future>
#include <unordered_map>


struct ALCcontext;
struct ALCdevice;

namespace Plight::Sound
{
    /*
        Creates, manages and plays sounds
    */
    class SoundManager final
    {
    public:

        explicit                SoundManager(size_t = 32,
                                             size_t = 3,
                                             size_t = 441000);
                                ~SoundManager();

        void                    reset();

        void                    createSound(String const&, bool = false);

        void                    playSound(String const&);

        void                    update();

    private:

        /*
            Source data for managing an OpenAL-internal source and its dedicated buffers
        */
        struct SourceData
        {
            // OpenAL-internal id of the audio source
            unsigned int        m_sourceId = 0;

            // OpenAL-internal ids of buffers that are bound to the audio source
            std::vector<unsigned int> m_bufferIds;
        };

        /*
            Sound status
        */
        enum class ESoundStatus
        {
            Stopped,
            Playing,
            Paused
        };

        /*
            Sound data for managing the buffers of a sound
        */
        struct SoundData
        {
                                SoundData(String const&,
                                          size_t);

            // The current status of the sound
            ESoundStatus        m_status = ESoundStatus::Stopped;

            // Data of the source where the sound is played
            SourceData const*   m_pSourceData = nullptr;

            // Buffer IO object for loading the sound
            SoundBufferIO       m_soundBufferIO;

            // Index of the buffer (in SourceData::m_bufferIds) that will be filled next
            size_t              m_nextBufferIndex = 0;

            // Future for buffer data that is loaded asynchronously
            std::future<std::vector<int16_t>> m_futureBufferData;
        };

        void                    initialize();
        void                    cleanUp();

        void                    preloadBuffers(SoundData&);
        void                    queueNextBuffer(SoundData&);
        void                    queueBuffer(SoundData&,
                                            std::vector<int16_t>&&);
        void                    loadFutureBufferData(SoundData&);

        // Global audio device
        static ALCdevice*       sm_pDevice;

        // Global audio context
        static ALCcontext*      sm_pContext;

        // All data of managed audio sources
        std::vector<SourceData> m_sourceData;

        // Map between sound names and data
        std::unordered_map<std::string, SoundData> m_soundMap;

        // Number of audio sources
        size_t const            m_sourceCount;

        // Number of buffers per audio source
        size_t const            m_buffersPerSource;

        // Size of each buffer in bytes
        size_t const            m_bufferByteSize;

        // Index of the next source (in m_sourceData) that will be used next
        size_t                  m_nextSourceIndex = 0;

    };
}
