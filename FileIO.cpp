#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm> 
#include <string>
#include "FileIO.h"
#include "PointCollection.h"
#include "TreeCollection.h"

using namespace std;


FileIO::FileParts FileIO::GetFileParts(const string& s)
{
	FileIO::FileParts file;
	string path, name, extension;
	
    char sep = '/';

	#ifdef _WIN32
	   sep = '\\';
	#endif
	
   size_t i = s.rfind(sep, s.length());

	   path = s.substr(0, i + 1);
	   extension = s.substr(s.rfind("."), s.length() - s.rfind("."));
	   name = s.substr(i + 1, s.length() - (path.length() + extension.length()));
	  
	   file.path = path;
	   file.name = name;
	   file.extension = extension;
	   
	  return(file);
}


string FileIO::GetInputFilepath()
{
	return i_filepath_;
}

	
string FileIO::GetPointOutputFilepath()
{
	return o_filepath_points_;	
}			


string FileIO::GetTreeOutputFilepath()
{
	return o_filepath_trees_;	
}			


// Read a point cloud from a .csv file into a vector of Points
PointCollection FileIO::ReadCsvPoints()
{
	ifstream i_file(i_filepath_);
	
	if(i_file){
	
		cout << "Reading " << i_filepath_ << "...";
	
		// Read line by line to determine the number of lines in the input file
		string line;
		unsigned int counter = 0;
	    while ( getline(i_file, line) ) { 
	        counter++;
	    }

		// Return to the beginning of the file
		i_file.clear();
		i_file.seekg(0, ios::beg);
		
		PointCollection point_collection;
		PointCollection::Point initial_val = {0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 2, false, {0,0,0}};
		point_collection.points_.resize(counter, initial_val);
		
		unsigned int k = 0;
		string field;
		PointCollection::Point point;
		
		while(!i_file.eof()){
		
			getline(i_file, field,',');
			//point.x = stod(field);
			stringstream str1(field);
			str1 >> point.x;

			getline(i_file, field, ',');
			//point.y = stod(field);
			stringstream str2(field);
			str2 >> point.y;
			
			getline(i_file, field, ',');
			//point.z = stod(field);
			stringstream str3(field);
			str3 >> point.z;
			
			getline(i_file, field);
			//point.classification = stoi(field);
			stringstream str4(field);
			str4 >> point.classification;
			
			// Set additional attribute values
			point.tree_idx = 0;
			point.segmentation_status = false;
			point.local_maxima_status = 2;
			
			// Add Point to PointCollection
			point_collection.points_.push_back(point);
			
			k++;
			
			i_file >> ws;
		}
		i_file.close();
		
		cout << "Done!" << endl;	
		
		return point_collection;
		
	} else {
		
		cerr << "FAILURE: unable to open input file " << i_filepath_ << endl;
		exit(1);
			
	}
}


// Write a vector of segmented Points to .csv file
void FileIO::WritePointsToCSV(PointCollection& point_collection, unsigned int precision)
{
	
	// Create output file name
	FileParts current_file_parts = GetFileParts(i_filepath_);
	
	o_filepath_points_ = current_file_parts.path + current_file_parts.name  + "_seg.csv";	
	ofstream o_file(o_filepath_points_);
			
	if(o_file){
		
		// Set decimal precision
		o_file << fixed << setprecision(precision);
		
		cout << "Writing points to " << o_filepath_points_ << endl;
		
		// Print header
		o_file << "X, Y, H, ID, R, G, B" << endl;
		
		// Print content
		for(unsigned int j(0); j < point_collection.points_.size(); j++){
			
			o_file << point_collection.points_[j].x << ", " << point_collection.points_[j].y << ", " << point_collection.points_[j].z << ", " << point_collection.points_[j].tree_idx << ", " << point_collection.points_[j].rgb_color[0] << ", " << point_collection.points_[j].rgb_color[1] << ", " << point_collection.points_[j].rgb_color[2] <<  endl;
		}
		
	} else{
		
		cerr << "FAILURE: unable to open output file ";
		exit(1);
		
	}

}


// Write a TreeCollection to .csv file
void FileIO::WriteTreesToCSV(TreeCollection& tree_collection, unsigned int precision)
{
	
	// Create output file name
	FileParts current_file_parts = GetFileParts(i_filepath_);
	
	o_filepath_trees_ = current_file_parts.path + current_file_parts.name  + "_trees.csv";	
	ofstream o_file(o_filepath_trees_);
			
	if(o_file){
		
		// Set decimal precision
		o_file << fixed << setprecision(precision);
		
		cout << "Writing trees to " << o_filepath_trees_ << endl;
		
		// Print header
		o_file << "ID, X_TOP, Y_TOP, H_TOP, X_BARYCENTER, Y_BARYCENTER, H_BARYCENTER, H_REL_BARYCENTER, N_POINTS" << endl;
		
		// Print content
		for(unsigned int j(0); j < tree_collection.trees_.size(); j++){
			
			o_file 
			<< tree_collection.trees_[j].tree_idx << ", " 
			<< tree_collection.trees_[j].x_top << ", " 
			<< tree_collection.trees_[j].y_top << ", " 
			<< tree_collection.trees_[j].h_top << ", " 
			<< tree_collection.trees_[j].x_barycenter << ", " 
			<< tree_collection.trees_[j].y_barycenter << ", " 
			<< tree_collection.trees_[j].h_barycenter << ", " 
			<< tree_collection.trees_[j].rel_h_barycenter << ", " 
			<< tree_collection.trees_[j].n_points
			<< endl;
		
		}
		
	} else{
		
		cerr << "FAILURE: unable to open output file ";
		exit(1);
		
	}

}

// Constructor
FileIO::FileIO(std::string i_filepath)
{
	i_filepath_ = i_filepath;
	
}
