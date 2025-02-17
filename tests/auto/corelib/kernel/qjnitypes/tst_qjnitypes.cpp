// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include <QtTest>

#include <QtCore/qjnitypes.h>

class tst_QJniTypes : public QObject
{
    Q_OBJECT

public:
    tst_QJniTypes() = default;

private slots:
    void initTestCase();
    void nativeMethod();
};

struct QtJavaWrapper {};
template<>
struct QtJniTypes::Traits<QtJavaWrapper>
{
    static constexpr auto signature()
    {
        return QtJniTypes::CTString("Lorg/qtproject/qt/android/QtJavaWrapper;");
    }
};

template<>
struct QtJniTypes::Traits<QJniObject>
{
    static constexpr auto signature()
    {
        return QtJniTypes::CTString("Ljava/lang/Object;");
    }
};

struct QtCustomJniObject : QJniObject {};

template<>
struct QtJniTypes::Traits<QtCustomJniObject>
{
    static constexpr auto signature()
    {
        return QtJniTypes::CTString("Lorg/qtproject/qt/android/QtCustomJniObject;");
    }
};

static_assert(QtJniTypes::Traits<QtJavaWrapper>::signature() == "Lorg/qtproject/qt/android/QtJavaWrapper;");
static_assert(QtJniTypes::Traits<QtJavaWrapper>::signature() != "Ljava/lang/Object;");
static_assert(!(QtJniTypes::Traits<QtJavaWrapper>::signature() == "X"));

Q_DECLARE_JNI_CLASS(JavaType, "org/qtproject/qt/JavaType");
static_assert(QtJniTypes::Traits<QtJniTypes::JavaType>::signature() == "Lorg/qtproject/qt/JavaType;");
Q_DECLARE_JNI_TYPE(ArrayType, "[Lorg/qtproject/qt/ArrayType;")
static_assert(QtJniTypes::Traits<QtJniTypes::ArrayType>::signature() == "[Lorg/qtproject/qt/ArrayType;");

Q_DECLARE_JNI_CLASS(QtTextToSpeech, "org/qtproject/qt/android/speech/QtTextToSpeech")
static_assert(QtJniTypes::Traits<QtJniTypes::QtTextToSpeech>::className() == "org/qtproject/qt/android/speech/QtTextToSpeech");

static_assert(QtJniTypes::fieldSignature<jint>() == "I");
static_assert(QtJniTypes::fieldSignature<jint[]>() == "[I");
static_assert(QtJniTypes::fieldSignature<jint>() != "X");
static_assert(QtJniTypes::fieldSignature<jint>() != "Ljava/lang/Object;");
static_assert(QtJniTypes::fieldSignature<jlong>() == "J");
static_assert(QtJniTypes::fieldSignature<jstring>() == "Ljava/lang/String;");
static_assert(QtJniTypes::fieldSignature<jobject>() == "Ljava/lang/Object;");
static_assert(QtJniTypes::fieldSignature<jobject[]>() == "[Ljava/lang/Object;");
static_assert(QtJniTypes::fieldSignature<jobjectArray>() == "[Ljava/lang/Object;");
static_assert(QtJniTypes::fieldSignature<QJniObject>() == "Ljava/lang/Object;");
static_assert(QtJniTypes::fieldSignature<QtJavaWrapper>() == "Lorg/qtproject/qt/android/QtJavaWrapper;");
static_assert(QtJniTypes::fieldSignature<QtJavaWrapper[]>() == "[Lorg/qtproject/qt/android/QtJavaWrapper;");
static_assert(QtJniTypes::fieldSignature<QtCustomJniObject>() == "Lorg/qtproject/qt/android/QtCustomJniObject;");

static_assert(QtJniTypes::methodSignature<void>() == "()V");
static_assert(QtJniTypes::methodSignature<void>() != "()X");
static_assert(QtJniTypes::methodSignature<void, jint>() == "(I)V");
static_assert(QtJniTypes::methodSignature<void, jint, jstring>() == "(ILjava/lang/String;)V");
static_assert(QtJniTypes::methodSignature<jlong, jint, jclass>() == "(ILjava/lang/Class;)J");
static_assert(QtJniTypes::methodSignature<jobject, jint, jstring>() == "(ILjava/lang/String;)Ljava/lang/Object;");
static_assert(QtJniTypes::methodSignature<QtJniTypes::JavaType, jint, jstring>()
                                      == "(ILjava/lang/String;)Lorg/qtproject/qt/JavaType;");

