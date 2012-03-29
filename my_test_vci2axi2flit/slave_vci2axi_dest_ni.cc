/**
 * @filename  vci2axi_dest_ni.cc
 *
 * @brief     File that includes TLM model of SystemC destination 
 *            network interface (i.e. receiver or sink module) for Rabbits.
 *
 * @author    George Faldamis
 *
 * @note      This file will be included in vci2axi_dest_ni.h
 * 
 * @sa        vci2axi_dest_ni.h
 */

//namespace approx_axi_noc {

/**
 * Constructor for the class
 *
 * @param[in] nb            a reference of network_base class
 * @param[in] pid           rabbit identifier either slave or slave id.
 * @param[in] num_out_trans total number of outstanding transaction
 * @param[in] _n            name of the interface module
 */
//template <typename trans_type>
slave_vci2axi_dest_ni::slave_vci2axi_dest_ni (int pid, int num_out_trans, sc_module_name _n)
  : sc_module( _n ),
    dest_ni_base<vci_request>(num_out_trans, pid)
{
    //this->trans_fifo.nb_bound(num_out_trans); //because current_trans holds one more transaction
    //head_flit.flit_id = -1;
    SC_THREAD( waddr_process );
    SC_THREAD( raddr_process );
    SC_THREAD( wdata_process );
}

inline void slave_vci2axi_dest_ni::put(const axi_wdata &trans) 
{
    //bool ret_val = start_transaction(const_cast<trans_type &>(trans));
    //if(ret_val)
	cout<<"MASTER_SOURCE_AXI2FLIT: fifo received wdata packet @ "<<sc_time_stamp() <<endl;
      axi_wdata_fifo.put(trans);
}

inline void slave_vci2axi_dest_ni::put(const axi_waddr &trans) 
{
    //bool ret_val = start_transaction(const_cast<trans_type &>(trans));
    //if(ret_val)
	cout<<"MASTER_SOURCE_AXI2FLIT: fifo received waddr packet @ "<<sc_time_stamp() <<endl;
      axi_waddr_fifo.put(trans);
}

inline void slave_vci2axi_dest_ni::put(const axi_raddr &trans) 
{
    //bool ret_val = start_transaction(const_cast<trans_type &>(trans));
    //if(ret_val)
	cout<<"SLAVE_DEST_AXI2FLIT: fifo received raddr packet @ "<<sc_time_stamp() <<endl;
      axi_raddr_fifo.put(trans);
}

void slave_vci2axi_dest_ni::raddr_process()
{
	axi_raddr temp;
	vci_request temp_vci;
	cout<<"SLAVE_DEST_AXI2FLIT: raddr_process up!!" <<endl;
	while(1)
	{
		if (this->axi_raddr_fifo.used()==0)
		{	
			wait(this->axi_raddr_fifo.ok_to_get());
		}
		temp = this->axi_raddr_fifo.get();
		cout<<"SLAVE_DEST_AXI2FLIT: forwarded raddr packet @ "<<sc_time_stamp() <<endl;
		temp_vci.address= 32;
		this->trans_fifo.put(temp_vci);
	}
}

void slave_vci2axi_dest_ni::waddr_process()
{
	axi_waddr temp;
	cout<<"SLAVE_DEST_AXI2FLIT: waddr_process up!!" <<endl;
	while(1)
	{
		if (this->axi_waddr_fifo.used()==0)
		{	
			wait(this->axi_waddr_fifo.ok_to_get());
		}
		temp = this->axi_waddr_fifo.get();
		cout<<"SLAVE_DEST_AXI2FLIT: forwarded waddr packet @ "<<sc_time_stamp() <<endl;
	}
}

void slave_vci2axi_dest_ni::wdata_process()
{
	cout<<"SLAVE_DEST_AXI2FLIT: wdata_process up!!" <<endl;
	axi_wdata temp;
	while(1)
	{
		if (this->axi_wdata_fifo.used()==0)
		{	
			wait(this->axi_wdata_fifo.ok_to_get());
		}
		temp = this->axi_wdata_fifo.get();
		cout<<"SLAVE_DEST_AXI2FLIT: forwarded wdata packet @ "<<sc_time_stamp() <<endl;
	}
}
			

