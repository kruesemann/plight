#include "plight/include/sound/sound_manager.h"

#include "openAl/include/al.h"
#include "openAl/include/alc.h"

#include <fstream>
#include <iostream>
#include <vector>


namespace Plight::Sound
{
    ALCdevice* SoundManager::sm_pDevice = nullptr;
    ALCcontext* SoundManager::sm_pContext = nullptr;

    /*

    */
    void
    printAlError()
    {
        switch (alGetError())
        {
        case AL_INVALID_NAME:
            std::cout << "AL error: AL_INVALID_NAME\n";
            throw;
        case AL_INVALID_ENUM:
            std::cout << "AL error: AL_INVALID_ENUM\n";
            throw;
        case AL_INVALID_VALUE:
            std::cout << "AL error: AL_INVALID_VALUE\n";
            throw;
        case AL_INVALID_OPERATION:
            std::cout << "AL error: AL_INVALID_OPERATION\n";
            throw;
        case AL_OUT_OF_MEMORY:
            std::cout << "AL error: AL_OUT_OF_MEMORY\n";
            throw;
        default:
            break;
        }
    }

    //-----------------------------------------------
    // SoundManager::SoundData

    SoundManager::SoundData::SoundData(String const& rFilePath,
                                       size_t bufferByteSize)
        : m_soundBufferIO(rFilePath, bufferByteSize)
    {
    }

    //-----------------------------------------------
    // SoundManager

    SoundManager::SoundManager(size_t sourceCount,
                               size_t buffersPerSource,
                               size_t bufferByteSize)
        : m_sourceCount(sourceCount)
        , m_buffersPerSource(buffersPerSource)
        , m_bufferByteSize(bufferByteSize)
    {
        if (m_buffersPerSource < 2)
            throw std::exception("Sound error: Audio sources need at least 2 buffers");

        initialize();
    }

    SoundManager::~SoundManager()
    {
        cleanUp();
    }

    /*
        Deletes all created sounds
    */
    void
    SoundManager::reset()
    {
        cleanUp();
        initialize();
    }

    /*
        Setup OpenAL, generate sources and buffers
    */
    void
    SoundManager::initialize()
    {
        // Setup audio device and context if necessary
        if (!sm_pDevice)
        {
            sm_pDevice = alcOpenDevice(0);
            printAlError();
            if (!sm_pDevice)
                throw std::exception("Sound error: Unable to open sound device");

            sm_pContext = alcCreateContext(sm_pDevice, 0);
            printAlError();
            alcMakeContextCurrent(sm_pContext);
            printAlError();
        }

        // Setup sources and their dedicated buffers
        m_sourceData.reserve(m_sourceCount);
        for (size_t i = 0; i < m_sourceCount; ++i)
        {
            auto& rSourceData = m_sourceData.emplace_back();
            rSourceData.m_bufferIds.resize(m_buffersPerSource);
            alGenBuffers(static_cast<ALsizei>(rSourceData.m_bufferIds.size()), rSourceData.m_bufferIds.data());
            printAlError();
            alGenSources(1, &rSourceData.m_sourceId);
            printAlError();

            // Deactivate looping (looping is done by SoundManager)
            alSourcei(rSourceData.m_sourceId, AL_LOOPING, false);
            printAlError();
        }
    }

    /*
        Delete all sources and buffers, clear sound and source data
    */
    void
    SoundManager::cleanUp()
    {
        for (auto const& rSourceData : m_sourceData)
        {
            alDeleteSources(1, &rSourceData.m_sourceId);
            printAlError();
            alDeleteBuffers(static_cast<ALsizei>(rSourceData.m_bufferIds.size()), rSourceData.m_bufferIds.data());
            printAlError();
        }
        m_sourceData.clear();
        m_soundMap.clear();
    }

    /*
        Create a sound identified by given name
    */
    void
    SoundManager::createSound(String const& rName, bool loopEnabled)
    {
        if (m_nextSourceIndex >= m_sourceCount)
            throw std::exception(String("Sound error: Unable to create sound '%'. Buffers full").arg(rName).c_str());

        auto const [itSoundData, ok] = m_soundMap.emplace(rName,
                                                          SoundData(String("resource/%.wav").arg(rName),
                                                                    m_bufferByteSize));

        if (!ok)
            throw std::exception(String("Sound error: Unable to create sound '%'. Already exists").arg(rName).c_str());

        itSoundData->second.m_pSourceData = &m_sourceData[m_nextSourceIndex];
        itSoundData->second.m_soundBufferIO.setLoopEnabled(loopEnabled);
        preloadBuffers(itSoundData->second);

        ++m_nextSourceIndex;
    }

    /*
        Preload the buffers (not necessarily the complete sound file)
    */
    void
    SoundManager::preloadBuffers(SoundData& rSoundData)
    {
        queueNextBuffer(rSoundData);
        for (size_t i = 1; i < m_buffersPerSource - 1 && (!rSoundData.m_soundBufferIO.hasReachedEnd() || rSoundData.m_soundBufferIO.isLoopEnabled()); ++i)
            queueNextBuffer(rSoundData);

        if (!rSoundData.m_soundBufferIO.hasReachedEnd() || rSoundData.m_soundBufferIO.isLoopEnabled())
            loadFutureBufferData(rSoundData);
    }

