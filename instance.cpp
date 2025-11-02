#include <iostream>
#include <string>
#include <vector>
#include "instance.h"
using namespace std;

// Classifying instances from the ARFF file

/// @brief Sets the attributes of the instance
/// @param c The list of attributes this instance will hold
void Instance::Set_Attributes(const vector<Attribute>& c){
    Attribute_List = c;
    Element_List.resize(Attribute_List.size());
    for(int i = 0; i < Element_List.size(); i++){
        Element_List[i] = "?";
    }
}

/// @brief Add a new value to the attribute specified by the number
/// @param attribute_num The attribute location we want to add to
/// @param val Value we want to assign to it
/// @return True if the value is legal, false otherwise
bool Instance::Add_Value(int attribute_num, string val){
    if (!Is_Valid_Attribute(attribute_num)) {
        cout << "Attribute number is too high/low\n";
        return false;
    }

    // Always allow missing value
    if (val == "?") {
        Element_List[attribute_num] = val;
        return true;
    }

    // Numeric attributes: accept anything
    if (Is_Numeric_Attribute(attribute_num)) {
        Element_List[attribute_num] = val;
        return true;
    }

    // Nominal attributes: must be in the domain
    if (!Attribute_List[attribute_num].In_Domain(val)) {
        cout << "Error: value " << val << " not valid for attribute " << Attribute_List[attribute_num].Get_Name() << endl;
        return false;
    }

    Element_List[attribute_num] = val;
    return true;
}

/// @brief Returns a nominal value
/// @param attribute_num The attribute location we want to check
/// @return The nominal value if available, otherwise "?"
string Instance::Get_Nominal_Value(int attribute_num){
    if(Is_Valid_Attribute(attribute_num)){
        if(!Is_Numeric_Attribute(attribute_num))
            return Element_List[attribute_num];
    }
    return "?";
}

/// @brief Retrieves a numeric value from a desire location if possible
/// @param attribute_num The location of the attribute we want to find
/// @return The numeric value if possible, 0 otherwise
double Instance::Get_Numeric_Value(int attribute_num){
    if(Is_Valid_Attribute(attribute_num)){
        if(Is_Numeric_Attribute(attribute_num) && Element_List[attribute_num] != "?")
            return stod(Element_List[attribute_num]);
    }
    return 0;
}

/// @brief Checks if the attribute at the given number is numeric
/// @param attribute_num The attribute location we want to check
/// @return True if attribute is numeric, false otherwise
bool Instance::Is_Numeric_Attribute(int attribute_num){
    if(Is_Valid_Attribute(attribute_num)){
        if(Attribute_List[attribute_num].Numeric())
            return true;
    }
    return false;
}

/// @brief Get's the name of the attribute at the desired location
/// @param attribute_num The attribute location we want to check 
/// @return 
string Instance::Get_Attribute_Name(int attribute_num){
    if(Is_Valid_Attribute(attribute_num)){
        return Attribute_List[attribute_num].Get_Name();
    }
    return "?";
}

/// @brief Returns the number of attributes of the given instance
/// @return The number of attributes
int Instance::Get_Num_Attributes() const {
    return Attribute_List.size();
}

/// @brief See if an attribute number is legal (between 0 and Attribute_List.size()-1)
/// @param attribute_num The attribute location we want to check 
/// @return True if the attribute is valid, false otherwise
bool Instance::Is_Valid_Attribute(int attribute_num){
    if(attribute_num <= Attribute_List.size()-1){
        return true;
    }
    return false;
}