#ifndef COR_CLOSURE_HPP
#define COR_CLOSURE_HPP

#include "cor/resources/resource.hpp"
#include "cor/elements/static_organizer.hpp"

namespace cor {

// struct Closure: public Resource, hpx::components::component_base<Closure>
struct Closure: public hpx::components::abstract_migration_support< hpx::components::component_base<Closure>, Resource >
{

using base_type = hpx::components::abstract_migration_support<
    hpx::components::component_base<Closure>, Resource >;

typedef hpx::components::component_base<Closure>::wrapping_type wrapping_type;
typedef Closure type_holder;
typedef Resource base_type_holder;

friend class ResourceManager;

// protected:
//     Closure();
//     explicit Closure(idp_t idp, unsigned int total_members, idp_t parent);

public:
    Closure();
    explicit Closure(idp_t idp, unsigned int total_members, idp_t parent);
    ~Closure();

    // Components that should be migrated using hpx::migrate<> need to
    // be Serializable and CopyConstructable. Components can be
    // MoveConstructable in which case the serialized data is moved into the
    // component's constructor.
    Closure(Closure&& rhs) :
        base_type(std::move(rhs)),
        staticOrganizer(rhs.staticOrganizer)
    {}

    Closure& operator=(Closure&& rhs)
    {
        this->Resource::operator=(std::move(static_cast<Resource&>(rhs)));
        staticOrganizer = rhs.staticOrganizer;
        return *this;
    }

    // Closure(const Closure&) = delete;
    // Closure& operator=(const Closure&) = delete;

    // Closure(Closure&&) noexcept;
    // Closure& operator=(Closure&&) noexcept;


    /* StaticOrganizer interface */
    void Join(idp_t idp, std::string const& name);
    void Leave(idp_t idp);

    idp_t GetParent();
    std::size_t GetTotalMembers();
    std::size_t GetFixedTotalMembers();
    std::vector<idp_t> GetMemberList();

    // metodos com o mesmo nome é complicado para fazer as action, portanto usei 1 e 2, o que nao altera em anda a API final
    idp_t GetIdp1(idm_t idm);
    idp_t GetIdp2(std::string const& name);

    idm_t GetIdm1(idp_t idp);
    idm_t GetIdm2(std::string const& name);

    idp_t GetStaticIdp();

    HPX_DEFINE_COMPONENT_ACTION(Closure, Join, Join_action_Closure);
    HPX_DEFINE_COMPONENT_ACTION(Closure, Leave, Leave_action_Closure);
    HPX_DEFINE_COMPONENT_ACTION(Closure, GetParent, GetParent_action_Closure);
    HPX_DEFINE_COMPONENT_ACTION(Closure, GetTotalMembers, GetTotalMembers_action_Closure);
    HPX_DEFINE_COMPONENT_ACTION(Closure, GetFixedTotalMembers, GetFixedTotalMembers_action_Closure);
    HPX_DEFINE_COMPONENT_ACTION(Closure, GetMemberList, GetMemberList_action_Closure);
    HPX_DEFINE_COMPONENT_ACTION(Closure, GetIdp1, GetIdp1_action_Closure);
    HPX_DEFINE_COMPONENT_ACTION(Closure, GetIdp2, GetIdp2_action_Closure);
    HPX_DEFINE_COMPONENT_ACTION(Closure, GetIdm1, GetIdm1_action_Closure);
    HPX_DEFINE_COMPONENT_ACTION(Closure, GetIdm2, GetIdm2_action_Closure);
    HPX_DEFINE_COMPONENT_ACTION(Closure, GetStaticIdp, GetStaticIdp_action_Closure);

    template <typename Archive>
    void serialize(Archive& ar, unsigned version)
    {
        ar & hpx::serialization::base_object<Resource>(*this);
        ar & staticOrganizer;
    }

private:
    StaticOrganizer staticOrganizer;
};

}


/* StaticOrganizer actions */
typedef cor::Closure::Join_action_Closure Join_action_Closure;
typedef cor::Closure::Leave_action_Closure Leave_action_Closure;
typedef cor::Closure::GetParent_action_Closure GetParent_action_Closure;
typedef cor::Closure::GetTotalMembers_action_Closure GetTotalMembers_action_Closure;
typedef cor::Closure::GetFixedTotalMembers_action_Closure GetFixedTotalMembers_action_Closure;
typedef cor::Closure::GetMemberList_action_Closure GetMemberList_action_Closure;
typedef cor::Closure::GetIdp1_action_Closure GetIdp1_action_Closure;
typedef cor::Closure::GetIdp2_action_Closure GetIdp2_action_Closure;
typedef cor::Closure::GetIdm1_action_Closure GetIdm1_action_Closure;
typedef cor::Closure::GetIdm2_action_Closure GetIdm2_action_Closure;
typedef cor::Closure::GetStaticIdp_action_Closure GetStaticIdp_action_Closure;

HPX_REGISTER_ACTION_DECLARATION(Join_action_Closure);
HPX_REGISTER_ACTION_DECLARATION(Leave_action_Closure);
HPX_REGISTER_ACTION_DECLARATION(GetParent_action_Closure);
HPX_REGISTER_ACTION_DECLARATION(GetTotalMembers_action_Closure);
HPX_REGISTER_ACTION_DECLARATION(GetFixedTotalMembers_action_Closure);
HPX_REGISTER_ACTION_DECLARATION(GetMemberList_action_Closure);
HPX_REGISTER_ACTION_DECLARATION(GetIdp1_action_Closure);
HPX_REGISTER_ACTION_DECLARATION(GetIdp2_action_Closure);
HPX_REGISTER_ACTION_DECLARATION(GetIdm1_action_Closure);
HPX_REGISTER_ACTION_DECLARATION(GetIdm2_action_Closure);
HPX_REGISTER_ACTION_DECLARATION(GetStaticIdp_action_Closure);

#endif