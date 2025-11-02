#ifndef INSTANCE_H
#define INSTANCE_H

#include "attribute.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

// Header file for the functions that classify instances

class Instance{
 public:
  void Set_Attributes(const vector<Attribute>& c);
  bool Add_Value(int attribute_num, string val);
  string Get_Nominal_Value(int attribute_num);
  double Get_Numeric_Value(int attribute_num);
  bool Is_Numeric_Attribute(int attribute_num);
  string Get_Attribute_Name(int attribute_num);
  int Get_Num_Attributes() const;
 protected:
  bool Is_Valid_Attribute(int attribute_num);
  
 private:
  
  vector<Attribute> Attribute_List;
  vector<string> Element_List;
};

#endif
