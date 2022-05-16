#include "hard.hpp"

Hard::Hard(sc_core::sc_module_name name)
  : sc_module(name)
  , start(0)
  , ready(0)
  , done(1)
  , pivotRow(0)
  , pivotCol(0)
{
  from_bram.register_b_transport(this, &Hard::b_transport);
  from_interconnect_int.register_b_transport(this, &Hard::b_transport);
  from_interconnect_num_t.register_b_transport(this, &Hard::b_transport);
  SC_REPORT_INFO("Hard", "Constructed.");
}

Hard::~Hard()
{
  SC_REPORT_INFO("Hard", "Destroyed.");
}

void Hard::b_transport(pl_t &pl, sc_core::sc_time &offset)  //zakazuje transakciju, pl je deo transakcije, ono sto prebacujemo, a offset je vreme pocetka
{
  tlm::tlm_command cmd = pl.get_command();  //objekat klase tlm, cmd , 
  sc_dt::uint64 addr = pl.get_address();    //dobija adresu, 
  unsigned int len = pl.get_data_length();  //dobija duzinu podatka
  unsigned char *buf = pl.get_data_ptr();   //pointer na adresu
  pl.set_response_status( tlm::TLM_OK_RESPONSE );    //sve okej
	sc_dt::uint64 taddr=addr&0x000000FF;
	std::cout << taddr << std::endl;
	switch(cmd)   //u zavisnosti od komande
    {
    case tlm::TLM_WRITE_COMMAND:  //write
      switch(addr)
        {
        case ADDR_WIDTH:
          //width = to_int(buf)+1;  //pointer pretvori u broj i pomera za 1
          break;

        case ADDR_HEIGHT:
          //height = to_int(buf)+1;
          break;
        
        case ADDR_CMD:
          start = to_int(buf);
          done=!start;
          std::cout<<start<<done<<std::endl;
          doPivoting(offset);
          std::cout<<"didPivoting"<<endl;
          break;
         /*case ADDR_INIT:
         	std::cout<<"ReadformBram"<<std::endl;
         	startHard();
         	//std::cout<<"Hardbas1"<<std::endl;
         	break;*/
        default:
        	std::cout << "stich" << std::endl;
          pl.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
        }
      if (len != BUFF_SIZE) pl.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );  //do kraja bafera
      break;
    case tlm::TLM_READ_COMMAND:  //read
      switch(addr)
        {
        case ADDR_STATUS:
          to_uchar(buf, done);    //kaze da je spreman, dd
          break;
         
        default:
          pl.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
        }
      if (len != BUFF_SIZE) pl.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
      break;
    default:
      pl.set_response_status( tlm::TLM_COMMAND_ERROR_RESPONSE );
    }
  offset += sc_core::sc_time(10, sc_core::SC_NS);
}

