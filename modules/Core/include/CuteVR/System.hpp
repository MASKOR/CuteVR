/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_SYSTEM
#define CUTE_VR_SYSTEM

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QSet>
#include <QtGui/QMatrix4x4>

#include <CuteVR/Interface/Initializable.hpp>
#include <CuteVR/Interface/Destroyable.hpp>
#include <CuteVR/Interface/Updatable.hpp>
#include <CuteVR/Device.hpp>

namespace CuteVR {
    /// @brief This class helps to manage connected devices.
    /// @details It provides access to all head-mounted displays or audio setups, controllers, trackers, and tracking
    /// references that are connected to the virtual reality system. In addition, connected devices are brought into a
    /// spatial and user-related context. The cells -- also known as playing fields -- contain tracking references and
    /// several equipments -- often referred as users or players -- can be inside of them. A single equipment consists
    /// of usually one head-mounted display and one audio setup, and about one or two controllers.
    /// @note The availability of context information (currently) depends on the hardware or driver.
    /// @note The ability to use multiple cells or equipments within a single system depends on the hardware or driver.
    class System :
            public QObject,
            public Interface::Destroyable,
            public Interface::Initializable,
            public Interface::Updatable {
    Q_OBJECT
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief A map of all devices that are known to the system.
        /// @details There is an additional signal which only emits the actually changed device. To get devices by its
        /// type, equipment affiliation, or surrounding cell use the appropriate getters.
        // FIXME: doxygen bug, nested template with property not working
        Q_PROPERTY(ARG(QMap<CuteVR::Identifier, QSharedPointer<CuteVR::Device>>) devices
                   MEMBER devices NOTIFY devicesChanged FINAL)
        /// @brief A map of all the equipments are known to the system.
        /// @brief There is an additional signal which only emits the actually changed equipment. Functionality depends
        /// on Configurations::Core::Feature::equipment and Configurations::Core::Feature::multiEquipment.
        Q_PROPERTY(ARG(QMap<CuteVR::Identifier, CuteVR::System::Equipment>) equipments
                   MEMBER equipments NOTIFY equipmentsChanged FINAL)
        /// @brief A map of all the cells are known to the system.
        /// @brief There is an additional signal which only emits the actually changed cell. Functionality depends on
        /// Configurations::Core::Feature::cell and Configurations::Core::Feature::multiCell.
        Q_PROPERTY(ARG(QMap<CuteVR::Identifier, CuteVR::System::Cell>) cells
                   MEMBER cells NOTIFY cellsChanged FINAL)

    public: // types
        /// @brief An equipment that belongs to one user or player.
        struct Equipment {
            Identifier identifier{invalidIdentifier}; ///< An identifier to to distinguish this equipment from others.
            QSet<Identifier> headMountedDisplays{}; ///< Assigned head-mounted display(s).
            QSet<Identifier> headMountedAudios{}; ///< Assigned head-mounted audio setup(s).
            QSet<Identifier> controllers{}; ///< Assigned controllers.
        };

        /// @brief A shared cell for potentially multiple equipments.
        /// @details The boundaries of a cell are described with a geometry, which is positioned relative to the global
        /// transform. A cuboid, for example, could be positioned as follows, where `G` is the global transformation
        /// and `O` is the origin of the cuboid.
        /// @code
        ///  ●─────────●
        ///  │╲        │╲
        ///  │ ╲       │ ╲
        ///  │  ╲      │  ╲
        ///  │   ●─────┼───●
        ///  │   │ ○ O │   │
        ///  ●───┼─────●   │
        ///   ╲  │      ╲  │c/y
        /// b/z╲ │  ○ G  ╲ │
        ///     ╲│        ╲│
        ///      ●─────────●
        ///          a/x
        /// @endcode
        struct Cell {
            Identifier identifier{invalidIdentifier}; ///< An identifier to to distinguish this cell from others.
            QSet<Identifier> trackingReferences{}; ///< Tracking references for the cell.
            QSet<Identifier> trackers{}; ///< Trackers in the cell.
            QSet<Identifier> equipments{}; ///< Equipments that is currently in this cell.
            QPair<QMatrix4x4, CategorizedComponent<Component::Category::geometry>>
                    boundary{}; ///< The boundary of the cell, units are meters.
            QMatrix4x4 globalTransform{}; ///< Transformation of this cell into a global coordinate system.
        };

    public: // constructor/destructor
        System();

        ~System() override;

        Q_DISABLE_COPY(System)

    public: // getter
        /// @brief Query a list of device identifiers filtered by surrounding cell and/or equipment affinity.
        /// @param cell The cell in which the devices must be.
        /// @param equipment The equipment to which the devices must belong.
        /// @return The list of devices that meet both filter criteria.
        QList<Identifier> filteredDevices(Extension::Optional<Identifier> cell,
                                          Extension::Optional<Identifier> equipment) const noexcept;

        /// @brief Query a list of device identifiers filtered by their category.
        /// @param category The category the devices must have.
        /// @return The list of devices that are of that category.
        QList<Identifier> filteredDevices(Device::Category category) const noexcept;

        /// @brief Query the equipment to which a specific device belongs.
        /// @param device The device to be searched for.
        /// @return The equipment to which the device belongs, or nothing.
        Extension::Optional<Identifier> equipment(Identifier device) const noexcept;

        /// @brief Query the cell which surrounds a specific device.
        /// @param device The device to be searched for.
        /// @return The cell which surrounds the device, or nothing.
        Extension::Optional<Identifier> cell(Identifier device) const noexcept;

    public: // methods
        void destroy() override;

        bool isDestroyed() const noexcept override;

        void initialize() override;

        bool isInitialized() const noexcept override;

        void update() override;

        bool isCurrent() const noexcept override;

    public: // variables
        QMap<CuteVR::Identifier, QSharedPointer<CuteVR::Device>> devices{};
        QMap<CuteVR::Identifier, CuteVR::System::Equipment> equipments{};
        QMap<CuteVR::Identifier, CuteVR::System::Cell> cells{};

    private: // types
        class Private;

    private: // variables
        QScopedPointer<Private> _private;

    signals:
        /// @signal{map of devices}
        void devicesChanged(QMap<CuteVR::Identifier, QSharedPointer<CuteVR::Device>>);

        /// @signal{individual device}
        void deviceChanged(CuteVR::Identifier, QSharedPointer<CuteVR::Device>);

        /// @signal{map of equipments}
        void equipmentsChanged(QMap<CuteVR::Identifier, CuteVR::System::Equipment>);

        /// @signal{individual equipment}
        void equipmentChanged(CuteVR::Identifier, CuteVR::System::Equipment);

        /// @signal{map of cells}
        void cellsChanged(QMap<CuteVR::Identifier, CuteVR::System::Cell>);

        /// @signal{individual cell}
        void cellChanged(CuteVR::Identifier, CuteVR::System::Cell);
    };

    /// @equality{equipments};
    bool operator==(System::Equipment const &left, System::Equipment const &right);

    /// @inequality{equipments}
    bool operator!=(System::Equipment const &left, System::Equipment const &right);

    /// @equality{cells};
    bool operator==(System::Cell const &left, System::Cell const &right);

    /// @inequality{cells}
    bool operator!=(System::Cell const &left, System::Cell const &right);
}

Q_DECLARE_METATYPE(CuteVR::System::Equipment)

Q_DECLARE_METATYPE(CuteVR::System::Cell)

#endif // CUTE_VR_SYSTEM
