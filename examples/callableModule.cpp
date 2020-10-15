#include "cor/cor.hpp"

extern "C"
{
    idp_t Test(idp_t rsc_idp);
}

idp_t Test(idp_t rsc_idp)
{
    // obter o domínio local
    auto domain = cor::GetDomain();

    // obter o idp do agent que está a executar uma função preente no módulo do utilizador
    auto agent_idp = domain->GetActiveResourceIdp();

    auto group_idp = domain->GetPredecessorIdp(agent_idp);
    auto group = domain->GetLocalResource<cor::Group_Client>(group_idp);
    auto data_idp = group->GetIdp("data");
    auto data = domain->GetLocalResource<cor::Data_Client<idp_t>>(data_idp);

    data->Acquire();
    auto value = *data;
    *value = agent_idp;
    data->Release();

    // obter uma referência para o agente que está a executar a função
    auto agent = domain->GetLocalResource<cor::ProtoAgent_Client<idp_t(idp_t)>>(agent_idp);

    // retorna o idp do agente
    return agent_idp;
}
