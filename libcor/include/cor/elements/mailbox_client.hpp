
#ifndef COR_MAILBOX_CLIENT_HPP
#define COR_MAILBOX_CLIENT_HPP

#include <hpx/hpx.hpp>

#include "cor/elements/mailbox.hpp"
#include "cor/message.hpp"

namespace cor {

struct Mailbox_Client: hpx::components::client_base<Mailbox_Client, Mailbox>
{

private:
	static hpx::future<hpx::id_type> create_server(idp_t idp) {
		return hpx::local_new<Mailbox>(idp);
	}

public:
	typedef hpx::components::client_base<Mailbox_Client, Mailbox> base_type;

	friend class hpx::serialization::access;

	/// Default construct an empty client side representation (not
	/// connected to any existing component). Also needed for serialization
	Mailbox_Client()
	{}

	/// Create a client side representation for the existing
	/// Closure_Component instance with the given GID
	Mailbox_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id))
	{}

	Mailbox_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id))
	{}

	Mailbox_Client(hpx::id_type && id) :
		base_type(std::move(id))
	{}

	/// Standard constructor with parameters
	Mailbox_Client(idp_t idp) :
		base_type(create_server(idp)),
		_idp(idp)
	{}


    void Send(idp_t dest, Message const& msg)
	{
		typedef Mailbox::Send1_action_Mailbox action_type;
		return hpx::async<action_type>(this->get_id(), dest, msg).get();
	}

    void Send(std::vector<idp_t> const& dests, Message const& msg)
	{
		typedef Mailbox::Send2_action_Mailbox action_type;
		return hpx::async<action_type>(this->get_id(), dests, msg).get();
	}

    Message Receive()
	{
		typedef Mailbox::Receive1_action_Mailbox action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

    Message Receive(idp_t source)
	{
		typedef Mailbox::Receive2_action_Mailbox action_type;
		return hpx::async<action_type>(this->get_id(), source).get();
	}

    void Broadcast(idp_t clos, Message const& msg)
	{
		typedef Mailbox::Broadcast_action_Mailbox action_type;
		return hpx::async<action_type>(this->get_id(), clos, msg).get();
	}

    void Send(idm_t rank, idp_t clos, Message const& msg)
	{
		typedef Mailbox::Send3_action_Mailbox action_type;
		return hpx::async<action_type>(this->get_id(), rank, clos, msg).get();
	}

    Message Receive(idm_t rank, idp_t clos)
	{
		typedef Mailbox::Receive3_action_Mailbox action_type;
		return hpx::async<action_type>(this->get_id(), rank, clos).get();
	}



	// /** Local interface **/
	// // local idp of this resource
	// idp_t Idp() {
	// 	return _idp;
	// }

	// Returns component's GID
	hpx::id_type GetGid() {
	  return this->get_id();
	}



private:
	template <typename Archive>
	void serialize(Archive& ar, unsigned) {   
		ar & _idp;
		// std::cout << "serialized\n";
	}

	idp_t _idp;

};


}

#endif