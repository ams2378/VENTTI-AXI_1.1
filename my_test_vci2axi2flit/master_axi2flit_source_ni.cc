/**
 * @filename  master_axi2flit_source_ni.cc
 *
 * @brief     File that includes TLM model of SystemC source 
 *            network interface (i.e. sender module) for Rabbits.
 *
 * @author    George Faldamis
 *
 * @note      This file will be included in master_axi2flit_source_ni.h
 * 
 * @sa        master_axi2flit_source_ni.h
 */


//namespace approx_axi_noc {

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
#if 0
// MY COMMENTS
template <typename trans_type>
master_axi2flit_source_ni<trans_type>::master_axi2flit_source_ni (approx_network_base *nb, sc_time clk, int flit_width, sc_module_name _n)
  : sc_module( _n ),
    approx_source_ni_base(flit_width, clk),
    axi_waddr_fifo(1), axi_wdata_fifo(1), axi_rdata_fifo(1)

{
    //this->trans_fifo.nb_bound(num_out_trans); //because current_trans holds one more transaction
    m_nb=nb;
    SC_THREAD( process_main );
}
#endif
// END OF MY COMMENTS
master_axi2flit_source_ni::master_axi2flit_source_ni ( sc_time clk, int flit_width, sc_module_name _n)
  : sc_module( _n ),
    approx_source_ni_base(flit_width, clk),
    axi_waddr_fifo(1), axi_wdata_fifo(1), axi_raddr_fifo(1)

{
    //this->trans_fifo.nb_bound(num_out_trans); //because current_trans holds one more transaction
	temp_trans_id_cnt=0;
    SC_THREAD( process_main );
}


/**
 * Main function that gets the transaction and divide it into multiple flits.
 */
