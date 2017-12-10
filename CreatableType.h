//
// Created by minirop on 10/12/17.
//

#ifndef LOXPLUS_CREATABLETYPE_H
#define LOXPLUS_CREATABLETYPE_H

#include <vector>
#include <memory>

template <typename T>
class CreatableType
{
public:
    template <typename... Args>
    static T* create(Args&&... args)
    {
        objects.emplace_back(new T { std::forward<Args>(args)... });
        return objects.back().get();
    }

private:
    inline static std::vector<std::unique_ptr<T>> objects;
};

#endif //LOXPLUS_CREATABLETYPE_H
