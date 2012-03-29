/**
 * @filename approx_ni_base.h
 *
 * @brief File that includes the common abstraction of network interface.
 *
 * @author Young Jin Yoon <youngjin@cs.columbia.edu>
 */

#ifndef __APPROX_NOC_APPROX_NI_BASE_H__
#define __APPROX_NOC_APPROX_NI_BASE_H__

#include <map>

#include <systemc>

#include "flit.h"
#include "abstract_noc.h"
//#include "approx_network_base.h"

//namespace approx_noc {

using namespace tlm;
/**
 * @brief       a class that contains network interface abstraction.
 */
class approx_ni_base {

    protected:
        /// Constructor
        approx_ni_base(int fw, sc_time clk) 
          : m_flit_width(fw),
            m_clk(clk) {}
        
        int m_flit_width;           //< flit width
        sc_time m_clk;              //< clock period
};

class approx_dest_ni_base : public approx_ni_base,
public tlm_blocking_put_if < flit > {
  public:
    virtual void put(const flit &t)=0;
  
  protected:
    approx_dest_ni_base(int fw, sc_time clk)
      : approx_ni_base(fw, clk) {}
  
};

class approx_source_ni_base : public approx_ni_base {
  public:
    sc_port < tlm_blocking_put_if < flit > > flit_out;
  
  protected:
    approx_source_ni_base(int fw, sc_time clk)
      : approx_ni_base (fw,clk) {}
};
//} //namespace approx_noc
#endif //__APPROX_NOC_APPROX_NI_BASE_H__
