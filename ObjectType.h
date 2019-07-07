//
// Created by Ingwar on 06.07.2019.
//

#ifndef TESTAPP_OBJECTTYPE_H
#define TESTAPP_OBJECTTYPE_H


#include "PrimitiveTypes.h"
#include "Method.h"
#include <jni.h>


struct String {
    using CppType = std::string;
    using JavaType = jstring;

    static constexpr auto name = BOOST_HANA_STRING("java/lang/String");

    String(JNIEnv* env) { }

    CppType toCpp(JNIEnv *env, JavaType jValue) {
        return std::string(env->GetStringUTFChars(jValue, nullptr));
    }

    JavaType toJava(JNIEnv *env, const CppType &cValue) { return env->NewStringUTF(cValue.c_str()); }
};


template<typename _CppType, typename ClassName, typename ...Fields>
struct Object {
    using CppType = _CppType;
    using JavaType = jobject;

    static constexpr auto name = ClassName::name;

    jclass clazz;
    Method<Void, typename Fields::Type...> constructorMethod;
    std::tuple<Fields...> fields;

    Object(JNIEnv *env) :
            clazz((jclass) env->NewGlobalRef(env->FindClass(name.c_str()))),
            constructorMethod(env, clazz, "<init>"),
            fields(std::make_tuple(Fields(env, clazz) ...)) {
    }

    JavaType toJava(JNIEnv *env, const CppType &cppValue);

    CppType toCpp(JNIEnv *env, JavaType jValue);

    template<typename Field>
    typename Field::JavaType getJava(JNIEnv *env, jobject object) {
        return std::get<Field>(fields).getJava(env, object);
    }

    template<typename Field>
    typename Field::CppType getCpp(JNIEnv *env, jobject object) {
        return std::get<Field>(fields).getCpp(env, object);
    }

    template<typename Field>
    Field &field() {
        return std::get<Field>(fields);
    }
};


template<typename ContainedType>
struct ObjectArray {
    using ItemType = typename ContainedType::CppType;
    using CppType = std::vector<ItemType>;
    using JavaType = jobjectArray;

    static constexpr auto name = BOOST_HANA_STRING("[") + typeSignature<ContainedType>();

    ContainedType containedType;
    ObjectArray(JNIEnv* env) : containedType(env) {

    }

    CppType toCpp(JNIEnv *env, JavaType jValue) {
        auto result = CppType();
        auto size = env->GetArrayLength(jValue);
        result.reserve(size);
        for (auto i = 0; i < size; ++i) {
            result.emplace_back(containedType.toCpp(env, env->GetObjectArrayElement(jValue, i)));
        }
        return result;
    }

    JavaType toJava(JNIEnv *env, const CppType &cValue) {
        if (cValue.empty()) {
            return env->NewObjectArray(0, containedType.clazz, nullptr);
        }
        auto result = env->NewObjectArray(cValue.size(), containedType.clazz,
                                          containedType.toJava(env, cValue.front()));
        for (auto i = 1; i < cValue.size(); ++i) {
            const auto &cItem = cValue[i];
            auto jItem = containedType.toJava(env, cItem);
            env->DeleteLocalRef(jItem);
            env->SetObjectArrayElement(result, i, jItem);
        }
        return result;
    }
};

#endif //TESTAPP_OBJECTTYPE_H
