/**
 * @filename  approx_network_base.cpp
 *
 * @brief     File that includes the base network implementation of Rabbits.
 *
 * @author    Young Jin Yoon <youngjin@cs.columbia.edu>
 *
 * @sa        approx_network_base.h
 */


#include "approx_network_base.h"
#include "master_vci2axi_source_ni.h"
/*
#include "ni_base.h"
#include "source_ni.h"
#include "dest_ni.h"
#include "approx_ni_base.h"
*/

//namespace approx_noc {

/**
 * Constructor for the class
 *
 * @param[in] clk             one clock cycle delay represented by a sc_time object
 * @param[in] flit_width      flit width of the network
 * @param[in] n_masters       total number of masters in the system.
 * @param[in] n_slaves        total number of slaves in the system.
 * @param[in] src_queue_size  default number of outstanding transaction for source_ni 
 * @param[in] dest_queue_size default number of outstanding transaction for dest_ni 
 * @param[in] _n              name of the network module
 */
approx_network_base::approx_network_base (sc_time clk, int flit_width, int n_masters, int n_slaves, bool sample_bandwidth, bool sample_latency, bool print_trans, bool sample_energy, int src_queue_size, int dest_queue_size, sc_module_name _n)
  : sc_module( _n ),
    abstract_noc::network_base(n_masters, n_slaves, sample_bandwidth, sample_latency, print_trans, sample_energy),
    m_clk ( clk ),
    m_flit_width ( flit_width )
{
#if 0    
    for(int i=0; i< m_n_masters+m_n_slaves; i++) {
        if(i < m_n_masters) {
            map<int, vci_request> toPut;
            
            m_req_trans_map.push_back(toPut);
            stringstream ss1;
            ss1 << "source_ni(mstr="<< i << ",cosi=" << i << ")";
            source_ni<vci_request> *s_ni = new source_ni<vci_request>(this, m_clk, i, m_flit_width, src_queue_size, sc_gen_unique_name(ss1.str().c_str())); 
            
            stringstream ss2;
            ss2 << "dest_ni(mstr="<< i << ",cosi=" << i << ")";
            dest_ni<vci_response> *d_ni = new dest_ni<vci_response>(this, m_clk, i, m_flit_width, dest_queue_size, sc_gen_unique_name(ss2.str().c_str()));
            
            m_source_ni[i] = s_ni;
            m_dest_ni[i] = d_ni;
        } else {
            map<int, vci_response> toPut;
            
            m_res_trans_map.push_back(toPut);
            stringstream ss1;
            ss1 << "source_ni(slave="<< i-m_n_masters << ",cosi=" << i << ")";
            source_ni<vci_response> *s_ni = new source_ni<vci_response>(this, m_clk, i-m_n_masters, m_flit_width, src_queue_size, sc_gen_unique_name(ss1.str().c_str()));
     
            stringstream ss2;
            ss2 << "dest_ni(slave=" << i-m_n_masters << ",cosi=" << i << ")";
            dest_ni<vci_request> *d_ni = new dest_ni<vci_request>(this, m_clk, i-m_n_masters, m_flit_width, dest_queue_size, sc_gen_unique_name(ss2.str().c_str()));
            m_source_ni[i] = s_ni;
            m_dest_ni[i] = d_ni;
        }
    }
#endif
		int i=0;

            map<int, vci_request> toPut;
            
            m_req_trans_map.push_back(toPut);
            stringstream ss1;
            ss1 << "master_vci2axi_source_ni(mstr="<< i << ",cosi=" << i << ")";
            master_vci2axi_source_ni<vci_request> *s_ni = new master_vci2axi_source_ni<vci_request>(this, 0, 1, sc_gen_unique_name(ss1.str().c_str())); 
            m_source_ni[i] = s_ni;
}

/**
 * Destructor for the class
 */
approx_network_base::~approx_network_base ()
{
#if 0
// MY COMMENTS
    std::map<unsigned int, abstract_noc::ni_base *>::iterator sit;
    for(sit = this->m_source_ni.begin(); sit != this->m_source_ni.end(); sit++) {
        delete sit->second;
    }
    std::map<unsigned int, abstract_noc::ni_base *>::iterator dit;
    for(dit = this->m_dest_ni.begin(); dit != this->m_dest_ni.end(); dit++) {
        delete dit->second;
    }
#endif
;
}


#if 0
// MY COMMENTS
approx_source_ni_base *approx_network_base::get_source_ni(unsigned int cosi_id)
{
  assert(m_source_ni.find(cosi_id) != m_source_ni.end());
  approx_source_ni_base *retVal = dynamic_cast< approx_source_ni_base *>(m_source_ni[cosi_id]);
  assert(retVal != NULL);
  return retVal;
}

approx_dest_ni_base *approx_network_base::get_dest_ni(unsigned int cosi_id)
{
  assert(m_dest_ni.find(cosi_id) != m_dest_ni.end());
  approx_dest_ni_base *retVal = dynamic_cast< approx_dest_ni_base *>(m_dest_ni[cosi_id]);
  assert(retVal != NULL);
  return retVal;
}
#endif

#if 0
//	WRITTEN FROM BEFORE
/**
 * return the source network interface of a master with given cosi identifier
 * @param[in]   cosi_id     cosi identifier for a master
 * @return                  source network interface with template vci_request
 */
inline source_ni<vci_request> *
approx_network_base::get_mstr_source_ni(unsigned int cosi_id)
{
    // Sanity check with cosi_id
    assert(m_source_ni.find(cosi_id) != m_source_ni.end());
    // return an actual object
    return static_cast< source_ni<vci_request> *>(m_source_ni[cosi_id]);
}

/**
 * return the destination network interface of a master with given cosi identifier
 * @param[in]   cosi_id     cosi identifier for a master
 * @return                  destination network interface with template vci_response
 */
inline dest_ni<vci_response> *
approx_network_base::get_mstr_dest_ni(unsigned int cosi_id)
{
    //Sanity check with the cosi_id
    assert(m_dest_ni.find(cosi_id) != m_dest_ni.end());
    // return an actual object
    return static_cast< dest_ni<vci_response> *>(m_dest_ni[cosi_id]);
}
    
/**
 * return the source network interface of a slave with given cosi identifier
 * @param[in]   cosi_id     cosi identifier for a master
 * @return                  source network interface with template vci_response
 */
inline source_ni<vci_response> *
approx_network_base::get_slave_source_ni(unsigned int cosi_id)
{
    //Sanity check with the cosi_id
    assert(m_source_ni.find(cosi_id) != m_source_ni.end());
    // return an actual object
    return static_cast< source_ni<vci_response> *>(m_source_ni[cosi_id]);
}

/**
 * return the destination network interface of a slave with given cosi identifier
 * @param[in]   cosi_id     cosi identifier for a master
 * @return                  destination network interface with template vci_request
 */
inline dest_ni<vci_request> *
approx_network_base::get_slave_dest_ni(unsigned int cosi_id)
{
    //Sanity check with the cosi_id
    assert(m_dest_ni.find(cosi_id) != m_dest_ni.end());
    // return an actual object
    return static_cast< dest_ni<vci_request> *>(m_dest_ni[cosi_id]);
}
#endif
//} //namespace approx_noc
