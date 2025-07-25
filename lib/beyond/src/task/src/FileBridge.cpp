#include "FileBridge.h"

FileBridgeTask* FileBridgeTask::Create(){
    return new FileBridgeTask(TaskType::FILEBRIDGE);
};

void FileBridgeTask::Transfer(std::ifstream& ifstream, std::ofstream& ofstream) {
    ofstream << ifstream.rdbuf();
}

void FileBridgeTask::Symlink(const std::string& input, const std::string& output) {
    try {
        std::string symlinkFullName = fs::path(output).parent_path() / (fs::path(output).stem().string() + "_symlink" + fs::path(input).extension().string());
        std::error_code ec;
        fs::remove(symlinkFullName, ec);
        fs::create_symlink(input, symlinkFullName);
    } catch (const std::exception& e) {
        std::cerr << "⛔[FileBridgeTask::Symlink] Failed to create symbolic link: " << e.what() << "\n";
    }
}

StreamMemory* FileBridgeTask::MallocArray(size_t cnt) {
    return static_cast<StreamMemory*>(av_malloc_array(cnt, sizeof(StreamMemory)));
}

void FileBridgeTask::FreeArray(StreamMemory* memory) {
    av_free(memory);
}

void FileBridgeTask::execute() {
    return;
}