/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_TEST_DEVICE
#define CUTE_VR_INTERNAL_TEST_DEVICE

#include <CuteVR/Device.hpp>

namespace CuteVR { namespace Internal {
    /// @internal@brief A simple Device for unit tests of abstract classes.
    /// @extends Device
    /// @tparam BaseT The class which will be inherited from.
    /// @pre BaseT extends Device.
    template<class BaseT = Device>
    class TestDevice final :
            public BaseT {
        static_assert(std::is_base_of<Device, BaseT>(), "Base class must be a device.");

    public: // destructor
        using BaseT::BaseT;

        ~TestDevice() override = default;

    public: // methods
        Device::Category category() const noexcept override {
            return dummyCategory;
        }

        void destroy() override {}

        bool isDestroyed() const noexcept override { return true; }

        void initialize() override {}

        bool isInitialized() const noexcept override { return true; }

        QDataStream &serialize(QDataStream &stream) const override {
            return BaseT::serialize(stream) << static_cast<quint8>(dummyCategory);
        }

        QDataStream &deserialize(QDataStream &stream) override {
            return BaseT::deserialize(stream) >> reinterpret_cast<quint8 &>(dummyCategory);
        }

        void update() override {}

        bool isCurrent() const noexcept override { return true; }

    public: // variables
        /// @internal@brief Used as return value for the category method.
        Device::Category dummyCategory{Device::Category::undefined};
    };
}}

#endif // CUTE_VR_INTERNAL_TEST_DEVICE
