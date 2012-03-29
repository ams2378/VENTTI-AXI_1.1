/**
 * @filename  dest_ni.cc
 *
 * @brief     File that includes TLM model of SystemC destination 
 *            network interface (i.e. receiver or sink module) for Rabbits.
 *
 * @author    Young Jin Yoon <youngjin@cs.columbia.edu>
 *
 * @note      This file will be included in dest_ni.h
 * 
 * @sa        dest_ni.h
 */

//namespace approx_noc {

/**
 * Constructor for the class
 *
 * @param[in] nb            a reference of network_base class
 * @param[in] clk           one clock cycle delay represented by a sc_time object
 * @param[in] pid           rabbit identifier either slave or master id.
 * @param[in] flit_width    flit width of the network
 * @param[in] num_out_trans total number of outstanding transaction
 * @param[in] _n            name of the interface module
 */

slave_flit2axi_dest_ni::slave_flit2axi_dest_ni (sc_time clk, int flit_width, sc_module_name _n)
  : sc_module( _n ),
    approx_dest_ni_base(flit_width, clk)
{
    //this->trans_fifo.nb_bound(num_out_trans); //because current_trans holds one more transaction
    head_flit.flit_id = -1;
}

/**
 * check whether the current flit is the tail flit of a transaction, 
 * and retrieve the corresponding transaction to send it to the destination.
 *
 * @note: template specialization for vci_request 
 * @note: source, destination ids are counted as the part of a payload.
 * @sa: vci_request
 */

#if 0
// MY COMMENTS
//MY CODE
axi_waddr slave_flit2axi_dest_ni::get(tlm_tag< axi_waddr > *t)
{
	if(this->dest_axi_waddr_fifo.used()==0)
	{
		axi_waddr temp;
		temp.slave_id = -1;
		return temp;
	}
	else
		return 	this->dest_axi_waddr_fifo.get();
}

axi_raddr slave_flit2axi_dest_ni::get(tlm_tag< axi_raddr > *t)
{
	if(this->dest_axi_raddr_fifo.used()==0)
	{
		axi_raddr temp;
		temp.slave_id = -1;
		return temp;
	}
	else
		return 	this->dest_axi_raddr_fifo.get();
}

axi_wdata slave_flit2axi_dest_ni::get(tlm_tag< axi_wdata > *t)
{
	if(this->dest_axi_wdata_fifo.used()==0)
	{
		axi_wdata temp;
		temp.n_bits = -1;
		return temp;
	}
	else
		return 	this->dest_axi_wdata_fifo.get();
}

#endif
// END OF MY COMMENTS


void slave_flit2axi_dest_ni::check_tail(const flit &t)
{
#if 0
// MY COMMENTS
    if(t.is_tail == true) { //No more flit will be expected. transaction is over.

	/* read from five map and combine the request into a single trans*/

        const vci_request &trans = get_approx_network_base()->get_req_trans(this->get_rabbit_id(t.src_id), t.trans_id);
        //cout << "[DEBUG]: vci_request sent from " << name() << " to the slave --" << trans << endl;
        //this->trans_fifo.put(trans);
        this->fifo_put(trans); //< put and record transaction
        get_approx_network_base()->erase_req_trans(this->get_rabbit_id(t.src_id), t.trans_id);
        head_flit.flit_id = -1;
    }
#endif
// END OF MY COMMENTS
	if(t.is_tail == true) {
		if (t.type == WADDR){
			cout<<"SLAVE_FLIT2AXI_DEST: Received last flit of WADDR packet @ "<<sc_time_stamp()<<endl;
		}
		else
		{
			if (t.type == WDATA){
				cout<<"SLAVE_FLIT2AXI_DEST: Received last flit of WDATA packet @ "<<sc_time_stamp()<<endl;
			}
			else
			{
				if (t.type == RADDR){
					cout<<"SLAVE_FLIT2AXI_DEST: Received last flit of RADDR packet @ "<<sc_time_stamp()<<endl;
					axi_raddr trans_raddr;
					trans_raddr.address = 1;
					trans_raddr.arlen =1;
					// !!!!! THE ABOVE VALUES MUST BE CHANGED ACCORING TO THE FLIT INPUT !!!!!!!!!
					axi_raddr_port->put(trans_raddr);
					cout<<"SLAVE_FLIT2AXI_DEST: sent RADDR packet to axi2vci dest @ "<<sc_time_stamp() <<endl;
				}
				else{
					cout<<"SLAVE_FLIT2AXI_DEST: Received last flit of unidentified packet @ "<<sc_time_stamp()<<endl;
				}
			}
		}

	        head_flit.flit_id = -1;
		// !!!! PUT TO NEXT STEP!!!!!!!!!!!!!!!!
	}
	else{
		cout<<"SLAVE_FLIT2AXI_DEST: Received flit#"<<t.flit_id<<" of packet @ "<<sc_time_stamp()<<endl;
	}
}

