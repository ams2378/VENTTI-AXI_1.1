/*
 *  Copyright (c) 2010 TIMA Laboratory
 *
 *  This file is part of Rabbits.
 *
 *  Rabbits is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Rabbits is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Rabbits.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _ABRSTACT_NOC_H
#define _ABRSTACT_NOC_H

#define MAP_FILES_DIR "./maps"

#include <stdint.h>
#include <systemc.h>
#ifdef USE_SYSTEMC_TLM
#include <tlm.h>
#endif

//namespace noc{
#ifdef USE_SYSTEMC_TLM
    using namespace tlm;
#endif
    // B = 8
    // K = 8
    // N = 32
    // E = 1
    // Q = 1
    // F = 1
    // S = 14
    // P = 4
    // T = 4
    // W = 1

    struct AWBURST{
	public:
	 	bool		cfixed;
		bool		wrap;	
		bool		cons;
		bool 		contig;						
	};

    struct ARBURST{
	public:
	 	bool		cfixed;
		bool		wrap;	
		bool		cons;
		bool 		contig;						
	};

    struct AWSIZE{
	public:
		bool            clen;
		uint8_t 	plen;			
	};

    struct ARSIZE{
	public:
		bool            clen;
		uint8_t 	plen;			
	};

    struct AWID{
	public:
        	uint16_t   	srcid;
       		uint8_t         trdid;		
        	uint8_t     	pktid;	
	};

    struct WID{
	public:
        	uint16_t   	srcid;
       		uint8_t         trdid;		
        	uint8_t     	pktid;	
	};

    struct ARID{
	public:
        	uint16_t   	srcid;
       		uint8_t         trdid;		
        	uint8_t     	pktid;	
	};

    class axi_waddr{
	public:
		 AWID		wawid;
		 uint32_t 	address;		// 32
		 uint8_t 	awlen;			// plen/8
		 AWSIZE		wawsize;
		 AWBURST	wawburst;
		 int 		awlock;
		 int 		awcache;
		 int		awport;

        	 uint32_t       initial_address;
    		 int            slave_id;
	};					

    class axi_raddr{
	public:
		 ARID		rarid;
		 uint32_t 	address;		// 32
		 uint8_t 	arlen;			// plen/8
		 ARSIZE		rarsize;
		 ARBURST	rarburst;
		 int 		arlock;		// cmd
		 int 		arcache;
		 int		arport;

	 	 uint32_t       initial_address;
    		 int            slave_id;
		// **************
		// NoC and Traffic Tracking
		int 		n_bits;
        	sc_time 	inj_time;
	};

    class axi_wdata{
	public:
		WID	    wwid;
		uint8_t     wdata[8];		
		uint8_t     wstrb;		//be
		bool 	    wlast;		// eop

		// **************
		// NoC and Traffic Tracking
		int 	    n_bits;
	        sc_time     inj_time;
	};

    class vci_request
    {
    public:
        uint32_t    address;
        uint8_t     be;
        uint8_t     cmd;
        bool        contig;
        uint8_t     wdata[8];
        bool        eop;
        bool        cons;
        uint8_t     plen;
        bool        wrap;
        bool        cfixed;
        bool        clen;
        uint16_t    srcid;
        uint8_t     trdid;
        uint8_t     pktid;

        uint32_t    initial_address;
        int         slave_id;

	// **************
	// NoC and Traffic Tracking
	int n_bits;
        sc_time inj_time;
#ifdef USE_SYSTEMC_TLM
        /**
        * equal operator overloading. 
        * @note make sure all member variables of the current object is compared to the rhs object
        */
        inline bool operator == (const vci_request& rhs) const {
            return (
                    rhs.address         == address
                &&  rhs.be              == be
                &&  rhs.cmd             == cmd
                &&  rhs.contig          == contig
                &&  rhs.wdata[0]        == wdata[0] 
                &&  rhs.wdata[1]        == wdata[1]
                &&  rhs.wdata[2]        == wdata[2]
                &&  rhs.wdata[3]        == wdata[3]
                &&  rhs.wdata[4]        == wdata[4]
                &&  rhs.wdata[5]        == wdata[5]
                &&  rhs.wdata[6]        == wdata[6]
                &&  rhs.wdata[7]        == wdata[7]
                &&  rhs.eop             == eop
                &&  rhs.cons            == cons
                &&  rhs.plen            == plen
                &&  rhs.wrap            == wrap
                &&  rhs.cfixed          == cfixed
                &&  rhs.clen            == clen
                &&  rhs.srcid           == srcid
                &&  rhs.trdid           == trdid
                &&  rhs.pktid           == pktid
                &&  rhs.initial_address == initial_address
                &&  rhs.slave_id        == slave_id
                &&  rhs.n_bits         == n_bits
                &&  rhs.inj_time        == inj_time
            );
        };

        /**
         * assignment operator overloading. 
         * @note make sure all member variables of the rhs object is assigned to the current object
         */
        inline vci_request& operator = (const vci_request& rhs) {
            address = rhs.address;
            be = rhs.be;
            cmd = rhs.cmd;
            contig = rhs.contig;
            for(int i=0; i<8; i++)
                wdata[i] = rhs.wdata[i];
            eop = rhs.eop;
            cons = rhs.cons;
            plen = rhs.plen;
            wrap = rhs.wrap;
            cfixed = rhs.cfixed;
            clen = rhs.clen;
            srcid = rhs.srcid;
            trdid = rhs.trdid;
            pktid = rhs.pktid;
            initial_address = rhs.initial_address;
            slave_id = rhs.slave_id;
            n_bits = rhs.n_bits;
            inj_time = rhs.inj_time;

            return *this;
        };

        /**
         * assignment operator overloading. 
         * @note this will be helpful later to trace information. Just make sure all 
         * member variables of the v object is used by sc_trace() function.
         */
        inline friend void sc_trace(sc_trace_file *tf, const vci_request & v, const std::string& NAME ) {
            sc_trace(tf, v.address, NAME + ".address");
            sc_trace(tf, v.be, NAME + ".be");
            sc_trace(tf, v.cmd, NAME + ".cmd");
            sc_trace(tf, v.contig, NAME + ".contig");
            for(int i=0; i<8; i++)
            {
                std::stringstream ss;
                ss << i;
                sc_trace(tf, v.wdata[i], NAME + ".wdata["+ss.str()+"]");
                ss.clear();
            }
            sc_trace(tf, v.eop, NAME + ".eop");
            sc_trace(tf, v.cons, NAME + ".cons");
            sc_trace(tf, v.plen, NAME + ".plen");
            sc_trace(tf, v.wrap, NAME + ".wrap");
            sc_trace(tf, v.cfixed, NAME + ".cfixed");
            sc_trace(tf, v.clen, NAME + ".clen");
            sc_trace(tf, v.srcid, NAME + ".srcid");
            sc_trace(tf, v.trdid, NAME + ".trdid");
            sc_trace(tf, v.pktid, NAME + ".pktid");
            sc_trace(tf, v.initial_address, NAME + ".initial_address");
            sc_trace(tf, v.slave_id, NAME + ".slave_id");
            sc_trace(tf, v.n_bits, NAME + ".n_bits");
            sc_trace(tf, v.inj_time.to_double(), NAME + ".inj_time");
        };

        /**
         * shift-left operator overloading.
         * @note this is necessary to print the contents of an object with std::cout. Just make sure
         * all member variables of the v object is "shift-lefted" with ostream class.
         */
        inline friend ostream& operator << ( ostream& os,  vci_request const & v ) {
            os << "(";
            os << "address=" << (long int)v.address;
            os << ", be=" << (long int)v.be;
            os << ", cmd=" << (long int)v.cmd;
            os << ", contig=" << (long int)v.contig;
            os << ", wdata={";
            for(int i=0; i<7; i++)
                os << (long int)v.wdata[i] << ", ";
            os << (long int)v.wdata[8] << "}";
            os << ", eop=" << (long int)v.eop;
            os << ", cons=" << (long int)v.cons;
            os << ", plen=" << (long int)v.plen;
            os << ", wrap=" << (long int)v.wrap;
            os << ", cfixed=" << (long int)v.cfixed;
            os << ", clen=" << (long int)v.clen;
            os << ", srcid=" << (long int)v.srcid;
            os << ", trdid=" << (long int)v.trdid;
            os << ", pktid=" << (long int)v.pktid;
            os << ", initial_address=" << (long int)v.initial_address;
            os << ", slave_id=" << (long int)v.slave_id;
            os << ", n_bits=" << (long int)v.n_bits;
            os << ", inj_time=" << v.inj_time;
            os << ")";
            return os;
        };
