#pragma once
#include <iostream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

using namespace std;
class Serializer2
{
public:
  template <typename T>
  void serialize(stringstream &ss, T &data)
  {
    boost::archive::text_oarchive oa(ss);
    oa << data;
  }
  template <typename T>
  void serialize(stringstream &ss, vector<T> &data)
  {
    boost::archive::text_oarchive oa(ss);
    oa << data;
  }
  template <typename T ,typename...params>
  void serialize(stringstream &ss, T &data , params&... args)
  {
    serialize(ss,data);
    if(sizeof...(args)!=0)
      serialize(ss,args...);
  }
};

class Deserializer2
{
public:
  template <typename T>
  void deserialize(stringstream &ss, T &data)
  {
    boost::archive::text_iarchive ia(ss);
    ia >> data;
  }
  template <typename T>
  void deserialize(stringstream &ss, vector<T> &data)
  {
    boost::archive::text_iarchive oa(ss);
    oa << data;
  }
   template <typename T ,typename...params>
  void deserialize(stringstream &ss, T &data , params&... args)
  {
    deserialize(ss,data);
    if(sizeof...(args)!=0)
      deserialize(ss,args...);
  }
};
