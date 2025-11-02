#include <iostream>
#include <string>
#include <vector>
#include "attribute.h"

using namespace std;

// Classifying attributes from the ARFF file

/// @brief Set the name of an attibute
/// @param s Inputted string to make the name
void Attribute::Set_Name(const string& s){
    name = s;
}

/// @brief Get the name of an attribute
/// @return The name of the attribute
string Attribute::Get_Name() const{
    return name;
}

/// @brief Set an attribute to be numeric
/// @param b the boolean value pertaining to if it is numeric or not
void Attribute::Set_Numeric(bool b){
    is_numeric = b;
}

/// @brief Check if an attribute is numeric or not
/// @return True if attribute is numeric, false otherwise
bool Attribute::Numeric() const {
    return is_numeric;
}

/// @brief Add an attribute to the category list if it is not numeric
/// @param s String input for the addition of a category
void Attribute::Add_Category(string s){
    if(!Numeric() && !In_Domain(s)){
        categories.push_back(s);
    }
}

/// @brief Returns the number of categories we have
/// @return The number of attribute categories
int Attribute::Num_Categories() const {
    return categories.size();
}

/// @brief Check to see if a nominal value is in the list of possible categories
/// @param s The string name of the attribute we are checking
/// @return True if the attribute is in the domain, or "?", false otherwise
bool Attribute::In_Domain(string s){
    if(s == "?")
        return true;
    for(int i = 0; i < categories.size(); i++){
        if(categories[i] == s)
            return true;
    }
    return false;
}

/// @brief Returns the whole list of possible categories
/// @return The list of categories
vector<string> Attribute::Get_Category_List() const{
    return categories;
}
