#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP

#include <AL/al.h>
#include <AL/alc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <unordered_map>

class AudioManager
{
public:
    AudioManager()
    {
        device = nullptr;
        context = nullptr;
    }

    ~AudioManager()
    {
        cleanupOpenAL();
    }

    bool initOpenAL()
    {
        device = alcOpenDevice(NULL);
        if (!device)
        {
            printf("Erro ao abrir dispositivo OpenAL.\n");
            return false;
        }

        context = alcCreateContext(device, NULL);
        if (!context)
        {
            printf("Erro ao criar contexto OpenAL.\n");
            alcCloseDevice(device);
            return false;
        }
        alcMakeContextCurrent(context);
        printf("OpenAL inicializado com sucesso.\n");

        return true;
    }

    void cleanupOpenAL()
    {
        alcMakeContextCurrent(NULL);
        alcDestroyContext(context);
        alcCloseDevice(device);
        printf("Recursos OpenAL liberados.\n");
    }

    bool loadWavFile(const char *filename, int id)
    {
        FILE *file = fopen(filename, "rb");
        if (!file)
        {
            printf("Erro ao abrir arquivo WAV: %s\n", filename);
            return false;
        }
        char riffHeader[4];
        fread(riffHeader, sizeof(char), 4, file);
        if (strncmp(riffHeader, "RIFF", 4) != 0)
        {
            printf("Erro: Arquivo não é um RIFF válido.\n");
            fclose(file);
            return false;
        }
        fseek(file, 8, SEEK_SET);
        char waveHeader[4];
        fread(waveHeader, sizeof(char), 4, file);
        if (strncmp(waveHeader, "WAVE", 4) != 0)
        {
            printf("Erro: Arquivo não é um WAV válido.\n");
            fclose(file);
            return false;
        }
        char fmtHeader[4];
        while (fread(fmtHeader, sizeof(char), 4, file))
        {
            if (strncmp(fmtHeader, "fmt ", 4) == 0)
                break;
            fseek(file, -3, SEEK_CUR); 
        }
        int fmtChunkSize;
        fread(&fmtChunkSize, sizeof(int), 1, file);
        short audioFormat, numChannels;
        fread(&audioFormat, sizeof(short), 1, file);
        fread(&numChannels, sizeof(short), 1, file);
        int sampleRate;
        fread(&sampleRate, sizeof(int), 1, file);
        fseek(file, 6, SEEK_CUR);
        short bitsPerSample;
        fread(&bitsPerSample, sizeof(short), 1, file);
        if (audioFormat != 1)
        {
            printf("Erro: Formato de áudio não suportado (apenas PCM linear).\n");
            fclose(file);
            return false;
        }
        char dataHeader[4];
        while (fread(dataHeader, sizeof(char), 4, file))
        {
            if (strncmp(dataHeader, "data", 4) == 0)
                break;
            fseek(file, -3, SEEK_CUR);
        }
        int dataSize;
        fread(&dataSize, sizeof(int), 1, file);
        short *data = (short *)malloc(dataSize);
        fread(data, sizeof(char), dataSize, file);
        fclose(file);
        ALenum format;
        if (numChannels == 1)
        {
            format = (bitsPerSample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
        }
        else if (numChannels == 2)
        {
            format = (bitsPerSample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
        }
        else
        {
            printf("Erro: Número de canais não suportado.\n");
            free(data);
            return false;
        }
        ALuint buffer;
        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, data, dataSize, sampleRate);
        free(data);
        ALenum error = alGetError();
        if (error != AL_NO_ERROR)
        {
            printf("Erro ao carregar buffer de áudio: %d\n", error);
            alDeleteBuffers(1, &buffer);
            return false;
        }

        buffers[id] = buffer;
        printf("Áudio carregado com sucesso: %s\n", filename);
        printf("Canais: %d, Taxa de Amostragem: %d Hz, Tamanho: %d bytes\n", numChannels, sampleRate, dataSize);

        return true;
    }

    void playAudio(int id)
    {
        if (buffers.find(id) == buffers.end())
        {
            printf("Áudio com ID %d não encontrado.\n", id);
            return;
        }

        if (sources.find(id) != sources.end())
        {
            ALuint source = sources[id];
            alSourceStop(source);
            alSourcePlay(source);
            return;
        }
        ALuint source;
        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, buffers[id]);
        alSourcePlay(source);
        sources[id] = source;
        printf("Reprodução do áudio %d iniciada.\n", id);
    }

    void playAudioRepeter(int id)
    {
        if (buffers.find(id) == buffers.end())
        {
            printf("Áudio com ID %d não encontrado.\n", id);
            return;
        }
        ALuint source = 0;
        for (auto it = activeSources.begin(); it != activeSources.end(); ++it)
        {
            ALint state;
            alGetSourcei(*it, AL_SOURCE_STATE, &state);
            if (state == AL_STOPPED)
            {
                source = *it; 
                break;
            }
        }
        if (source == 0 && activeSources.size() < MAX_SOURCES)
        {
            alGenSources(1, &source);
            activeSources.push_back(source);
        }
        if (source == 0)
        {
            printf("Limite de fontes atingido. Som descartado.\n");
            return;
        }
        alSourcei(source, AL_BUFFER, buffers[id]);
        alSourceStop(source);
        alSourcePlay(source);
    }


    void stopAudio(int id)
    {
        if (sources.find(id) == sources.end())
        {
            printf("Fonte de áudio com ID %d não encontrada.\n", id);
            return;
        }
        ALuint source = sources[id];
        alSourceStop(source);
        cleanupSource(source);
        sources.erase(id);
        buffers.erase(id);
        printf("Reprodução do áudio %d parada e recursos liberados.\n", id);
    }


    bool isAudioPlaying(int id)
    {
        if (sources.find(id) == sources.end())
        {
            printf("Fonte de áudio com ID %d não encontrada.\n", id);
            return false;
        }

        ALint state;
        alGetSourcei(sources[id], AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }


    void stopAllAudio()
    {
        for (auto &source : sources)
        {
            alSourceStop(source.second);
            cleanupSource(source.second);
        }

        sources.clear();
        buffers.clear();
        printf("Todos os áudios foram parados e recursos liberados.\n");
    }

private:
    ALCdevice *device;
    ALCcontext *context;
    std::unordered_map<int, ALuint> buffers;
    std::vector<ALuint> activeSources;       // Pool de fontes ativas
    const int MAX_SOURCES = 20;              // Mapeia IDs para buffers
    std::unordered_map<int, ALuint> sources; // Mapeia IDs para fontes

    void cleanupSource(ALuint source)
    {
        alDeleteSources(1, &source);
    }
};

#endif // AUDIOMANAGER_HPP
