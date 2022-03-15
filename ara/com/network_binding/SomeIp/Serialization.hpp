#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

class Serializer{

public:
  // Serialize any number of arguments
  template<typename ...Ts>
  inline void Serialize (std::stringstream& ss,Ts && ... multi_inputs)
  {
      ([&ss,this] (auto input)
      {
        this->Serialize(ss,input);
      } (multi_inputs), ...);
  }

  /* SHOW DATA INSIDE THE STREAM */
  inline void ShowSerializedData(std::stringstream &ss)
  {
    std::cout<< ss.str() << std::endl;
  }
  /* BOOL , CHAR , INT .. ETC */
  template <typename T> 
  inline void Serialize(std::stringstream &ss, T data)
  {
    if(sizeof(data) == sizeof(uint8_t)){
      /* TAG FOR WIRE TYPE 0 ID = 0 (SINGLE VARIABLE) */
      ss<< "<0x0000> ";   
    }

    else if(sizeof(data) == sizeof(uint16_t)){
      ss<< "<0x1000> ";
    }

    else if(sizeof(data) == sizeof(uint32_t)){
      ss<< "<0x2000> ";
    }

    else if(sizeof(data) == sizeof(uint64_t)){
      ss<< "<0x3000> ";
    }

    try{    
    ss<< data;
    ss<< " ";
    }
    catch(const std::exception &e) 
    {
      std::cout<< "NOT SUPPORTED IN SERIALIZE \n";
      std::cout<<e.what()<<std::endl;
    }
  }


  /* UNSIGNED CHAR  */
  inline void Serialize(std::stringstream &ss, unsigned char data)
  {
    /* TAG FOR WIRE TYPE 0 ID = 0 (SINGLE VARIABLE) */
    ss<< "<0x0000> ";
    // Adding a unary + operator before the variable of any primitive data type 
    // will give printable numerical value instead of ASCII character(in case of char type).
    ss<< +data;
    ss<< " ";
  }


  /* ARRAY OF BOOL ,CHAR , INT .. ETC. */
  template <typename T, size_t N>
  inline void Serialize(std::stringstream &ss, T (&data)[N] ) 
  {
    /*  PUT SIZE OF ARRAY */
    ss << " <Length=" << " " << N << "> ";

    std::string str;
    if(sizeof(data[0]) == sizeof(uint8_t)){
      /* TAG FOR WIRE TYPE 0 ID = 0 (SINGLE VARIABLE) */
      str = "<0x0001> ";
    }

    else if(sizeof(data[0]) == sizeof(uint16_t)){
      str = "<0x1001> ";
    }

    else if(sizeof(data[0]) == sizeof(uint32_t)){
      str = "<0x2001> ";
    }

    else if(sizeof(data[0]) == sizeof(uint64_t)){
      str = "<0x3001> ";
    }

    char dataID= '2';
    for( auto element : data)
    {
      ss<< str;
      str[6] = dataID;
      dataID++;
      ss<< element<<" ";
    }

  }

  /* ARRAY OF CHAR */
  template <size_t N>
  inline void Serialize(std::stringstream &ss, const char (&data)[N] ) 
  {
    std::string serializeString = data;
    Serialize(ss, serializeString);
  };



  /* ARRAY OF UNSIGNED CHAR */
  template <size_t N>
  inline void Serialize(std::stringstream &ss, unsigned char (&data)[N] ) 
  {
    int arrSize = *(&data + 1) - data;
    ss << " <Length=" << " " <<arrSize << "> ";

    std::string str = "<0x0001> ";
    char dataID= '2';

    for( auto element : data)
    {
    // Adding a unary + operator before the variable of any primitive data type 
    // will give printable numerical value instead of ASCII character(in case of char type).
      ss<< str;
      str[6] = dataID;
      dataID++;
      ss<< +element<<" ";
    }

  };


    /* VECTORS STRING*/
  inline void Serialize(std::stringstream &ss, std::vector<std::string> &data ) 
  {
    ss << " <Length=" << " " <<data.size() << "> ";

    for( auto element : data)
    {
      Serialize(ss,element);
    }

  }


  /* VECTORS */
  template <typename T>
  inline void Serialize(std::stringstream &ss, std::vector<T> &data ) 
  {
    ss << " <Length=" << " " <<data.size() << "> ";

    std::string str;
    if(sizeof(T) == sizeof(uint8_t)){
      /* TAG FOR WIRE TYPE 0 ID = 0 (SINGLE VARIABLE) */
      str = "<0x0001> ";
    }

    else if(sizeof(T) == sizeof(uint16_t)){
      str = "<0x1001> ";
    }

    else if(sizeof(T) == sizeof(uint32_t)){
      str = "<0x2001> ";
    }

    else if(sizeof(T) == sizeof(uint64_t)){
      str = "<0x3001> ";
    }


    char dataID= '2';
    for( auto element : data)
    {
      ss<< str;
      str[6] = dataID;
      dataID++;
      ss<< element<<" ";
    }

  }


