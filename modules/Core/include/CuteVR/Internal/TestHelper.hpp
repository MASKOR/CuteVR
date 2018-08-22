/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_TEST_HELPER
#define CUTE_VR_INTERNAL_TEST_HELPER

#include <type_traits>
#include <QtTest/QTest>

namespace CuteVR { namespace Internal {
    /// @internal@private
    template<class TraitT>
    struct isQSharedPointer : std::false_type {};

    /// @internal@private
    template<class InnerT>
    struct isQSharedPointer<QSharedPointer<InnerT>> : std::true_type {};

    /// @internal@private
    template<class TraitT>
    struct decayQSharedPointer { using type = TraitT; };

    /// @internal@private
    template<class InnerT>
    struct decayQSharedPointer<QSharedPointer<InnerT>> { using type = InnerT; };

    /// @internal@private
    template<class TraitT>
    using decayQSharedPointerT = typename decayQSharedPointer<TraitT>::type;

    /// @internal@private
    template<class TestClassT>
    typename std::enable_if_t<!!isQSharedPointer<TestClassT>(), typename TestClassT::value_type &>
    autoRef(TestClassT &testClass) {
        return *testClass;
    }

    /// @internal@private
    template<class TestClassT>
    typename std::enable_if_t<!isQSharedPointer<TestClassT>(), TestClassT &>
    autoRef(TestClassT &testClass) {
        return testClass;
    }

    /// @internal@brief Verifies the correctness of the copy constructor's semantics by checking whether original and
    /// copy are equal.
    /// @pre A test object has been added to the test environment via `QTest::addColumn<TestClassT>("object")`.
    /// @tparam TestClassT The class to be tested.
    /// @pre TestClassT must be copy constructible and equality comparable.
    template<class TestClassT>
    void copyConstructorTestHelper() {
        static_assert(std::is_copy_constructible<TestClassT>(), "The tested class must be copy constructible.");
        QFETCH(TestClassT, object);
        auto objectCopy{autoRef(object)}; // NOLINT
        QVERIFY(autoRef(object) == objectCopy);
    }

    /// @internal@brief Verifies the correctness of the move constructor's semantics by checking whether original and
    /// an empty object are equal as well as a copy and the moved object are equal.
    /// @pre A test object has been added to the test environment via `QTest::addColumn<TestClassT>("object")`.
    /// @tparam TestClassT The class to be tested.
    /// @pre TestClassT must be default, copy and move constructible, and equality comparable.
    template<class TestClassT>
    void moveConstructorTestHelper() {
        static_assert(std::is_default_constructible<TestClassT>(), "The tested class must be default constructible.");
        static_assert(std::is_copy_constructible<TestClassT>(), "The tested class must be copy constructible.");
        static_assert(std::is_move_constructible<TestClassT>(), "The tested class must be move constructible.");
        QFETCH(TestClassT, object);
        TestClassT objectCopy{object}, empty{};
        auto objectMove(std::move(object));
        QVERIFY(objectCopy == objectMove);
        QVERIFY(object == empty); // NOLINT
    }

    /// @internal@brief Verifies the correctness of the copy assignment's semantics by checking whether original and
    /// copy are equal.
    /// @pre A test object has been added to the test environment via `QTest::addColumn<TestClassT>("object")`.
    /// @tparam TestClassT The class to be tested.
    /// @pre TestClassT must be default constructible, copy assignable and equality comparable.
    template<class TestClassT>
    void copyAssignmentOperatorTestHelper() {
        static_assert(std::is_default_constructible<TestClassT>(), "The tested class must be default constructible.");
        static_assert(std::is_copy_assignable<TestClassT>(), "The tested class must be copy assignable.");
        QFETCH(TestClassT, object);
        decayQSharedPointerT<TestClassT> objectCopy{};
        objectCopy = autoRef(object);
        QVERIFY(autoRef(object) == objectCopy);
    }

    /// @internal@brief Verifies the correctness of the move assignment's semantics by checking whether original and an
    /// empty object are equal as well as a copy and the moved object are equal.
    /// @pre A test object has been added to the test environment via `QTest::addColumn<TestClassT>("object")`.
    /// @tparam TestClassT The class to be tested.
    /// @pre TestClassT must be default and copy constructible, move assignable and equality comparable.
    template<class TestClassT>
    void moveAssignmentOperatorTestHelper() {
        static_assert(std::is_default_constructible<TestClassT>(), "The tested class must be default constructible.");
        static_assert(std::is_copy_constructible<TestClassT>(), "The tested class must be copy constructible.");
        static_assert(std::is_move_assignable<TestClassT>(), "The tested class must be move assignable.");
        QFETCH(TestClassT, object);
        TestClassT objectCopy{object}, empty{}, objectMove{};
        objectMove = std::move(object);
        QVERIFY(objectCopy == objectMove);
        QVERIFY(object == empty); // NOLINT
    }

