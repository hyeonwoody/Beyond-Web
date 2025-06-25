#pragma once
#include <string>

class IClass {
    protected:
        std::string className;
    protected:
    explicit IClass(const std::string& name) : className(name) {}
    public:
        virtual ~IClass() = default;
    protected:
        virtual const std::string& getClassName() const { return className; }
    };