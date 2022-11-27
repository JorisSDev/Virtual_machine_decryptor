#include <bits/stdc++.h>
#include "colors.h"

using namespace std;

ofstream fcout("output.txt");

class Command{
  public:
    unsigned int code1;
    unsigned int code2;

    string hex_code1;
    string hex_code2;
    string hex_code;

    bitset<16> binCode;
    
    int type;

    void Check(){
      cout<<"Code:    "<<code1<<" "<<code2<<endl;
      cout<<"HexCode: "<<hex_code1<<" "<<hex_code2<<endl;
    }

    void ConvertToHex(){
        unsigned int temp;

        ostringstream ss1;
        temp = code1;
        ss1 << hex << temp;
        hex_code1 = ss1.str();

        ostringstream ss2;
        temp = code2;
        ss2 << hex << temp;
        hex_code2 = ss2.str();

        if(hex_code1.size()==1){
          hex_code1 = "0"+hex_code1;
        }
        if(hex_code2.size()==1){
          hex_code2 = "0"+hex_code2;
        }

        hex_code = hex_code1 + hex_code2;
    }
   
};

void Clear()
{
  #if defined _WIN32
      system("cls");
      //clrscr(); // including header file : conio.h
  #elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
      system("clear");
      //std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences 
  #elif defined (__APPLE__)
      system("clear");
  #endif
}

unsigned int HexToDec(string n) { return stoi(n, 0, 16); }

string decToHex(unsigned int x){
  stringstream ss;
  ss<< hex << x; // int decimal_value
  string res ( ss.str() );

  return res;
}

void Visualize(int code[], int n, int size){
  //Clear();
  if(n<10)
    cout<<"i: "<<n<<" || ";
  else
    cout<<"i:"<<n<<" || ";
  n*=2;
  Color::Modifier red(Color::FG_RED);
  Color::Modifier def(Color::FG_DEFAULT);

  for(int i=0; i<n-1; i++){
    string temp1 = decToHex(code[i]);
    string temp2 = decToHex(code[i+1]);

    if(temp1.size()==1)
      temp1 = "0" + temp1;
    if(temp2.size()==1)
      temp2 = "0" + temp2;

    cout<<temp1<<" "<<temp2<<" ";
    i++;
  }

  string temp1 = decToHex(code[n]);
  string temp2 = decToHex(code[n+1]);

  if(temp1.size()==1)
    temp1 = "0" + temp1;
  if(temp2.size()==1)
    temp2 = "0" + temp2;

  cout << red << temp1 << " " << temp2 << def << " ";

  for(int i=n+2; i<size; i++){
    temp1 = decToHex(code[i]);
    temp2 = decToHex(code[i+1]);

    if(temp1.size()==1)
      temp1 = "0" + temp1;
    if(temp2.size()==1)
      temp2 = "0" + temp2;

    cout<<temp1<<" "<<temp2<<" ";
    i++;
  }
  
}