/**
 * check whether the current flit is the tail flit of a transaction, 
 * and retrieve the corresponding transaction to send it to the destination.
 *
 * @note: template specialization for vci_request 
 * @note: source, destination ids are counted as the part of a payload.
 * @sa: vci_request
 */
/*
template <>
void dest_ni<vci_request>::check_tail(const flit &t)
{
    if(t.is_tail == true) { //No more flit will be expected. transaction is over.
        const vci_request &trans = get_approx_network_base()->get_req_trans(this->get_rabbit_id(t.src_id), t.trans_id);
        //cout << "[DEBUG]: vci_request sent from " << name() << " to the slave --" << trans << endl;
        //this->trans_fifo.put(trans);
        this->fifo_put(trans); //< put and record transaction
        get_approx_network_base()->erase_req_trans(this->get_rabbit_id(t.src_id), t.trans_id);
        head_flit.flit_id = -1;
    }
}
*/
/*
 * check whether the current flit is the tail flit of a transaction, 
 * and retrieve the corresponding transaction to send it to the destionation.
 *
 * @note: template specialization for vci_response
 * @note: only request source id (i.e. dest_id in terms of network layer) is  
 * counted as the part of a payload.
 * @sa: vci_response
 */
/*
template <>
void dest_ni<vci_response>::check_tail(const flit &t)
{
    if(t.is_tail == true) { //No more flit will be expected. transaction is over.
        const vci_response &trans = get_approx_network_base()->get_res_trans(this->get_rabbit_id(t.src_id), t.trans_id);
        //cout << "[DEBUG]: vci_response sent from " << name() << " to the master --" << trans << endl;
        //this->trans_fifo.put(trans);
        this->fifo_put(trans);
        get_approx_network_base()->erase_res_trans(this->get_rabbit_id(t.src_id), t.trans_id);
        head_flit.flit_id = -1;
    }
}
*/

/**
 * check whether the current flit is the tail flit of a transaction, 
 * and retrieve the corresponding transaction to send it to the destionation.
 *
 * @note: This is a generic template implementation, and this function will 
 * not be executed if the template is instantiated with vci_request 
 * or vci_response.
 * @warning: the current implementation of dest_ni only accepts vci_request
 * and vci_response as a template. Thus, the program should not reach here.
 */
/*
template <typename trans_type>
void dest_ni<trans_type>::check_tail(const flit &t)
{
   std::cout << "[ERROR] The template class dest_ni can only process vci_request and vci_response classes." << std::endl; 
   sc_stop();
}
*/

/**
 * implement the put interface of tlm_blocking_put < flit >
 */
/*
template <typename trans_type>
void dest_ni<trans_type>::put(const flit &t)
{
    //std::cout << "[DEBUG]: flit received in " << name() << "--" << t << endl;
    assert(t.flit_id == (head_flit.flit_id +1));
    assert(this->get_rabbit_id(t.dest_id) == this->m_pid);
    if(this->trans_fifo.used() == this->m_num_out_trans)  {
        //cout << "[DEBUG] FIFO full, waiting..." << endl;
        wait(this->trans_fifo.ok_to_put()); //cannot proceeed if the buffer is full
    }
    if (t.flit_id == 0) { // First flit detected: save it into head_flit
        head_flit = t;
    } else { //second to the last flit: compare all infos from the head_flit
        assert(head_flit.trans_id == t.trans_id 
            && head_flit.src_id == t.src_id
            && head_flit.dest_id == t.dest_id);
        head_flit.flit_id++;
    }
    
    check_tail(t); //check whether the current flit is a tail flit or not.
}
*/

#if 0
/**
 * implement the get interface of tlm_blocking_get < typename trans_type >
 * @note: trans_type will be either vci_request or vci_response.
 */
template <typename trans_type>
trans_type dest_ni<trans_type>::get(tlm_tag< trans_type > *t)
{
    return this->trans_fifo.get(t); // simply return the result of this->trans_fifo.get()
}
#endif
//} //namespace approx_axi_noc
