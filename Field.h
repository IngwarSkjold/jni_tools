//
// Created by Ingwar on 06.07.2019.
//

#ifndef TESTAPP_FIELD_H
#define TESTAPP_FIELD_H

#include <jni.h>
#include "Signature.h"

template<typename Name, typename _Type>
struct JField {
    using Type = _Type;

    static constexpr auto name = Name::name;
    static constexpr auto signature = typeSignature<Type>();

    using JavaType = typename Type::JavaType;
    using CppType = typename Type::CppType;

    Type type;

    jfieldID fieldId;

    JField(JNIEnv *env, jclass clazz) {
        fieldId = env->GetFieldID(clazz, name.c_str(), signature.c_str());
    }

    JavaType toJava(JNIEnv* env, const CppType & cppValue) {
        return type.toJava(env, cppValue);
    }

    JavaType getJava(JNIEnv *env, jobject object) {
        if constexpr (std::is_same<JavaType, jboolean>::value) {
            return env->GetBooleanField(object, fieldId);
        } else if constexpr (std::is_same<JavaType, jint>::value) {
            return env->GetIntField(object, fieldId);
        } else if constexpr (std::is_same<JavaType, jlong>::value) {
            return env->GetLongField(object, fieldId);
        } else if constexpr (std::is_same<JavaType, jbyte>::value) {
            return env->GetByteField(object, fieldId);
        } else if constexpr (std::is_same<JavaType, jshort>::value) {
            return env->GetShortField(object, fieldId);
        } else if constexpr (std::is_same<JavaType, jfloat>::value) {
            return env->GetFloatField(object, fieldId);
        } else if constexpr (std::is_same<JavaType, jdouble>::value) {
            return env->GetDoubleField(object, fieldId);
        } else {
            return (JavaType)env->GetObjectField(object, fieldId);
        }
    }
        CppType getCpp(JNIEnv *env, jobject object) {
        return type.toCpp(env, getJava(env, object));
    }
};

#define DECLARE_FIELD(Name, Type) namespace details { struct _field##Name { static constexpr auto name = BOOST_HANA_STRING("##Name"); }; } \
                                  using Name = JField<details::_field##Name, Type>;

#endif //TESTAPP_FIELD_H
