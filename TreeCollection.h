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
 * This class represents trees and their properties.
 * 
 */
 
#ifndef TREECOLLECTION_H
#define TREECOLLECTION_H

#include <iostream>
#include <vector>
#include "PointCollection.h"

class TreeCollection {

friend class FileIO;

public:
	
	TreeCollection(PointCollection& point_collection, unsigned int min_n_points, unsigned int min_height); // Constructor
	~TreeCollection(){}; // Destructor
	
private:

	/**
	 * Structure representing a tree.
	 *
	 */
	struct Tree {
	
		double x_top;
		double y_top;
		double h_top;
		double x_barycenter;
		double y_barycenter;
		double h_barycenter;
		double rel_h_barycenter;
		unsigned int n_points;
		unsigned int tree_idx;
	
	};
	
	/**
	 * Computes the barycenter of the points constituing a tree.
	 *
	 * @param  point_collection A PointCollection representing a tree.
	 * @return Return the x, y, z coordinates of the barycenter.
	 */
	std::array<double, 3> ComputeBarycenter(PointCollection point_collection);
	
	std::vector<Tree> trees_;
	
};

#endif
