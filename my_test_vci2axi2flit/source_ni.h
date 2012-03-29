/**
 * @filename  source_ni.h
 *
 * @brief     File that includes trans_typeLM model of SystemC source 
 *            network interface (i.e. sender module) for Rabbits.
 *
 * @author    Young Jin Yoon <youngjin@cs.columbia.edu>
 */

#ifndef __APPROX_NOC_SOURCE_NI_H__
#define __APPROX_NOC_SOURCE_NI_H__

#include <map>

#include <systemc>
#include <tlm.h>

#include "abstract_noc.h"
#include "flit.h"
#include "ni_base.h"
#include "approx_ni_base.h"
#include "sc_clked_mutex.h"
#include "approx_network_base.h"

namespace approx_noc {

using namespace tlm;
using namespace sc_core;
using namespace sc_ext;
using namespace std;

/**
 * @brief source network interface for the approximated network model.
 *
 * @tparam      trans_type   class for transaction type, either vci_request or vci_response
 * @param[out]  flit_out    sc_port that will be connected to the router
 * 
 * @warning         trans_typehis module assumes that there is only one flit per clock cycle 
 *                  from the network.
 */
template <typename trans_type>
class source_ni : public sc_module, 
public approx_source_ni_base,
public abstract_noc::source_ni_base <trans_type> { 

  public:
    //sc_port< tlm_blocking_put_if < flit > > flit_out;

    SC_HAS_PROCESS(source_ni);
    /// Constructor
    source_ni(approx_network_base *nb, sc_time clk, int pid, int flit_width=64, int num_out_trans=1, sc_module_name _n = sc_gen_unique_name("approx_noc::source_ni"));
    
    /// destructor
    ~source_ni() {};

    //Derived from tlm_blocking_put_if<trans_type>
    //void put(const trans_type &t);

  protected:
    inline approx_network_base *get_approx_network_base() 
      { return static_cast<approx_network_base *>(this->m_nb); }
    
    //Derived from abstract_noc::source_ni_base <trans_type>
    virtual void process_main();
    virtual void fliticize(std::vector< flit > &v, trans_type& trans);
    //virtual void start_transaction(trans_type &trans);

    //tlm_fifo < trans_type > trans_fifo;  //< transaction fifo
};

} //namespace approx_noc
#include "source_ni.cc"

#endif //__SOURCE_NI_H__
