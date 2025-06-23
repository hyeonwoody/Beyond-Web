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

void FileBridgeTask::execute() {
    return;
}