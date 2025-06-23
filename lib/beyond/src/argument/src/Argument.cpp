#include "Argument.h"



CArgumentMapper::CArgumentMapper() {}

CArgumentMapper* CArgumentMapper::Create() {
    CArgumentMapper* arg = new CArgumentMapper();
    arg->initOptions();
    arg->initFlags();
    
    return arg;
}

/**
 * When you add option, make sure you edit the OPTION_NUM in Argument.h
*/
void CArgumentMapper::initOptions() {
    this->options = new COption*[OPTION_NUM];
    int optionIdx = 0;
    this->options[optionIdx++] = COption::Create("-i,--input", "Input target", &optionVariables.inputUrl, optionIdx, true);
    this->options[optionIdx++] = COption::Create("-l,--loop", "-1: Infinite loop, 0: run once, positive numbers: Number of times to loop/retry", &optionVariables.loop, optionIdx, true);
    this->options[optionIdx++] = COption::Create("-o,--outputUrl", "Output URL including protocol", &optionVariables.outputUrl, optionIdx, true);
    this->options[optionIdx++] = COption::Create("-ss,--startOffset", "Start offset in second or hh:mm:ss for cut flag (-c)", &optionVariables.startOffset, optionIdx);
    this->options[optionIdx++] = COption::Create("-to,--endOffset", "End offset in second or hh:mm:ss for cut flag (-c)", &optionVariables.startOffset, optionIdx);
    this->options[optionIdx++] = COption::Create("-d,--duration", "Duration, used for cut flag (-c)", &optionVariables.outputUrl, optionIdx);
}

/**
 * When you add  flag, make sure you edit the FLAG_NUM in Argument.h
*/
void CArgumentMapper::initFlags() {
    this->flags = new CFlag*[OPTION_NUM];  // Allocate an array of SFlag pointers
    int flagIdx = 0;
    this->flags[flagIdx++] = CFlag::Create("-cp,--copy", "Copy from input_url (-i) to output_url (-o), usually the file protocol", &flagVariables.copy, flagIdx);
    this->flags[flagIdx++] = CFlag::Create("-mv,--move", "Move from input_url (-i) to output_url (-o), must be the file protocol", &flagVariables.move, flagIdx);
    this->flags[flagIdx++] = CFlag::Create("-st,--stream", "Stream from input_url (-i) to output_url (-o)", &flagVariables.stream, flagIdx);
    this->flags[flagIdx++] = CFlag::Create("-c,--cut", "Video cut with certain part, and duration", &flagVariables.cut, flagIdx);
    this->flags[flagIdx++] = CFlag::Create("-sy,--symbolic", "Create symbolic link from input_url (-i) to output_url (-o)", &flagVariables.cut, flagIdx);
}   

CArgumentMapper::~CArgumentMapper()
{
    for (int i = 0; i < OPTION_NUM; i++) {
        if (options[i]) {
            delete options[i];
        }
        
    }
    delete[] options;  

    for (int i = 0; i < FLAG_NUM; i++) {
        if (flags[i]) {
            delete flags[i];

        }
    }
    delete[] flags;
}

int CArgumentMapper::Parse(int argc, char** argv) {
    
    COption** options = this->GetOptions();
    CFlag** flags = this->GetFlags();
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.push_back(std::string(argv[i]));    
    }


    std::cout<<"🔊[CArgumentMapper::Parse] argc : "<<argc<<std::endl;
    for (int i=0; i < args.size(); ++i) {
        if (argv[i] == nullptr) {
            std::cerr << "🔊[CArgumentMapper::Parse] Parse error: argv[" << i << "] is nullptr" << std::endl;
            return false;
        }
        std::string& arg = args[i];
        if (parseFlags(arg)) {
            std::cout<<"🔊[CArgumentMapper::Parse] parsed : "<<arg<<std::endl;
            continue;
        }
        std::string& val = args[i+1];
        if (parseOptions(arg, val)) {
            i++;
        }
    }
    return 0;
}

bool CArgumentMapper::parseOptions(std::string arg, std::string val) {
    for (int j = 0; j < OPTION_NUM; j++) {
        if (options[j]->Compare(arg)) {
            options[j]->Assign(val);
            options[j]->used = true;
            return true;
        }
    }
    return false;
}

bool CArgumentMapper::parseFlags(std::string arg) {
    for (int j = 0; j < FLAG_NUM; j++) {
        if (flags[j]->Compare(arg)) {
            //flags[j]->Assign(val);
            flags[j]->used = true;  
            return true;
        }
    }
    return false;
} 

COption** CArgumentMapper::GetOptions() {
    return this->options;
}

CFlag** CArgumentMapper::GetFlags() {
    return this->flags;
}

const SOptionVariables* CArgumentMapper::GetOptionVariables() {
    return &this->optionVariables;
}

const SFlagVariables* CArgumentMapper::GetFlagVariables() {
    return &this->flagVariables;
}