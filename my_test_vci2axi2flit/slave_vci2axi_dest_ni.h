/**
 * @filename  vci2axi_dest_ni.h
 *
 * @brief     File that includes TLM model of SystemC destination 
 *            network interface (i.e. receiver or sink module) for Rabbits.
 *
 * @author    George Faldamis
 */

#ifndef __APPROX_AXI_NOC_SLAVE_VCI2AXI_DEST_NI_H__
#define __APPROX_AXI_NOC_SLAVE_VCI2AXI_DEST_NI_H__

//#include <map>

#include <systemc>
#include <tlm.h>

#include "flit.h"
#include "abstract_noc.h"
#include "ni_base.h"
//#include "sc_clked_mutex.h"
//#include "approx_network_base.h"
//namespace approx_axi_noc {

using namespace tlm;
using namespace sc_core;
//using namespace sc_ext;
using namespace std;

/**
 * @brief systemC-TLM destination network interface.
 *
 * @tparam      T   class for transaction type, either vci_request or vci_response
 * 
 * @warning         This module assumes that there is only one flit per clock cycle 
 *                  from the network.
 */

class slave_vci2axi_dest_ni : public sc_module, 
public dest_ni_base <vci_request>,
public tlm_blocking_put_if<axi_waddr>,
public tlm_blocking_put_if<axi_wdata>,
public tlm_blocking_put_if<axi_raddr> { 

  public:
    SC_HAS_PROCESS (slave_vci2axi_dest_ni);

    /// Constructor
    slave_vci2axi_dest_ni(int pid, int num_out_trans=1, sc_module_name _n = sc_gen_unique_name("approx_noc::slave_vci2axi_dest_ni"));
    /// destructor
    ~slave_vci2axi_dest_ni() {};


    virtual void put(const axi_waddr &trans);
    virtual void put(const axi_wdata &trans);
    virtual void put(const axi_raddr &trans);

    //Derived from approx_dest_ni_base
    //virtual void put(const flit &t);
   
    //Derived from abstract_noc::dest_ni_base <trans_type>
    virtual vci_request get (tlm_tag< vci_request > *t=0)
	{	return this->trans_fifo.get();};
    //virtual void complete_transaction(const trans_type &trans, unsigned int &size, sc_time &latency);
    
    //ports

protected:
#if 0
// MY COMMENTS
    inline approx_network_base *get_approx_network_base() 
      { return static_cast<approx_network_base *>(this->m_nb); }
#endif
// END OF MY COMMENTS
    
//    virtual void check_tail(const flit &t);

//    flit head_flit;            //< head flit of a transaction
    
    //tlm_fifo < T > trans_fifo; //< transaction fifo

    tlm_fifo < axi_waddr > axi_waddr_fifo;
    tlm_fifo < axi_wdata > axi_wdata_fifo;
    tlm_fifo < axi_raddr > axi_raddr_fifo;




    void waddr_process();
    void wdata_process();
    void raddr_process();
};

//} //namespace approx_axi_noc
#include "slave_vci2axi_dest_ni.cc"

#endif //__DEST_NI_H__
