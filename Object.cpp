//
// Created by minirop on 29/11/17.
//
#include <iostream>
#include "Object.h"
#include "LoxCallable.h"
#include "LoxInstance.h"
#include "LoxFunction.h"

Object::Object()
    : Object(nullptr)
{
}

Object::Object(int i)
    : data { new ObjectVar(static_cast<double>(i)) }
{
}

Object::Object(double d)
    : data { new ObjectVar(d) }
{
}

Object::Object(bool b)
    : data { new ObjectVar(b) }
{
}

Object::Object(std::nullptr_t)
    : data { new ObjectVar(nullptr) }
{
}

Object::Object(std::string string)
    : data { new ObjectVar(string) }
{
}

Object::Object(std::shared_ptr<LoxFunction> function)
    : data { new ObjectVar(std::move(function)) }
{
}

Object::Object(std::shared_ptr<LoxClass> klass)
    : data { new ObjectVar(std::move(klass)) }
{
}

Object::Object(std::shared_ptr<LoxInstance> instance)
    : data { new ObjectVar(std::move(instance)) }
{
}

bool Object::isDouble() const
{
    return std::holds_alternative<double>(*data);
}

bool Object::isNull() const
{
    return std::holds_alternative<std::nullptr_t>(*data);
}

bool Object::isString() const
{
    return std::holds_alternative<std::string>(*data);
}

bool Object::isBool() const
{
    return std::holds_alternative<bool>(*data);
}

bool Object::isCallable() const
{
    return isFunction() || isClass();
}

bool Object::isInstance() const
{
    return std::holds_alternative<std::shared_ptr<LoxInstance>>(*data);
}

bool Object::isFunction() const
{
    return std::holds_alternative<std::shared_ptr<LoxFunction>>(*data);
}

bool Object::isClass() const
{
    return std::holds_alternative<std::shared_ptr<LoxClass>>(*data);
}

int Object::index() const
{
    return data->index();
}

double Object::asDouble() const
{
    return std::get<double>(*data);
}

bool Object::asBool() const
{
    return std::get<bool>(*data);
}

std::string Object::asString() const
{
    return std::get<std::string>(*data);
}

LoxCallable * Object::asCallable() const
{
    if (isFunction())
    {
        return std::get<std::shared_ptr<LoxFunction>>(*data).get();
    }
    else if (isClass())
    {
        return std::get<std::shared_ptr<LoxClass>>(*data).get();
    }
    else
    {
        throw "LOL";
    }
}

LoxInstance * Object::asInstance() const
{
    return std::get<std::shared_ptr<LoxInstance>>(*data).get();
}

std::string to_string(const Object & object)
{
    std::string ret;

    if (object.isDouble())
    {
        ret = std::to_string(object.asDouble());
    }
    else if (object.isCallable())
    {
        ret = object.asCallable()->getName();
    }
    else if (object.isBool())
    {
        ret = std::to_string(object.asBool());
    }
    else if (object.isNull())
    {
        ret = "nil";
    }
    else if (object.isString())
    {
        ret = object.asString();
    }
    else if (object.isInstance())
    {
        ret = to_string(object.asInstance()->getClass()) + " instance";
    }
    else
    {
        ret = "Unknown index: " + std::to_string(object.index());
    }

    return ret;
}