#endif //USE_SYSTEMC_TLM

    };
    enum {
        CMD_NOP,
        CMD_READ,
        CMD_WRITE,
        CMD_LOCKED_READ,
        CMD_STORE_COND = CMD_NOP,
    };

    struct BID{
	public:
        	uint16_t   	rsrcid;
       		uint8_t         rtrdid;		
        	uint8_t     	rpktid;	
	};

    struct RID{
	public:
        	uint16_t   	rsrcid;
       		uint8_t         rtrdid;		
        	uint8_t     	rpktid;	
	};

    class axi_wresp{
	public:
		BID      	rbid;
        	bool      	rerror;
	};

    class axi_rdata{
	public:
		RID	 	rrid;
        	uint8_t   	rdata[8];
        	bool      	rerror;
		bool		reop;

        // **************
        // Extra field
        	uint8_t   rbe;
	// **************
	// NoC and Traffic Tracking
		uint16_t slave_id; 
		uint8_t rcmd;
		int n_bits;
		sc_time inj_time;
	};


    class vci_response
    {
    public:
        uint8_t   rdata[8];
        bool      reop;
        bool      rerror;
        uint16_t  rsrcid;
        uint8_t   rtrdid;
        uint8_t   rpktid;

        // **************
        // Extra field
        uint8_t   rbe;
	// **************
	// NoC and Traffic Tracking
	uint16_t slave_id; // sending slave
	uint8_t rcmd;
	int n_bits;
	sc_time inj_time;
#ifdef USE_SYSTEMC_TLM
        /**
        * equal operator overloading. 
        * @note make sure all member variables of the current object is compared to the rhs object
        */
        inline bool operator == (const vci_response& rhs) const {
            return (
                    rhs.rdata[0]    == rdata[0] 
                &&  rhs.rdata[1]    == rdata[1]
                &&  rhs.rdata[2]    == rdata[2]
                &&  rhs.rdata[3]    == rdata[3]
                &&  rhs.rdata[4]    == rdata[4]
                &&  rhs.rdata[5]    == rdata[5]
                &&  rhs.rdata[6]    == rdata[6]
                &&  rhs.rdata[7]    == rdata[7]
                &&  rhs.reop        == reop
                &&  rhs.rerror      == rerror
                &&  rhs.rsrcid      == rsrcid
                &&  rhs.rtrdid      == rtrdid
                &&  rhs.rpktid      == rpktid
                &&  rhs.rbe         == rbe
                &&  rhs.slave_id    == slave_id
                &&  rhs.rcmd        == rcmd
                &&  rhs.n_bits     == n_bits
                &&  rhs.inj_time     == inj_time
            );
        };

        /**
         * assignment operator overloading. 
         * @note make sure all member variables of the rhs object is assigned to the current object
         */
        inline vci_response& operator = (const vci_response& rhs) {
            for(int i=0; i<8; i++)
                rdata[i] = rhs.rdata[i];
            
            reop = rhs.reop;
            rerror = rhs.rerror;
            rsrcid = rhs.rsrcid;
            rtrdid = rhs.rtrdid;
            rpktid = rhs.rpktid;
            rbe = rhs.rbe;
            slave_id = rhs.slave_id;
            rcmd = rhs.rcmd;
            n_bits = rhs.n_bits;
            inj_time = rhs.inj_time;
            
            return *this;
        };

        /**
         * assignment operator overloading. 
         * @note this will be helpful later to trace information. Just make sure all 
         * member variables of the v object is used by sc_trace() function.
         */
        inline friend void sc_trace(sc_trace_file *tf, const vci_response & v, const std::string& NAME ) {
            for(int i=0; i<8; i++) {
                std::stringstream ss;
                ss << i;
                sc_trace(tf, v.rdata[i], NAME + ".rdata["+ss.str()+"]");
                ss.clear();
            }
            sc_trace(tf, v.reop, NAME + ".reop");
            sc_trace(tf, v.rerror, NAME + ".rerror");
            sc_trace(tf, v.rsrcid, NAME + ".rsrcid");
            sc_trace(tf, v.rtrdid, NAME + ".rtrdid");
            sc_trace(tf, v.rpktid, NAME + ".rpktid");
            sc_trace(tf, v.rbe, NAME + ".rbe");
            sc_trace(tf, v.slave_id, NAME + ".slave_id");
            sc_trace(tf, v.rcmd, NAME + ".rcmd");
            sc_trace(tf, v.n_bits, NAME + ".n_bits");
            sc_trace(tf, v.inj_time.to_double(), NAME + ".inj_time");
        };

        /**
         * shift-left operator overloading.
         * @note this is necessary to print the contents of an object with std::cout. Just make sure
         * all member variables of the v object is "shift-lefted" with ostream class.
         */
        inline friend ostream& operator << ( ostream& os,  vci_response const & v ) {
            os << "(";
            os << "rdata={";
            for(int i=0; i<7; i++)
                os << (long int)v.rdata[i] << ", ";
            os << (long int)v.rdata[8] << "}";
            os << ", reop=" << (long int)v.reop;
            os << ", rerror=" << (long int)v.rerror;
            os << ", rsrcid=" << (long int)v.rsrcid;
            os << ", rtrdid=" << (long int)v.rtrdid;
            os << ", rpktid=" << (long int)v.rpktid;
            os << ", rbe=" << (long int)v.rbe;
            os << ", slave_id=" << (long int)v.slave_id;
            os << ", rcmd=" << (long int)v.rcmd;
            os << ", n_bits=" << (long int)v.n_bits;
            os << ", inj_time=" << v.inj_time;
            os << ")";
            return os;
        };
#endif  //USE_SYSTEMC_TLM
    };
