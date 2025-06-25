#pragma once
#include "Bridge.h"
#include <fstream>
#include <filesystem>
#include <iostream>

extern "C" {
    #include "libavutil/mem.h"
}

namespace fs = std::filesystem;

typedef int StreamMemory;

class FileBridgeTask : public IBridge {
    private:
    FileBridgeTask(TaskType taskType) 
    : IBridge("FileBridgeTask",taskType) {}
    public:
        static FileBridgeTask* Create();
        void Transfer(std::ifstream& ifstream, std::ofstream& ofstream);
        void Symlink(const std::string& input, const std::string& output);
        void execute() override;
        StreamMemory* MallocArray(size_t cnt);
        void FreeArray(StreamMemory* memory);
};