void Hard::startHard()
{	
	for(int j=0;j<ROWSIZE;j++)
	
	{
		for(int i=0;i<COLSIZE;i++)
		{
			wv_fixed[j][i]=read_bram(j*COLSIZE+i);
			
		}
	}
	pivotRow=read_bram(ROWSIZE*COLSIZE);
	pivotCol=read_bram(ROWSIZE*COLSIZE+1);
	cout<<"row="<<pivotRow<<", col="<<pivotCol<<endl;
	pivot_fixed=wv_fixed[pivotRow][pivotCol];
	cout<<pivot_fixed<<endl;
	//
  /* for(int j=ROWSIZE-2;j<ROWSIZE;j++)
        {
            for(int i=0;i<COLSIZE;i++)
            {
                cout<<wv_fixed[j][i]<<" ";
            }
            cout<<endl;
        }
        cout<<endl<<endl<<endl;
        std::cout<<"Hardbas2"<<std::endl;*/
//
	
}
void Hard::doPivoting(sc_core::sc_time &offset)
{
	/* if (start == 1 && ready == 1)
    {
      ready = 0;
      offset += sc_core::sc_time(10, sc_core::SC_NS);
      std::cout << "start = 1 && ready == 1" << endl;
    }
  else if (start == 1 && ready == 0)
    {
      offset += sc_core::sc_time(10, sc_core::SC_NS);
      std::cout << "start = 1 && ready == 0" << endl;
    }
  else if (start == 0 && ready == 0)
    {
    std::cout << "start = 0 && ready == 0" << endl;*/
    //num_t wv_fixed[ROWSIZE][COLSIZE];
    //num_t pivot_fixed;
    num_t newRow[COLSIZE];
    num_t pivotColVal[ROWSIZE];
    num_t nr;
    num_t pcv;
    num_t pivot;
    num_t temp;
    //int pivotRow;
    //int pivotCol;
    pivotRow = read_bram(ROWSIZE*COLSIZE);
    pivotCol = read_bram(ROWSIZE*COLSIZE+1);
    pivot= read_bram(pivotRow*COLSIZE+pivotCol);
    pl_t pl;
	unsigned char buf[BUFF_SIZE];
	/*if(start==1 && ready==1)
	{
		ready=0;
		offset += sc_core::sc_time(10, sc_core::SC_NS);
	}
	else if (start == 1 && ready == 0)
    {
      offset += sc_core::sc_time(10, sc_core::SC_NS);
      // std::cout << "start = 1 && ready == 0" << endl;
    }
    else if (start == 0 && ready == 0)
    */
    for(int i=0;i<COLSIZE;i++)
        {
            nr = (read_bram(pivotRow*COLSIZE+i))/pivot;
            offset += sc_core::sc_time(8, sc_core::SC_NS);
            newRow[i]= nr;
            offset += sc_core::sc_time(8, sc_core::SC_NS);
        }

        for(int j=0;j<ROWSIZE;j++)
        {
            pcv = read_bram(j*COLSIZE+pivotCol);
            offset += sc_core::sc_time(8, sc_core::SC_NS);
            pivotColVal[j]=pcv;
            offset += sc_core::sc_time(8, sc_core::SC_NS);
        }

        for(int j=0;j<ROWSIZE;j++)
        {
            if(j==pivotRow)
            {
                for(int i=0;i<COLSIZE;i++)
                {
                	wv_fixed[j][i]=newRow[i];
                    write_bram(j*COLSIZE+i,newRow[i]);
                    offset += sc_core::sc_time(8, sc_core::SC_NS);
                }
            }
            else
            {
                for(int i=0;i<COLSIZE;i++)
                {
                    temp = read_bram(j*COLSIZE+i)-newRow[i]*pivotColVal[j];
                    write_bram(j*COLSIZE+i,temp);
                    wv_fixed[j][i]=temp;
                    //wv[j][i]=wv[j][i]-newRow[i]*pivotColVal[j];//write_bram(j*COLSIZE+i,newRow[i]);
                    offset += sc_core::sc_time(8, sc_core::SC_NS);
                }
            }
        }
        //offset+=sc_core::sc_time(100,sc_core::SC_NS);
        done=1;
	//}
}



num_t Hard::read_bram(int addr) 
{
  pl_t pl;
  unsigned char buf[BUFF_SIZE];
  pl.set_address(addr*BUFF_SIZE);
  pl.set_data_length(BUFF_SIZE);
  pl.set_data_ptr(buf);
  pl.set_command( tlm::TLM_READ_COMMAND );
  pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );
  sc_core::sc_time offset = sc_core::SC_ZERO_TIME;

   to_bram->b_transport(pl, offset);  //from hard to bram
  
  return to_fixed(buf);
}

void Hard::write_bram(int addr, num_t val)
{
  pl_t pl;
  unsigned char buf[BUFF_SIZE];
  to_uchar(buf,val);
  pl.set_address(addr*BUFF_SIZE);  //postavi adresu u bram na svaka 4 
  pl.set_data_length(BUFF_SIZE);  //velicina podatka=4
  pl.set_data_ptr(buf);      //pointuje na adresu
  pl.set_command( tlm::TLM_WRITE_COMMAND ); //komanda za upis
  pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );  //ili ne
  
   to_bram->b_transport(pl, offset);
 
}

