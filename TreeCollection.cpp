#include <iostream>
#include <vector>
#include <array>
#include "PointCollection.h"
#include "TreeCollection.h"

using namespace std;

TreeCollection::TreeCollection(PointCollection& point_collection, unsigned int min_n_points, unsigned int min_height)
{
	
	TreeCollection::Tree tree;
	PointCollection temp_point_collection;
	array<double, 3> barycenter;
	unsigned int idx(0);
	unsigned int k(0);
	bool test(true);
	
	while(test){
	
		// Extract points belonging to the same tree
		for (unsigned int j(0); j < point_collection.points_.size(); j++){
			
			// Extract points belonging to the same tree
			if (point_collection.points_[j].tree_idx == k){
				
				temp_point_collection.points_.push_back(point_collection.points_[j]);
				
			}
			
		}
		
		if (temp_point_collection.points_.size() != 0){
			
			test = true;
			
			// Compute tree attributes
			tree.x_top = temp_point_collection.points_[0].x;
			tree.y_top = temp_point_collection.points_[0].y;
			tree.h_top = temp_point_collection.points_[0].z;
			tree.n_points = temp_point_collection.points_.size();
				
			// Filter trees based on minimum number of points and height
			if ((tree.n_points >= min_n_points) and (tree.h_top >= min_height)){
				
				tree.tree_idx = idx;
				
				// Compute the tree barycenter
				barycenter = ComputeBarycenter(temp_point_collection);
				tree.x_barycenter = barycenter[0];
				tree.y_barycenter = barycenter[1];
				tree.h_barycenter = barycenter[2];
				tree.rel_h_barycenter = tree.h_barycenter/tree.h_top;
				
				trees_.push_back(tree);
				idx++;
				
			}
			

		} else {
			
			test = false;
			
		}
			
		
		temp_point_collection.points_.clear();
		k++;
	}
	
}

// Compute the trees barycenter
array<double, 3> TreeCollection::ComputeBarycenter(PointCollection point_collection){
	
	array<double, 3> barycenter;
	double x_sum, y_sum, z_sum;
	
	for (unsigned int j(0); j < point_collection.points_.size(); j++){
		
		x_sum += point_collection.points_[j].x;
		y_sum += point_collection.points_[j].y;
		z_sum += point_collection.points_[j].z;
		
	}
	
	double n_points = (double) point_collection.points_.size();
	
	barycenter = {x_sum/n_points ,y_sum/n_points ,z_sum/n_points};
	
	return barycenter;
	
}
