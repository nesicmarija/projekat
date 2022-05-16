#include "utils.hpp"            //PREPRAVITI NA CASU!!  //PREPRAVITI NA CASU!!  //PREPRAVITI NA CASU!!


char flip(char c) {return (c == '0')? '1': '0';}   //za negativne vred.

string twos_complement(string bin)
{
  int n = bin.length();
  int i;

  string ones, twos;
  ones = twos = "";

  //  for ones complement flip every bit
  for (i = 0; i < n; i++)
    ones += flip(bin[i]);

  //  for two's complement go from right to left in
  //  ones complement and if we get 1 make, we make
  //  them 0 and keep going left when we get first
  //  0, make that 1 and go out of loop
  twos = ones;
  for (i = n - 1; i >= 0; i--)
    {
      if (ones[i] == '1')
        twos[i] = '0';
      else
        {
          twos[i] = '1';
          break;
        }
    }

  // If No break : all are 1  as in 111  or  11111;
  // in such case, add extra 1 at beginning
  if (i == -1)
    twos = '1' + twos;

  // cout << "1's complement: " << ones << endl;
  // cout << "2's complement: " << twos << endl; 
  return twos;
}

double to_fixed (unsigned char *buf)
{
  /*string concated = "";
  for (int i = 0; i<CHARS_AMOUNT; ++i) // concatenate char array into eg. "10101101000"
    concated += bitset<CHAR_LEN>((int)buf[i]).to_string();
  // cout << "concated = " << concated << endl;
  double multiplier = 1;
  if (concated[0] == '1')
    {
      concated = twos_complement(concated);
      multiplier = -1;
    }
  // cout << "concated = " << concated << endl;
  double sum = 0;
  for (int i = 0; i<DATA_WIDTH; ++i)
    {
      // cout << "[" << i << "] sum = " << sum << endl;
      sum += (concated[i]-'0') * pow(2.0, DATA_WIDTH - FIXED_POINT_WIDTH - i - 1);
    }
  // cout << "sum = " << sum << endl;
  return sum*multiplier;*/
  
  string s;
    num_t mult = 1;//sign

    for (int i = 0; i < BUFF_SIZE; ++i){
        s += bitset<CHAR_LEN>((int)buf[i]).to_string();//from unsigned char into binary
    }

    if (s[0] == '1'){
        s =bitset< TOTAL_SIZE + BUFF_EXTRA >( -stoi(twos_complement(s), 0, 2) ).to_string();//turn negative into positive and change mult
        mult = 1;
    }
	//std::cout<<"PRoslo"<<std::endl;
    string w, f;//whole and fraction parts
    for(int i = 0; i < WHOLE_SIZE; i++){
        w += s[i];//whole part
    }
    for(int i = WHOLE_SIZE; i < TOTAL_SIZE; i++){
        f += s[i];//fraction part
    }

    int w_i = stoi(w, 0, 2);//turn from string into int
    double f_i = (double)stoi(f, 0, 2);//turn from string into double

    return (num_t) ( mult*(w_i + f_i / (1 << FRAC_SIZE)));//set put it back together and set sign
}

void to_char (unsigned char *buf, string s)
{
  s.erase(0,2); // remove "0b"
  s.erase(DATA_WIDTH-FIXED_POINT_WIDTH, 1); // remove the dot
  char single_char[CHAR_LEN];
  for (int i = 0; i<CHARS_AMOUNT; ++i)
    {
      s.copy(single_char,CHAR_LEN,i*CHAR_LEN); // copy 8 letters (0s and 1s) to char array
      int char_int = stoi(single_char, nullptr, 2); // binary string -> int
      buf[i] = (unsigned char) char_int;
    }
}

void to_uchar(unsigned char *buf, num_t d)
{
  //to_char(c,d.to_bin());
  string s = d.to_bin();//from num_t into binary

    s.erase(0,2);//erase 0b
    s.erase(I, 1);//erase .

    char single_char[CHAR_LEN];
    for (int i = 0; i < BUFF_SIZE; i++){
        s.copy(single_char, CHAR_LEN, i*CHAR_LEN);//copy first BUFF_SIZE bits
        buf[i] = (unsigned char) stoi(single_char, 0, 2);//change to unsigned char
    }
}


       

int to_int (unsigned char *buf)
{
  int sum = 0;
  //sum += ((int)buf[0]) << 32;
  sum += ((int)buf[0]) << 24;
  sum += ((int)buf[1]) << 16;
  sum += ((int)buf[2]) << 8;
  sum += ((int)buf[3]);
  return sum;
}

void to_uchar (unsigned char *buf, int d)
{
  //buf[0] = (char) (d >> 32);
  buf[0] = (char) (d >> 24);
  buf[1] = (char) (d >> 16);
  buf[2] = (char) (d >> 8);
  buf[3] = (char) (d); 
}

int highest_bit(int n)
{
  int ret = 0;
  while (n>0) n>>=1,++ret;
  return ret;
}



// e.g. 1 001 001 010 -> 1 111 111 111 -> 10 000 000 000
int setBitNumber(int n)
{
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  n = n + 1;
  return n;
}

