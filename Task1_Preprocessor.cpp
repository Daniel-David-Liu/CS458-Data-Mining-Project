#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

void sort_by_marks(int data[][2], int n){
	auto ptr = (pair<int, int>*) data;
	sort(ptr, ptr+n);
}

int main(){

	// Open up the files
	// Default setup for preparing the training sparse arff
	// For testing, switch all the training files to testing
	// and testing to training
	ifstream training;
	training.open("training.csv");
	ifstream testing;
	testing.open("testing.csv");
	ifstream training_label;
	training_label.open("training_label.csv");
	ofstream out;
	out.open("training.arff");

	// Variables
	char delete_comma;
	float useless_while_obtaining_attributes;
	int value, attributes[100000][2], array_size = 0, found_index;
	bool found;

	cout << "Outputting attributes" << endl;

	// Obtain TRAINING attributes and number of occurences
	while(!training.eof()){
		training >> value >> delete_comma >> value;
		found = false;
		for(int i = 0; i < 100000 && !found; ++i){
			if(attributes[i][0] == value){
				found = true;
				found_index = i;
			}
		}
		if(found){
			attributes[found_index][1]++;
		}
		else{
			attributes[array_size][0] = value;
			attributes[array_size][1] = 1;
			array_size++;
		}
		training >> delete_comma >> useless_while_obtaining_attributes;
	}
	training.close();

	// Obtain TESTING attributes and number of occurences
	while(!testing.eof()){
		testing >> value >> delete_comma >> value;
		found = false;
		for(int i = 0; i < 100000 && !found; ++i){
			if(attributes[i][0] == value){
				found = true;
				found_index = i;
			}
		}
		if(found){
			attributes[found_index][1]++;
		}
		else{
			attributes[array_size][0] = value;
			attributes[array_size][1] = 1;
			array_size++;
		}
		testing >> delete_comma >> useless_while_obtaining_attributes;
	}
	training.close();

	// Start process data
	out << "@RELATION output" << endl << endl;

	// Sort array because Weka needs indices in order
	cout << "Sorting attributes" << endl;
	sort_by_marks(attributes, array_size);
	cout << attributes[0][0] << endl;

/* This section removed so test and training attributes can be combined
	
	// Filter out attributes that occur less than 5 times
	int num_attributes = 0, new_array[array_size][2];
	for(int i = 0; i < array_size; ++i){
		if(attributes[i][1] >= 5){
			out << "@ATTRIBUTE " << attributes[i][0] << " NUMERIC" << endl;
			new_array[num_attributes][0] = attributes[i][0];
			new_array[num_attributes][1] = attributes[i][1];
			num_attributes++;
		}
	}
	sort_by_marks(new_array, num_attributes);

*/

	int num_attributes = 0;
	for(int i = 0; i < array_size; ++i){
		out << "@ATTRIBUTE " << attributes[i][0] << " NUMERIC" << endl;
		num_attributes++;
	}

	out << "@ATTRIBUTE label {-1,1}" << endl << endl << "@DATA" << endl;
	cout << "Done\nWorking on the data section now..." << endl;

	// Create the data section
	training.open("training.csv");
	int instance_index = -1, new_index, index_difference, label;
	while(!training.eof()){
		training >> new_index >> delete_comma >> value;
		found = false;
		for(int i = 0; i < num_attributes && !found; ++i){
			if(attributes[i][0] == value){
				found = true;
				found_index = i;
			}
		}
		// Don't add attribute if it occurs less than 5 times
		if(attributes[found_index][1] >= 5 && found){
			if(instance_index == -1){
				// Just started adding data
				instance_index = new_index;
				training >> delete_comma >> useless_while_obtaining_attributes;
				out << "{" << found_index << " " << useless_while_obtaining_attributes;
			}
			else if(new_index != instance_index){
				// Move on to new instance
				index_difference = new_index - instance_index;
				instance_index = new_index;
				if(index_difference != 1){
					for(int i = 1; i < index_difference; ++i){
						training_label >> label;
					}
				}
				training_label >> label;
				training >> delete_comma >> useless_while_obtaining_attributes;
				out << "," << num_attributes << " " << label << "}" << endl << "{" << found_index << " " << useless_while_obtaining_attributes;
			}
			else{
				// Continue adding attributes
				training >> delete_comma >> useless_while_obtaining_attributes;
				out << "," << found_index << " " << useless_while_obtaining_attributes;
			}
		}
		else{
			training >> delete_comma >> useless_while_obtaining_attributes;
		}
	}
	// Finishing touches
	index_difference = new_index - instance_index;
	instance_index = new_index;
	if(index_difference != 1){
		for(int i = 1; i < index_difference; ++i){
			training_label >> label;
		}
	}
	training_label >> label;
	training >> delete_comma >> useless_while_obtaining_attributes;
	out << "," << num_attributes << " " << label << "}";

	cout << "Done" << endl;

	training.close();
	training_label.close();
	out.close();

	return 0;
}
