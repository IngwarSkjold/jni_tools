//
// Created by Ingwar on 06.07.2019.
//

#ifndef TESTAPP_PRIMITIVETYPES_H
#define TESTAPP_PRIMITIVETYPES_H

#include "Signature.h"
#include <jni.h>

struct Void : PrimitiveType {
    using CppType = void;
    using JavaType = void;

    static constexpr auto name = BOOST_HANA_STRING("V");

//        static CppType toCpp(JNIEnv *env, JavaType jValue) {}

//        static JavaType toJava(JNIEnv *env, CppType cValue) {}
};

struct Boolean : PrimitiveType {
    using CppType = bool;
    using JavaType = jboolean;

    static constexpr auto name = BOOST_HANA_STRING("Z");

    CppType toCpp(JNIEnv *env, JavaType jValue) { return jValue; }

    JavaType toJava(JNIEnv *env, CppType cValue) { return cValue; }
};

struct Byte : PrimitiveType {
    using CppType = std::int8_t;
    using JavaType = jbyte;

    static constexpr auto name = BOOST_HANA_STRING("B");

    static CppType toCpp(JNIEnv *env, JavaType jValue) { return jValue; }

    static JavaType toJava(JNIEnv *env, CppType cValue) { return cValue; }
};

struct Short : PrimitiveType {
    using CppType = std::int16_t;
    using JavaType = jshort;

    static constexpr auto name = BOOST_HANA_STRING("S");

    static CppType toCpp(JNIEnv *env, JavaType jValue) { return static_cast<CppType>(jValue); }

    static JavaType toJava(JNIEnv *env, CppType cValue) { return static_cast<JavaType>(cValue); }
};


template <typename _CppType>
struct Int32Compatible : PrimitiveType {
    using CppType = _CppType;
    using JavaType = jint;

    static constexpr auto name = BOOST_HANA_STRING("I");

    static CppType toCpp(JNIEnv *env, JavaType jValue) { return static_cast<CppType>(jValue); }

    static JavaType toJava(JNIEnv *env, CppType cValue) { return static_cast<JavaType>(cValue); }
};

using Int32 = Int32Compatible<std::int32_t>;


template <typename _CppType>
struct Int64Compatible : PrimitiveType {
    using CppType = _CppType;
    using JavaType = jlong;

    static constexpr auto name = BOOST_HANA_STRING("J");

    static CppType toCpp(JNIEnv *env, JavaType jValue) { return static_cast<CppType>(jValue); }

    static JavaType toJava(JNIEnv *env, CppType cValue) { return static_cast<JavaType>(cValue); }
};

using Int64 = Int64Compatible<std::int64_t>;

template <typename ContainedField, typename PrimitiveArrayType>
struct PrimitiveArray {
    using ItemType = typename ContainedField::CppType;
    using CppType = std::vector<ItemType>;
    using JavaType = PrimitiveArrayType;

    static constexpr auto name = BOOST_HANA_STRING("[") + typeSignature<ContainedField>();

    static CppType toCpp(JNIEnv *env, JavaType jValue) {
        auto result = CppType();
        auto size = env->GetArrayLength(jValue);
        result.reserve(size);
        return result;
    }

    static JavaType toJava(JNIEnv *env, const CppType &cValue) {
        if constexpr (std::is_same<PrimitiveArrayType, jbooleanArray>()) {
            auto result = env->NewBooleanArray(cValue.size());
            env->SetBooleanArrayRegion(result, 0, cValue.size(), &*cValue.begin());
            return result;
        } else if constexpr (std::is_same<PrimitiveArrayType, jbyteArray >()) {
            auto result = env->NewByteArray(cValue.size());
            env->SetByteArrayRegion(result, 0, cValue.size(), &*cValue.begin());
            return result;
        } else if constexpr (std::is_same<PrimitiveArrayType, jshortArray >()) {
            auto result = env->NewShortArray(cValue.size());
            env->SetShortArrayRegion(result, 0, cValue.size(), &*cValue.begin());
            return result;
        } else if constexpr (std::is_same<PrimitiveArrayType, jintArray >()) {
            auto result = env->NewIntArray(cValue.size());
            env->SetIntArrayRegion(result, 0, cValue.size(), &*cValue.begin());
            return result;
        } else if constexpr (std::is_same<PrimitiveArrayType, jlongArray >()) {
            auto result = env->NewLongArray(cValue.size());
            env->SetLongArrayRegion(result, 0, cValue.size(), &*cValue.begin());
            return result;
        } else {
            return nullptr;
        }
    }
};

using BooleanArray = PrimitiveArray<Boolean, jbooleanArray>;

template<typename IntType>
using Int32CompatibleArray = PrimitiveArray<Int32Compatible<IntType>, jintArray>;

using IntArray = PrimitiveArray<Int32, jintArray>;

#endif //TESTAPP_PRIMITIVETYPES_H
