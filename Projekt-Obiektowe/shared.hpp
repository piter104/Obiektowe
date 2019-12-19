#ifndef Shared_hpp
#define Shared_hpp

#include <functional>

struct BindingString {
    std::string name;
    std::string help;
    std::function<void()> func;
};

class Backend;

class Tool {
public:
    
    virtual void setEntry(std::string field, std::string value) = 0;
    virtual std::string getEntry(std::string field) = 0;
    virtual void init() = 0;
    virtual void redraw() = 0;
    Backend* backend;
};

class Backend {
public:
    Tool* tool;
    virtual void bind(std::string str, std::function<void()> func, std::string help) = 0;
    virtual void start() = 0;
};

#endif
