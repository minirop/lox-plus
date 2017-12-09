//
// Created by minirop on 04/12/17.
//

#ifndef LOXPLUS_OBJECTVAR_H
#define LOXPLUS_OBJECTVAR_H

using ObjectVar = std::variant<std::nullptr_t, std::string, double, bool, std::shared_ptr<LoxFunction>, std::shared_ptr<LoxClass>, std::shared_ptr<LoxInstance>>;

#endif //LOXPLUS_OBJECTVAR_H