#ifndef USE_SYSTEMC_TLM
    template <typename T>
        class tlm_blocking_put_if : public sc_interface
    {
    public:
        virtual void put (T&) = 0;
    };

    template <typename T>
        class tlm_blocking_get_if : public sc_interface
    {
    public:
        virtual void get (T&) = 0;
    };
#endif //USE_SYSTEMC_TLM

    typedef tlm_blocking_put_if<axi_waddr> AXI_PUT_WADDR_IF;
    typedef tlm_blocking_put_if<axi_wdata> AXI_PUT_WDATA_IF;
    typedef tlm_blocking_put_if<axi_wresp>  AXI_PUT_WRSP_IF;
    typedef tlm_blocking_put_if<axi_raddr> AXI_PUT_RADDR_IF;
    typedef tlm_blocking_put_if<axi_rdata> AXI_PUT_RDATA_IF;

    typedef tlm_blocking_get_if<axi_waddr> AXI_GET_WADDR_IF;
    typedef tlm_blocking_get_if<axi_wdata> AXI_GET_WDATA_IF;
    typedef tlm_blocking_get_if<axi_wresp>  AXI_GET_WRSP_IF;
    typedef tlm_blocking_get_if<axi_raddr> AXI_GET_RADDR_IF;
    typedef tlm_blocking_get_if<axi_rdata> AXI_GET_RDATA_IF;

    typedef tlm_blocking_put_if<vci_request>  VCI_PUT_REQ_IF;
    typedef tlm_blocking_put_if<vci_response> VCI_PUT_RSP_IF;

    typedef tlm_blocking_get_if<vci_response> VCI_GET_RSP_IF;
    typedef tlm_blocking_get_if<vci_request>  VCI_GET_REQ_IF;

//}

//using namespace noc;
#ifndef USE_SYSTEMC_TLM
ostream &operator<<(ostream& output, const vci_request& value);
ostream &operator<<(ostream& output, const vci_response& value);
#endif
#endif /* _ABRSTACT_NOC_H */

/*
 * Vim standard variables
 * vim:set ts=4 expandtab tw=80 cindent syntax=c:
 *
 * Emacs standard variables
 * Local Variables:
 * mode: c
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
