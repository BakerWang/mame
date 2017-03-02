// license:BSD-3-Clause
// copyright-holders:Vas Crabb

#include "teeconn.h"


device_type const TI8X_TEE_CONNECTOR = device_creator<bus::ti8x::tee_connector_device>;


namespace bus { namespace ti8x {

namespace {

MACHINE_CONFIG_FRAGMENT(tee_connector)
	MCFG_TI8X_LINK_PORT_ADD("a", default_ti8x_link_devices, nullptr)
	MCFG_TI8X_LINK_TIP_HANDLER(WRITELINE(tee_connector_device, tip_a_w))
	MCFG_TI8X_LINK_RING_HANDLER(WRITELINE(tee_connector_device, ring_a_w))

	MCFG_TI8X_LINK_PORT_ADD("b", default_ti8x_link_devices, nullptr)
	MCFG_TI8X_LINK_TIP_HANDLER(WRITELINE(tee_connector_device, tip_b_w))
	MCFG_TI8X_LINK_RING_HANDLER(WRITELINE(tee_connector_device, ring_b_w))
MACHINE_CONFIG_END

} // anonymous namespace


tee_connector_device::tee_connector_device(
		machine_config const &mconfig,
		char const *tag,
		device_t *owner,
		uint32_t clock)
	: device_t(mconfig, TI8X_TEE_CONNECTOR, "T-connector", tag, owner, clock, "ti8xtconn", __FILE__)
	, device_ti8x_link_port_interface(mconfig, *this)
	, m_port_a(*this, "a")
	, m_port_b(*this, "b")
	, m_tip_host(true)
	, m_tip_a(true)
	, m_tip_b(true)
	, m_ring_host(true)
	, m_ring_a(true)
	, m_ring_b(true)
{
}


WRITE_LINE_MEMBER(tee_connector_device::tip_a_w)
{
	m_tip_a = bool(state);
	output_tip((m_tip_a && m_tip_b) ? 1 : 0);
	m_port_b->tip_w((m_tip_host && m_tip_a) ? 1 : 0);
}


WRITE_LINE_MEMBER(tee_connector_device::ring_a_w)
{
	m_ring_a = bool(state);
	output_ring((m_ring_a && m_ring_b) ? 1 : 0);
	m_port_b->ring_w((m_ring_host && m_ring_a) ? 1 : 0);
}


WRITE_LINE_MEMBER(tee_connector_device::tip_b_w)
{
	m_tip_b = bool(state);
	output_tip((m_tip_a && m_tip_b) ? 1 : 0);
	m_port_a->tip_w((m_tip_host && m_tip_b) ? 1 : 0);
}


WRITE_LINE_MEMBER(tee_connector_device::ring_b_w)
{
	m_ring_b = bool(state);
	output_ring((m_ring_a && m_ring_b) ? 1 : 0);
	m_port_a->ring_w((m_ring_host && m_ring_b) ? 1 : 0);
}


machine_config_constructor tee_connector_device::device_mconfig_additions() const
{
	return MACHINE_CONFIG_NAME(tee_connector);
}


void tee_connector_device::device_start()
{
	save_item(NAME(m_tip_host));
	save_item(NAME(m_tip_a));
	save_item(NAME(m_tip_b));
	save_item(NAME(m_ring_host));
	save_item(NAME(m_ring_a));
	save_item(NAME(m_ring_b));

	m_tip_host = m_tip_a = m_tip_b = true;
	m_ring_host = m_ring_a = m_ring_b = true;
}


WRITE_LINE_MEMBER(tee_connector_device::input_tip)
{
	m_tip_host = bool(state);
	m_port_a->tip_w((m_tip_host && m_tip_b) ? 1 : 0);
	m_port_b->tip_w((m_tip_host && m_tip_a) ? 1 : 0);
}


WRITE_LINE_MEMBER(tee_connector_device::input_ring)
{
	m_ring_host = bool(state);
	m_port_a->ring_w((m_ring_host && m_ring_b) ? 1 : 0);
	m_port_b->ring_w((m_ring_host && m_ring_a) ? 1 : 0);
}

} } // namespace bus::ti8x