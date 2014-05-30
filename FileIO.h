/**
 * @file
 * @author  Matthew Parkan <matthew.parkan@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This class provides an input/output interface for the following formats:
 * -csv (comma separated value)
 * 
 */

#ifndef FILEIO_H
#define FILEIO_H

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "PointCollection.h"
#include "TreeCollection.h"

class FileIO {

friend class PointCollection;

public:
	
	/**
	 * Reads the contents of csv file to a PointCollection object.
	 *
	 * The input csv should use a comma (,) as a separator without a header and have the following syntax:
	 * x, y, z, classification
	 * 
	 * Where:
	 * x (double) : projected x-coordinate (no geographic coordinate)
	 * y (double) : projected y-coordinate (no geographic coordinate)
	 * z (double) : normalized point elevation
	 * classification (unsigned integer): unsigned integer representing the point classification 
	 * 
	 * @return Returns a PointCollection.
	 */
	PointCollection ReadCsvPoints();
	
	
	/**
	 * Writes the contents of a PointCollection to a csv file. The output file is created in the same folder as the input file and has a "_seg" suffix appended.
	 * 
	 * The output csv contains the following fields:
	 * x, y, h, id, r, g, b
	 * 
	 * Where:
	 * x : projected x-coordinate
	 * y : projected y-coordinate
	 * h : normalized point elevation
	 * id : unique tree identifier
	 * r : int16 red color intensity
	 * g : int16 green color intensity
	 * b : int16 blue color intensity
	 * 
	 * @param  point_collection Reference to PointCollection to be written to the output file.
	 * @param  precision The decimal precision used for double values in the output file.
	 */
	void WritePointsToCSV(PointCollection& point_collection, unsigned int precision);
	
	
	/**
	 * Writes the contents of a TreeCollection to a csv file. The output file is created in the same folder as the input file and has a "_trees" suffix appended.
	 *
	 * @param  tree_collection Reference to TreeCollection to be written to the output file.
	 * @param  precision The decimal precision used for double values in the output file.
	 */
	void WriteTreesToCSV(TreeCollection& tree_collection, unsigned int precision);
	
	std::string GetInputFilepath();
	std::string GetPointOutputFilepath();
	std::string GetTreeOutputFilepath();

	FileIO(std::string i_filepath); // Constructor
	~FileIO(){} ; // Destructor
	
private:


	struct FileParts {
		
		std::string path;
		std::string name;
		std::string extension;
	
	};
	
	/**
	 * Splits an absolute file path into its path, name and extension components. 
	 *
	 * @param  s A Reference to an absolute file path string.
	 * @return Returns a FileParts structure.
	 */
	FileParts GetFileParts(const std::string& s);
	
	/**
	 * Input absolute filepath.
	 *
	 */
	std::string i_filepath_;
	
	/**
	 * Output absolute filepath for PointCollection.
	 *
	 */
	std::string o_filepath_points_;
	
	
	/**
	 * Output absolute filepath for TreeCollection.
	 *
	 */
	std::string o_filepath_trees_;

};

#endif
