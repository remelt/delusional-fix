#pragma once
#include "../../sdk/sdk.hpp"

class recv_prop_hook
{
public:
	recv_prop_hook( recv_prop* prop, const recv_var_proxy_fn proxy_fn ) :
		m_property( prop ),
		m_original_proxy_fn( prop->proxy_fn )
	{
		set_proxy_function( proxy_fn );
	}

	~recv_prop_hook( )
	{
		m_property->proxy_fn = m_original_proxy_fn;
	}

	auto get_original_function( ) const -> recv_var_proxy_fn
	{
		return m_original_proxy_fn;
	}

	auto set_proxy_function( const recv_var_proxy_fn proxy_fn ) const -> void
	{
		m_property->proxy_fn = proxy_fn;
	}

private:
	recv_prop* m_property;
	recv_var_proxy_fn m_original_proxy_fn;
};
