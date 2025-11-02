#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "instance.h"

using namespace std;

// A program to read an ARFF file and display its information

/// @brief Sets the new attribute for use in the parser
/// @param s The string we are currently reading
/// @param attribute_list The total list of attributes from the file
void New_Attribute(string s, vector<Attribute>& attribute_list){
    Attribute att;
    stringstream ss;
    ss.str(s);

    // Get rid of, and check for, @attribute before each line is processed
    string attribute_remover;
    ss >> attribute_remover;
    if(attribute_remover != "@attribute"){
        cerr << "Error: expected @attribute, got " << attribute_remover << endl;
        return;
    }
    // Now the @attribute is found and removed from each line
    
    // Get the attribute's name
    string att_name;
    ss >> att_name;
    // Set the attribute's name
    att.Set_Name(att_name);

    // Get the attribute's type from the string
    string att_type;
    ss >> att_type;
    // If it's numeric, set it to be numeric
    if(att_type == "numeric"){
        att.Set_Numeric(true);
    }
    // If it's nominal, set it to not be numeric, and go on to process the categories
    else if(!att_type.empty() && att_type[0] == '{'){
        att.Set_Numeric(false);
        string category;
        // Keep reading until we see a closing '}'
        while(ss >> category){
            // If category ends with a comma, strip it
            if(!category.empty() && category[category.size() - 1] == ','){
                category = category.substr(0, category.size() - 1);
            }
            // If category ends with '}', strip it and stop
            else if(category[category.size() - 1] == '}'){
                category = category.substr(0, category.size() - 1);
                if(category != ""){
                    att.Add_Category(category);
                }
                break; // done
            }

            // Add category if it’s not empty
            if(category != ""){
                att.Add_Category(category);
            }
        }
    }
    // This will result in an error since there is no type listed
    else{
        cerr << "Invalid type";
        return;
    }

    // Now that we're done with the attribute, add it to the list
    attribute_list.push_back(att);
}

/// @brief Sifts through the input from the file to be able to show it to the user
/// @param s The string we are currently reading
/// @param attribute_list The total list of attributes from the file
/// @param examples The total list of instances from the file
void Parse_Data(string s, const vector<Attribute>& attribute_list, vector<Instance>& examples){
    Instance in;
    // Add the attributes to the instances
    in.Set_Attributes(attribute_list);

    stringstream ss(s);
    string value;
    int index = 0;
    
    // Find the values in the instances to add
    while(ss >> value && index < attribute_list.size()){
        // Remove trailing comma if present
        if(!value.empty() && value[value.size() - 1] == ','){
            value = value.substr(0, value.size() - 1);
        }
        // Add the value to the current attribute
        if(!in.Add_Value(index, value)){
            cerr << "Error: value " << value << " not valid for attribute " << attribute_list[index].Get_Name() << endl;
            return;
        }
        index++;
    }

    // Check that we got the right number of values
    if(index != attribute_list.size()){
        cerr << "Error: line has " << index << " values but expected " << attribute_list.size() << endl;
        return;
    }

    // Now that we're done with the instance, add it to the list
    examples.push_back(in);
}

/// @brief Menu driven way to print the information in each vector
/// @param attribute_list The total list of attributes from the file
/// @param examples The total list of instances from the file
void Print_Data(const vector<Attribute>& attribute_list, vector<Instance>& examples){
    char choice = 'y';
    while(choice == 'y' || choice == 'Y'){
        // Select an example from however many there are
        cout << "There are " << examples.size() << " examples. Which one would you like? (0-" << examples.size()-1 << ") ";
        int input;
        cin >> input;
        cout << endl;

        // Make sure the input is within acceptable values
        if(input < 0 || input >= examples.size()){
            cout << "Invalid example number\n";
        }
        // If so, run the main input menu to view attributes and instances
        else{
            // Display all possible options
            cout << "Type 0 to see all attributes\n";
            for(int i = 0; i < attribute_list.size(); i++){
                cout << "Type " << (i+1) << " to see " << attribute_list[i].Get_Name() << endl;
            }

            // Which attributes does the user want to see?
            cout << "Your choice: ";
            int which_att;
            cin >> which_att;
            cout << endl;

            if(which_att == 0){
                // Show all attributes
                for(int i = 0; i < attribute_list.size(); i++){
                    cout << attribute_list[i].Get_Name() << ": ";
                    // If it's a numeric attribute, show it, otherwise it's nominal and it also gets displayed
                    if(examples[input].Is_Numeric_Attribute(i)){
                        cout << examples[input].Get_Numeric_Value(i);
                    }
                    else{
                        cout << examples[input].Get_Nominal_Value(i);
                    }
                    cout << endl;
                }
            }
            // Show the specific attribute the user is looking for
            else if(which_att >= 1 && which_att <= attribute_list.size()){
                int i = which_att - 1;
                cout << attribute_list[i].Get_Name() << ": ";
                // If it's a numeric attribute, show it, otherwise it's nominal and it also gets displayed
                if(examples[input].Is_Numeric_Attribute(i)){
                    cout << examples[input].Get_Numeric_Value(i);
                }
                else{
                    cout << examples[input].Get_Nominal_Value(i);
                }
            }
            // This will let the user know of an error
            else {
                cout << "Invalid attribute number." << endl;
            }
        }
        // Does the user want another attribute?
        cout << endl << "Another? (y/n) ";
        cin >> choice;
    }
}

int main(){
    // open the file
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    ifstream fin;
    fin.open(filename.data());
    while(!fin){
        cout << "File not found" << endl;
        cout << "Enter the filename: ";
        cin >> filename;
        fin.clear();
        fin.open(filename.data());
    }

    // Once we get here the file is open
    vector<Attribute> attribute_list;
    vector<Instance> examples;

    string s;
    bool data_mode = false;

    while(getline(fin, s)){
        if(s.size() > 0 && s[0] != '%'){   // Skip empty lines and comments
            if(data_mode){
                if(s[0] == '@'){
                    cerr << "Error: command inside data section: " << s << endl;
                } 
                else{
                    Parse_Data(s, attribute_list, examples);
                }
            } 
            else{ // Not in data section yet
                if(s == "@data"){
                    data_mode = true;
                } 
                else if(s.rfind("@attribute", 0) == 0){ // Starts with "@attribute"
                    New_Attribute(s, attribute_list);
                } 
                else{
                    cerr << "Error: unknown line: " << s << endl;
                    exit(1);
                }
            }
        }
    }

    // Now we can print the data for the user
    Print_Data(attribute_list, examples);
    return 0;
}