void master_axi2flit_source_ni::process_main()
{
    while(true) {

	axi_waddr waddr_receive;
	axi_wdata wdata_receive;	
	axi_raddr raddr_receive;

	cout << "MASTER_SOURCE_AXI2FLIT: waiting to receive new data @ "<< sc_time_stamp() <<endl;

	std::vector< flit > flit_vector_waddr;
        std::vector< flit > flit_vector_wdata;
        std::vector< flit > flit_vector_raddr;
        std::vector< flit >::iterator it;
#if 0
// MY COMMENTS

	if (axi_waddr_fifo.nb_read(waddr_receive)){

		fliticize (flit_vector_waddr, waddr_receive);
#if 0
// MY COMMENTS
        //Align transaction to the clock of the network.
        const sc_dt::uint64 current = sc_time_stamp().value();
        const sc_dt::uint64 g_quant = m_clk.value();
        const sc_dt::uint64 tmp = (current/g_quant+sc_dt::uint64(1)) * g_quant;
        const sc_time remainder = sc_time(tmp - current, false);
        assert(remainder <= m_clk);


        //Only call wait with remainder if it is smaller than the clock cycle.
        if(remainder < m_clk) wait(remainder);
#endif
// END OF MY COMMENTS

       		 for(it=flit_vector_waddr.begin(); it != flit_vector_waddr.end(); it++) {
      	 	     wait(m_clk);
      	 	     flit_out->put(*it);
      	 	 }
	}	
        
	if (axi_waddr_fifo.nb_read(wdata_receive)){

		fliticize (flit_vector_wdata, wdata_receive);
#if 0
// MY COMMENTS
        //Align transaction to the clock of the network.
        const sc_dt::uint64 current = sc_time_stamp().value();
        const sc_dt::uint64 g_quant = m_clk.value();
        const sc_dt::uint64 tmp = (current/g_quant+sc_dt::uint64(1)) * g_quant;
        const sc_time remainder = sc_time(tmp - current, false);
        assert(remainder <= m_clk);

        //Only call wait with remainder if it is smaller than the clock cycle.
        if(remainder < m_clk) wait(remainder);
#endif
// END OF MY COMMENTS
       		 for(it=flit_vector_wdata.begin(); it != flit_vector_wdata.end(); it++) {
      	 	     wait(m_clk);
      	 	     flit_out->put(*it);
      	 	 }
	}	

	if (axi_waddr_fifo.nb_read(raddr_receive)){

		fliticize (flit_vector_raddr, raddr_receive);
#if 0
// MY COMMENTS
        //Align transaction to the clock of the network.
        const sc_dt::uint64 current = sc_time_stamp().value();
        const sc_dt::uint64 g_quant = m_clk.value();
        const sc_dt::uint64 tmp = (current/g_quant+sc_dt::uint64(1)) * g_quant;
        const sc_time remainder = sc_time(tmp - current, false);
        assert(remainder <= m_clk);

        //Only call wait with remainder if it is smaller than the clock cycle.
        if(remainder < m_clk) wait(remainder);
#endif
// END OF MY COMMENTS
       		 for(it=flit_vector_raddr.begin(); it != flit_vector_raddr.end(); it++) {
      	 	     wait(m_clk);
      	 	     flit_out->put(*it);
      	 	 }
	}
#endif
// END OF MY COMMENTS	
//        this->fifo_get(); // "DONE" with the transaction, so pop it out from the tlm_fifo
	
	raddr_receive = axi_raddr_fifo.peek();
	cout << "MASTER_SOURCE_AXI2FLIT: received new data @ "<< sc_time_stamp() <<endl;
	fliticize (flit_vector_raddr, raddr_receive);
	cout << "MASTER_SOURCE_AXI2FLIT: fliticized data @ "<< sc_time_stamp() <<" and sending per "<< m_clk <<endl;
	for(it=flit_vector_raddr.begin(); it != flit_vector_raddr.end(); it++) {
		cout << "MASTER_SOURCE_AXI2FLIT: sending flit#" << it->flit_id<<" @ "<< sc_time_stamp() <<endl;
 	     wait(m_clk);
 	     flit_out->put(*it);
 	}
	cout << "MASTER_SOURCE_AXI2FLIT: done sending packet @ "<< sc_time_stamp() <<endl;
	raddr_receive = axi_raddr_fifo.get();
    }
}

/**
 * Divide a transaction into multiple flits
 *
 * @note: template specialization for vci_request 
 * @note: source, destination ids are counted as the part of a payload.
 * @sa: vci_request
 */
void master_axi2flit_source_ni::fliticize(std::vector< flit > &v, axi_waddr &trans)
{
    int total_bytes = get_num_bits_waddr();
    int num_flits;
  
    num_flits = total_bytes / m_flit_width;
    num_flits += (((total_bytes % m_flit_width == 0)) && (num_flits != 0))?0:1;
    assert(num_flits > 0);    
#if 0
// MY COMMENTS
    int trans_id = get_approx_network_base()->set_req_trans(trans.srcid, trans);			// done

    
    for(int i=0; i<num_flits; i++) {
        flit toPut(trans_id, get_cosi_id_from_mstr(trans.srcid), get_cosi_id_from_slave(trans.slave_id), i, (i == (num_flits - 1)), WADDR);
        v.push_back(toPut);
    }
    assert(v.size() == num_flits);
    assert(v[v.size()-1].is_tail == true);
#endif
// END OF MY COMMENTS

// CREATE HERE toPut flit AND PUT IT IN THE MAP
}

inline void master_axi2flit_source_ni::put(const axi_wdata &trans) 
{
    //bool ret_val = start_transaction(const_cast<trans_type &>(trans));
    //if(ret_val)
	cout<<"MASTER_SOURCE_AXI2FLIT: received wdata packet @ "<<sc_time_stamp() <<endl;
      axi_wdata_fifo.put(trans);
}

