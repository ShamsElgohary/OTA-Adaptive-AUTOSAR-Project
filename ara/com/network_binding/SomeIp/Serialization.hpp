#pragma once

#include<iostream>
#include<sstream>
#include<string>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;


/* SERIALIZATION OF MULTIPLE PARAMETERS */
template <typename T, typename... params>
inline void Serialize(stringstream &ss, T data, params... param)
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
    std::cout<<e.what()<<endl;
  }
  if constexpr (sizeof...(params) > 0) 
  {
  Serialize(ss,param...);
  }
}


/* BOOL , CHAR , INT .. ETC */
template <typename T> 
inline void Serialize(stringstream &ss, T data)
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
    std::cout<<e.what()<<endl;
  }
}


/* UNSIGNED CHAR  */
inline void Serialize(stringstream &ss, unsigned char data)
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
inline void Serialize(stringstream &ss, T (&data)[N] ) 
{
  /*  PUT SIZE OF ARRAY */
  ss << "Length=" << " " << N << " ";

  string str;
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

/* ARRAY OF UNSIGNED CHAR */
template <size_t N>
inline void Serialize(stringstream &ss, unsigned char (&data)[N] ) 
{
  int arrSize = *(&data + 1) - data;
  ss << "Length=" << " " <<arrSize << " ";

  string str = "<0x0001> ";
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

}


/* VECTORS */
template <typename T>
inline void Serialize(stringstream &ss, vector<T> data ) 
{
  ss << "Length=" << " " <<data.size() << " ";

  string str;
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

/* SHOW DATA INSIDE THE STREAM */
inline void ShowSerializedData(stringstream &ss)
{
  cout<< ss.str() << endl;
}