    /// @internal@brief Tests if a cloned object of a cloneable class is equal to the original object.
    /// @pre A test object has been added to the test environment via `QTest::addColumn<TestClassT>("object")`.
    /// @tparam TestClassT The class to be tested.
    /// @pre TestClassT must be cloneable (e.g. through Interface::Cloneable) and must be equality comparable.
    template<class TestClassT>
    void cloneableInterfaceTestHelper() {
        QFETCH(TestClassT, object);
        auto clonedObject{autoRef(object).clone()};
        QVERIFY(!clonedObject.isNull());
        QVERIFY(autoRef(object) == *qSharedPointerDynamicCast<TestClassT>(clonedObject));
    }

    /// @internal@brief Verifies that a destroyable object returns that it is destroyed after destroying it.
    /// @pre A test object has been added to the test environment via `QTest::addColumn<TestClassT>("object")`.
    /// @tparam TestClassT The class to be tested.
    /// @pre TestClassT must be destroyable (e.g. through Interface::Destroyable).
    template<class TestClassT>
    void destroyableInterfaceTestHelper() {
        QFETCH(TestClassT, object);
        QVERIFY(!autoRef(object).isDestroyed());
        autoRef(object).destroy();
        QVERIFY(autoRef(object).isDestroyed());
    }

    /// @internal@brief Tests whether the equality comparison of two given objects leads to the given result, and
    /// whether the inequality comparison leads to the negated result.
    /// @pre Operands and result have been added to the test environment via `QTest::addColumn<TestClassT>("left")`,
    /// `QTest::addColumn<TestClassT>("right")`, and `QTest::addColumn<bool>("result")`.
    /// @tparam TestClassT The class to be tested.
    /// @pre TestClassT must be equality comparable (e.g. through Interface::EqualityComparable).
    template<class TestClassT>
    void equalityComparableInterfaceTestHelper() {
        QFETCH(TestClassT, left);
        QFETCH(TestClassT, right);
        QFETCH(bool, result);
        QVERIFY((autoRef(left) == autoRef(right)) == result);
        QVERIFY((autoRef(left) != autoRef(right)) != result);
    }

    /// @internal@brief Verifies that an initializable object returns that it is initialized after initializing it.
    /// @pre A test object has been added to the test environment via `QTest::addColumn<TestClassT>("object")`.
    /// @tparam TestClassT The class to be tested.
    /// @pre TestClassT must be initializable (e.g. through Interface::Initializable).
    template<class TestClassT>
    void initializableInterfaceTestHelper() {
        QFETCH(TestClassT, object);
        QVERIFY(!autoRef(object).isInitialized());
        autoRef(object).initialize();
        QVERIFY(autoRef(object).isInitialized());
    }

    /// @internal@brief Tests whether the order comparison of two given objects (`left > right`, `right < left`) leads
    /// to the given result, and whether the reversion (`left <= right`, `right >= left`) leads to the negated result.
    /// @pre Operands and result have been added to the test environment via `QTest::addColumn<TestClassT>("left")`,
    /// `QTest::addColumn<TestClassT>("right")`, and `QTest::addColumn<bool>("result")`.
    /// @tparam TestClassT The class to be tested.
    /// @pre TestClassT must be order comparable (e.g. through Interface::OrderComparable).
    template<class TestClassT>
    void orderComparableInterfaceTestHelper() {
        QFETCH(TestClassT, left);
        QFETCH(TestClassT, right);
        QFETCH(bool, result);
        QVERIFY((autoRef(left) > autoRef(right)) == result);
        QVERIFY((autoRef(right) < autoRef(left)) == result);
        QVERIFY((autoRef(left) <= autoRef(right)) != result);
        QVERIFY((autoRef(right) >= autoRef(left)) != result);
    }

    /// @internal@brief Tests whether an object can be first serialized and then deserialized without a change based on
    /// an equality comparison, and whether the stream remains intact.
    /// @pre A test object has been added to the test environment via `QTest::addColumn<TestClassT>("object")`.
    /// @tparam TestClassT The class to be tested.
    /// @pre TestClassT must be default constructible and serializable (e.g. through Interface::Serializable).
    template<class TestClassT>
    void serializableInterfaceTestHelper() {
        static_assert(std::is_default_constructible<TestClassT>(), "The tested class must be default constructible.");
        QFETCH(TestClassT, object);
        decayQSharedPointerT<TestClassT> streamedObject{};
        QByteArray bytes;
        QDataStream outputStream{&bytes, QIODevice::WriteOnly}, inputStream{&bytes, QIODevice::ReadOnly};
        outputStream << autoRef(object);
        inputStream >> streamedObject;
        QVERIFY(autoRef(object) == streamedObject);
        QVERIFY(inputStream.atEnd());
    }

    /// @internal@brief Tests if an updatable object returns that it is current after updating it.
    /// @pre A test object has been added to the test environment via `QTest::addColumn<TestClassT>("object")`.
    /// @tparam TestClassT The class to be tested.
    /// @pre TestClassT must be updatable (e.g. through Interface::Updatable).
    template<class TestClassT>
    void updatableInterfaceTestHelper() {
        QFETCH(TestClassT, object);
        QVERIFY(!autoRef(object).isCurrent());
        autoRef(object).update();
        QVERIFY(autoRef(object).isCurrent());
    }
}}

#endif // CUTE_VR_INTERNAL_TEST_HELPER