inline void master_axi2flit_source_ni::put(const axi_waddr &trans) 
{
    //bool ret_val = start_transaction(const_cast<trans_type &>(trans));
    //if(ret_val)
	cout<<"MASTER_SOURCE_AXI2FLIT: received waddr packet @ "<<sc_time_stamp() <<endl;
      axi_waddr_fifo.put(trans);
}

inline void master_axi2flit_source_ni::put(const axi_raddr &trans) 
{
    //bool ret_val = start_transaction(const_cast<trans_type &>(trans));
    //if(ret_val)
	cout<<"MASTER_SOURCE_AXI2FLIT: received raddr packet @ "<<sc_time_stamp() <<endl;
      axi_raddr_fifo.put(trans);
}

void master_axi2flit_source_ni::fliticize(std::vector< flit > &v, axi_wdata &trans)
{
    int total_bytes = get_num_bits_wdata();
    int num_flits;
  
    num_flits = total_bytes / m_flit_width;
    num_flits += (((total_bytes % m_flit_width == 0)) && (num_flits != 0))?0:1;
    assert(num_flits > 0);    
#if 0
// MY COMMENTS
    int trans_id = get_approx_network_base()->set_req_trans(trans.copy_of_srcid, trans);			// done

    
    for(int i=0; i<num_flits; i++) {
        flit toPut(trans_id, get_cosi_id_from_mstr(trans.srcid), get_cosi_id_from_slave(trans.slave_id), i, (i == (num_flits - 1)), WDATA);
        v.push_back(toPut);
    }

    for(int i=0; i<num_flits; i++) {
        flit toPut(temp_trans_id_cnt, 1, get_cosi_id_from_slave(trans.slave_id), i, (i == (num_flits - 1)), WDATA);
        v.push_back(toPut);
    }
    assert(v.size() == num_flits);
	temp_trans_id_cnt++;
    assert(v[v.size()-1].is_tail == true);
#endif
// END OF MY COMMENTS
}

/**
 * Divide a transaction into multiple flits
 *
 * @note: template specialization for vci_response
 * @note: only request source id (i.e. dest_id in terms of network layer) is  
 * counted as the part of a payload.
 * @sa: vci_response
 */
void master_axi2flit_source_ni::fliticize(std::vector< flit > &v, axi_raddr &trans)
{
    int total_bytes = get_num_bits_waddr();
    int num_flits;
  
    num_flits = total_bytes / m_flit_width;
    num_flits += (((total_bytes % m_flit_width == 0)) && (num_flits != 0))?0:1;
    assert(num_flits > 0);   
#if 0
// MY COMMENTS 
    int trans_id = get_approx_network_base()->set_req_trans(trans.srcid, trans);			// needs to be done
    
    for(int i=0; i<num_flits; i++) {
        flit toPut(trans_id, get_cosi_id_from_mstr(trans.srcid), get_cosi_id_from_slave(trans.slave_id), i, (i == (num_flits - 1)), RADDR);
        v.push_back(toPut);
    }
    assert(v.size() == num_flits);
    assert(v[v.size()-1].is_tail == true);
#endif
// END OF MY COMMENTS

    for(int i=0; i<num_flits; i++) {
        flit toPut(temp_trans_id_cnt, 1, 1, i, (i == (num_flits - 1)), RADDR);
        v.push_back(toPut);
    }
    assert(v.size() == num_flits);
	temp_trans_id_cnt++;
    assert(v[v.size()-1].is_tail == true);
}
//} //namespace approx_noc

/**
* total number of bits for each packet
*
*/
inline int  master_axi2flit_source_ni::get_num_bits_waddr(){

	int total_bits = 16+8+8+8+32+1+8+1+1+1+1+8+8+8+16;	 	
    return total_bits;
		
    }

inline int master_axi2flit_source_ni:: get_num_bits_wdata(){

	int total_bits = 16 + 8 + 8 + 64 + 1 + 8;

    return total_bits;
		
    }
// NEED FUNCTION HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
inline int master_axi2flit_source_ni::get_num_bits_raddr()
	{return 64;}