/*
 * check whether the current flit is the tail flit of a transaction, 
 * and retrieve the corresponding transaction to send it to the destionation.
 *
 * @note: template specialization for vci_response
 * @note: only request source id (i.e. dest_id in terms of network layer) is  
 * counted as the part of a payload.
 * @sa: vci_response
 */
#if 0
// MY COMMENTS
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
template <typename trans_type>
void dest_ni<trans_type>::check_tail(const flit &t)
{
   std::cout << "[ERROR] The template class dest_ni can only process vci_request and vci_response classes." << std::endl; 
   sc_stop();
}
#endif
// END OF MY COMMENTS

/**
 * implement the put interface of tlm_blocking_put < flit >
 */

void slave_flit2axi_dest_ni::put(const flit &t)
{
    //std::cout << "[DEBUG]: flit received in " << name() << "--" << t << endl;

    assert(t.flit_id == (head_flit.flit_id +1));

#if 0
// MY COMMENTS
// MY CODE VERSION 1
	if (t.type == RADDR)
	{
		axi_raddr temp_raddr;
		this->axi_raddr_port.put(temp_raddr);
	}
	else
	{
		if (t.type == WADDR)
		{
			axi_raddr temp_raddr;
			this->axi_raddr_port.put(temp_raddr);
		}	
		else
		{
			if (t.type == WADDR)
			{
				axi_raddr temp_raddr;
				this->axi_raddr_port.put(temp_raddr);
			}
			else
				cout<<"SLAVE_FLIT2AXI_DEST: Received flit of incorrect type @ "<<sc_time_stamp()<<endl;
		}
	}

// MY CODE VERSION 1
	if (t.type == RADDR)
	{
// !!!!!!!!!!! MUST CHANGE CHECKING ACCORDING TO FIFOS SIZE !!!!!!!!!!!
		if(this->dest_axi_raddr_fifo.used() ==dest_axi_raddr_fifo.size())  {
			cout << "[DEBUG] FIFO full, waiting..." << endl;
			wait(this->dest_axi_raddr_fifo.ok_to_put()); //cannot proceeed if the buffer is full
		}
	}
	else
		if (t.type == WADDR)
		{

			if(this->dest_axi_waddr_fifo.used() == 1)  {
				cout << "[DEBUG] FIFO full, waiting..." << endl;
				wait(this->dest_axi_waddr_fifo.ok_to_put()); //cannot proceeed if the buffer is full
			}
		}
		else
			if (t.type == WDATA)
			{

				if(this->dest_axi_wdata_fifo.used() == 1)  {
					cout << "[DEBUG] FIFO full, waiting..." << endl;
					wait(this->dest_axi_wdata_fifo.ok_to_put()); //cannot proceeed if the buffer is full
				}
			}
			else
				cout<<"SLAVE_FLIT2AXI_DEST: Received flit of incorrect type @ "<<sc_time_stamp()<<endl;


// THIS IS PREVIOUS CODE
    assert(this->get_rabbit_id(t.dest_id) == this->m_pid);
    if(this->trans_fifo.used() == this->m_num_out_trans)  {
        //cout << "[DEBUG] FIFO full, waiting..." << endl;
        wait(this->trans_fifo.ok_to_put()); //cannot proceeed if the buffer is full
    }
#endif
// END OF MY COMMENTS
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
//} //namespace approx_noc
