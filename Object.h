//
// Created by minirop on 04/11/17.
//

#ifndef LOXPLUS_OBJECT_H
#define LOXPLUS_OBJECT_H

#include <string>
#include <variant>
#include <memory>

class LoxFunction;
class LoxClass;
class LoxInstance;
class LoxCallable;

class Object
{
    using ObjectVar = std::variant<std::nullptr_t, std::string, double, bool, LoxFunction*, LoxClass*, LoxInstance*>;

public:
    Object();
    Object(int i);
    Object(double d);
    Object(bool b);
    Object(std::nullptr_t);
    Object(std::string string);
    Object(LoxFunction * function);
    Object(LoxClass * klass);
    Object(LoxInstance * instance);

    template <typename T>
    Object(T*) = delete;

    Object(const Object &) = default;
    Object(Object &&) = default;

    Object & operator=(const Object &) = default;
    Object & operator=(Object &&) = default;

    ~Object() = default;

    bool isDouble() const;
    bool isNull() const;
    bool isString() const;
    bool isBool() const;
    bool isCallable() const;
    bool isInstance() const;
    bool isFunction() const;
    bool isClass() const;

    int index() const;

    double asDouble() const;
    bool asBool() const;
    std::string asString() const;
    LoxCallable* asCallable() const;
    LoxInstance* asInstance() const;

private:
    ObjectVar data;

    friend std::string to_string(const Object & object);
};

#endif //LOXPLUS_OBJECT_H
