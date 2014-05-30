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
 * This program implements the sequential nearest cluster segmentation algorithm proposed in [1].
 * 
 * [1]W. Li, Q. Guo, M. K. Jakubowski, and M. Kelly, 
 * “A new method for segmenting individual trees from the lidar point cloud,” 
 * Photogrammetric Engineering and Remote Sensing, vol. 78, no. 1, pp. 75–84, 2012.

 */
 
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include "TreeCollection.h"
#include "FileIO.h"
#include "PointCollection.h"
#include "SegmenterSNC.h"
#include "CircularBuffer.h"
#include "CircularBufferCollection.h"

using namespace std;


int main(int argc, char *argv[]) {
	
	// Validate user input
	string i_filepath;
	
	if (argc <= 1 or argc > 2){
		
		cerr << "Usage: h_sequence_segmentation src_datasource_name" << endl;
		cerr << endl;
		cerr << "FAILURE: wrong syntax or no data source provided" << endl;
		exit(1);
		
	} else {
		
		// Check input file name extension (.csv)
		i_filepath = argv[1];
		if (not (i_filepath.rfind(".csv") == (i_filepath.size()-4))){
			
			cerr << "FAILURE: unsupported data source format" << endl;
			exit(1);
			
		}
	}
	
	
	// Set the display decimal precision
	cout << setprecision(2) << fixed;
    
    
	// Read the csv file
	FileIO file_io(i_filepath);
	PointCollection point_collection = file_io.ReadCsvPoints();
	

	// Set the 16 bit hsv colormap
	vector<array<unsigned int, 3>> hsv_colormap;
	
	hsv_colormap.push_back({32767,      0,      0});
	hsv_colormap.push_back({32767,  19660,      0});
	hsv_colormap.push_back({26214,  32767,      0});
	hsv_colormap.push_back({ 6553,  32767,      0});
	hsv_colormap.push_back({    0,  32767,  13107});
	hsv_colormap.push_back({    0,  32767,  32767});
	hsv_colormap.push_back({    0,  13107,  32767});
	hsv_colormap.push_back({ 6553,      0,  32767});
	hsv_colormap.push_back({26214,      0,  32767});
	hsv_colormap.push_back({32767,      0,  19660});
	
	
	// Create a subset of PointCollection containing only points with high vegetation classification
	vector<unsigned int> keep_classes = {5};
	cout << "Extracting subset...";
	PointCollection point_collection_subset = point_collection.FilterPointsByClass(keep_classes);
	cout << "Done!" << endl;

	
	// Sort the PointCollection by height
	cout << "Sorting points by height...";
	point_collection_subset.SortByZ();
	cout << "Done!" << endl;
	
	
	// Recompute the Point indexes
	point_collection_subset.ComputePointIndexes();
	
	
	// Compute the bounding box
	cout << "Computing bounding box... ";
	point_collection_subset.ComputeBoundingBox();
	cout << "Done!" << endl;
	
	
	// Compute the associated grid
	cout << "Gridding points...";
	//point_collection_subset.ComputeGridCoordinates(scaling_factor);
	point_collection_subset.ComputeGridCoordinates();
	point_collection_subset.AssignGridCells();
	cout << "Done!" << endl;
	
	
	// Create a circular buffer collection
	cout << "Creating circular buffers...";
	vector<unsigned int> radius_list = {2, 4, 9, 14};
	CircularBufferCollection circular_buffer_collection(radius_list, point_collection.GetScalingFactor());
	cout << "Done!" << endl;
	
	
	// Find all local maxima
	cout << "Finding local maxima...";
	CircularBuffer circular_buffer_0 = circular_buffer_collection.GetCircularBuffer(0);
	point_collection_subset.FindLocalMaxima(circular_buffer_0);
	cout << "Done!" << endl;
	
	
	// Segment the point cloud
	SegmenterSNC segmenter;
	segmenter.SegmentPointCollection(point_collection_subset, circular_buffer_collection, true);
	
	
	// Set RGB color values for each segmented point
	point_collection_subset.SetRGBColors(hsv_colormap);
	
	
	// Extract individual tree attributes (x, y, h)
	cout << "Computing tree attributes...";
	TreeCollection tree_collection(point_collection_subset, 20, 3);
	cout << "Done!" << endl;
	
	
	// Write the segmented points to .csv
	file_io.WritePointsToCSV(point_collection_subset, 2);

	
	// Write the tree attribute to .csv
	file_io.WriteTreesToCSV(tree_collection, 2);
	

	return 0;

}
