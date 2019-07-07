//
// Created by Ingwar on 06.07.2019.
//

#ifndef TESTAPP_SIGNATURE_H
#define TESTAPP_SIGNATURE_H

#include <boost/hana/string.hpp>

struct PrimitiveType {
};

template<typename Type>
static constexpr auto typeSignature() {
    if constexpr (std::is_base_of<PrimitiveType, Type>::value) {
        return Type::name;
    } else {
        return BOOST_HANA_STRING("L") + Type::name + BOOST_HANA_STRING(";");
    }
}

#endif //TESTAPP_SIGNATURE_H