    /*
        Load the next data chunk into a buffer and queue it
    */
    void
    SoundManager::queueNextBuffer(SoundData& rSoundData)
    {
        queueBuffer(rSoundData, rSoundData.m_soundBufferIO.readNextBuffer());
    }

    /*
        Load given data chunk into a buffer and queue it
    */
    void
    SoundManager::queueBuffer(SoundData& rSoundData,
                              std::vector<int16_t>&& rBufferData)
    {
        if (!rSoundData.m_pSourceData)
            return;

        alBufferData(rSoundData.m_pSourceData->m_bufferIds[rSoundData.m_nextBufferIndex],
                     AL_FORMAT_STEREO16,
                     rBufferData.data(),
                     static_cast<ALsizei>(rBufferData.size() * sizeof(int16_t)),
                     rSoundData.m_soundBufferIO.getSampleRate());
        printAlError();

        alSourceQueueBuffers(rSoundData.m_pSourceData->m_sourceId, 1, &rSoundData.m_pSourceData->m_bufferIds[rSoundData.m_nextBufferIndex]);
        printAlError();
        rSoundData.m_nextBufferIndex = (rSoundData.m_nextBufferIndex + 1) % m_buffersPerSource;
    }

    /*
        Load the next data chunk asynchronously
    */
    void
    SoundManager::loadFutureBufferData(SoundData& rSoundData)
    {
        rSoundData.m_futureBufferData = std::async([&rSoundData]()
                                                   {
                                                       return rSoundData.m_soundBufferIO.readNextBuffer();
                                                   });
    }

    /*
        Try to play sound with given name
    */
    void
    SoundManager::playSound(String const& rName)
    {
        auto const itSound = m_soundMap.find(rName);
        if (itSound == m_soundMap.end())
            throw std::exception(String("Sound error: Unable to find sound '%'").arg(rName).c_str());

        if (!itSound->second.m_pSourceData)
            return;

        int state = 0;
        alGetSourcei(itSound->second.m_pSourceData->m_sourceId, AL_SOURCE_STATE, &state);
        printAlError();
        if (state == AL_PLAYING)
            return;

        alSourcePlay(itSound->second.m_pSourceData->m_sourceId);
        printAlError();

        itSound->second.m_status = ESoundStatus::Playing;
    }

    /*
        Update the buffers of all currently playing sounds
    */
    void
    SoundManager::update()
    {
        for (auto& rSoundData : m_soundMap)
        {
            if (rSoundData.second.m_status != ESoundStatus::Playing ||
                !rSoundData.second.m_pSourceData)
                continue;

            // Find buffers to dequeue
            int buffersProcessed = 0;
            alGetSourcei(rSoundData.second.m_pSourceData->m_sourceId, AL_BUFFERS_PROCESSED, &buffersProcessed);
            printAlError();

            if (buffersProcessed > 0)
            {
                // Dequeue processed buffers
                std::vector<unsigned int> unqueuedBufferIds(buffersProcessed, 0);
                alSourceUnqueueBuffers(rSoundData.second.m_pSourceData->m_sourceId, buffersProcessed, unqueuedBufferIds.data());
                printAlError();

                // Load the asynchronously loaded data chunk into a buffer and queue the buffer
                if (rSoundData.second.m_futureBufferData.valid())
                    queueBuffer(rSoundData.second, rSoundData.second.m_futureBufferData.get());

                if (rSoundData.second.m_soundBufferIO.hasReachedEnd() &&
                    !rSoundData.second.m_soundBufferIO.isLoopEnabled())
                {
                    // If the complete sound has been loaded, check if there are still buffers queued ...
                    int buffersQueued = 0;
                    alGetSourcei(rSoundData.second.m_pSourceData->m_sourceId, AL_BUFFERS_QUEUED, &buffersQueued);
                    printAlError();

                    // ... If not, stop the playback ...
                    if (buffersQueued == 0)
                    {
                        alSourceStop(rSoundData.second.m_pSourceData->m_sourceId);
                        printAlError();

                        rSoundData.second.m_status = ESoundStatus::Stopped;

                        // ... and prepare for next playback
                        preloadBuffers(rSoundData.second);
                    }
                }
                else
                {
                    // If the sound is not completely loaded yet, load more buffers and queue them
                    for (int i = 0; i < buffersProcessed - 1 && (!rSoundData.second.m_soundBufferIO.hasReachedEnd() || rSoundData.second.m_soundBufferIO.isLoopEnabled()); ++i)
                        queueNextBuffer(rSoundData.second);
                    if (!rSoundData.second.m_soundBufferIO.hasReachedEnd() || rSoundData.second.m_soundBufferIO.isLoopEnabled())
                        loadFutureBufferData(rSoundData.second);
                }
            }
        }
    }
}
