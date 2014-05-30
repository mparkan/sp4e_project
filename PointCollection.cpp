#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include "PointCollection.h"
#include "CircularBuffer.h"
#include "CircularBufferCollection.h"

using namespace std;


array<unsigned int,2> PointCollection::IndexToSubscript(unsigned int ncols, unsigned int nrows, unsigned int idx){
	unsigned int row, col;
	col = idx % ncols;
	row = (idx - col) / ncols;
	return {row, col};
}


unsigned int PointCollection::SubscriptToIndex(unsigned int ncols, unsigned int row, unsigned int col){
	return row*ncols + col;
}


// Sort PointCollection by z 
void PointCollection::SortByZ()
{
	sort(points_.begin(), points_.end(), [](Point a, Point b) { return a.z > b.z;  });
}


// Set the int16 RGB colormap
void PointCollection::SetRGBColors(vector<array<unsigned int, 3>> colormap)
{	
	unsigned int ncolors = colormap.size();
	
	for (unsigned int j(0); j < points_.size(); j++){
		
		points_[j].rgb_color = colormap[points_[j].tree_idx % ncolors];	

	}	
	
}


// Extract and copy a subset from a vector of Points based on the classification attribute
PointCollection PointCollection::FilterPointsByClass(vector<unsigned int>& keep_classes)
{
	PointCollection point_collection_subset;
	for (unsigned int j(0); j < points_.size(); j++){
		for (unsigned int k(0); k < keep_classes.size(); k++){
			if (points_[j].classification == keep_classes[k]){

					//point_collection_subset.PushBackPoint(points_[j]);
					point_collection_subset.points_.push_back(points_[j]);
					
			}
		}
	}
	return point_collection_subset;
}


// Compute point indexes
void PointCollection::ComputePointIndexes()
{
	for (unsigned int j(0); j < points_.size(); j++){
		
		points_[j].point_idx = j;
		
	}	
}


// Compute the BoundingBox object of the PointCollection object
void PointCollection::ComputeBoundingBox()
{	
	
	auto result1 = minmax_element(points_.begin(), points_.end(), [](Point a, Point b) { return a.x >= b.x;  });
	auto result2 = minmax_element(points_.begin(), points_.end(), [](Point a, Point b) { return a.y >= b.y;  }); 
	
	BoundingBox bounding_box;
	
	bounding_box.x_max_idx = result1.first - points_.begin();
	bounding_box.x_min_idx = result1.second - points_.begin();
	bounding_box.y_max_idx = result2.first - points_.begin();
	bounding_box.y_min_idx = result2.second - points_.begin();
	
	bounding_box.x_max = points_[bounding_box.x_max_idx].x;
	bounding_box.x_min = points_[bounding_box.x_min_idx].x;
	bounding_box.x_max = points_[bounding_box.y_max_idx].y;
	bounding_box.y_min = points_[bounding_box.y_min_idx].y;
	
	bounding_box.availability = true;
	
	bounding_box_ = bounding_box;

}


// Compute the grid coordinates
void PointCollection::ComputeGridCoordinates()
{
	if (coordinate_scaling_ != 1 and coordinate_scaling_ != 10){
		
		coordinate_scaling_ = 1;
		cerr << "Warning: scaling factor should be 1 or 10. Defaulting to 1." << endl;
	
	} 
	
	if (not bounding_box_.availability){
		
		ComputeBoundingBox();
		
	}
	
	for(unsigned int j(0); j < points_.size(); j++){
		
		points_[j].row = (int) round((points_[j].y - points_[bounding_box_.y_min_idx].y) * double(coordinate_scaling_)); //round(
		points_[j].col = (int) round((points_[j].x - points_[bounding_box_.x_min_idx].x) * double(coordinate_scaling_));

	}
	
	n_cols_ = points_[bounding_box_.x_max_idx].col + 1; // Number of columns
	n_rows_ = points_[bounding_box_.y_max_idx].row + 1; // Number of rows
	cout << "nrows: " <<  n_rows_ << endl;
	cout << "ncols: " <<  n_cols_ << endl;

}


// Compute grid node membership
void PointCollection::AssignGridCells()
{
	vector<vector<int>> idx_grid(n_cols_*n_rows_);
	cout << "size: " <<  n_cols_*n_rows_ << endl;
	
	for(unsigned int j(0); j < points_.size(); j++){
		
		idx_grid[SubscriptToIndex(n_cols_, points_[j].row, points_[j].col)].push_back(j);
		
	}
	
	idx_grid_ = idx_grid;
	
}
	

// Extract the grid values located within the given CircularBuffer
void PointCollection::ExtractPointsInBuffer(CircularBuffer& circular_buffer, PointCollection& sample, int& col_0, int& row_0)
{
	vector<int> tmp_idx;
	int col_idx, row_idx;

	for(unsigned int j(0); j < circular_buffer.coordinate_offsets_.size(); j++){
		
		// Check if row and column are inside the grid
		col_idx = col_0 + circular_buffer.coordinate_offsets_[j][0];
		row_idx = row_0 + circular_buffer.coordinate_offsets_[j][1];

		// Check if the kernel cell is located within the grid
		if (((unsigned int)row_idx <= n_rows_-1) and ((unsigned int)row_idx >= 0) and ((unsigned int)col_idx <= n_cols_-1) and ((unsigned int)col_idx >= 0)){
			
			tmp_idx = idx_grid_[SubscriptToIndex(n_cols_, row_idx, col_idx)]; // Grid cell linear index
			
			// Check if the grid cell is non-empty
			if (not tmp_idx.empty()){

				 for(unsigned int k(0); k < tmp_idx.size(); k++){
					 
					// Check if the point is non-segmented
					if (not points_[tmp_idx[k]].segmentation_status){
					
						sample.points_.push_back(points_[tmp_idx[k]]); 

					}
						
				 } 
			}
		}
	}

} 


// Find the local maxima
void PointCollection::FindLocalMaxima(CircularBuffer& circular_buffer)
{
	bool locmax;
	PointCollection local_points;
	local_points.points_.reserve(1000); // Value based on max point density (current max. density is ~70 pts per square meter for ALS)
	
	for(unsigned int j(0); j < points_.size(); j++){
		
		if (points_[j].local_maxima_status == 2){ // If the LocalMaximaStatus is undetermined
			
			int col_0 = points_[j].col;
			int row_0 = points_[j].row;
			ExtractPointsInBuffer(circular_buffer, local_points, col_0, row_0);
			
			locmax = true;
			unsigned int k(0);
			while(locmax and (k < local_points.points_.size())){
				
				locmax = (points_[j].z >= local_points.points_[k].z);
				points_[local_points.points_[k].point_idx].local_maxima_status = 0;
				k++;
				
			}
			
			if (locmax){
				
				points_[j].local_maxima_status = 1;
				
			} else {
				
				points_[j].local_maxima_status = 0;
				
			}
			
		}
		
		local_points.points_.clear(); 

	}
	
}

// Get coordinate scaling factor
unsigned int PointCollection::GetScalingFactor()
{
		
	return coordinate_scaling_;
	
}	