static_assert(QtJniTypes::isPrimitiveType<jint>());
static_assert(QtJniTypes::isPrimitiveType<void>());
static_assert(!QtJniTypes::isPrimitiveType<jobject>());
static_assert(!QtJniTypes::isPrimitiveType<QtCustomJniObject>());

static_assert(!QtJniTypes::isObjectType<jint>());
static_assert(!QtJniTypes::isObjectType<void>());
static_assert(QtJniTypes::isObjectType<jobject>());
static_assert(QtJniTypes::isObjectType<jobjectArray>());
static_assert(QtJniTypes::isObjectType<QtCustomJniObject>());

static_assert(!QtJniTypes::isArrayType<jint>());
static_assert(QtJniTypes::isArrayType<jint[]>());
static_assert(QtJniTypes::isArrayType<jobject[]>());
static_assert(QtJniTypes::isArrayType<jobjectArray>());
static_assert(QtJniTypes::isArrayType<QtJavaWrapper[]>());

static_assert(QtJniTypes::CTString("ABCDE").startsWith("ABC"));
static_assert(QtJniTypes::CTString("ABCDE").startsWith("A"));
static_assert(QtJniTypes::CTString("ABCDE").startsWith("ABCDE"));
static_assert(!QtJniTypes::CTString("ABCDE").startsWith("ABCDEF"));
static_assert(!QtJniTypes::CTString("ABCDE").startsWith("9AB"));
static_assert(QtJniTypes::CTString("ABCDE").startsWith('A'));
static_assert(!QtJniTypes::CTString("ABCDE").startsWith('B'));

static_assert(QtJniTypes::CTString("ABCDE").endsWith("CDE"));
static_assert(QtJniTypes::CTString("ABCDE").endsWith("E"));
static_assert(QtJniTypes::CTString("ABCDE").endsWith("ABCDE"));
static_assert(!QtJniTypes::CTString("ABCDE").endsWith("DEF"));
static_assert(!QtJniTypes::CTString("ABCDE").endsWith("ABCDEF"));
static_assert(QtJniTypes::CTString("ABCDE").endsWith('E'));
static_assert(!QtJniTypes::CTString("ABCDE").endsWith('F'));

enum UnscopedEnum {};
enum class ScopedEnum {};
enum class IntEnum : int {};
enum class UnsignedEnum : unsigned {};
enum class Int8Enum : int8_t {};
enum class ShortEnum : short {};
enum class LongEnum : long {};
enum class JIntEnum : jint {};

static_assert(QtJniTypes::Traits<UnscopedEnum>::signature() == "I");
static_assert(QtJniTypes::Traits<ScopedEnum>::signature() == "I");
static_assert(QtJniTypes::Traits<IntEnum>::signature() == "I");
static_assert(QtJniTypes::Traits<UnsignedEnum>::signature() == "I");
static_assert(QtJniTypes::Traits<Int8Enum>::signature() == "B");
static_assert(QtJniTypes::Traits<LongEnum>::signature() == "J");
static_assert(QtJniTypes::Traits<JIntEnum>::signature() == "I");

void tst_QJniTypes::initTestCase()
{

}

static bool nativeFunction(JNIEnv *, jclass, int, jstring, long)
{
    return true;
}
Q_DECLARE_JNI_NATIVE_METHOD(nativeFunction)

static_assert(QtJniTypes::nativeMethodSignature(nativeFunction) == "(ILjava/lang/String;J)Z");

void tst_QJniTypes::nativeMethod()
{
    const auto method = Q_JNI_NATIVE_METHOD(nativeFunction);
    QVERIFY(method.fnPtr == nativeFunction);
    QCOMPARE(method.name, "nativeFunction");
    QCOMPARE(method.signature, "(ILjava/lang/String;J)Z");
}

QTEST_MAIN(tst_QJniTypes)

#include "tst_qjnitypes.moc"
