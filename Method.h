//
// Created by Ingwar on 06.07.2019.
//

#ifndef TESTAPP_METHOD_H
#define TESTAPP_METHOD_H

#include "Signature.h"
#include <jni.h>

template<typename Return, typename ...Args>
struct Method {
    using ResultJavaType = typename Return::JavaType;
    using ResultCppType = typename Return::CppType;

    static constexpr auto signature =
            BOOST_HANA_STRING("(") + (typeSignature<Args>() + ... + BOOST_HANA_STRING("")) + BOOST_HANA_STRING(")") +
            typeSignature<Return>();

    jmethodID methodID;

    Method(JNIEnv *env, jclass clazz, const char* name) :
    methodID(env->GetMethodID(clazz, name, signature.c_str())){

    }

    template<typename  ...JavaArguments>
    ResultJavaType call(JNIEnv *env, jobject object, JavaArguments ... args) {
        if constexpr (std::is_same<ResultJavaType, void>::value) {
            return env->CallVoidMethod(object, methodID, std::forward<JavaArguments>(args)...);
        } else if constexpr (std::is_same<ResultJavaType, jboolean>::value) {
            return env->CallBooleanMethod(object, methodID, std::forward<JavaArguments>(args)...);
        } else if constexpr (std::is_same<ResultJavaType, jint>::value) {
            return env->CallIntMethod(object, methodID, std::forward<JavaArguments>(args)...);
        } else if constexpr (std::is_same<ResultJavaType, jlong>::value) {
            return env->CallLongMethod(object, methodID, std::forward<JavaArguments>(args)...);
        } else if constexpr (std::is_same<ResultJavaType, jbyte>::value) {
            return env->CallByteMethod(object, methodID, std::forward<JavaArguments>(args)...);
        } else if constexpr (std::is_same<ResultJavaType, jshort>::value) {
            return env->CallShortMethod(object, methodID, std::forward<JavaArguments>(args)...);
        } else if constexpr (std::is_same<ResultJavaType, jfloat>::value) {
            return env->CallFloatMethod(object, methodID, std::forward<JavaArguments>(args)...);
        } else if constexpr (std::is_same<ResultJavaType, jdouble>::value) {
            return env->CallDoubleMethod(object, methodID, std::forward<JavaArguments>(args)...);
        } else {
            return env->CallObjectMethod(object, methodID, std::forward<JavaArguments>(args)...);
        }
    }
};

#endif //TESTAPP_METHOD_H
