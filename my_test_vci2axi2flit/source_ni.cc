/**
 * @filename  source_ni.cc
 *
 * @brief     File that includes TLM model of SystemC source 
 *            network interface (i.e. sender module) for Rabbits.
 *
 * @author    Young Jin Yoon <youngjin@cs.columbia.edu>
 *
 * @note      This file will be included in source_ni.h
 * 
 * @sa        source_ni.h
 */


namespace approx_noc {

/**
 * Constructor for the class
 *
 * @param[in] nb            a reference of network_base class
 * @param[in] clk           one clock cycle delay represented by a sc_time object
 * @param[in] pid           rabbit identifier either slave or master id.
 * @param[in] flit_width    flit width of the network
 * @param[in] num_out_trans total number of outstanding transaction
 * @param[in] _n            name of the router module
 */
template <typename trans_type>
source_ni<trans_type>::source_ni (approx_network_base *nb, sc_time clk, int pid, int flit_width, int num_out_trans, sc_module_name _n)
  : sc_module( _n ),
    abstract_noc::source_ni_base<trans_type>(nb, num_out_trans, pid),
    approx_source_ni_base(flit_width, clk)

{
    //this->trans_fifo.nb_bound(num_out_trans); //because current_trans holds one more transaction

    SC_THREAD( process_main );
}

/**
 * Main function that gets the transaction and divide it into multiple flits.
 */
template <typename trans_type>
void source_ni<trans_type>::process_main()
{
    while(true) {
        trans_type current_trans = this->trans_fifo.peek(); // "PEEK" the first transaction
        
        //divide a transaction into multiple flits
        std::vector< flit > flit_vector;
        fliticize (flit_vector, current_trans);
        
        //Align transaction to the clock of the network.
        const sc_dt::uint64 current = sc_time_stamp().value();
        const sc_dt::uint64 g_quant = m_clk.value();
        const sc_dt::uint64 tmp = (current/g_quant+sc_dt::uint64(1)) * g_quant;
        const sc_time remainder = sc_time(tmp - current, false);
        assert(remainder <= m_clk);

        //Only call wait with remainder if it is smaller than the clock cycle.
        if(remainder < m_clk) wait(remainder);
        
        //send flit over the interface
        std::vector< flit >::iterator it;
        for(it=flit_vector.begin(); it != flit_vector.end(); it++) {
            wait(m_clk);
            flit_out->put(*it);
        }

        this->fifo_get(); // "DONE" with the transaction, so pop it out from the tlm_fifo
    }
}

/**
 * Divide a transaction into multiple flits
 *
 * @note: template specialization for vci_request 
 * @note: source, destination ids are counted as the part of a payload.
 * @sa: vci_request
 */
template <>
void source_ni<vci_request>::fliticize(std::vector< flit > &v, vci_request &trans)
{
    int total_bytes = get_num_bits(trans);
    int num_flits;
  
    num_flits = total_bytes / m_flit_width;
    num_flits += (((total_bytes % m_flit_width == 0)) && (num_flits != 0))?0:1;
    assert(num_flits > 0);    
    int trans_id = get_approx_network_base()->set_req_trans(trans.srcid, trans);
    
    for(int i=0; i<num_flits; i++) {
        flit toPut(trans_id, get_cosi_id_from_mstr(trans.srcid), get_cosi_id_from_slave(trans.slave_id), i, (i == (num_flits - 1)));
        v.push_back(toPut);
    }
    assert(v.size() == num_flits);
    assert(v[v.size()-1].is_tail == true);
}

/**
 * Divide a transaction into multiple flits
 *
 * @note: template specialization for vci_response
 * @note: only request source id (i.e. dest_id in terms of network layer) is  
 * counted as the part of a payload.
 * @sa: vci_response
 */
template <>
void source_ni<vci_response>::fliticize(std::vector< flit > &v, vci_response &trans)
{
    int total_bytes = get_num_bits(trans);

    int num_flits;
    
    num_flits = total_bytes / m_flit_width;
    num_flits += (((total_bytes % m_flit_width == 0)) && (num_flits != 0))?0:1;
    assert(num_flits > 0);    
    int trans_id = get_approx_network_base()->set_res_trans(m_pid, trans);

    for(int i=0; i<num_flits; i++) {
        flit to_put(trans_id, get_cosi_id_from_slave(m_pid), get_cosi_id_from_mstr(trans.rsrcid), i, (i == (num_flits - 1)));
        v.push_back(to_put);
    }
    assert(v.size() == num_flits);
    assert(v[v.size()-1].is_tail == true);
}
} //namespace approx_noc
