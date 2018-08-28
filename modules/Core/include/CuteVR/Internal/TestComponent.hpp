/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_TEST_COMPONENT
#define CUTE_VR_INTERNAL_TEST_COMPONENT

#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Internal {
    /// @internal@brief A simple Component for unit tests of abstract classes.
    /// @extends Component
    /// @tparam BaseT The class which will be inherited from.
    /// @pre BaseT extends Component.
    template<class BaseT = Component>
    struct TestComponent final :
            public BaseT {
        static_assert(std::is_base_of<Component, BaseT>(), "Base class must be a component.");

    public: // destructor
        ~TestComponent() override = default;

    public: // methods
        Component::Category category() const noexcept override {
            return dummyCategory;
        }

        QSharedPointer<Interface::Cloneable> clone() const override {
            return QSharedPointer<Interface::Cloneable>{new TestComponent{*this}};
        }

        QDataStream &serialize(QDataStream &stream) const override {
            return BaseT::serialize(stream) << static_cast<quint8>(dummyCategory);
        }

        QDataStream &deserialize(QDataStream &stream) override {
            return BaseT::deserialize(stream) >> reinterpret_cast<quint8 &>(dummyCategory);
        }

    public: // variables
        /// @internal@brief Used as return value for the category method.
        CuteVR::Component::Category dummyCategory{Component::Category::undefined};
    };
}}

#endif // CUTE_VR_INTERNAL_TEST_COMPONENT
