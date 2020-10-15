#include "cor/elements/static_organizer.hpp"

#include "cor/system/system.hpp"
// #include "cor/system/pod.hpp"
// #include "cor/message.hpp"

#include <algorithm>
#include <hpx/hpx.hpp>

namespace cor {

StaticOrganizer::StaticOrganizer() = default;

StaticOrganizer::StaticOrganizer(idp_t idp, unsigned int total_members, idp_t parent) :
    _idp{idp},
    _total_members{total_members},
    _parent{parent},
    _members{},
    _next_idm{0}
{
    std::cout << "Criado um objeto da classe \"StaticOrganizer\", com idp: " << _idp << std::endl;
}

StaticOrganizer::~StaticOrganizer() = default;

// StaticOrganizer::StaticOrganizer(StaticOrganizer&&) noexcept = default;

// StaticOrganizer& StaticOrganizer::operator=(StaticOrganizer&&) noexcept = default;

void StaticOrganizer::Join(idp_t idp, std::string const& name)
{
    // std::cout << "Join STATIC ORGANIZER ---------------------------------------------" << std::endl;

    // verify if the resource has already been attached
    if (_members.find(idp) != _members.end())
        throw std::logic_error("Resource " + std::to_string(idp) + " already joined!");
    
    // verify if the name of the resource is unique in the context of the ancestor
    for (const auto& p: _members) {
        if (p.second.second == name)
            throw std::logic_error("Resource name <" + name + "> is not unique in the context of the ancestor!");
    }

    // C++ 17
    // for (const auto& [key, value] : _members) {
    //     auto&& [my_idm, my_name] = value;
    //     if (my_name == name) {
    //         throw std::logic_error("Resource name <" + name + "> is not unique in the context of the ancestor!");
    //     }
    // }
    
    // generate new idm
    auto idm = _next_idm++;

    // insert new resource
    if (name.empty())
        _members.emplace(idp, std::make_pair(idm, std::to_string(idp)));
    else
        _members.emplace(idp, std::make_pair(idm, name));

    // Se este organizador for criado por uma clausura que nao a primeira, e for feito o primeiro join, envia a seguinte mensagem
    // Ver se isto é necessário
    // if (idm == 0 && _parent != 0) {
    //     Message msg;
    //     global::pod->SendMessage(_idp, _parent, msg);
    // }
}

void StaticOrganizer::Leave(idp_t idp)
{
    // erase resource idp
    _members.erase(idp);
}


idp_t StaticOrganizer::GetParent() const
{
    return _parent;
}

size_t StaticOrganizer::GetTotalMembers() const
{
    return _members.size();
}

size_t StaticOrganizer::GetFixedTotalMembers() const
{
    return _total_members;
}

std::vector<idp_t> StaticOrganizer::GetMemberList() const
{
    std::vector<idp_t> list;

    for (auto const& member: _members)
        list.push_back(member.first);

    return list;
}

idp_t StaticOrganizer::GetIdp(idm_t idm) const
{
    auto it = std::find_if(_members.begin(), _members.end(),
        [idm](auto&& member) -> bool {
            return member.second.first == idm;
        });

    if (it != _members.end()) {

        auto idp = it->first;
        return idp;
    } else {
        throw std::runtime_error("Resource with idm <" + std::to_string(idm) + "> does not exist!");
    }
}

idp_t StaticOrganizer::GetIdp(std::string const& name) const
{
    auto it = std::find_if(_members.begin(), _members.end(),
        [name](auto&& member) -> bool {
            return member.second.second == name;
        });

    if (it != _members.end()) {

        auto idp = it->first;
        return idp;
    } else {
        throw std::runtime_error("Resource with name <" + name + "> does not exist!");
    }
}

idm_t StaticOrganizer::GetIdm(idp_t idp) const
{
    auto idm = _members.at(idp).first;

    return idm;
}

idm_t StaticOrganizer::GetIdm(std::string const& name) const
{
    auto it = std::find_if(_members.begin(), _members.end(),
        [name](auto&& member) -> bool {
            return member.second.second == name;
        });

    if (it != _members.end()) {

        auto idm = it->second.first;
        return idm;
    } else {
        throw std::runtime_error("Resource with name <" + name + "> does not exist!");
    }
}

idp_t StaticOrganizer::GetStaticIdp() const {
    return _idp;
}

}