int main(int argc, char** argv) {
  int i=0;
  ifstream fcin1;
  ifstream fcin2;

  fcin1.open (argv[1], ios::in | ios::binary);

  if (!fcin1) {
    cout<<"ERROR cannot open file, named: "<<argv[1]<<endl;
  }
  
  fcin2.open(argv[2]);

  if (!fcin2) {
    cout<<"ERROR cannot open file, named: "<<argv[2]<<endl;
  }

  int R[16]={}; // registers
  bool zeroFlag = false;
  bool endOfFileFlag = false;
  bool halt = false;

  Command decryptor[128];
  int fullDecryptor[32];
  int size=32;
  i=0;
  int codeSize;
  while(fcin1){
    int8_t num;
	  fcin1.read((char*)&num, sizeof(num));
    fullDecryptor[i] = (int)num;
    codeSize++;
    i++;
  }
  fcin1.close();
  fcin1.clear();
  fcin1.open (argv[1], ios::in | ios::binary);

  codeSize--;
  cout<<"SIZE: "<<size<<endl;
  

  int cycleCounter = 0;
  i=0;
  int j=0;

  while(halt==false&&cycleCounter<20000){
    Command x = decryptor[i];
    
    cout<<"Zero/EOF = "<<zeroFlag<<"/"<<endOfFileFlag<<" | ";
    cout<<"R:"<<R[0]<<"/"<<R[1]<<"/"<<R[2]<<"/"<<R[3]<<"/"<<R[4]<<" || ";
    Visualize(fullDecryptor, i, size);

    //Input from bin file + convert to int + -> class
    uint8_t num;
	  fcin1.read((char*)&num, sizeof(num));
    x.code1 = (int)num;
    fcin1.read((char*)&num, sizeof(num));
    x.code2 = (int)num;
    x.ConvertToHex();

    if(x.hex_code1=="0"){ //ERROR 
      x.type =-1;
      cout<<"ERROR NO code 0 can exist"<<endl;
    }
    else if(x.hex_code1=="01"){ // INC
      if(zeroFlag==true)
        zeroFlag = false;
      if(endOfFileFlag)
        endOfFileFlag = false;

      cout<<"|| INC"<<endl;
      x.type =1;
      R[x.code2]++;
    }
    else if(x.hex_code1=="02"){ // DEC
      if(zeroFlag==true)
        zeroFlag = false;
      if(endOfFileFlag)
        endOfFileFlag = false;
      cout<<"|| DEC"<<endl;
      x.type =1;
      R[x.code2]++;
    }
    else if(x.hex_code1=="03"){ // MOV
      if(zeroFlag==true)
        zeroFlag = false;
      if(endOfFileFlag)
        endOfFileFlag = false;
      cout<<"|| MOV"<<endl;
      x.type =1;
      string temp1 {x.hex_code2[0]};
      string temp2 {x.hex_code2[1]};
      R[HexToDec(temp1)] = R[HexToDec(temp2)];
    }
    else if(x.hex_code1=="04"){ // MOVC
      cout<<"|| MOVC"<<endl;
      x.type =2;
      R[0] = x.code2;
    }
    else if(x.hex_code1=="05"){ // LSL
      if(zeroFlag==true)
        zeroFlag = false;
      if(endOfFileFlag)
        zeroFlag = false;
      cout<<"|| LSL"<<endl;
      x.type =1;
      string temp {x.hex_code2[1]};
      R[HexToDec(temp)] = R[HexToDec(temp)] << 1;
    }
    else if(x.hex_code1=="06"){ // LSR
      if(zeroFlag==true)
        zeroFlag = false;
      if(endOfFileFlag)
        zeroFlag = false;
      cout<<"|| LSR"<<endl;
      x.type =1;
      string temp {x.hex_code2[1]};
      R[HexToDec(temp)] = R[HexToDec(temp)] >> 1;

    }
    else if(x.hex_code1=="07"){ // JMP
      cout<<"|| JMP";
      x.type =2;
     
      int jumpAmount=0;
      if(HexToDec(x.hex_code2)>=size){
        jumpAmount = HexToDec(x.hex_code2)%size;
      }
      else{
        jumpAmount = HexToDec(x.hex_code2);
      }
      cout<<" || Jumping: "<< jumpAmount<<endl;
      jumpAmount/=2;
      for(int h=0; h<jumpAmount; h++){
        if(fcin1.eof()){
            fcin1.close();
            fcin1.clear();
            fcin1.open (argv[1], ios::in | ios::binary);
        }
        uint8_t burn;
        fcin1.read((char*)&burn, sizeof(burn));
        fcin1.read((char*)&burn, sizeof(burn));
      }
      i+=jumpAmount;
    }
    else if(x.hex_code1=="08"){ // JZ
      cout<<"|| JZ ";
      x.type =2;
      if(zeroFlag == true){
        int jumpAmount=0;
        if(HexToDec(x.hex_code2)>=size){
          jumpAmount = HexToDec(x.hex_code2)%size;
        }
        else{
          jumpAmount = HexToDec(x.hex_code2);
        }
        cout<<" || Jumping: "<< jumpAmount<<endl;
        jumpAmount/=2;
        for(int h=0; h<jumpAmount; h++){
          if(fcin1.eof()){
            fcin1.close();
            fcin1.clear();
            fcin1.open (argv[1], ios::in | ios::binary);
          }
          uint8_t burn;
          fcin1.read((char*)&burn, sizeof(burn));
          fcin1.read((char*)&burn, sizeof(burn));
        }
        i+=jumpAmount;
      }
      else{
        cout<<" Jump failed"<<endl;
      }
    }
    else if(x.hex_code1=="09"){ // JNZ
      cout<<"|| JNZ";
      x.type =2;
      if(zeroFlag==false){
        int jumpAmount=0;
        if(HexToDec(x.hex_code2)>=size){
          jumpAmount = HexToDec(x.hex_code2)%size;
        }
        else{
          jumpAmount = HexToDec(x.hex_code2);
        }
        cout<<" || Jumping: "<< jumpAmount<<endl;
        jumpAmount/=2;
        for(int h=0; h<jumpAmount; h++){
          if(fcin1.eof()){
            fcin1.close();
            fcin1.clear();
            fcin1.open (argv[1], ios::in | ios::binary);
          }
          int8_t burn;
          fcin1.read((char*)&burn, sizeof(burn));
          fcin1.read((char*)&burn, sizeof(burn));
        }
        i+=jumpAmount;
      }
      else{
        cout<<" Jump failed"<<endl;
      }
    }
    else if(x.hex_code1=="0a"){ // JFE
      cout<<"|| JFE ";
      x.type =2;
      if(endOfFileFlag==true){
        int jumpAmount=0;
        if(HexToDec(x.hex_code2)>=size){
          jumpAmount = HexToDec(x.hex_code2)%size;
        }
        else{
          jumpAmount = HexToDec(x.hex_code2);
        }
        cout<<" || Jumping: "<< jumpAmount<<endl;
        jumpAmount/=2;
        for(int h=0; h<jumpAmount; h++){
          uint8_t burn;
          fcin1.read((char*)&burn, sizeof(burn));
          fcin1.read((char*)&burn, sizeof(burn));
          if(fcin1.eof()){
              fcin1.close();
              fcin1.clear();
              fcin1.open (argv[1], ios::in | ios::binary);
          }
        }
        i+=jumpAmount-1;
      }
      else{
        cout<<" Jump failed"<<endl;
      }
    }
    else if(x.hex_code1=="0b"||i==15){ // RET
      cout<<"|| RET"<<endl;
      //x.Check();
      x.type =3;
      cout<<"Virtual machine END"<<endl;
      halt=true;
      break;
    }
    else if(x.hex_code1=="0c"){ // ADD
      if(zeroFlag==true)
        zeroFlag = false;
      if(endOfFileFlag)
        endOfFileFlag = false;
      cout<<"|| ADD";
      x.type =1;
      string temp1 {x.hex_code2[0]};
      string temp2 {x.hex_code2[1]};
      R[HexToDec(temp2)]+=R[HexToDec(temp1)];

      cout<<" || R["<<HexToDec(temp2)<<"]+=R["<<HexToDec(temp1)<<"]"<<endl;
      
    }
    else if(x.hex_code1=="0d"){ // SUB
      if(zeroFlag==true)
        zeroFlag = false;
      if(endOfFileFlag)
        endOfFileFlag = false;
      cout<<"|| SUB";
      x.type =1;
      string temp1 {x.hex_code2[0]};
      string temp2 {x.hex_code2[1]};
      R[HexToDec(temp2)]-=R[HexToDec(temp1)];

      cout<<" || R["<<HexToDec(temp2)<<"]-=R["<<HexToDec(temp1)<<"]"<<endl;

    }
    else if(x.hex_code1=="0e"){ // XOR
      if(zeroFlag==true)
        zeroFlag = false;
      if(endOfFileFlag)
        endOfFileFlag = false;
      cout<<"|| XOR"<<endl;
      x.type =1;
      string temp1 {x.hex_code2[0]};
      string temp2 {x.hex_code2[1]};
      R[HexToDec(temp2)]^=R[HexToDec(temp1)];

      if(HexToDec(temp2)==0)
        zeroFlag = true;
      else
        zeroFlag = false;
    }
    else if(x.hex_code1=="0f"){ // OR
      if(zeroFlag==true)
        zeroFlag = false;
      if(endOfFileFlag)
        endOfFileFlag = false;
      cout<<"|| OR"<<endl;
      x.type =1;
      string temp1 {x.hex_code2[0]};
      string temp2 {x.hex_code2[1]};
      R[HexToDec(temp2)]|=R[HexToDec(temp1)];

      if(HexToDec(temp2)==0)
        zeroFlag = true;
      else
        zeroFlag = false;
    }
    else if(x.hex_code1=="10"){ // IN
      cout<<"|| IN";
      x.type =2;
      char temp; fcin2>>temp;
      cout<<" Inputed char: "<<temp<<endl;
      //text[j] = temp;
      R[HexToDec(x.hex_code2)] = (int)temp;
      j++;
      if(fcin2.eof()){
        endOfFileFlag = true;
        fcin2.close();
        fcin2.clear();
        fcin2.open(argv[2]);
        cout<<"EOF FLAG ACTIVATED"<<endl;
      }
    }
    else if(x.hex_code1=="11"){ // OUT
      if(zeroFlag==true)
        zeroFlag = false;
      if(endOfFileFlag)
        endOfFileFlag = false;

      cout<<"|| OUT"<<endl;
      x.type =1;
      string tempNr {x.hex_code2[1]};
      fcout<<(char)R[HexToDec(tempNr)];
    }
    
    if(i>=codeSize/2){
      while(i>=codeSize/2){
        i = i - codeSize/2;
      }
    }
    else{
      i++;
    }
    cycleCounter++;
    //x.Check();
  }
  fcin1.close();
  fcin2.close();
  fcout.close();
}
