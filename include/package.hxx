#ifndef PACKAGE_HXX
#define PACKAGE_HXX

#include "types.hxx"
#include <set>

class Package {
public:
    Package();                    
    explicit Package(ElementID);  

    Package(Package&& other) noexcept;
    Package& operator=(Package&& other) noexcept;

    Package(const Package&) = delete;
    Package& operator=(const Package&) = delete;

    ElementID get_id() const;

    ~Package();

private:
    ElementID id_;

    static std::set<ElementID> assigned_ids_;
    static std::set<ElementID> freed_ids_;

    static ElementID generate_id();
};

#endif
