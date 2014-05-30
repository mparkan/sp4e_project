#include <iostream>
#include <vector>
#include "PointCollection.h"
#include "SegmenterSNC.h"
#include "CircularBuffer.h"
#include "CircularBufferCollection.h"

using namespace std;

void SegmenterSNC::SegmentPointCollection(PointCollection& point_collection, CircularBufferCollection& circular_buffer_collection, bool verbosity)
{
	PointCollection N, P, sample;
	N.points_.reserve(20000);
	P.points_.reserve(20000);
	sample.points_.reserve(20000);
	
	unsigned int buffer_idx, iteration_idx(0);
	double offset;
	
	n_unsegmented_ = point_collection.points_.size();
	
	while (n_unsegmented_ >  0)
	{
		
		// Determine the index of the highest unsegmented Point in the PointCollection
		unsigned int j(0), max_idx(0);
		while(point_collection.points_[j].segmentation_status){
			
			max_idx++;
			j++;
			
		}
		
		double max_z = point_collection.points_[max_idx].z;
		//cout << "max z: " << max_z << endl;
		
		// Find column and row of the highest unsegmented point in the cloud
		int col_0 = point_collection.points_[max_idx].col;
		int row_0 = point_collection.points_[max_idx].row;
		
		// Set the search radius as a function of height
		if(max_z > 15){
			
			buffer_idx  = 3; 
			
		} else if((max_z <= 15) and (max_z > 8)){
			
			buffer_idx  = 2;
			
		} else{
			
			buffer_idx = 1;
			
		}
		
		// Extract the points located within the circular buffer
		point_collection.ExtractPointsInBuffer(circular_buffer_collection.circular_buffers_[buffer_idx], sample, col_0, row_0);
		
		// Sort sample by height
		//cout << "Sorting sample...";
		sample.SortByZ();
		//cout << "Done!" << endl;
		
		// Add the Point with the maximum height to P as an initial seed
		P.points_.push_back(sample.points_[0]);
		
		// Add a random point to N as initial seed
		//offset = 2 * double(current_buffer.GetRadius());
		offset = 2 * double(circular_buffer_collection.circular_buffers_[buffer_idx].radius_);
		PointCollection::Point seed_n; 
		seed_n.x = point_collection.points_[max_idx].x + offset;
		seed_n.y = point_collection.points_[max_idx].y + offset;
		seed_n.z = point_collection.points_[max_idx].z;
		seed_n.row = 0;
		seed_n.col = 0;
		seed_n.classification = 0;
		seed_n.point_idx = 0;
		seed_n.tree_idx = 0;
		seed_n.local_maxima_status = 2;
		seed_n.segmentation_status = false;
		seed_n.rgb_color = {0, 0, 0};
		N.points_.push_back(seed_n);
		
		// Classify sample points
		//cout << "Classifying sample..." << endl;
		ClassifySample(P, N, sample);
		//cout << "Done!" << endl;
		
		for (unsigned int j(0); j < P.points_.size(); j++){
			
			point_collection.points_[P.points_[j].point_idx].segmentation_status = true; // Set "segmentation_status" attribute to true for segmented points
			point_collection.points_[P.points_[j].point_idx].tree_idx = iteration_idx; // Set "tree_idx" attribute to current iteration index for segmented points
			
		}
		
		n_unsegmented_ = n_unsegmented_ - P.points_.size(); // Update the number of remaining unsegmented points 
		
		if (verbosity){
			
			cout << "Iteration: "  << iteration_idx <<  endl;
			cout << "Col: " << col_0 << endl;
		    cout << "Row: " << row_0 << endl;
			cout << "Tree size: "  << P.points_.size() <<  endl;
			cout << "Remaining points: " << n_unsegmented_ << endl;
			cout << "****************************************" <<  endl;
			
		}
		
		iteration_idx++; // Increment tree index at each successful segmentation
		
		// Clear current sample contents
		sample.points_.clear();
		N.points_.clear();
		P.points_.clear();
		
	}
		
}


void SegmenterSNC::ClassifySample(PointCollection& P, PointCollection& N, PointCollection& sample)
{
	double dmin1, dmin2, dt;	
			
	for (unsigned int j(1); j < sample.points_.size(); j++){
		
		//if (not sample.points_[j].segmentation_status){
			
			// Compute minimal distance from u to any point in P_i
			dmin1 = FindMinDistance(sample.points_[j], P);
			//cout << "dmin1 = " << dmin1 << endl;
			
			// Compute minimal distance from u to any point in N_i
			dmin2 = FindMinDistance(sample.points_[j], N);
			//cout << "dmin2 = " << dmin2 << endl;
		
			if (not sample.points_[j].local_maxima_status) { // If the point is the local maximum 
				
				if (dmin1 <= dmin2){
					
					P.points_.push_back(sample.points_[j]);
					
				} else if (dmin1 > dmin2) {

					N.points_.push_back(sample.points_[j]);
					
				}
			}
			else {

				if (sample.points_[j].z > 15) {
					
					dt = 4;
					
				} else {
					
					dt = 2.89;
					
				}
				
				// Compare dmin1 and dmin2 to threshold
				if (dmin1 > dt) {

					N.points_.push_back(sample.points_[j]);
					
				} 
				else if (dmin1 <= dt and dmin1 <= dmin2) {

					P.points_.push_back(sample.points_[j]);
					
				} 
				else if (dmin1 <= dt and dmin1 > dmin2) {
					
					N.points_.push_back(sample.points_[j]);
					
				}
			}
	
		//}
	}
	
}


inline double SegmenterSNC::FindMinDistance(PointCollection::Point& point, PointCollection& point_collection)
{
	vector<double> d;
	d.reserve(point_collection.points_.size());
	
	// Compute pairwise squared distances
	double nx, ny;
	for (unsigned int j(0); j < point_collection.points_.size(); j++){
		
		nx = point_collection.points_[j].x - point.x;
		ny = point_collection.points_[j].y - point.y;
		d.push_back(nx*nx + ny*ny); // Use the squared distance to avoid square root computation
		
	}
	
	// Find minimum distance
	double min = d[0];
	for (unsigned int j(0); j < d.size(); j++){
		
		if (d[j] < min){
			
			min = d[j];
			
		}
		
	}
	
	return min; 
	
}
