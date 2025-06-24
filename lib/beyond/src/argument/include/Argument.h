#pragma once
#include "StringParser.h"
#include "Kamsi.h"
#include <string>
#include <cstring>
#include <vector>
#define OPTION_NUM 6
#define FLAG_NUM 5

struct SOptionVariables{
std::string inputUrl;
    std::string outputUrl;
    std::string startOffset;
    std::string endOffset;
    int loop;
};

struct SFlagVariables {
    bool copy;
    bool move;
    bool stream;
    bool cut;
};

enum class ArgType { INT, STRING, NONE };

template <typename T>
class BaseArgument {
public:
    std::string shortName;
    std::vector<std::string> longNames;
    std::string description;
    void* ptr;
    ArgType type = ArgType::NONE;
    int index;
    bool essential;
    bool used;

    virtual ~BaseArgument() {
        if (!used && ptr) {
            switch (type) {
                case ArgType::INT:
                    delete static_cast<int*>(ptr);
                    break;
                case ArgType::STRING:
                    delete static_cast<std::string*>(ptr);
                    break;
                default:
                    break;
            }
            ptr = nullptr;
        }
    }

    static T* Create(const std::string& _name, 
        const std::string& _description, 
        void* _ptr, 
        int _index, 
        bool _essential = false) {
        
        std::vector<std::string> names = StringParser::Split(_name, ',');
        std::string shortName;
        std::vector<std::string> longNames;

        if (!names.empty()) {
            shortName = std::move(names[0]);
            longNames.insert(longNames.end(), names.begin() + 1, names.end());
        }

        T* current = new T();
        current->shortName = shortName;
        current->longNames = longNames;
        current->description = _description;
        current->ptr = _ptr;
        current->index = _index;
        current->essential = _essential;
        current->used = false;
        return current;
    }
    bool Compare(std::string arg) {
        if (shortName.compare(arg) == 0) {
            return true;
        }
        for (int i = 0; i < longNames.size(); ++i) {
            if (longNames[i].compare(arg) == 0) {
                return true;
            }
        }
        return false;
    }
    void Assign (std::string arg) {
        
        if (StringParser::IsNumber(arg)) {
            *((int*)ptr) = std::stoi(arg);
            type = ArgType::INT;
            return;
        }
        *((std::string*)ptr) = arg;
        type = ArgType::STRING;
    }
};

//Curiously Recurring Template Pattern (CRTP)
class COption : public BaseArgument<COption> {
    using BaseArgument<COption>::BaseArgument;
};

class CFlag : public BaseArgument<CFlag> {
    using BaseArgument<CFlag>::BaseArgument;
};



class CArgumentMapper
{
private:
    SOptionVariables optionVariables;
    SFlagVariables flagVariables;
    COption** options;
    CFlag** flags;
    Kamsi* kamsi;

    public:
        static CArgumentMapper* Create(); 
        ~CArgumentMapper();
        int Parse(int argc, char** argv);
        COption** GetOptions();
        CFlag** GetFlags();
        const SOptionVariables* GetOptionVariables();
        const SFlagVariables* GetFlagVariables();
    private:
        CArgumentMapper();
        void initOptions();
        void initFlags();
        bool parseOptions(std::string arg, std::string val);
        bool parseFlags(std::string arg);

        template<typename T> bool addOption(std::string nameName, T& variable, std::string optionDescription = "", bool essential = false);
        template<typename T>bool addFlag(std::string name, T& variable, std::string optionDescription = "", bool essential = false);
};