  /* std::string */
  inline void Serialize(std::stringstream &ss, std::string str) 
  {
    ss << " <Length=" << " " << str.size() << "> ";
    ss << str;
  }
};


class Deserializer{

public:
  // Serialize any number of arguments
  template<typename ...Ts>
  inline void Deserialize (std::stringstream& ss,Ts && ... multi_inputs)
  {
      ([&ss,this] (auto &input)
      {
        this->Deserialize(ss,input);
      } (multi_inputs), ...);
  }

  /* BOOL , CHAR , INT .. ETC */
  template <typename T> 
  inline void Deserialize(std::stringstream &ss, T &data)
  {
    std::string strTag;

    ss >> strTag;

    try{    
    ss >> data;
    }
    catch(const std::exception &e) 
    {
      std::cout<< "NOT SUPPORTED IN DESERIALIZE \n";
      std::cout<<e.what()<<std::endl;
    }
  }


  /* ARRAY OF BOOL ,CHAR , INT .. ETC. */
  template <typename T, size_t N>
  inline void Deserialize(std::stringstream &ss, T (&data)[N] ) 
  {
    std::string strTag;
    int size;

    /* LENGTH= */
    ss >> strTag;
    /* ARRAY SIZE */
    ss >> size;
    /* > */
    ss >> strTag;

    for( int i =0 ; i < size ; i++)
    {
      ss >> strTag;
      ss >> data[i];
    }

  }

  /* VECTORS OF STRING */
  inline void Deserialize(std::stringstream &ss, std::vector<std::string> &data ) 
  {
    std::string strTag;
    int size;

    /* LENGTH= */
    ss >> strTag;
    /* ARRAY SIZE */
    ss >> size;
    /* > */
    ss >> strTag;

    if (data.capacity() < size)
    {
      data.resize(size);
    } 

    for(int i = 0; i < size; i++)
    {
      Deserialize(ss,data[i]);
    }
  }


  /* VECTORS */
  template <typename T>
  inline void Deserialize(std::stringstream &ss, std::vector<T> &data ) 
  {
    std::string strTag;
    int size;

    /* LENGTH= */
    ss >> strTag;
    /* ARRAY SIZE */
    ss >> size;
    /* > */
    ss >> strTag;
    
    if (data.capacity() < size)
    {
      data.resize(size);
    } 

    for(int i = 0; i < size; i++)
    {
      ss >> strTag;
      ss >> data[i];
    }
  }


  /* std::string */
  inline void Deserialize(std::stringstream &ss, std::string &str) 
  {
    std::string strTag;
    int strSize;
    /* LENGTH= */
    ss >> strTag;
    /* ARRAY SIZE */
    ss >> strSize;
    /* > */
    ss >> strTag;

    while(str.size() < strSize)
    {
      std::string strInput;
      ss >> strInput;
      str += strInput + " ";
    }
  }
};



	// void writeValue(bool v) {
	// 	writeBasicTypeValue(v);
	// }

	// void writeValue(const std::string& s) {
	// 	writeValue( s.c_str(), s.size() );
	// }

	// void writeValue(const char* s, size_t length) {
	// 	writeBasicTypeValue(length);
	// 	writeRawData(s, length);
	// }

	// void writeValue(int8_t v) {
	// 	writeBasicTypeValue(v);
	// }

	// void writeValue(int16_t v) {
	// 	writeBasicTypeValue(v);
	// }

	// void writeValue(int32_t v) {
	// 	writeBasicTypeValue(v);
	// }

	// void writeValue(int64_t v) {
	// 	writeBasicTypeValue(v);
	// }

	// void writeValue(uint8_t v) {
	// 	writeBasicTypeValue(v);
	// }

	// void writeValue(uint16_t v) {
	// 	writeBasicTypeValue(v);
	// }

	// void writeValue(uint32_t v) {
	// 	writeBasicTypeValue(v);
	// }

	// void writeValue(uint64_t v) {
	// 	writeBasicTypeValue(v);
	// }

	// void writeValue(float v) {
	// 	writeBasicTypeValue(v);
	// }

	// void writeValue(double v) {
	// 	writeBasicTypeValue(v);
	// }
